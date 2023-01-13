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
	{ 0xc1514a3b, "free_irq" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x1f3ac02b, "cdev_del" },
	{ 0xbc6cff2b, "class_destroy" },
	{ 0x87447f11, "device_destroy" },
	{ 0xfe990052, "gpio_free" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xcdcef9e6, "gpiod_to_irq" },
	{ 0x1087712d, "gpiod_set_debounce" },
	{ 0xcf441394, "gpiod_direction_input" },
	{ 0xf62472ea, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x7b5cc495, "device_create" },
	{ 0xebe637dc, "__class_create" },
	{ 0x417ad940, "cdev_add" },
	{ 0x628ec589, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x9f49dcc4, "__stack_chk_fail" },
	{ 0x4b0a3f52, "gic_nonsecure_priorities" },
	{ 0xd697e69a, "trace_hardirqs_on" },
	{ 0xaa9405fc, "send_sig_info" },
	{ 0xec3d2e1b, "trace_hardirqs_off" },
	{ 0xc5850110, "printk" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "66C59E2A4E94D63BABD64E6");
