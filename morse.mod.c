#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x568fba06, "module_layout" },
	{ 0x999e8297, "vfree" },
	{ 0x8e950d31, "remove_proc_entry" },
	{ 0x76b9aa18, "proc_create_data" },
	{ 0x77e2f33, "_copy_from_user" },
	{ 0x91715312, "sprintf" },
	{ 0x85abc85f, "strncmp" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x71de9b3f, "_copy_to_user" },
	{ 0xa1c76e0a, "_cond_resched" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

