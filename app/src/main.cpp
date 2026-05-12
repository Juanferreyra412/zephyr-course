#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

// #define SLEEP_TIME_MS CONFIG_LED_BLINK_INTERVAL

#define LED_NODE DT_ALIAS(led)

static const struct device *my_led = DEVICE_DT_GET(LED_NODE);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static int cmd_sensor_sample_fetch(const struct shell *sh, size_t argc, char *argv[])
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (sensor_sample_fetch(my_led) < 0) {
		shell_error(sh, "Failed to fetch sample from my_led");
	}

	return 0;
}

static int cmd_sensor_channel_get(const struct shell *sh, size_t argc, char *argv[])
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	if (sensor_channel_get(my_led, SENSOR_CHAN_ALL, NULL) < 0) {
		shell_error(sh, "Failed to get channel from my_led");
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

SHELL_STATIC_SUBCMD_SET_CREATE(
    sub_sensor, SHELL_CMD(fetch, NULL, "Sensor fetch command", cmd_sensor_sample_fetch),
    SHELL_CMD(channel_get, NULL, "Sensor channel get command", cmd_sensor_channel_get),
    SHELL_CMD(info, NULL, "Sensor info command", cmd_sensor_info), SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Sensor subcommands", NULL);

int main(void)
{
	return 0;
}
