APPLICATION = task7

# Real board: STM32F072B-DISCO.
# RIOT target used as workaround: nucleo-f072rb, because it has the same MCU.
BOARD ?= nucleo-f072rb

# GPIO interrupt is used to inspect variables inside an interrupt callback.
USEMODULE += periph_gpio
USEMODULE += periph_gpio_irq

# xtimer is used only for small sleeps while waiting for the button press.
USEMODULE += xtimer

# printf()/puts() output through UART.
USEMODULE += stdio_uart

# This project is in RIOT/examples-miem/task7, so ../.. points to RIOT/.
RIOTBASE ?= $(CURDIR)/../..

DEVELHELP ?= 1

# Board mismatch workaround for STM32F072B-DISCO.
CFLAGS += -DCONFIG_BOARD_HAS_HSE=0
CFLAGS += -DCONFIG_BOARD_HAS_LSE=0

# Use USART1 on PA9/PA10, connected through FTDI.
CFLAGS += -DSTDIO_UART_DEV=UART_DEV\(1\)

include $(RIOTBASE)/Makefile.include