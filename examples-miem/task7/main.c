#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"

/* =========================
   GLOBAL MEMORY (RAM / FLASH)
   ========================= */
int global_var = 10;
static int global_static = 20;
static const int global_const = 30;

/* =========================
   INTERRUPT COUNTER
   ========================= */

/* =========================
   THREAD STACK
   ========================= */
static char stack[THREAD_STACKSIZE_SMALL];

/* =========================
   ISR (GPIO INTERRUPT)
   ========================= */
/* =========================
   THREAD DEMO MEMORY
   ========================= */
static void *thread_func(void *arg)
{
    (void)arg;

    int local_var = 5;
    static int thread_static = 99;

    printf("\n=== THREAD MEMORY ===\n");
    printf("local_var     = %p\n", (void*)&local_var);
    printf("thread_static = %p\n", (void*)&thread_static);

    return NULL;
}
static volatile int irq_counter = 0;
static volatile int irq_stop = 0;

static void gpio_isr(void *arg)
{
    (void)arg;

    if (irq_stop) {
        return;   // полностью блокируем дальнейшую обработку
    }

    irq_counter++;

    irq_stop = 1;
    int irq_local = 123;

    printf("%p\n", (void*)&irq_local);
    static int irq_static = 0;
    irq_static++;

    printf("%p\n", (void*)&irq_static);
}
/* =========================
   MAIN
   ========================= */
int main(void)
{
    puts("=== BLUE PILL MEMORY + ISR DEMO ===");

    /* GLOBAL MEMORY ADDRESSES */
    printf("\n=== GLOBAL MEMORY ===\n");
    printf("global_var    = %p\n", (void*)&global_var);
    printf("global_static = %p\n", (void*)&global_static);
    printf("global_const  = %p\n", (void*)&global_const);

    /* CODE (FLASH) ADDRESSES */
    printf("\n=== CODE MEMORY (FLASH) ===\n");
    printf("main() addr   = %p\n", (void*)main);
    printf("gpio_isr addr = %p\n", (void*)gpio_isr);

    /* THREAD */
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  thread_func, NULL, "t");

    /* GPIO INTERRUPT SETUP */
    gpio_t pin = GPIO_PIN(PORT_A, 1);

    int a = 7;
    puts("\nInit GPIO interrupt...");

    if (gpio_init_int(pin,
                      GPIO_IN_PU,
                      GPIO_BOTH,
                      gpio_isr,
                      NULL) < 0) {
        puts("GPIO INIT FAILED");
        return 1;
    }

    puts("READY: connect PA0 to GND");

    /* MAIN LOOP */
while (1) {
    xtimer_usleep(500000);

    if (irq_stop) {
        break;   // полностью выходим из цикла
    }

//    printf("[MAIN] irq_counter = %d\n", irq_counter);
}

puts("[MAIN] STOPPED COMPLETELY");




    return 0;
}
