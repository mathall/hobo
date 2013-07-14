SOURCE_ROOT := $(abspath ../../..)
OS := android
TARGET_ARCH ?= arm
ARM_VERSION ?= 7
USE_CLANG ?= 0

ANDROID_ROOT := $(SOURCE_ROOT)/platforms/android

ASSETS_DIR = $(SOURCE_ROOT)/hobo/assets
ASSETS_OUT_DIR = $(ANDROID_ROOT)/assets

include $(SOURCE_ROOT)/nanaka/build/main.mk
include $(SOURCE_ROOT)/nanaka/build/assets.mk

GYP_FLAGS += -Dandroid_ndk_root=$(ANDROID_NDK)

NDK_BUILD_FLAGS := APP_STL=gnustl_shared
NDK_BUILD_FLAGS += NDK_TOOLCHAIN_VERSION=4.7
NDK_BUILD_FLAGS += NANAKA_LIB_DIR=../../../../$(OUT_DIR)/$(BUILDTYPE)
NDK_BUILD_FLAGS += GAME_LIB=../../../../$(OUT_DIR)/$(BUILDTYPE)/libhobo.a
ifeq ($(DEBUG), 1)
NDK_LOCAL_CPPFLAGS := "-std=c++11 -O0 -g"
NDK_LOCAL_LDFLAGS := ""
ANT_TARGET := debug
else
NDK_LOCAL_CPPFLAGS := "-std=c++11 -O3"
NDK_LOCAL_LDFLAGS := "-Wl,-s"
ANT_TARGET := release
endif
NDK_BUILD_FLAGS += LOCAL_CPPFLAGS=$(NDK_LOCAL_CPPFLAGS)
NDK_BUILD_FLAGS += LOCAL_LDFLAGS=$(NDK_LOCAL_LDFLAGS)
ifeq ($(TARGET_ARCH), x86)
NDK_BUILD_FLAGS += APP_ABI=x86
else
ifeq ($(ARM_VERSION), 7)
NDK_BUILD_FLAGS += APP_ABI=armeabi-v7a
else
NDK_BUILD_FLAGS += APP_ABI=armeabi
endif
endif

.PHONY: help

all:
	$(MAKE) gyp
	$(MAKE) nanaka
	$(MAKE) assets
	$(MAKE) android

gyp:
	gyp $(GYP_FLAGS) android.gyp

android:
	cd $(ANDROID_ROOT) ; ndk-build $(NDK_BUILD_FLAGS) && ant $(ANT_TARGET)

help::
	@echo No
