SOURCE_ROOT := $(abspath ../../..)

ASSETS_DIR = $(SOURCE_ROOT)/hobo/assets
ASSETS_OUT_DIR = $(SOURCE_ROOT)/$(OUT_DIR)/$(BUILDTYPE)/assets

include $(SOURCE_ROOT)/nanaka/build/main.mk
include $(SOURCE_ROOT)/nanaka/build/assets.mk

.PHONY: help

all:
	$(MAKE) gyp
	$(MAKE) nanaka
	$(MAKE) assets

gyp:
	gyp $(GYP_FLAGS) chuckachicken.gyp

help:
	@echo No
