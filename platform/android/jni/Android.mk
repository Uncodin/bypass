LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= bypass
LOCAL_SRC_FILES := $(wildcard ../../../src/*.cpp) ../../../dep/libsoldout/markdown.c ../../../dep/libsoldout/buffer.c ../../../dep/libsoldout/array.c
LOCAL_C_INCLUDES:= ../../dep/libsoldout ../../src

include $(BUILD_SHARED_LIBRARY)
