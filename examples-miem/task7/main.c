#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "periph/gpio.h"
#include "thread.h"
#include "xtimer.h"

#define BUTTON_PIN      GPIO_PIN(PORT_A, 0)
#define LED_PIN         GPIO_PIN(PORT_C, 9)

#define THREAD_STACK_SIZE   THREAD_STACKSIZE_DEFAULT

/*
 * Global variables.
 *
 * These exist for the whole program lifetime.
 * Their exact memory section depends on initialization and const/static.
 */
int global_uninitialized;
int global_initialized = 123;
const int global_const = 123;

static int static_global_uninitialized;
static int static_global_initialized = 123;
static const int static_global_const = 123;

/*
 * Thread stack.
 *
 * This is a global array, so the stack memory for the created thread is also
 * stored in global RAM. The local variables of that thread will live inside
 * this array while the thread is running.
 */
static char worker_thread_stack[THREAD_STACK_SIZE];

/*
 * Values captured inside the interrupt callback.
 *
 * We do not print directly inside the interrupt callback. Instead, the callback
 * saves addresses here, and main() prints them later.
 */
static volatile bool interrupt_was_called = false;
static volatile uintptr_t interrupt_local_address = 0;
static volatile uintptr_t interrupt_static_local_address = 0;
static volatile uintptr_t interrupt_static_const_local_address = 0;

static void print_separator(const char *title)
{
    printf("\n--- %s ---\n", title);
}

static void print_global_addresses(void)
{
    print_separator("global variables");

    printf("global_uninitialized              at %p\n",
           (void *)&global_uninitialized);

    printf("global_initialized                at %p\n",
           (void *)&global_initialized);

    printf("global_const                      at %p\n",
           (void *)&global_const);

    printf("static_global_uninitialized       at %p\n",
           (void *)&static_global_uninitialized);

    printf("static_global_initialized         at %p\n",
           (void *)&static_global_initialized);

    printf("static_global_const               at %p\n",
           (void *)&static_global_const);
}

static void print_main_local_addresses(void)
{
    int local_variable = 123;
    const int local_const = 123;

    static int static_local_uninitialized;
    static int static_local_initialized = 123;
    static const int static_local_const = 123;

    print_separator("local variables in main helper function");

    printf("local_variable                    at %p\n",
           (void *)&local_variable);

    printf("local_const                       at %p\n",
           (void *)&local_const);

    printf("static_local_uninitialized        at %p\n",
           (void *)&static_local_uninitialized);

    printf("static_local_initialized          at %p\n",
           (void *)&static_local_initialized);

    printf("static_local_const                at %p\n",
           (void *)&static_local_const);
}

static void *worker_thread(void *arg)
{
    (void)arg;

    int thread_local_variable = 123;
    const int thread_local_const = 123;

    static int thread_static_local_uninitialized;
    static int thread_static_local_initialized = 123;
    static const int thread_static_local_const = 123;

    print_separator("local variables in another RIOT thread");

    printf("thread_local_variable             at %p\n",
           (void *)&thread_local_variable);

    printf("thread_local_const                at %p\n",
           (void *)&thread_local_const);

    printf("thread_static_local_uninitialized at %p\n",
           (void *)&thread_static_local_uninitialized);

    printf("thread_static_local_initialized   at %p\n",
           (void *)&thread_static_local_initialized);

    printf("thread_static_local_const         at %p\n",
           (void *)&thread_static_local_const);

    return NULL;
}

static void button_interrupt_callback(void *arg)
{
    (void)arg;

    int interrupt_local_variable = 123;

    static int interrupt_static_local_variable;
    static const int interrupt_static_const_local = 123;

    interrupt_local_address = (uintptr_t)&interrupt_local_variable;
    interrupt_static_local_address = (uintptr_t)&interrupt_static_local_variable;
    interrupt_static_const_local_address = (uintptr_t)&interrupt_static_const_local;

    interrupt_was_called = true;

    gpio_set(LED_PIN);
    gpio_irq_disable(BUTTON_PIN);
}

