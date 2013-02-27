LOCAL_PATH := $(call my-dir)
BYPASS_SRC := $(LOCAL_PATH)/../../../src/
BYPASS_SORUCES := $(wildcard $(BYPASS_SRC)/*.cpp)

include $(CLEAR_VARS)

LOCAL_MODULE	:= bypass
LOCAL_SRC_FILES := \
	bypass.cpp \
	 $(subst jni/, , $(BYPASS_SORUCES)) \
	../../../dep/libsoldout/markdown.c \
	../../../dep/libsoldout/buffer.c \
	../../../dep/libsoldout/array.c
	
LOCAL_C_INCLUDES:= ../../dep/libsoldout ../../src

include $(BUILD_SHARED_LIBRARY)
