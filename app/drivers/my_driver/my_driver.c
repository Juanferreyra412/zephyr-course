#define DT_DRV_COMPAT my_driver

#include "my_driver.h"

static int my_driver_led_init_impl(const struct device *dev)
{
	const struct my_driver_config *config = dev->config;
	struct my_driver_data *data = dev->data;

	if (!gpio_is_ready_dt(&config->led_pin))
		return -ENODEV;
	if (gpio_pin_configure_dt(&config->led_pin, GPIO_OUTPUT_ACTIVE) < 0)
		return -EIO;

	if (gpio_pin_set_dt(&config->led_pin, IS_ENABLED(CONFIG_LED_DEFAULT_ON)) < 0)
		return -EIO;

	data->led_state = IS_ENABLED(CONFIG_LED_DEFAULT_ON);

	return 0;
}

static int my_driver_led_on_impl(const struct device *dev)
{
	const struct my_driver_config *config = dev->config;
	struct my_driver_data *data = dev->data;

	if (gpio_pin_set_dt(&config->led_pin, 1) < 0)
		return -EIO;

	data->led_state = true;

	return 0;
}

static int my_driver_led_off_impl(const struct device *dev)
{
	const struct my_driver_config *config = dev->config;
	struct my_driver_data *data = dev->data;

	if (gpio_pin_set_dt(&config->led_pin, 0) < 0)
		return -EIO;

	data->led_state = false;

	return 0;
}

static int my_driver_led_toggle_impl(const struct device *dev)
{
	const struct my_driver_config *config = dev->config;
	struct my_driver_data *data = dev->data;

	if (gpio_pin_toggle_dt(&config->led_pin) < 0)
		return -EIO;

	data->led_state = !data->led_state;

	return 0;
}

static bool my_driver_led_get_state_impl(const struct device *dev)
{
	struct my_driver_data *data = dev->data;

	return data->led_state;
}

static const struct my_driver_api my_api = {
    .led_init = my_driver_led_init_impl,
    .led_on = my_driver_led_on_impl,
    .led_off = my_driver_led_off_impl,
    .led_toggle = my_driver_led_toggle_impl,
    .led_get_state = my_driver_led_get_state_impl,
};

#define MY_DRIVER_INIT(inst)                                                                       \
	static struct my_driver_config my_config_##inst = {.led_pin =                                  \
	                                                       GPIO_DT_SPEC_INST_GET(inst, gpios)};    \
	static struct my_driver_data my_data_##inst;                                                   \
	DEVICE_DT_INST_DEFINE(inst, my_driver_led_init_impl, NULL, &my_data_##inst, &my_config_##inst, \
	                      POST_KERNEL, MY_DRIVER_INIT_PRIORITY, &my_api);

DT_INST_FOREACH_STATUS_OKAY(MY_DRIVER_INIT)
