#include <zephyr/init.h>
#include <zephyr/kernel.h>

int my_esp32_devkit_init(void)
{
	printk("\n********************************************************\n");
	printk("           Board Initialized\n");
	printk("********************************************************\n\n");
	return 0;
}

SYS_INIT(my_esp32_devkit_init, APPLICATION, 10);
