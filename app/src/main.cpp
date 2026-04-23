/*
 * Blink LED (L4 - Task 1)
 *
 * Este programa utiliza el subsistema GPIO de Zephyr para hacer un efecto de "heartbeat" con el
 * LED. El LED se define en el archivo app.overlay
 */

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS CONFIG_APP_HEARTBEAT_PERIOD_MS

/* The devicetree node identifier for the "app-led" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	printk("Iniciando heartbeat con LED\n");

	if (!gpio_is_ready_dt(&led)) {
		printk("Error: El dispositivo GPIO %s no está listo\n", led.port->name);
		return 0;
	}

	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
		printk("Error: No se pudo configurar el pin GPIO %d\n", led.pin);
		return 0;
	}

	while (1) {

		gpio_pin_set_dt(&led, 1);
		printk("LED ENCENDIDO\n");
		k_sleep(K_MSEC(100));
		gpio_pin_set_dt(&led, 0);
		printk("LED APAGADO\n");
		k_sleep(K_MSEC(100));

		gpio_pin_set_dt(&led, 1);
		printk("LED ENCENDIDO\n");
		k_sleep(K_MSEC(100));
		gpio_pin_set_dt(&led, 0);
		printk("LED APAGADO\n");

		k_sleep(K_MSEC(SLEEP_TIME_MS));
	}
	return 0;
}
