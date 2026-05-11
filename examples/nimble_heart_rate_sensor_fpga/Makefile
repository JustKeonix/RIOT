# name of your application
APPLICATION = nimble_heart_rate_sensor

# If no BOARD is found in the environment, use this default:
CONTINUE_ON_EXPECTED_ERRORS=1
BOARD = nucleo-f401re
USEMODULE += ztimer_usec

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Some RIOT modules needed for this example
USEMODULE += event_timeout_ztimer

# This is replaced with simple source files now
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-unused-const-variable

# Include NimBLE
USEPKG += nimble
USEMODULE += nimble_svc_gap
USEMODULE += nimble_svc_gatt

# Use automated advertising
USEMODULE += nimble_autoadv
CFLAGS += -DCONFIG_NIMBLE_AUTOADV_DEVICE_NAME='"RIOT Heart Rate Sensor"'
CFLAGS += -DCONFIG_NIMBLE_AUTOADV_START_MANUALLY=1

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
