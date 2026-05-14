#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "periph/gpio.h"
#include "xtimer.h"

#define BUTTON_PIN          GPIO_PIN(PORT_A, 0)
#define LED_PIN             GPIO_PIN(PORT_C, 9)

#define DEBOUNCE_MS         30
#define LONG_PRESS_MS       1000

#define SLOW_BLINK_DELAY_MS 500
#define FAST_BLINK_DELAY_MS 125

static xtimer_t debounce_timer;

static bool last_stable_button_state = false;
static uint32_t button_press_start_ms = 0;
static volatile uint32_t current_blink_delay_ms = SLOW_BLINK_DELAY_MS;

static void switch_blink_speed(void)
{
    if (current_blink_delay_ms == SLOW_BLINK_DELAY_MS) {
        current_blink_delay_ms = FAST_BLINK_DELAY_MS;
    }
    else {
        current_blink_delay_ms = SLOW_BLINK_DELAY_MS;
    }
}

static void debounce_callback(void *arg)
{
    (void)arg;

    bool current_stable_button_state = gpio_read(BUTTON_PIN);

    if (current_stable_button_state == last_stable_button_state) {
        gpio_irq_enable(BUTTON_PIN);
        return;
    }

    last_stable_button_state = current_stable_button_state;

    uint32_t current_time_ms = xtimer_now_usec() / 1000;

    if (current_stable_button_state) {
        button_press_start_ms = current_time_ms;
    }
    else if ((current_time_ms - button_press_start_ms) >= LONG_PRESS_MS) {
        switch_blink_speed();
    }

    gpio_irq_enable(BUTTON_PIN);
}

static void button_interrupt_callback(void *arg)
{
    (void)arg;

    gpio_irq_disable(BUTTON_PIN);
    xtimer_set(&debounce_timer, DEBOUNCE_MS * 1000);
}

int main(void)
{
    puts("button-led long-press frequency example");

    gpio_init(LED_PIN, GPIO_OUT);

    debounce_timer.callback = debounce_callback;
    debounce_timer.arg = NULL;

    if (gpio_init_int(BUTTON_PIN, GPIO_IN_PD, GPIO_BOTH,
                      button_interrupt_callback, NULL) < 0) {
        puts("gpio_init_int failed");
        return 1;
    }

    while (1) {
        gpio_toggle(LED_PIN);
        xtimer_msleep(current_blink_delay_ms);
    }

    return 0;
}