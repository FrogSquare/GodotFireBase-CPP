/** firebase.h **/

#include "utils.h"

#include "gd_admob.h"
#include "gd_analytics.h"
#include "gd_firebase.h"
#include "gd_firestore.h"
#include "gd_invite.h"
#include "gd_notification.h"
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
#if GD_FIREBASE_ANDROID_IOS
#if GD_FIREBASE_DEBUG
	prctl(PR_SET_DUMPABLE, 1);
#endif
#endif
}

void GDFireBase::initFireBase() {
#if GD_FIREBASE_ANDROID_IOS
#ifdef ANDROID_ENABLED
	LOGI("Firebase:Initializing");
	this->app = ::firebase::App::Create(JNIHelper::getEnv(), JNIHelper::getActivity());
	LOGI("Firebase:App:Created");
#else
	this->app = ::firebase::App::Create();
#endif

	GDAnalytics::getInstance()->init(this->app);

    if (_config["AdMob"]) {
    	GDAdMob::getInstance()->init(this->app, _config["Ads"]);
    }
    if (_config["Authentication"]) {
        //GDAuthentication::getInstance()->init(this->app, _config["AuthConf"]);
    }
    if (_config["Invites"]) {
#if GD_FIREBASE_INVITES
        GDInvite::getInstance()->init(this->app);
#endif
    }
    if (_config["RemoteConfig"]) {
    	GDRemoteConfig::getInstance()->init(this->app);
    }
    if (_config["Notification"]) {
    	GDNotification::getInstance()->init(this->app);
    }
    if (_config["Storage"]) {
        //GDStorage::getInstance()->init(this->app);
    }
    if (_config["Firestore"]) {
        add_child(GDFirestore::getInstance());

        GDFirestore::getInstance()->init(this->app);
    }

#endif
}

void GDFireBase::initWithFile(String p_filename, int p_script_id) {
#if GD_FIREBASE_ANDROID_IOS

	Error err = Utils::parse_file_dict(p_filename, _config);
	if (err != OK) {
		ERR_FAIL_COND(err != OK);
	}

	initFireBase();

	Utils::set_script_id(p_script_id);
#endif
}

void GDFireBase::init(Dictionary p_data, int p_script_id) {
#if GD_FIREBASE_ANDROID_IOS
	_config = p_data;

	initFireBase();

	Utils::set_script_id(p_script_id);
#endif
}

#if GD_FIREBASE_ANALYTICS
void GDFireBase::setScreenName(String screen_name) {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->set_screen_name(screen_name.utf8().get_data());
#endif
}

void GDFireBase::sendAchievement(String a_id) {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_achievement(a_id.utf8().get_data());
#endif
}

void GDFireBase::join_group(String id) {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_group(id.utf8().get_data());
#endif
}

void GDFireBase::level_up(String character, int level) {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_level_up(character.utf8().get_data(), level);
#endif
}

void GDFireBase::post_score(String character, int level, int score) {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_score(character.utf8().get_data(), level, score);
#endif
}

void GDFireBase::content_select(String content, String item_id) {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_content(content.utf8().get_data(), item_id.utf8().get_data());
#endif
}

void GDFireBase::earn_currency(String currency_name, int value) {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->earn_currency(currency_name.utf8().get_data(), value);
#endif
}

void GDFireBase::spend_currency(String item_name, String currency, int value) {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->spend_currency(
			item_name.utf8().get_data(), currency.utf8().get_data(), value);
#endif
}

void GDFireBase::tutorial_begin() {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_tutorial_begin();
#endif
}

void GDFireBase::tutorial_complete() {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_tutorial_complete();
#endif
}

void GDFireBase::send_events(String key, Dictionary data) {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_events(key.utf8().get_data(), data);
#endif
}

void GDFireBase::send_custom(String key, String value) {
#if GD_FIREBASE_ANDROID_IOS
	GDAnalytics::getInstance()->send_custom(key.utf8().get_data(), value.utf8().get_data());
#endif
}
#endif

#if GD_FIREBASE_ADMOB
void GDFireBase::showBannerAd(bool show) {
#if GD_FIREBASE_ANDROID_IOS
	GDAdMob::getInstance()->showBannedAd(show);
#endif
}

void GDFireBase::showInterstitialAd() {
#if GD_FIREBASE_ANDROID_IOS
	GDAdMob::getInstance()->showInterstitialAd();
#endif
}

