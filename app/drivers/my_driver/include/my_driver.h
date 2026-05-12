#ifndef MY_DRIVER_H
#define MY_DRIVER_H

#include <zephyr/toolchain.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>

#define MY_DRIVER_INIT_PRIORITY 60

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*led_init_t)(const struct device *dev);
typedef int (*led_on_t)(const struct device *dev);
typedef int (*led_off_t)(const struct device *dev);
typedef int (*led_toggle_t)(const struct device *dev);
typedef bool (*led_get_state_t)(const struct device *dev);

struct my_driver_config {
	const struct gpio_dt_spec led_pin;
};

struct my_driver_data {
	bool led_state;
};

__subsystem struct my_driver_api {
	struct sensor_driver_api parent_api;
	led_init_t led_init;
	led_on_t led_on;
	led_off_t led_off;
	led_toggle_t led_toggle;
	led_get_state_t led_get_state;
};

static inline int my_driver_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	return DEVICE_API_GET(sensor, dev)->sample_fetch(dev, chan);
}

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
