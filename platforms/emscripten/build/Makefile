# GYP is seemingly not intended to target emscripten, so here's some hacks!
#
# * The build script generated for library target types doesn't work with the
# emscripten toolchain, so we can't build libraries. Instead we have to gather
# up the object files and link final output manually using line at the bottom of
# this file.
#
# * For the above reason, library target types are set to "none", and GYP
# doesn't seem willing to generate make files for us in this case. Just stick
# with ninja instead which was the default anyway.

SOURCE_ROOT := $(abspath ../../..)
OS := web
TARGET_ARCH := js
USE_NINJA := 1

ASSETS_DIR = $(SOURCE_ROOT)/hobo/assets
ASSETS_OUT_DIR = $(SOURCE_ROOT)/$(OUT_DIR)/$(BUILDTYPE)/assets

include $(SOURCE_ROOT)/nanaka/build/main.mk
include $(SOURCE_ROOT)/nanaka/build/assets.mk

GYP_FLAGS += -Demscripten_sdk_root=$(EMSCRIPTEN_SDK)

.PHONY: help

all:
	$(MAKE) gyp
	$(MAKE) nanaka
	$(MAKE) assets

gyp:
	gyp $(GYP_FLAGS) chuckachicken.gyp

help:
	@echo No

# Link final target:
#
# in out_web_js/Release:
# $EMSCRIPTEN_SDK/em++ $(find . -name "*.o") $(for x in $(find assets -type f); do echo --preload-file $x; done) -s TOTAL_MEMORY=33554432 -o chuckachicken.html -s ASM_JS=1 -O2
#
# in out_web_js/Debug:
# $EMSCRIPTEN_SDK/em++ $(find . -name "*.o") $(for x in $(find assets -type f); do echo --preload-file $x; done) -s TOTAL_MEMORY=33554432 -o chuckachicken.html