void GDFireBase::showRewardedVideo() {
#if GD_FIREBASE_ANDROID_IOS
	GDAdMob::getInstance()->showRewardedVideo();
#endif
}

void GDFireBase::requestRewardedVideoStatus() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}
#endif

#if GD_FIREBASE_NOTIFICATION
void GDFireBase::notifyInMin(String p_message, int p_mins) {
#if GD_FIREBASE_ANDROID_IOS
	GDNotification::getInstance()->notifyInMin(p_message, p_mins);
#endif
}

void GDFireBase::subscribeToTopic(String p_topic) {
#if GD_FIREBASE_ANDROID_IOS
	GDNotification::getInstance()->subscribeToTopic(p_topic);
#endif
}

String GDFireBase::getToken() {
#if GD_FIREBASE_ANDROID_IOS
	return GDNotification::getInstance()->getToken();
#endif
	return "";
}
#endif

void GDFireBase::invite(String p_message, String p_deep_link) {
#if GD_FIREBASE_ANDROID_IOS && GD_FIREBASE_INVITES
	GDInvite::getInstance()->invite(p_message);
#endif
}

#if GD_FIREBASE_REMOTECONFIG
void GDFireBase::getRemoteValue(String p_key) {
#if GD_FIREBASE_ANDROID_IOS
	GDRemoteConfig::getInstance()->getValue(p_key);
#endif
}

void GDFireBase::setRemoteDefaults(Dictionary p_dict) {
#if GD_FIREBASE_ANDROID_IOS
	GDRemoteConfig::getInstance()->setDefaults(p_dict);
#endif
}

void GDFireBase::setRemoteDefaultsFile(String p_path) {
#if GD_FIREBASE_ANDROID_IOS
	GDRemoteConfig::getInstance()->setDefaultsFile(p_path);
#endif
}
#endif

#if GD_FIREBASE_STORAGE
void GDFireBase::download(String p_file, String p_path) {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDFireBase::upload(String p_file, String p_path) {
#if GD_FIREBASE_ANDROID_IOS
#endif
}
#endif

#if GD_FIREBASE_AUTHENTICATION

#if GD_AUTH_GOOGLE
void GDFireBase::googleSignIn() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDFireBase::googleSignOut() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

bool GDFireBase::isGoogleConnected() {
#if GD_FIREBASE_ANDROID_IOS
#endif
	return false;
}

void GDFireBase::getGoogleUser() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDFireBase::googleRevokeAccess() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}
#endif

#if GD_AUTH_TWITTER
void GDFireBase::twitterSignIn() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDFireBase::twitterSignOut() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

bool GDFireBase::isTwitterConnected() {
#if GD_FIREBASE_ANDROID_IOS
#endif
	return false;
}
#endif

#if GD_AUTH_FACEBOOK
void GDFireBase::facebookSignIn() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDFireBase::facebookSignOut() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

bool GDFireBase::isFacebookConnected() {
#if GD_FIREBASE_ANDROID_IOS
#endif
	return false;
}

void GDFireBase::getFacebookPermissions() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

bool GDFireBase::facebookHasPermission() {
#if GD_FIREBASE_ANDROID_IOS
#endif
	return false;
}

void GDFireBase::revokeFacebookPermision() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDFireBase::facebookRevokeAccess() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDFireBase::askFacebookReadPermission() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDFireBase::askFacebookPublishPermission() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDFireBase::getFacebookUser() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}
#endif
#endif


void GDFireBase::load_document(String p_col_name, String p_doc_name, int callback_id) {
    GDFirestore::getInstance()->load_document(p_col_name, p_doc_name, callback_id);
}

