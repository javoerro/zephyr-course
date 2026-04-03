#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 400

/* The devicetree node identifier for the "led0" alias. */

static const struct gpio_dt_spec led[] = {
    GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios),
    GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios),
};

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    for(size_t i = 0; i < ARRAY_SIZE(led); i++) {
        if (!gpio_is_ready_dt(&led[i])) return 0;
        if (gpio_pin_configure_dt(&led[i], GPIO_OUTPUT_ACTIVE) < 0) return 0;
    }

    while (1) {
        for(size_t i = 0; i < ARRAY_SIZE(led); i++) {
            if (gpio_pin_toggle_dt(&led[i]) < 0) return 0;
            k_msleep(SLEEP_TIME_MS/4);
        }

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
