LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libretro
FCEU_SRC_DIRS := ../../../../src ../../../../src/boards ../../../../src/input ../../../../src/mappers

FCEU_SRC_FILES := $(wildcard $(LOCAL_PATH)/../../../../src/*.c) \
		$(wildcard $(LOCAL_PATH)/../../../../src/boards/*.c) \
		$(wildcard $(LOCAL_PATH)/../../../../src/input/*.c) \
		$(wildcard $(LOCAL_PATH)/../../../../src/mappers/*.c)

FCEU_SRC_FILES := $(FCEU_SRC_FILES:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES  = ../libretro/libretro.c \
		../libretro/memstream.c \
		$(FCEU_SRC_FILES)

LOCAL_CFLAGS = -DINLINE=inline -DSOUND_QUALITY=0 -DPATH_MAX=1024 -DPSS_STYLE=1 -DLSB_FIRST -D__LIBRETRO__ -DHAVE_ASPRINTF

include $(BUILD_SHARED_LIBRARY)
