/** firebase.h **/

#ifndef __GD_FIREBASE_H__
#define __GD_FIREBASE_H__

#include <version_generated.gen.h>

#include "utils.h"
#include "scene/main/node.h"

#if GD_FIREBASE_ANDROID_IOS
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

class GDFireBase : public Node {
#if VERSION_MAJOR == 3
    GDCLASS(GDFireBase, Reference);
#else
    OBJ_TYPE(GDFireBase, Reference);
#endif

public:
	GDFireBase();

	void initFireBase();
	void init(Dictionary p_data, int p_script_id);
	void initWithFile(String filename, int p_script_id);

#if GD_FIREBASE_ANALYTICS
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

#if GD_FIREBASE_ADMOB
	void showBannerAd(bool show);
	void showInterstitialAd();
	void showRewardedVideo();
	void requestRewardedVideoStatus();
#endif

#if GD_FIREBASE_NOTIFICATION
	void notifyInMin(String p_message, int p_mins);
	void subscribeToTopic(String p_topic);
	String getToken();
#endif

	void invite(String p_message, String p_deep_link);

#if GD_FIREBASE_REMOTECONFIG
	void getRemoteValue(String p_key);
	void setRemoteDefaults(Dictionary p_dict);
	void setRemoteDefaultsFile(String p_path);
#endif

#if GD_FIREBASE_STORAGE
	void download(String p_file, String p_path);
	void upload(String p_file, String p_path);
#endif

    void load_document(String p_col_name, String p_doc_name, int callback_id);

#if GD_FIREBASE_AUTHENTICATION
	//AUth Google
	void googleSignIn();
	void googleSignOut();
	bool isGoogleConnected();
	void getGoogleUser();
	void googleRevokeAccess();

	//Auth Twitter
	void twitterSignIn();
	void twitterSignOut();
	bool isTwitterConnected();

	//Auth Facebook
	void facebookSignIn();
	void facebookSignOut();
	bool isFacebookConnected();
	void getFacebookPermissions();
	bool facebookHasPermission();
	void revokeFacebookPermision();
	void facebookRevokeAccess();
	void askFacebookReadPermission();
	void askFacebookPublishPermission();
	void getFacebookUser();
#endif

#ifdef ANDROID_ENABLED
	void update_env(JNIEnv *p_env) {
		env = p_env;
	}
	jobject get_instance() const { return instance; }
	void set_instance(jobject p_instance) { instance = p_instance; }

	JNIEnv *env;
	static jobject instance;
#endif

protected:
	static void _bind_methods();

private:
#if GD_FIREBASE_ANDROID_IOS
	::firebase::App *app;
#endif

	Dictionary _config;
};

NS_GODOT_END

#endif // __GD_FIREBASE_H__
