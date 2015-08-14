LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := CrossApp_extension_static

LOCAL_MODULE_FILENAME := libextension

LOCAL_SRC_FILES := network/HttpClient.cpp \
network/DownloadManager.cpp \
network/WebSocket.cpp \
LocalStorage/LocalStorageAndroid.cpp \
Json/DictionaryHelper.cpp \
Json/CSContentJsonDictionary.cpp \
Json/lib_json/json_value.cpp \
Json/lib_json/json_reader.cpp \
Json/lib_json/json_writer.cpp \
sqlite3/include/sqlite3.c \
device/Device_android/CADevice.cpp \
studio/CAStudioViewParser.cpp \
gameswf/base/component_hackery.cpp \
gameswf/base/configvars.cpp \
gameswf/base/container.cpp \
gameswf/base/ear_clip_triangulate_float.cpp \
gameswf/base/ear_clip_triangulate_sint16.cpp \
gameswf/base/file_util.cpp \
gameswf/base/image.cpp \
gameswf/base/image_filters.cpp \
gameswf/base/jpeg.cpp \
gameswf/base/logger.cpp \
gameswf/base/membuf.cpp \
gameswf/base/triangulate_float.cpp \
gameswf/base/triangulate_sint32.cpp \
gameswf/base/tu_file.cpp \
gameswf/base/tu_file_SDL.cpp \
gameswf/base/tu_gc_singlethreaded_marksweep.cpp \
gameswf/base/tu_gc_test.cpp \
gameswf/base/tu_loadlib.cpp \
gameswf/base/tu_random.cpp \
gameswf/base/tu_timer.cpp \
gameswf/base/tu_types.cpp \
gameswf/base/utf8.cpp \
gameswf/base/utility.cpp \
gameswf/base/zlib_adapter.cpp \
gameswf/gameswf/gameswf_abc.cpp \
gameswf/gameswf/gameswf_action.cpp \
gameswf/gameswf/gameswf_as_classes/as_array.cpp \
gameswf/gameswf/gameswf_as_classes/as_boolean.cpp \
gameswf/gameswf/gameswf_as_classes/as_broadcaster.cpp \
gameswf/gameswf/gameswf_as_classes/as_class.cpp \
gameswf/gameswf/gameswf_as_classes/as_color.cpp \
gameswf/gameswf/gameswf_as_classes/as_color_transform.cpp \
gameswf/gameswf/gameswf_as_classes/as_date.cpp \
gameswf/gameswf/gameswf_as_classes/as_event.cpp \
gameswf/gameswf/gameswf_as_classes/as_flash.cpp \
gameswf/gameswf/gameswf_as_classes/as_geom.cpp \
gameswf/gameswf/gameswf_as_classes/as_global.cpp \
gameswf/gameswf/gameswf_as_classes/as_key.cpp \
gameswf/gameswf/gameswf_as_classes/as_loadvars.cpp \
gameswf/gameswf/gameswf_as_classes/as_math.cpp \
gameswf/gameswf/gameswf_as_classes/as_matrix.cpp \
gameswf/gameswf/gameswf_as_classes/as_mcloader.cpp \
gameswf/gameswf/gameswf_as_classes/as_mouse.cpp \
gameswf/gameswf/gameswf_as_classes/as_mouse_event.cpp \
gameswf/gameswf/gameswf_as_classes/as_netconnection.cpp \
gameswf/gameswf/gameswf_as_classes/as_netstream.cpp \
gameswf/gameswf/gameswf_as_classes/as_number.cpp \
gameswf/gameswf/gameswf_as_classes/as_point.cpp \
gameswf/gameswf/gameswf_as_classes/as_selection.cpp \
gameswf/gameswf/gameswf_as_classes/as_sharedobject.cpp \
gameswf/gameswf/gameswf_as_classes/as_sound.cpp \
gameswf/gameswf/gameswf_as_classes/as_string.cpp \
gameswf/gameswf/gameswf_as_classes/as_textformat.cpp \
gameswf/gameswf/gameswf_as_classes/as_transform.cpp \
gameswf/gameswf/gameswf_as_classes/as_xmlsocket.cpp \
gameswf/gameswf/gameswf_as_sprite.cpp \
gameswf/gameswf/gameswf_avm2.cpp \
gameswf/gameswf/gameswf_avm2_jit.cpp \
gameswf/gameswf/gameswf_button.cpp \
gameswf/gameswf/gameswf_canvas.cpp \
gameswf/gameswf/gameswf_character.cpp \
gameswf/gameswf/gameswf_disasm.cpp \
gameswf/gameswf/gameswf_dlist.cpp \
gameswf/gameswf/gameswf_environment.cpp \
gameswf/gameswf/gameswf_filters.cpp \
gameswf/gameswf/gameswf_font.cpp \
gameswf/gameswf/gameswf_fontlib.cpp \
gameswf/gameswf/gameswf_freetype.cpp \
gameswf/gameswf/gameswf_function.cpp \
gameswf/gameswf/gameswf_impl.cpp \
gameswf/gameswf/gameswf_jit.cpp \
gameswf/gameswf/gameswf_jit_opcode.cpp \
gameswf/gameswf/gameswf_listener.cpp \
gameswf/gameswf/gameswf_log.cpp \
gameswf/gameswf/gameswf_morph2.cpp \
gameswf/gameswf/gameswf_movie_def.cpp \
gameswf/gameswf/gameswf_mutex.cpp \
gameswf/gameswf/gameswf_object.cpp \
gameswf/gameswf/gameswf_player.cpp \
gameswf/gameswf/gameswf_render.cpp \
gameswf/gameswf/gameswf_render_handler_ogles.cpp \
gameswf/gameswf/gameswf_root.cpp \
gameswf/gameswf/gameswf_shape.cpp \
gameswf/gameswf/gameswf_sound.cpp \
gameswf/gameswf/gameswf_sprite.cpp \
gameswf/gameswf/gameswf_sprite_def.cpp \
gameswf/gameswf/gameswf_stream.cpp \
gameswf/gameswf/gameswf_styles.cpp \
gameswf/gameswf/gameswf_tesselate.cpp \
gameswf/gameswf/gameswf_text.cpp \
gameswf/gameswf/gameswf_types.cpp \
gameswf/gameswf/gameswf_value.cpp \
gameswf/gameswf/gameswf_video_impl.cpp \
gameswf/CAFlash.cpp \
gameswf/CAFlashView.cpp \

LOCAL_WHOLE_STATIC_LIBRARIES := CrossApp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
LOCAL_WHOLE_STATIC_LIBRARIES += libwebsockets_static

LOCAL_CFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1
LOCAL_EXPORT_CFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1
LOCAL_CPPFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1
LOCAL_EXPORT_CPPFLAGS += -DCC_ENABLE_CHIPMUNK_INTEGRATION=1

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
                           $(LOCAL_PATH)/network \
                           $(LOCAL_PATH)/LocalStorage \
						   $(LOCAL_PATH)/Json \
						   $(LOCAL_PATH)/Json/lib_json \
                           $(LOCAL_PATH)/GUI \
                           $(LOCAL_PATH)/gameswf \
                           $(LOCAL_PATH)/gameswf/base \
                           $(LOCAL_PATH)/gameswf/gameswf/gameswf \
                           $(LOCAL_PATH)/gameswf/gameswf/gameswf/gameswf_as_classes



LOCAL_CFLAGS := -fexceptions

include $(BUILD_STATIC_LIBRARY)

$(call import-module,CrossApp)
$(call import-module,CocosDenshion/android)
$(call import-module,CrossApp/the_third_party/curl/prebuilt/android)
$(call import-module,extensions/libwebsockets/android)
