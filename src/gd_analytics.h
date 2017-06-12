/**
 * Copyright 2017 RameshRavone. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#ifndef GD_ANALYTICS_H
#define GD_ANALYTICS_H

#include "reference.h"
#include "gd_config.h"

#if GD_FIREBASE_ANALYTICS

#if (defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED))
#include "firebase/analytics.h"
#include "firebase/analytics/event_names.h"
#include "firebase/analytics/parameter_names.h"
#include "firebase/analytics/user_property_names.h"
#include "firebase/app.h"

#ifdef ANDROID_ENABLED
#include "platform/android/thread_jandroid.h"
#endif

namespace analytics = ::firebase::analytics;
#endif

NS_GODOT_BEGINE

class GDAnalytics : public Reference {
public:
	GDAnalytics();

#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	void init(::firebase::App* app);
#endif

	void set_screen_name(const char* screenName);
	void send_achievement(const char* id);
	void send_group(const char* group_id);
	void send_level_up(const char* character, int level);
	void send_score(const char* character, int level, int score);
	void send_content(const char* content_type, const char* item_id);

	void send_share();

	void earn_currency(const char* currency_name, int value);
	void spend_currency(const char* item_name, const char* currency_name, int value);

	void send_tutorial_begin();
	void send_tutorial_complete();

	void send_events(const char* eventName, Dictionary keyValues);
	void send_custom(const char* key, const char* value);

	static GDAnalytics *getInstance();

private:
	static GDAnalytics *mInstance;

#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	::firebase::App* _app;

	static jobject _activity;
#endif
};

NS_GODOT_END

#endif
#endif // GD_ANALYTICS_H
