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
	{ 0x37a0cba, "kfree" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x1f3ac02b, "cdev_del" },
	{ 0xbc6cff2b, "class_destroy" },
	{ 0x87447f11, "device_destroy" },
	{ 0xfe990052, "gpio_free" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x68ed7a09, "kmem_cache_alloc_trace" },
	{ 0x382686b7, "kmalloc_caches" },
	{ 0x7b5cc495, "device_create" },
	{ 0xebe637dc, "__class_create" },
	{ 0x417ad940, "cdev_add" },
	{ 0x628ec589, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x9f49dcc4, "__stack_chk_fail" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xe914e41e, "strcpy" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x6db81eac, "gpiod_get_raw_value" },
	{ 0xcf441394, "gpiod_direction_input" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xebad1472, "gpiod_direction_output_raw" },
	{ 0xf62472ea, "gpio_to_desc" },
	{ 0xc5850110, "printk" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "16F70F29D40E6171FAF90FF");