void GDFireBase::_bind_methods() {
	ObjectTypeDB::bind_method("init", &GDFireBase::init);
	ObjectTypeDB::bind_method("init_with_file", &GDFireBase::initWithFile);

	// AdMob
	ObjectTypeDB::bind_method("show_banner_ad", &GDFireBase::showBannerAd);
	ObjectTypeDB::bind_method("show_interstitial_ad", &GDFireBase::showInterstitialAd);
	ObjectTypeDB::bind_method("show_rewarded_video", &GDFireBase::showRewardedVideo);
	ObjectTypeDB::bind_method("request_rewarded_video_status", &GDFireBase::requestRewardedVideoStatus);

	// Notification
	ObjectTypeDB::bind_method("notify_in_min", &GDFireBase::notifyInMin);
	ObjectTypeDB::bind_method("subscribe_to_topic", &GDFireBase::subscribeToTopic);
	ObjectTypeDB::bind_method("get_token", &GDFireBase::getToken);

	//Invite
	ObjectTypeDB::bind_method("invite", &GDFireBase::invite);

	// RemoteConfig
	ObjectTypeDB::bind_method("get_remote_value", &GDFireBase::getRemoteValue);
	ObjectTypeDB::bind_method("set_remote_defaults", &GDFireBase::setRemoteDefaults);
	ObjectTypeDB::bind_method("set_remote_defaults_file", &GDFireBase::setRemoteDefaultsFile);

	ObjectTypeDB::bind_method("download", &GDFireBase::download);
	ObjectTypeDB::bind_method("upload", &GDFireBase::upload);

	//Auth Google
	ObjectTypeDB::bind_method("google_sign_in", &GDFireBase::googleSignIn);
	ObjectTypeDB::bind_method("google_sign_out", &GDFireBase::googleSignOut);
	ObjectTypeDB::bind_method("is_google_connected", &GDFireBase::isGoogleConnected);
	ObjectTypeDB::bind_method("get_google_user", &GDFireBase::getGoogleUser);
	ObjectTypeDB::bind_method("google_revoke_access", &GDFireBase::googleRevokeAccess);

	//Auth Twitter
	ObjectTypeDB::bind_method("twitter_sign_in", &GDFireBase::twitterSignIn);
	ObjectTypeDB::bind_method("twitter_sign_out", &GDFireBase::twitterSignOut);
	ObjectTypeDB::bind_method("is_twitter_connected", &GDFireBase::isTwitterConnected);

	//Auth Facebook
	ObjectTypeDB::bind_method("facebook_sign_in", &GDFireBase::facebookSignIn);
	ObjectTypeDB::bind_method("facebook_sign_out", &GDFireBase::facebookSignOut);
	ObjectTypeDB::bind_method("is_facebook_connected", &GDFireBase::isFacebookConnected);
	ObjectTypeDB::bind_method("get_facebook_permissions", &GDFireBase::getFacebookPermissions);
	ObjectTypeDB::bind_method("facebook_has_permission", &GDFireBase::facebookHasPermission);
	ObjectTypeDB::bind_method("revoke_facebook_permission", &GDFireBase::revokeFacebookPermision);
	ObjectTypeDB::bind_method("facebook_revoke_access", &GDFireBase::facebookRevokeAccess);
	ObjectTypeDB::bind_method("ask_facebook_read_permission", &GDFireBase::askFacebookReadPermission);
	ObjectTypeDB::bind_method("ask_facebook_publish_permission", &GDFireBase::askFacebookPublishPermission);
	ObjectTypeDB::bind_method("get_facebook_user", &GDFireBase::getFacebookUser);

//ObjectTypeDB::bind_method("", &GDFireBase::);

/**
	//Auth++
	"anonymous_sign_in", "anonymous_sign_out", "is_anonymous_connected",
	"authConfig",
	//Auth--

**/

#if GD_FIREBASE_ANALYTICS
	// Analytics
	ObjectTypeDB::bind_method("set_screen_name", &GDFireBase::setScreenName);
	ObjectTypeDB::bind_method("join_group", &GDFireBase::join_group);

	ObjectTypeDB::bind_method("send_achievement", &GDFireBase::sendAchievement);
	ObjectTypeDB::bind_method("level_up", &GDFireBase::level_up);
	ObjectTypeDB::bind_method("post_score", &GDFireBase::post_score);
	ObjectTypeDB::bind_method("content_select", &GDFireBase::content_select);
	ObjectTypeDB::bind_method("earn_currency", &GDFireBase::earn_currency);
	ObjectTypeDB::bind_method("spend_currency", &GDFireBase::spend_currency);
	ObjectTypeDB::bind_method("tutorial_begin", &GDFireBase::tutorial_begin);
	ObjectTypeDB::bind_method("tutorial_complete", &GDFireBase::tutorial_complete);
	ObjectTypeDB::bind_method("send_events", &GDFireBase::send_events);
	ObjectTypeDB::bind_method("send_custom", &GDFireBase::send_custom);

	BIND_CONSTANT(GDAnalytics::Params::ACHIEVEMENT_ID);
	BIND_CONSTANT(GDAnalytics::Params::ACLID);
	BIND_CONSTANT(GDAnalytics::Params::AFFILIATION);
#endif
	//	ObjectTypeDB::bind_method("getValue", );
}

NS_GODOT_END
}
