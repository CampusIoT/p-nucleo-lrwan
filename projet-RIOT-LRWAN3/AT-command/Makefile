# Name of the application
APPLICATION = AT-command

# Default target board
BOARD ?= native

FEATURES_REQUIRED += periph_uart
FEATURES_OPTIONAL += periph_lpuart  # STM32 L0 and L4 provides lpuart support
FEATURES_OPTIONAL += periph_uart_modecfg

USEMODULE += shell
USEMODULE += xtimer

# Path to the RIOT base directory
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Include all build system logic for building the application
include $(RIOTBASE)/Makefile.include

