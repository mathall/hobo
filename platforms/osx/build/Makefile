SOURCE_ROOT := $(abspath ../../..)
OS := osx

ASSETS_DIR = $(SOURCE_ROOT)/hobo/assets
ASSETS_OUT_DIR = $(SOURCE_ROOT)/$(OUT_DIR)/$(BUILDTYPE)/assets

.PHONY: help

include $(SOURCE_ROOT)/nanaka/build/main.mk
include $(SOURCE_ROOT)/nanaka/build/assets.mk

all:
	$(MAKE) gyp
	$(MAKE) nanaka
	$(MAKE) assets

gyp:
	$(GYP) $(GYP_FLAGS) chuckachicken.gyp

help:
	@echo No
