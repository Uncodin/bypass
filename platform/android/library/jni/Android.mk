LOCAL_PATH := $(call my-dir)
BYPASS_SRC := $(LOCAL_PATH)/../../../../src/
BYPASS_SOURCES := $(wildcard $(BYPASS_SRC)/*.cpp)

include $(CLEAR_VARS)

LOCAL_MODULE	:= bypass
LOCAL_SRC_FILES := \
	 $(subst jni/, , $(BYPASS_SOURCES)) \
	bypass.cpp \
	../../../../dep/libsoldout/markdown.c \
	../../../../dep/libsoldout/buffer.c \
	../../../../dep/libsoldout/array.c
	
LOCAL_C_INCLUDES:= ../../../dep/libsoldout ../../../src /opt/local/include /usr/local/include $(BYPASS_INCLUDE_PATH)

include $(BUILD_SHARED_LIBRARY)
