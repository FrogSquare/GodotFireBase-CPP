/** firebase.h **/

#include "gd_firebase.h"

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

int common_main () {
#ifdef ANDROID_ENABLED
	JavaVM *jvm;
	JNIEnv *env = ThreadAndroid::get_env();
	env->GetJavaVM(&jvm);

	JNIHelper::setJavaVM(jvm);

	jclass cls = env->FindClass("org/godotengine/godot/Godot");
	jmethodID getIns = env->GetStaticMethodID(cls,"getInstance","()Lorg/godotengine/godot/Godot;");

	if (getIns == NULL) {
		LOGI("Error::Method..!");
	} else {
		GDFireBase::instance = env->CallStaticObjectMethod(cls, getIns);
		JNIHelper::setClassLoaderFrom(GDFireBase::instance);
	}
#endif
	return 0;
}

GDFireBase::GDFireBase() {
#if GD_FIREBASE_DEBUG
	prctl(PR_SET_DUMPABLE, 1);
#endif

	common_main ();
	initFireBase();
}

void GDFireBase::initFireBase () {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
#ifdef ANDROID_ENABLED

	LOGI("Firebase:Initializing");
	this->app = ::firebase::App::Create(JNIHelper::getEnv(), JNIHelper::getActivity());
#else
	this->app = ::firebase::App::Create();
#endif
	initAnalytics ();
#endif
}

void GDFireBase::init (String data, int script_id) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)

#endif
}

#if GD_FIREBASE_ANALYTICS
void GDFireBase::initAnalytics () {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	LOGI("Initializing:Analytics");

	GDAnalytics::getInstance ()->init(this->app);
#endif
}

void GDFireBase::setScreenName (String screen_name) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->set_screen_name (screen_name.utf8 ().get_data ());
#endif
}

void GDFireBase::sendAchievement (String a_id) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->send_achievement (a_id.utf8 ().get_data ());
#endif
}

void GDFireBase::join_group (String id) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->send_group (id.utf8 ().get_data ());
#endif
}

void GDFireBase::level_up (String character, int level) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->send_level_up (character.utf8 ().get_data (), level);
#endif
}

void GDFireBase::post_score (String character, int level, int score) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->send_score (character.utf8 ().get_data (), level, score);
#endif
}

void GDFireBase::content_select (String content, String item_id) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->send_content (content.utf8 ().get_data (), item_id.utf8 ().get_data ());
#endif
}

void GDFireBase::earn_currency (String currency_name, int value) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->earn_currency (currency_name.utf8 ().get_data (), value);
#endif
}

void GDFireBase::spend_currency (String item_name, String currency, int value) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->spend_currency (
				item_name.utf8 ().get_data (), currency.utf8 ().get_data (), value);
#endif
}

void GDFireBase::tutorial_begin () {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->send_tutorial_begin ();
#endif
}

void GDFireBase::tutorial_complete () {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->send_tutorial_complete ();
#endif
}

void GDFireBase::send_events (String key, Dictionary data) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->send_events (key.utf8 ().get_data (), data);
#endif
}

void GDFireBase::send_custom (String key, String value) {
#if defined(ANDROID_ENABLED) || defined(IPHONE_ENABLED)
	GDAnalytics::getInstance ()->send_custom (key.utf8 ().get_data (), value.utf8 ().get_data ());
#endif
}

#endif

void GDFireBase::_bind_methods() {
	ObjectTypeDB::bind_method("init", &GDFireBase::init);

#if GD_FIREBASE_ANALYTICS
	ObjectTypeDB::bind_method("initAnalytics", &GDFireBase::initFireBase);
	ObjectTypeDB::bind_method("set_screen_name", &GDFireBase::setScreenName);
#endif
}

NS_GODOT_END
}

