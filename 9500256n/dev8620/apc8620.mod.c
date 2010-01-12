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
	{ 0x903a5ec4, "module_layout" },
	{ 0xd8778690, "per_cpu__current_task" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xb8e7ce2c, "__put_user_8" },
	{ 0xea147363, "printk" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x13eb0703, "pci_bus_write_config_dword" },
	{ 0xfda85a7d, "request_threaded_irq" },
	{ 0x61651be, "strcat" },
	{ 0x42c8de35, "ioremap_nocache" },
	{ 0x7bbb691f, "pci_bus_read_config_dword" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x1000e51, "schedule" },
	{ 0x6d334118, "__get_user_8" },
	{ 0x44a9f0a6, "register_chrdev" },
	{ 0x642e54ac, "__wake_up" },
	{ 0x33d92f9a, "prepare_to_wait" },
	{ 0xedc03953, "iounmap" },
	{ 0x9ef749e2, "unregister_chrdev" },
	{ 0xc9f9fb62, "pci_get_device" },
	{ 0x9ccb2622, "finish_wait" },
	{ 0x7efb4482, "pci_enable_device" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "04FAAE8F793D2A34445CE28");
