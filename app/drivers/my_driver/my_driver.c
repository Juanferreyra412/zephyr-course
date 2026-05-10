#define DT_DRV_COMPAT my_driver

#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

struct my_driver_config {
	const struct gpio_dt_spec led_pin;
};

struct my_driver_data {
	bool led_state;
};

static int my_driver_sample_fetch_impl(const struct device *dev, enum sensor_channel chan)
{
	// Enciender el LED
	struct my_driver_data *data = dev->data;
	const struct my_driver_config *config = dev->config;

	data->led_state = true;
	if (gpio_pin_set_dt(&config->led_pin, data->led_state) < 0)
		return -EIO;

	return 0;
}

static int my_sensor_channel_get_impl(const struct device *dev, enum sensor_channel chan,
                                      struct sensor_value *val)
{
	(void)chan;
	(void)val;
	// Apagar el LED
	struct my_driver_data *data = dev->data;
	const struct my_driver_config *config = dev->config;

	data->led_state = false;
	if (gpio_pin_set_dt(&config->led_pin, data->led_state) < 0)
		return -EIO;

	return 0;
}

static const struct sensor_driver_api my_driver_api = {
    .sample_fetch = my_driver_sample_fetch_impl,
    .channel_get = my_sensor_channel_get_impl,
};

static int my_driver_init_function(const struct device *dev)
{
	const struct my_driver_config *config = dev->config;

	if (!gpio_is_ready_dt(&config->led_pin))
		return -ENODEV;
	if (gpio_pin_configure_dt(&config->led_pin, GPIO_OUTPUT_ACTIVE) < 0)
		return -EIO;

	return 0;
}

#define MY_DRIVER_INIT(inst)                                                                       \
	static struct my_driver_config my_config_##inst = {.led_pin =                                  \
	                                                       GPIO_DT_SPEC_INST_GET(inst, gpios)};    \
	static struct my_driver_data my_data_##inst;                                                   \
	SENSOR_DEVICE_DT_INST_DEFINE(inst, my_driver_init_function, NULL, &my_data_##inst,             \
	                             &my_config_##inst, POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,      \
	                             &my_driver_api);

DT_INST_FOREACH_STATUS_OKAY(MY_DRIVER_INIT)
