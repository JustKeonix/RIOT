APPLICATION = button-led-irq-freq

BOARD ?= nucleo-f072rb

USEMODULE += periph_gpio
USEMODULE += periph_gpio_irq
USEMODULE += xtimer
USEMODULE += stdio_uart

RIOTBASE ?= $(CURDIR)/../..

DEVELHELP ?= 1

CFLAGS += -DCONFIG_BOARD_HAS_HSE=0
CFLAGS += -DCONFIG_BOARD_HAS_LSE=0

# Board mismatch workaround:
# The nucleo-f072rb default UART is USART2 on PA2/PA3.
# On STM32F072B-DISCO, PA2/PA3 are not convenient because they are tied to board features.
# We use USART1 instead: PA9 = TX, PA10 = RX, connected to the FTDI adapter.
# Works with picocom -b 57600 /dev/ttyUSB0
CFLAGS += -DSTDIO_UART_DEV=UART_DEV\(1\)

include $(RIOTBASE)/Makefile.include