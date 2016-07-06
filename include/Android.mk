LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= xduilib
LOCAL_SRC_FILES:= test.cpp
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/test.hpp
LOCAL_EXPORT_LDLIBS := -llog

include $(BUILD_STATIC_LIBRARY)
