LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE  := SWF_static

LOCAL_MODULE_FILENAME := libSWF

LOCAL_SRC_FILES := base/component_hackery.cpp \
base/configvars.cpp \
base/container.cpp \
base/ear_clip_triangulate_float.cpp \
base/ear_clip_triangulate_sint16.cpp \
base/file_util.cpp \
base/image.cpp \
base/image_filters.cpp \
base/jpeg.cpp \
base/logger.cpp \
base/membuf.cpp \
base/triangulate_float.cpp \
base/triangulate_sint32.cpp \
base/tu_file.cpp \
base/tu_file_SDL.cpp \
base/tu_gc_singlethreaded_marksweep.cpp \
base/tu_gc_test.cpp \
base/tu_loadlib.cpp \
base/tu_random.cpp \
base/tu_timer.cpp \
base/tu_types.cpp \
base/utf8.cpp \
base/utility.cpp \
base/zlib_adapter.cpp \
gameswf/gameswf_abc.cpp \
gameswf/gameswf_action.cpp \
gameswf/gameswf_as_classes/as_array.cpp \
gameswf/gameswf_as_classes/as_boolean.cpp \
gameswf/gameswf_as_classes/as_broadcaster.cpp \
gameswf/gameswf_as_classes/as_class.cpp \
gameswf/gameswf_as_classes/as_color.cpp \
gameswf/gameswf_as_classes/as_color_transform.cpp \
gameswf/gameswf_as_classes/as_date.cpp \
gameswf/gameswf_as_classes/as_event.cpp \
gameswf/gameswf_as_classes/as_flash.cpp \
gameswf/gameswf_as_classes/as_geom.cpp \
gameswf/gameswf_as_classes/as_global.cpp \
gameswf/gameswf_as_classes/as_key.cpp \
gameswf/gameswf_as_classes/as_loadvars.cpp \
gameswf/gameswf_as_classes/as_math.cpp \
gameswf/gameswf_as_classes/as_matrix.cpp \
gameswf/gameswf_as_classes/as_mcloader.cpp \
gameswf/gameswf_as_classes/as_mouse.cpp \
gameswf/gameswf_as_classes/as_mouse_event.cpp \
gameswf/gameswf_as_classes/as_netconnection.cpp \
gameswf/gameswf_as_classes/as_netstream.cpp \
gameswf/gameswf_as_classes/as_number.cpp \
gameswf/gameswf_as_classes/as_point.cpp \
gameswf/gameswf_as_classes/as_selection.cpp \
gameswf/gameswf_as_classes/as_sharedobject.cpp \
gameswf/gameswf_as_classes/as_sound.cpp \
gameswf/gameswf_as_classes/as_string.cpp \
gameswf/gameswf_as_classes/as_textformat.cpp \
gameswf/gameswf_as_classes/as_transform.cpp \
gameswf/gameswf_as_classes/as_xmlsocket.cpp \
gameswf/gameswf_as_sprite.cpp \
gameswf/gameswf_avm2.cpp \
gameswf/gameswf_avm2_jit.cpp \
gameswf/gameswf_button.cpp \
gameswf/gameswf_canvas.cpp \
gameswf/gameswf_character.cpp \
gameswf/gameswf_disasm.cpp \
gameswf/gameswf_dlist.cpp \
gameswf/gameswf_environment.cpp \
gameswf/gameswf_filters.cpp \
gameswf/gameswf_font.cpp \
gameswf/gameswf_fontlib.cpp \
gameswf/gameswf_freetype.cpp \
gameswf/gameswf_function.cpp \
gameswf/gameswf_impl.cpp \
gameswf/gameswf_jit.cpp \
gameswf/gameswf_jit_opcode.cpp \
gameswf/gameswf_listener.cpp \
gameswf/gameswf_log.cpp \
gameswf/gameswf_morph2.cpp \
gameswf/gameswf_movie_def.cpp \
gameswf/gameswf_mutex.cpp \
gameswf/gameswf_object.cpp \
gameswf/gameswf_player.cpp \
gameswf/gameswf_render.cpp \
gameswf/gameswf_render_handler_ogles.cpp \
gameswf/gameswf_root.cpp \
gameswf/gameswf_shape.cpp \
gameswf/gameswf_sound.cpp \
gameswf/gameswf_sprite.cpp \
gameswf/gameswf_sprite_def.cpp \
gameswf/gameswf_stream.cpp \
gameswf/gameswf_styles.cpp \
gameswf/gameswf_tesselate.cpp \
gameswf/gameswf_text.cpp \
gameswf/gameswf_types.cpp \
gameswf/gameswf_value.cpp \
gameswf/gameswf_video_impl.cpp \



LOCAL_EXPORT_C_INCLUDES :=  $(LOCAL_PATH) \
                            $(LOCAL_PATH)/base \
                            $(LOCAL_PATH)/gameswf \
                            $(LOCAL_PATH)/gameswf/gameswf_as_classes\
                            $(LOCAL_PATH)/../platform/android

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/base \
                    $(LOCAL_PATH)/gameswf \
                    $(LOCAL_PATH)/gameswf/gameswf_as_classes\
                    $(LOCAL_PATH)/../platform/android

LOCAL_WHOLE_STATIC_LIBRARIES += cocos_jpeg_static
LOCAL_WHOLE_STATIC_LIBRARIES += CrossApp_static


#LOCAL_CFLAGS += -Wno-psabi
#LOCAL_EXPORT_CFLAGS += -Wno-psabi

include $(BUILD_STATIC_LIBRARY)




