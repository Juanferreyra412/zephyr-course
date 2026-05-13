#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "my_driver.h"
#include <zephyr/shell/shell.h>

#define SLEEP_TIME_MS CONFIG_LED_BLINK_INTERVAL

#define LED_NODE DT_ALIAS(led)

static const struct device *my_led = DEVICE_DT_GET(LED_NODE);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static int cmd_sensor_sample_fetch(const struct shell *sh, size_t argc, char *argv[])
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (my_driver_sample_fetch(my_led, SENSOR_CHAN_ALL) < 0) {
		shell_error(sh, "Failed to fetch from my_led");
	}

	return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char *argv[])
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_fprintf(sh, SHELL_INFO, "\nSensor Device Information:\n");
	shell_fprintf(sh, SHELL_INFO, "    Name: %s\n", my_led->name);
	shell_fprintf(sh, SHELL_INFO, "    Ready: %s\n",
	              (my_led->state->initialized ? "true" : "false"));

	return 0;
}

static int cmd_sensor_set_value(const struct shell *sh, size_t argc, char *argv[])
{
	int err = 0;

	long valor = shell_strtol(argv[1], 10, &err);

	if (err != 0) {
		shell_error(sh, "Error: El argumento debe ser un número válido.");
		return -EINVAL;
	}
	if (valor < 0 || valor > 5000) {
		shell_error(sh, "Error: El valor %ld está fuera de rango. Debe estar entre 0 y 5000.",
		            valor);
		return -EINVAL;
	}

	my_driver_set_value(my_led, valor);

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sub_sensor, SHELL_CMD_ARG(fetch, NULL, "Sensor fetch command", cmd_sensor_sample_fetch, 1, 0),
    SHELL_CMD_ARG(info, NULL, "Sensor info command", cmd_sensor_info, 1, 0),
    SHELL_CMD_ARG(set_value, NULL, "Set LED value (0-5000)", cmd_sensor_set_value, 2, 0),
    SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Sensor subcommands", NULL);

int main(void)
{
	my_driver_set_value(my_led, SLEEP_TIME_MS);

	while (1) {

		k_msleep(my_driver_get_value(my_led));

		if (my_driver_led_toggle(my_led) < 0) {
			LOG_ERR("Error");
		}

		if (my_driver_led_get_state(my_led)) {
			LOG_INF("LED state: ON");
		} else {
			LOG_INF("LED state: OFF");
		}
	}

	return 0;
}
