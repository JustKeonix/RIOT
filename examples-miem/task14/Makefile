APPLICATION = task14

# Real board: STM32F072B-DISCO.
# Workaround target: nucleo-f072rb, because it has the same STM32F072RB MCU.
BOARD ?= nucleo-f072rb

# GPIO is needed for LED and button.
USEMODULE += periph_gpio

# RIOT shell module.
USEMODULE += shell

# UART output/input for shell over FTDI.
USEMODULE += stdio_uart

# This app is in RIOT/examples-miem/task14, so ../.. points to RIOT root.
RIOTBASE ?= $(CURDIR)/../..

DEVELHELP ?= 1

# Board mismatch workaround: our Discovery board clock differs from Nucleo.
CFLAGS += -DCONFIG_BOARD_HAS_HSE=0
CFLAGS += -DCONFIG_BOARD_HAS_LSE=0

# Use USART1 on PA9/PA10, connected to FTDI.
CFLAGS += -DSTDIO_UART_DEV=UART_DEV\(1\)

include $(RIOTBASE)/Makefile.include