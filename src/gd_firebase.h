/** firebase.h **/

#ifndef __GD_FIREBASE_H__
#define __GD_FIREBASE_H__

#include "gd_config.h"
#include "reference.h"

#ifdef GD_FIREBASE_ANDROID_IOS
#include "firebase/analytics.h"
#include "firebase/analytics/event_names.h"
#include "firebase/analytics/parameter_names.h"
#include "firebase/analytics/user_property_names.h"
#include "firebase/app.h"

#ifdef ANDROID_ENABLED
#include "platform/android/thread_jandroid.h"
#endif
#endif

NS_GODOT_BEGINE

class GDFireBase : public Reference {
	OBJ_TYPE(GDFireBase, Reference)

public:
	GDFireBase();

	void initFireBase();
	void init(String data, int script_id);
	void initWithFile(String filename);

#if GD_FIREBASE_ANALYTICS
	void initAnalytics();
	void initRemoteConfig();

	void setScreenName(String screen_name);
	void sendAchievement(String a_id);
	void join_group(String id);
	void level_up(String character, int level);
	void post_score(String character, int level, int score);
	void content_select(String content, String item_id);
	void earn_currency(String currency_name, int value);
	void spend_currency(String item_name, String currency, int value);
	void tutorial_begin();
	void tutorial_complete();
	void send_events(String key, Dictionary data);
	void send_custom(String key, String value);
#endif

#ifdef ANDROID_ENABLED
	void update_env(JNIEnv *p_env) { env = p_env; }
	jobject get_instance() const { return instance; }
	void set_instance(jobject p_instance) { instance = p_instance; }

	JNIEnv *env;
	static jobject instance;
#endif

protected:
	static void _bind_methods();

private:
#ifdef GD_FIREBASE_ANDROID_IOS
	::firebase::App *app;
#endif
};

NS_GODOT_END

#endif // __GD_FIREBASE_H__
