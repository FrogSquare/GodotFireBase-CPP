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

#include "gd_analytics.h"

#if GD_FIREBASE_ANALYTICS

NS_GODOT_BEGINE

GDAnalytics *GDAnalytics::mInstance = nullptr;

GDAnalytics::GDAnalytics() {
}

#if GD_FIREBASE_ANDROID_IOS
void GDAnalytics::init(::firebase::App *app) {
	this->_app = app;
	analytics::Initialize(*app);

	LOGI("Initialized:FireBase");

	analytics::SetAnalyticsCollectionEnabled(true);
	analytics::SetMinimumSessionDuration(1000);
	analytics::SetSessionTimeoutDuration(5000);
}
#endif

GDAnalytics *GDAnalytics::getInstance() {
	if (mInstance == nullptr) {
		mInstance = memnew(GDAnalytics);
	}

	return mInstance;
}

void GDAnalytics::set_screen_name(const char *screenName) {
#if GD_FIREBASE_ANDROID_IOS
	//analytics::SetCurrentScreen(screenName, NULL);
	analytics::LogEvent("current_screen", "screen_name", screenName);

	LOGI("Setting current screen to: %s", screenName);
#endif
}

void GDAnalytics::send_achievement(const char *id) {
#if GD_FIREBASE_ANDROID_IOS
	analytics::LogEvent(analytics::kEventUnlockAchievement, analytics::kParameterAchievementID, id);

	LOGI("Sending:AchievementUnlock: %s", id);
#endif
}

void GDAnalytics::send_group(const char *group_id) {
#if GD_FIREBASE_ANDROID_IOS
	analytics::LogEvent(analytics::kEventJoinGroup, analytics::kParameterGroupID, group_id);

	LOGI("Sending:JoinGroup: %s", group_id);
#endif
}

void GDAnalytics::send_level_up(const char *character, int level) {
#if GD_FIREBASE_ANDROID_IOS
	analytics::Parameter parameters[] = {
		analytics::Parameter(analytics::kParameterCharacter, character),
		analytics::Parameter(analytics::kParameterLevel, level),
	};

	analytics::LogEvent(analytics::kEventLevelUp, parameters, sizeof(parameters) / sizeof(parameters[0]));

	LOGI("Sending:Character:LevelUP:%s:%d", character, level);
#endif
}

void GDAnalytics::send_score(const char *character, int level, int score) {
#if GD_FIREBASE_ANDROID_IOS
	analytics::Parameter parameters[] = {
		analytics::Parameter(analytics::kParameterCharacter, character),
		analytics::Parameter(analytics::kParameterLevel, level),
		analytics::Parameter(analytics::kParameterScore, score),
	};

	analytics::LogEvent(analytics::kEventPostScore, parameters, sizeof(parameters) / sizeof(parameters[0]));

	LOGI("Sending:Level:Score:%s:%d:%d", character, level, score);
#endif
}

void GDAnalytics::send_content(const char *content_type, const char *item_id) {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDAnalytics::send_share() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDAnalytics::earn_currency(const char *currency_name, int value) {
#if GD_FIREBASE_ANDROID_IOS
	analytics::Parameter parameters[] = {
		analytics::Parameter(analytics::kParameterVirtualCurrencyName, currency_name),
		analytics::Parameter(analytics::kParameterValue, value),
	};

	analytics::LogEvent(analytics::kEventEarnVirtualCurrency, parameters, sizeof(parameters) / sizeof(parameters[0]));

	LOGI("Sending:Currency:Earned:%s:%d", currency_name, value);
#endif
}

void GDAnalytics::spend_currency(const char *item_name, const char *currency_name, int value) {
#if GD_FIREBASE_ANDROID_IOS
	analytics::Parameter parameters[] = {
		analytics::Parameter(analytics::kParameterItemName, item_name),
		analytics::Parameter(analytics::kParameterVirtualCurrencyName, currency_name),
		analytics::Parameter(analytics::kParameterValue, value),
	};

	analytics::LogEvent(analytics::kEventSpendVirtualCurrency, parameters, sizeof(parameters) / sizeof(parameters[0]));

	LOGI("Sending:Currency:Spent:%s:%s:%d", item_name, currency_name, value);
#endif
}

void GDAnalytics::send_tutorial_begin() {
#if GD_FIREBASE_ANDROID_IOS
	analytics::LogEvent(analytics::kEventTutorialBegin);

	LOGI("Sending:Tutorial:Begin");
#endif
}

void GDAnalytics::send_tutorial_complete() {
#if GD_FIREBASE_ANDROID_IOS
	analytics::LogEvent(analytics::kEventTutorialComplete);

	LOGI("Sending:Tutorial:Complete");
#endif
}

void GDAnalytics::send_events(const char *eventName, Dictionary keyValues) {
	/** TODO **/
}

void GDAnalytics::send_custom(const char *key, const char *value) {
#if GD_FIREBASE_ANDROID_IOS
	analytics::LogEvent("appEvent", key, value);

	LOGI("Sending:App:Event: {%s:%s}", key, value);
#endif
}

NS_GODOT_END
#endif
