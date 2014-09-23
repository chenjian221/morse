#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>


#define LETTER 1
#define WORD 2
#define BUFFER_SIZE 1024
#define TABLE_SIZE 36

const static char letterindent = ' ';
const static char* wordindent = "/";

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Morse Code");
MODULE_AUTHOR("Jian Chen, Xinyi Chen, Wen Xin");

char *inc_msg;
char *out_msg;

int ln,temp;


char* stringcutter(char* head, int size, int* status);
char tablelookup(char* query);
//char * morse_translation(char * data);
char * replace(char * data);


static struct proc_dir_entry *proc_entry;

typedef struct morse_table {
	char* letter;
	char* morse_code;
} Morse;

static Morse trans_vec[TABLE_SIZE] = {
	{"A", ".-"}, 
	{"B", "-..."},
	{"C", "-.-."},
	{"D" , "-.."},
	{"E" , "."},
	{"F" , "..-."},
	{"G" , "--."},
	{"H" , "...."},
	{"I" , ".."},
	{"J" , ".---"},
	{"K" , "-.-"},
	{"L" , ".-.."},
	{"M" , "--"},
	{"N" , "-."},
	{"O" , "---"},
	{"P" , ".--."},
	{"Q" , "--.-"},
	{"R" , ".-."},
	{"S" , "..."},
	{"T" , "-"},
	{"U" , "..-"},
	{"V" , "...-"},
	{"W" , ".--"},
	{"X" , "-..-"},
	{"Y" , "-.--"},
	{"Z" , "--.."},
	{"1" , ".----"},
	{"2" , "..---"},
	{"3", "...--"},
	{"4" , "....-"},
	{"5" , "....."},
	{"6" , "-...."},
	{"7" , "--..."},
	{"8" , "---.."},
	{"9" , "----."},
	{"0", "-----"},
	{" ", "------"},
};

int morse_read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ){
	if(count>temp){
		count=temp;
	}
	temp=temp-count;
	copy_to_user(buf,out_msg, count);
	if(count==0)
	temp=ln;
	return count;
}

int morse_write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp){
	memset(inc_msg, 0 , BUFFER_SIZE);
	copy_from_user(inc_msg,buf,count);
	out_msg = replace(inc_msg);
	ln = strlen(out_msg);
	temp=ln;
	return count;
}

struct file_operations proc_fops = {
    read:   morse_read_proc,
    write:  morse_write_proc
};

/*char * morse_translation(char * data) {
	int i = 0;
	int result;
	char * res = vmalloc(BUFFER_SIZE);
	for (i = 0; i < 2; i++) {
		result = strncmp(data,trans_vec[i].morse_code,strlen(trans_vec[i].morse_code));
		if(!result){
			sprintf(res, "%c", trans_vec[i].letter);
			ln = strlen(res);
			temp=ln;
			return res;
		}
	}
	return "wrong";
}*/

char * replace(char * data) {
	char * a = data;
	char * b = strstr(a,"=");
	if (b==NULL) {
		return data;
	}
	int offset = (strlen(a)-strlen(b));
	char * c = (char * ) vmalloc(sizeof(char)*(1024));
	strncpy(c,a,offset);
	strcat(c," ------ ");
	b = b +1;
	a = b;
	while ((b = strstr(a,"=")) != NULL) {
		offset = (strlen(a)-strlen(b));
		strncat(c,a,offset);
		strcat(c," ------ ");
		b = b +1;
		a = b;
	}
	strcat(c,a);
	return c;
}

int init_morse_module(void){
	proc_create("morse_code",0,NULL,&proc_fops);
    inc_msg=vmalloc(BUFFER_SIZE);
    return 0;
}

void cleanup_morse_module( void ){
    remove_proc_entry("morse_code",NULL);
    vfree(inc_msg);
    vfree(out_msg);
}
module_init(init_morse_module);
module_exit(cleanup_morse_module);