static void print_interrupt_addresses(void)
{
    print_separator("variables in GPIO interrupt callback");

    printf("interrupt_local_variable          at %p\n",
           (void *)interrupt_local_address);

    printf("interrupt_static_local_variable   at %p\n",
           (void *)interrupt_static_local_address);

    printf("interrupt_static_const_local      at %p\n",
           (void *)interrupt_static_const_local_address);
}

int main(void)
{
    puts("task7: memory address experiment");

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    print_global_addresses();
    print_main_local_addresses();

    thread_create(worker_thread_stack,
                  sizeof(worker_thread_stack),
                  THREAD_PRIORITY_MAIN - 1,
                  0,
                  worker_thread,
                  NULL,
                  "worker");

    xtimer_msleep(500);

    if (gpio_init_int(BUTTON_PIN, GPIO_IN_PD, GPIO_RISING,
                      button_interrupt_callback, NULL) < 0) {
        puts("gpio_init_int failed");
        return 1;
    }

    puts("\nPress USER button to call GPIO interrupt callback...");

    while (!interrupt_was_called) {
        xtimer_msleep(100);
    }

    print_interrupt_addresses();

    puts("\nDone.");

    while (1) {
        xtimer_sleep(60);
    }

    return 0;
}


/*
main(): This is RIOT! (Version: 2017.01-devel-30140-gd8210-miem_2023)␊
task7: memory address experiment␊
␊
--- global variables ---␊
global_uninitialized              at 0x200002ac␊
global_initialized                at 0x20000200␊
global_const                      at 0x8002ca4␊
static_global_uninitialized       at 0x200002c4␊
static_global_initialized         at 0x20000204␊
static_global_const               at 0x8002cac␊
␊
--- local variables in main helper function ---␊
local_variable                    at 0x20000c78␊
local_const                       at 0x20000c7c␊
static_local_uninitialized        at 0x200002c8␊
static_local_initialized          at 0x20000208␊
static_local_const                at 0x8002cb0␊
␊
--- local variables in another RIOT thread ---␊
thread_local_variable             at 0x20000688␊
thread_local_const                at 0x2000068c␊
thread_static_local_uninitialized at 0x200002cc␊
thread_static_local_initialized   at 0x2000020c␊
thread_static_local_const         at 0x8002cb4␊
␊
Press USER button to call GPIO interrupt callback...␊
␊
--- variables in GPIO interrupt callback ---␊
interrupt_local_variable          at 0x2000016c␊
interrupt_static_local_variable   at 0x200002bc␊
interrupt_static_const_local      at 0x8002ca8␊
␊
Done.␊
*/

/*
0x08000000  FLASH
            ┌─────────────────────────────────────────┐
            │ program code / functions                │
            │ string literals                         │
            │ const global/static variables           │
            │                                         │
            │ global_const                    0x8002ca4
            │ interrupt_static_const_local    0x8002ca8
            │ static_global_const             0x8002cac
            │ static_local_const              0x8002cb0
            │ thread_static_local_const       0x8002cb4
            └─────────────────────────────────────────┘


0x20000000  SRAM
            ┌─────────────────────────────────────────┐
            │ interrupt stack / exception stack area  │
            │ interrupt_local_variable        0x2000016c
            ├─────────────────────────────────────────┤
            │ .data: initialized writable static data │
            │ global_initialized              0x20000200
            │ static_global_initialized       0x20000204
            │ static_local_initialized        0x20000208
            │ thread_static_local_initialized 0x2000020c
            ├─────────────────────────────────────────┤
            │ .bss: zero-initialized static data      │
            │ global_uninitialized            0x200002ac
            │ interrupt_static_local_variable 0x200002bc
            │ static_global_uninitialized     0x200002c4
            │ static_local_uninitialized      0x200002c8
            │ thread_static_local_uninitialized
            │                                 0x200002cc
            ├─────────────────────────────────────────┤
            │ worker thread stack                     │
            │ thread_local_variable           0x20000688
            │ thread_local_const              0x2000068c
            ├─────────────────────────────────────────┤
            │ main thread stack                       │
            │ local_variable                  0x20000c78
            │ local_const                     0x20000c7c
            └─────────────────────────────────────────┘
*/