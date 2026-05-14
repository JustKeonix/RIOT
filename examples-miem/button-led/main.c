#include <stdio.h>

#include "periph/gpio.h"
#include "xtimer.h"

#define BUTTON_PIN   GPIO_PIN(PORT_A, 0)
#define LED_PIN      GPIO_PIN(PORT_C, 9)

int main(void)
{
    puts("button-led polling");

    gpio_init(BUTTON_PIN, GPIO_IN_PD);
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (gpio_read(BUTTON_PIN)) {
            gpio_set(LED_PIN);
        }
        else {
            gpio_clear(LED_PIN);
        }

        xtimer_msleep(20);
    }

    return 0;
}