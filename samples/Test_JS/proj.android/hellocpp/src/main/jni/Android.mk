LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := CrossApp_cpp_shared

LOCAL_MODULE_FILENAME := libCrossApp_cpp

LOCAL_ARM_MODE := arm

MY_FILES_PATH := $(LOCAL_PATH) \
                 $(LOCAL_PATH)/../../../../../Classes \
                 $(LOCAL_PATH)/../../../../../../../scripting/js-bindings/manual \
				 $(LOCAL_PATH)/../../../../../../../scripting/js-bindings/auto

MY_FILES_SUFFIX := %.cpp

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

MY_ALL_FILES := $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*.*) )
MY_ALL_FILES := $(MY_ALL_FILES:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(MY_ALL_FILES))
MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%)

define uniq =
  $(eval seen :=)
  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
  ${seen}
endef

MY_ALL_DIRS := $(dir $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*/) ) )
MY_ALL_DIRS := $(call uniq,$(MY_ALL_DIRS))

LOCAL_SRC_FILES  := $(MY_SRC_LIST)
LOCAL_C_INCLUDES := $(MY_ALL_DIRS)

LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += scriptingcore-spidermonkey

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../../../../..)
$(call import-add-path, $(LOCAL_PATH)/../../../../../../../CrossApp/the_third_party/)

$(call import-module,CrossApp)
$(call import-module,CocosDenshion/android)
$(call import-module,scripting/js-bindings)
