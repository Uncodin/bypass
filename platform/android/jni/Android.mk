LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= bypass
LOCAL_SRC_FILES := ../../../src/BypassParser.cpp ../../../deps/libsoldout/markdown.c ../../../deps/libsoldout/buffer.c ../../../deps/libsoldout/array.c
LOCAL_C_INCLUDES:= ../../deps/libsoldout

include $(BUILD_SHARED_LIBRARY)
