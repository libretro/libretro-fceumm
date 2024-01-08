LOCAL_PATH := $(call my-dir)

CORE_DIR := $(LOCAL_PATH)/../src

HAVE_NTSC := 1

include $(LOCAL_PATH)/../Makefile.common

COREFLAGS := $(COREDEFINES) -DPSS_STYLE=1 $(INCFLAGS)

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	COREFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

include $(CLEAR_VARS)
LOCAL_MODULE    := retro
LOCAL_SRC_FILES := $(SOURCES_C)
LOCAL_CFLAGS    := $(COREFLAGS)
LOCAL_LDFLAGS   := -Wl,-version-script=$(CORE_DIR)/libretro/link.T
include $(BUILD_SHARED_LIBRARY)
