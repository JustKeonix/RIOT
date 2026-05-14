APPLICATION = task7

BOARD ?= nucleo-f072rb

USEMODULE += periph_gpio_irq # поддержка прерываний от GPIO
USEMODULE += periph_gpio
USEMODULE += periph_i2c
USEMODULE += xtimer
USEMODULE += fmt
USEMODULE += stdio_uart
USEMODULE += periph_uart

RIOTBASE ?= $(CURDIR)/../..

DEVELHELP ?= 1

CFLAGS += -DCONFIG_BOARD_HAS_HSE=0
CFLAGS += -DCONFIG_BOARD_HAS_LSE=0
CFLAGS += -DSTDIO_UART_DEV=UART_DEV\(1\)

include $(RIOTBASE)/Makefile.include