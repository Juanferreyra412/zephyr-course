/*
 * Blink LED (L3 - Task 1)
 *
 * Este programa utiliza el subsistema PWM de Zephyr para controlar el brillo
 * y el parpadeo de un LED. Implementa un efecto de "fade out" (desvanecimiento)
 * basándose en las configuraciones definidas en Kconfig (brillo máximo, tiempo
 * de desvanecimiento y tiempo de parpadeo).
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS   CONFIG_APP_LED_BLINK_SLEEP_TIME_MS
#define LED_BRIGHTNESS  CONFIG_LED_BRIGHTNESS
#define PWM_PERIOD_MS   10
#define PWM_PERIOD_NS   PWM_MSEC(PWM_PERIOD_MS)
#define PWM_PULSE_WIDTH (uint32_t)(LED_BRIGHTNESS * (PWM_PERIOD_NS / 100))
#define FADE_STEPS      (uint32_t)(CONFIG_LED_FADE_TIME_MS / PWM_PERIOD_MS)
#define FADE_DECREMENT  (uint32_t)((FADE_STEPS != 0) ? (PWM_PULSE_WIDTH / FADE_STEPS) : 0)

#define PRINT_LOG(X, ...)                                                                          \
	if (IS_ENABLED(CONFIG_ENABLE_LED_DEBUGGING))                                                   \
	printk(X, ##__VA_ARGS__)

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

int main(void)
{

	if (IS_ENABLED(CONFIG_LED_SUBSYSTEM)) {
		int err;

		if (!pwm_is_ready_dt(&pwm_led)) {
			PRINT_LOG("Error: El dispositivo PWM %s no está listo\n", pwm_led.dev->name);
			return 0;
		}

		PRINT_LOG("Iniciando PWM...\n");
		PRINT_LOG("Brillo: %d\n", LED_BRIGHTNESS);
		PRINT_LOG("Periodo del PWM: %d\n", PWM_PERIOD_NS);
		PRINT_LOG("Pulso: %d\n", PWM_PULSE_WIDTH);
		PRINT_LOG("Fade decrement: %d\n", FADE_DECREMENT);
		PRINT_LOG("Fade steps: %d\n", FADE_STEPS);

		if (!IS_ENABLED(CONFIG_CUSTOM_BLINK_PATTERN)) {
			while (1) {

				uint32_t pulse_width_current = PWM_PULSE_WIDTH;
				err = pwm_set_dt(&pwm_led, PWM_PERIOD_NS, PWM_PULSE_WIDTH);
				if (err) {
					PRINT_LOG("Error al configurar el PWM (err %d)\n", err);
					return 0;
				} else {
					PRINT_LOG("LED ENCENDIDO\n");
				}

				k_sleep(K_MSEC(SLEEP_TIME_MS));

				for (uint16_t i = 0; i < FADE_STEPS; i++) {
					if (i == 0) {
						PRINT_LOG("Iniciando fading\n");
					}
					pulse_width_current -= FADE_DECREMENT;
					err = pwm_set_dt(&pwm_led, PWM_PERIOD_NS, pulse_width_current);
					k_sleep(K_MSEC(PWM_PERIOD_MS));
				}

				pwm_set_dt(&pwm_led, PWM_PERIOD_NS, 0);
				PRINT_LOG("LED APAGADO\n");
				k_sleep(K_MSEC(SLEEP_TIME_MS));
			}
		} else if (IS_ENABLED(CONFIG_CUSTOM_BLINK_PATTERN)) {

			// Patrón de latido (doble parpadeo)

			PRINT_LOG("Iniciando patrón personalizado\n");
			while (1) {
				pwm_set_dt(&pwm_led, PWM_PERIOD_NS, PWM_PULSE_WIDTH);
				PRINT_LOG("LED ENCENDIDO\n");
				k_sleep(K_MSEC(100));
				pwm_set_dt(&pwm_led, PWM_PERIOD_NS, 0);
				PRINT_LOG("LED APAGADO\n");
				k_sleep(K_MSEC(100));

				pwm_set_dt(&pwm_led, PWM_PERIOD_NS, PWM_PULSE_WIDTH);
				PRINT_LOG("LED ENCENDIDO\n");
				k_sleep(K_MSEC(100));
				pwm_set_dt(&pwm_led, PWM_PERIOD_NS, 0);
				PRINT_LOG("LED APAGADO\n");

				k_sleep(K_MSEC(700));
			}
		}
	}

	printk("Fin del programa\n");
	return 0;
}
