#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "my_driver.h"

#define SLEEP_TIME_MS CONFIG_LED_BLINK_INTERVAL

#define LED_NODE DT_ALIAS(led)

static const struct device *my_led = DEVICE_DT_GET(LED_NODE);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
	if (my_driver_led_get_state(my_led)) {
		LOG_INF("LED state: ON");
	} else {
		LOG_INF("LED state: OFF");
	}

	my_driver_led_off(my_led);
	k_msleep(SLEEP_TIME_MS);

	my_driver_sample_fetch(my_led, SENSOR_CHAN_ALL);
	k_msleep(SLEEP_TIME_MS);

	while (1) {

		k_msleep(SLEEP_TIME_MS);

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
