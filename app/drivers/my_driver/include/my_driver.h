#ifndef MY_DRIVER_H
#define MY_DRIVER_H

#include <zephyr/toolchain.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#define MY_DRIVER_INIT_PRIORITY 60

struct my_driver_config {
	const struct gpio_dt_spec led_pin;
};

struct my_driver_data {
	bool led_state;
};

#ifdef __cplusplus
extern "C" {
#endif

struct my_driver_api {
	int (*led_init)(const struct device *dev);
	int (*led_on)(const struct device *dev);
	int (*led_off)(const struct device *dev);
	int (*led_toggle)(const struct device *dev);
	bool (*led_get_state)(const struct device *dev);
};

static inline int my_driver_led_init(const struct device *dev)
{
	return DEVICE_API_GET(my, dev)->led_init(dev);
}

static inline int my_driver_led_on(const struct device *dev)
{
	return DEVICE_API_GET(my, dev)->led_on(dev);
}

static inline int my_driver_led_off(const struct device *dev)
{
	return DEVICE_API_GET(my, dev)->led_off(dev);
}

static inline int my_driver_led_toggle(const struct device *dev)
{
	return DEVICE_API_GET(my, dev)->led_toggle(dev);
}

static inline bool my_driver_led_get_state(const struct device *dev)
{
	return DEVICE_API_GET(my, dev)->led_get_state(dev);
}

#ifdef __cplusplus
}
#endif

#endif // MY_DRIVER_H
