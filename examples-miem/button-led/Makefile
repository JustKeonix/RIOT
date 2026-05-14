# Name of the RIOT application.
APPLICATION = button-led

# This is not our exact board.
# Our board is STM32F072B-DISCO, but this RIOT fork has no target for it.
# nucleo-f072rb is used because it has the same MCU: STM32F072RB.
BOARD ?= nucleo-f072rb

# GPIO support: needed for gpio_init(), gpio_read(), gpio_set(), gpio_clear().
USEMODULE += periph_gpio

# Timer support: needed for xtimer_msleep().
USEMODULE += xtimer

# UART stdio support: needed for puts()/printf() over serial.
USEMODULE += stdio_uart

# RIOT source tree location.
# This app is in RIOT/examples-miem/button-led, so ../.. points to RIOT/.
RIOTBASE ?= $(CURDIR)/../..

# Enable RIOT development/debug checks.
DEVELHELP ?= 1

# Board mismatch workaround:
# The nucleo-f072rb target assumes external clock sources that our Discovery board
# does not provide in the same way. Disable those assumptions.
CFLAGS += -DCONFIG_BOARD_HAS_HSE=0
CFLAGS += -DCONFIG_BOARD_HAS_LSE=0

# Board mismatch workaround:
# The nucleo-f072rb default UART is USART2 on PA2/PA3.
# On STM32F072B-DISCO, PA2/PA3 are not convenient because they are tied to board features.
# We use USART1 instead: PA9 = TX, PA10 = RX, connected to the FTDI adapter.
# Works with picocom -b 57600 /dev/ttyUSB0
CFLAGS += -DSTDIO_UART_DEV=UART_DEV\(1\)

include $(RIOTBASE)/Makefile.include