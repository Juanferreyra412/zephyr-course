#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS CONFIG_LED_BLINK_INTERVAL

#define LED_NODE DT_ALIAS(led)

static const struct device *my_led = DEVICE_DT_GET(LED_NODE);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{

	while (1) {

		if (sensor_sample_fetch(my_led) < 0) {
			LOG_ERR("Failed to fetch sample from my_led");
		}

		k_msleep(SLEEP_TIME_MS);

		// struct sensor_value value;
		if (sensor_channel_get(my_led, SENSOR_CHAN_ALL, NULL) < 0) {
			LOG_ERR("Failed to get channel from my_led");
		}

		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}
