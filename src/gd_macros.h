
#ifndef __GD_FIREBASE_MACROS_H__
#define __GD_FIREBASE_MACROS_H__

#include "print_string.h"

#include <stdarg.h>
#include <stdio.h>

#define NS_GODOT_BEGINE namespace godot {
#define NS_GODOT_END \
	}                \
	;

#define USING_NS_GODOT using namespace godot;

#define GD_FIREBASE_ANDROID_IOS 1

namespace godot {
const char *format_str(size_t c_size, const char *c_fmt, ...);
}

#ifdef ANDROID_ENABLED
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "godot", __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "godot", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "godot", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "godot", __VA_ARGS__))
#else
#define LOGI(string, ...) print_line(godot::format_str(string));
#endif

#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
#undef GD_FIREBASE_ANDROID_IOS
#define GD_FIREBASE_ANDROID_IOS 1
#else
#undef GD_FIREBASE_ANDROID_IOS
#define GD_FIREBASE_ANDROID_IOS 0
#endif

#endif // __GD_FIREBASE_MACROS_H__
