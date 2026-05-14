#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "periph/gpio.h"
#include "xtimer.h"

#define BUTTON_PIN       GPIO_PIN(PORT_A, 0)
#define LED_PIN          GPIO_PIN(PORT_C, 9)

#define DEBOUNCE_US      30000U
#define LONG_PRESS_US    1000000U

#define SLOW_PERIOD_US   1000000U
#define FAST_PERIOD_US   250000U

static xtimer_t debounce_timer;

static volatile bool button_is_pressed = false;
static volatile uint32_t press_start_us = 0;
static volatile uint32_t blink_period_us = SLOW_PERIOD_US;

static void debounce_cb(void *arg)
{
    (void)arg;

    bool pressed = gpio_read(BUTTON_PIN);
    uint32_t now = xtimer_now_usec();

    if (pressed && !button_is_pressed) {
        button_is_pressed = true;
        press_start_us = now;
    }
    else if (!pressed && button_is_pressed) {
        button_is_pressed = false;

        if ((now - press_start_us) >= LONG_PRESS_US) {
            if (blink_period_us == SLOW_PERIOD_US) {
                blink_period_us = FAST_PERIOD_US;
            }
            else {
                blink_period_us = SLOW_PERIOD_US;
            }
        }
    }

    gpio_irq_enable(BUTTON_PIN);
}

static void button_cb(void *arg)
{
    (void)arg;

    gpio_irq_disable(BUTTON_PIN);
    xtimer_set(&debounce_timer, DEBOUNCE_US);
}

int main(void)
{
    puts("button-led long-press frequency example");

    gpio_init(LED_PIN, GPIO_OUT);

    debounce_timer.callback = debounce_cb;
    debounce_timer.arg = NULL;

    if (gpio_init_int(BUTTON_PIN, GPIO_IN_PD, GPIO_BOTH, button_cb, NULL) < 0) {
        puts("gpio_init_int failed");
        return 1;
    }

    while (1) {
        gpio_toggle(LED_PIN);
        xtimer_usleep(blink_period_us / 2);
    }

    return 0;
}