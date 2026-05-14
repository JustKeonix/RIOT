#include "periph/gpio.h"
#include "xtimer.h"

#define LED_PIN        GPIO_PIN(PORT_C, 9)

#define UNIT_MS        200

#define DOT_UNITS      1
#define DASH_UNITS     3

#define SYMBOL_GAP     1
#define LETTER_GAP     3
#define WORD_GAP       7

static void led_on(void)
{
       gpio_set(LED_PIN);
}

static void led_off(void)
{
       gpio_clear(LED_PIN);
}

static void wait_units(int units)
{
       xtimer_msleep(units * UNIT_MS);
}

static void send_symbol(int units)
{
       led_on();
       wait_units(units);

       led_off();
       wait_units(SYMBOL_GAP);
}

static void send_s(void)
{
       send_symbol(DOT_UNITS);
       send_symbol(DOT_UNITS);
       send_symbol(DOT_UNITS);
}

static void send_o(void)
{
       send_symbol(DASH_UNITS);
       send_symbol(DASH_UNITS);
       send_symbol(DASH_UNITS);
}

static void send_sos(void)
{
       send_s();
       wait_units(LETTER_GAP - SYMBOL_GAP);

       send_o();
       wait_units(LETTER_GAP - SYMBOL_GAP);

       send_s();
       wait_units(WORD_GAP - SYMBOL_GAP);
}

int main(void)
{
       gpio_init(LED_PIN, GPIO_OUT);
       led_off();

       while (1) {
              send_sos();
       }

       return 0;
}