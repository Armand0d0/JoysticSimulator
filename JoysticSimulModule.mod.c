#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
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
__used __section(__versions) = {
	{ 0x9de7765d, "module_layout" },
	{ 0x8e51f20, "device_destroy" },
	{ 0x67d54299, "remove_proc_entry" },
	{ 0x378a81c2, "input_unregister_device" },
	{ 0x2142178f, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x3dc2395e, "class_destroy" },
	{ 0xabdd13a9, "sysfs_remove_file_ns" },
	{ 0xf4c6a598, "kobject_put" },
	{ 0x1cc8e120, "input_free_device" },
	{ 0xf6d07d4, "input_register_device" },
	{ 0x1bb865be, "input_set_abs_params" },
	{ 0xf6bdf445, "input_allocate_device" },
	{ 0xa419798f, "sysfs_create_file_ns" },
	{ 0x8318cef0, "kobject_create_and_add" },
	{ 0x2ac0c16c, "kernel_kobj" },
	{ 0x4aba4bc5, "proc_create" },
	{ 0x64fe0dcf, "device_create" },
	{ 0x296aaca8, "__class_create" },
	{ 0x19c48f0b, "cdev_add" },
	{ 0x18f0dc72, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0xb0d9a8a7, "input_event" },
	{ 0xb5f17439, "kmem_cache_alloc_trace" },
	{ 0xcbf895e0, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "E064A2AE53BC950860D13E3");
