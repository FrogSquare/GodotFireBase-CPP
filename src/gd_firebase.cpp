/** firebase.h **/

#include "gd_firebase.h"
#include "gd_analytics.h"
#include "gd_remote_config.h"

#ifdef ANDROID_ENABLED
#ifdef GD_FIREBASE_DEBUG
#include <sys/prctl.h>
#endif

#include "jnihelper.h"
#endif

extern "C" {
NS_GODOT_BEGINE

#ifdef ANDROID_ENABLED
jobject GDFireBase::instance;
#endif

GDFireBase::GDFireBase() {
#if GD_FIREBASE_DEBUG
	prctl(PR_SET_DUMPABLE, 1);
#endif

	initFireBase();
}

void GDFireBase::initFireBase() {
#ifdef GD_FIREBASE_ANDROID_IOS
#ifdef ANDROID_ENABLED
	LOGI("Firebase:Initializing");
	this->app = ::firebase::App::Create(JNIHelper::getEnv(), JNIHelper::getActivity());
#else
	this->app = ::firebase::App::Create();
#endif
	initAnalytics();
#endif
}

void GDFireBase::init(String data, int script_id) {
#ifdef GD_FIREBASE_ANDROID_IOS

#endif
}

#if GD_FIREBASE_ANALYTICS
void GDFireBase::initAnalytics() {
#ifdef GD_FIREBASE_ANDROID_IOS
	LOGI("Initializing:Analytics");

	GDAnalytics::getInstance()->init(this->app);
#endif
}

#if GD_FIREBASE_REMOTECONFIG
void GDFireBase::initRemoteConfig() {
	LOGI("Initializing:RemoteConfig");

	GDRemoteConfig::getInstance()->init(this->app);
}

#endif

void GDFireBase::setScreenName(String screen_name) {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->set_screen_name(screen_name.utf8().get_data());
#endif
}

void GDFireBase::sendAchievement(String a_id) {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_achievement(a_id.utf8().get_data());
#endif
}

void GDFireBase::join_group(String id) {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_group(id.utf8().get_data());
#endif
}

void GDFireBase::level_up(String character, int level) {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_level_up(character.utf8().get_data(), level);
#endif
}

void GDFireBase::post_score(String character, int level, int score) {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_score(character.utf8().get_data(), level, score);
#endif
}

void GDFireBase::content_select(String content, String item_id) {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_content(content.utf8().get_data(), item_id.utf8().get_data());
#endif
}

void GDFireBase::earn_currency(String currency_name, int value) {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->earn_currency(currency_name.utf8().get_data(), value);
#endif
}

void GDFireBase::spend_currency(String item_name, String currency, int value) {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->spend_currency(
			item_name.utf8().get_data(), currency.utf8().get_data(), value);
#endif
}

void GDFireBase::tutorial_begin() {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_tutorial_begin();
#endif
}

void GDFireBase::tutorial_complete() {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_tutorial_complete();
#endif
}

void GDFireBase::send_events(String key, Dictionary data) {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_events(key.utf8().get_data(), data);
#endif
}

void GDFireBase::send_custom(String key, String value) {
#ifdef GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_custom(key.utf8().get_data(), value.utf8().get_data());
#endif
}

#endif

void GDFireBase::_bind_methods() {
	ObjectTypeDB::bind_method("init", &GDFireBase::init);

#if GD_FIREBASE_ANALYTICS
	ObjectTypeDB::bind_method("initAnalytics", &GDFireBase::initFireBase);
	ObjectTypeDB::bind_method("set_screen_name", &GDFireBase::setScreenName);
	ObjectTypeDB::bind_method("join_group", &GDFireBase::join_group);

//	ObjectTypeDB::bind_method("getValue", );
#endif
}

NS_GODOT_END
}
