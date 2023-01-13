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
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x1f3ac02b, "cdev_del" },
	{ 0xbc6cff2b, "class_destroy" },
	{ 0x87447f11, "device_destroy" },
	{ 0x7b5cc495, "device_create" },
	{ 0xebe637dc, "__class_create" },
	{ 0x417ad940, "cdev_add" },
	{ 0x628ec589, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xc5850110, "printk" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "28C276AA24A9D7C8787B469");
