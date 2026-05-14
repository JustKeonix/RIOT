#include <stdio.h>
#include <string.h>

#include "periph/gpio.h"
#include "shell.h"

#define BUTTON_PIN    GPIO_PIN(PORT_A, 0)
#define LED_PIN       GPIO_PIN(PORT_C, 9)

static int led_command(int argc, char **argv)
{
       if (argc != 2) {
              puts("usage: led on|off|toggle|state");
              return 1;
       }

       if (strcmp(argv[1], "on") == 0) {
              gpio_set(LED_PIN);
              puts("LED on");
       }
       else if (strcmp(argv[1], "off") == 0) {
              gpio_clear(LED_PIN);
              puts("LED off");
       }
       else if (strcmp(argv[1], "toggle") == 0) {
              gpio_toggle(LED_PIN);
              puts("LED toggled");
       }
       else if (strcmp(argv[1], "state") == 0) {
              if (gpio_read(LED_PIN)) {
                     puts("LED is on");
              }
              else {
                     puts("LED is off");
              }
       }
       else {
              puts("usage: led on|off|toggle|state");
              return 1;
       }

       return 0;
}

static int button_command(int argc, char **argv)
{
       (void)argv;

       if (argc != 1) {
              puts("usage: button");
              return 1;
       }

       if (gpio_read(BUTTON_PIN)) {
              puts("button is pressed");
       }
       else {
              puts("button is released");
       }

       return 0;
}

static int pins_command(int argc, char **argv)
{
       (void)argc;
       (void)argv;

       puts("LED:    PC9");
       puts("Button: PA0");
       puts("UART:   PA9 TX, PA10 RX");

       return 0;
}

static const shell_command_t shell_commands[] = {
       { "led", "control LED: led on|off|toggle|state", led_command },
       { "button", "read USER button state",          button_command },
       { "pins",   "print used board pins",           pins_command },
       { NULL, NULL, NULL }
};

int main(void)
{
       char line_buf[SHELL_DEFAULT_BUFSIZE];

       puts("task14: RIOT shell example");

       gpio_init(LED_PIN, GPIO_OUT);
       gpio_init(BUTTON_PIN, GPIO_IN_PD);

       shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

       return 0;
}

/*
main(): This is RIOT! (Version: 2017.01-devel-30140-gd8210-miem_2023)␊
task14: RIOT shell example␊
> help␍␊
Command              Description␊
---------------------------------------␊
led                  control LED: led on|off|toggle|state␊
button               read USER button state␊
pins                 print used board pins␊
> led state␍␊
LED is off␊
> led on␍␊
LED on␊
> led state␍␊
LED is on␊
>
*/