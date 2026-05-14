#include <stdio.h>

#include "periph/gpio.h"
#include "xtimer.h"

#define BUTTON_PIN     GPIO_PIN(PORT_A, 0)
#define LED_PIN        GPIO_PIN(PORT_C, 9)

#define DEBOUNCE_US    30000

static xtimer_t debounce_timer;

static void debounce_cb(void *arg)
{
    (void)arg;

    gpio_write(LED_PIN, gpio_read(BUTTON_PIN));
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
    puts("button-led interrupt debounce example");

    gpio_init(LED_PIN, GPIO_OUT);

    debounce_timer.callback = debounce_cb;
    debounce_timer.arg = NULL;

    // BUTTON_PIN    PA0
    // GPIO_IN_PD    input with pull-down resistor
    // GPIO_BOTH     interrupt on both rising and falling edges
    // button_cb     function to call when interrupt happens
    // NULL          no extra callback argument
    if (gpio_init_int(BUTTON_PIN, GPIO_IN_PD, GPIO_BOTH, button_cb, NULL) < 0) {
        puts("gpio_init_int failed");
        return 1;
    }

    gpio_write(LED_PIN, gpio_read(BUTTON_PIN));

    while (1) {
        xtimer_sleep(60);
    }

    return 0;
}