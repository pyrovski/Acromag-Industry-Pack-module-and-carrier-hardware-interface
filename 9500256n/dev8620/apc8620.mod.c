#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

#undef unix
struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = __stringify(KBUILD_MODNAME),
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0x98af416e, "struct_module" },
	{ 0x9957ab9d, "autoremove_wake_function" },
	{ 0xa03d6a57, "__get_user_4" },
	{ 0x1d26aa98, "sprintf" },
	{ 0x9925ce9c, "__might_sleep" },
	{ 0x1b7d4074, "printk" },
	{ 0x65ed66d6, "pci_bus_write_config_dword" },
	{ 0x49e79940, "__cond_resched" },
	{ 0x3762cb6e, "ioremap_nocache" },
	{ 0xdff5f0ae, "pci_bus_read_config_dword" },
	{ 0x26e96637, "request_irq" },
	{ 0x4292364c, "schedule" },
	{ 0x2071e84e, "register_chrdev" },
	{ 0xdfb5f711, "__wake_up" },
	{ 0x2e60bace, "memcpy" },
	{ 0x83c10f50, "prepare_to_wait" },
	{ 0xedc03953, "iounmap" },
	{ 0xc192d491, "unregister_chrdev" },
	{ 0xe20caba9, "pci_get_device" },
	{ 0x5ac6c754, "finish_wait" },
	{ 0x8425183, "pci_enable_device" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

