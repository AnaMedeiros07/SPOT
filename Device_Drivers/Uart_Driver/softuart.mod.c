#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xfee48a5a, "module_layout" },
	{ 0x404c211a, "param_ops_int" },
	{ 0xbc6cff2b, "class_destroy" },
	{ 0x6e65558a, "class_unregister" },
	{ 0xedc03953, "iounmap" },
	{ 0x87447f11, "device_destroy" },
	{ 0xa28ed92a, "device_remove_file" },
	{ 0x46a4b118, "hrtimer_cancel" },
	{ 0xee2e1144, "device_create_file" },
	{ 0x7b5cc495, "device_create" },
	{ 0x2d0684a9, "hrtimer_init" },
	{ 0xebe637dc, "__class_create" },
	{ 0x6b4b2933, "__ioremap" },
	{ 0xaf56600a, "arm64_use_ng_mappings" },
	{ 0x1e6d26a8, "strstr" },
	{ 0xc5850110, "printk" },
	{ 0x828e22f4, "hrtimer_forward" },
	{ 0x3c5d543a, "hrtimer_start_range_ns" },
	{ 0x98cf60b3, "strlen" },
	{ 0xdcb764ad, "memset" },
	{ 0xe914e41e, "strcpy" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x9f49dcc4, "__stack_chk_fail" },
	{ 0x3854774b, "kstrtoll" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "73F98295DB3818F323C9245");
