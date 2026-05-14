APPLICATION = task4-morse-encode

# Real board: STM32F072B-DISCO.
# Workaround RIOT target: same MCU, different board.
BOARD ?= nucleo-f072rb

# GPIO for LED control.
USEMODULE += periph_gpio

# Timer/sleep functions.
USEMODULE += xtimer

# This app is in RIOT/examples-miem/task4-morse-encode.
RIOTBASE ?= $(CURDIR)/../..

DEVELHELP ?= 1

# Board clock workaround for STM32F072B-DISCO.
CFLAGS += -DCONFIG_BOARD_HAS_HSE=0
CFLAGS += -DCONFIG_BOARD_HAS_LSE=0

include $(RIOTBASE)/Makefile.include