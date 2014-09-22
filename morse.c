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
const static char wordindent ='/';

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Morse Code");
MODULE_AUTHOR("Jian Chen, Xinyi Chen, Wen Xin");

char *inc_msg;
char *out_msg;

int ln,temp;

char* morse_translation(char* data);
char* stringcutter(char* head, int size, int* status);
char tablelookup(char* query);

static struct proc_dir_entry *proc_entry;

typedef struct morse_table {
	char letter;
	char* morse_code;
} Morse;
static Morse trans_vec[TABLE_SIZE] = {
	{'A', "01"}, {'B', "1000"}
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
	
	out_msg = morse_translation(inc_msg);
	
	return count;
}

struct file_operations proc_fops = {
    read:   morse_read_proc,
    write:  morse_write_proc
};

char * morse_translation(char * data) {
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
}

/*char * morse_translation(char * data,int size) {

	int sizeleft = size;
	int status = 1;
	int textbuffercounter = 0;
	char letterbuffer = 0;
	char* codebuffer = vmalloc(sizeof(char) * 1024);
	char* textbuffer = vmalloc(sizeof(char) * 1024);
	while (1) {
		codebuffer = stringcutter(data, sizeleft, &status);
		sizeleft = sizeleft - strlen(codebuffer) - 1;
		data = data + strlen(codebuffer) + 1;
		letterbuffer = tablelookup(codebuffer);
		textbuffer[textbuffercounter] = letterbuffer;
		textbuffercounter++;
		if (status == 0) {
			textbuffer[textbuffercounter] = 0;
			return textbuffer;
		} else if (status == WORD) {
			textbuffer[textbuffercounter] = ' ';
			textbuffercounter++;
		}
	}
	return 0;
}*/


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


/*char tablelookup(char* query) {
	int i;
	for (i = 0; i < TABLE_SIZE; i++) {
		if (strcmp((const char*) query,
			(const char*) trans_vec[i].morse_code) == 0) {
			return trans_vec[i].letter;
		}
	}
	return 0;
}
*/
/*char* stringcutter(char* data) {


	int i = 0;
	char* cutoff = vmalloc(sizeof(char) * 10);
	while ((head[i] != letterindent) && (head[i] != wordindent) &&
		(size != 0)) {
		cutoff[i] = head[i];
		i++;
		size--;
	}
	if (head[i] == wordindent) {
		*status = WORD;
		cutoff[i] = 0; //EOS
		return cutoff;
	}
	if (head[i] == letterindent) {
		*status = LETTER;
		cutoff[i] = 0; //EOS
		return cutoff;
	}
	if (size == 0) {
		*status = 0;
		cutoff[i] = 0;
		return cutoff;
	}
	return cutoff;
}*/

module_init(init_morse_module);
module_exit(cleanup_morse_module);