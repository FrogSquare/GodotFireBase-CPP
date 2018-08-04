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

#include "gd_admob.h"

#if GD_FIREBASE_ADMOB
NS_GODOT_BEGINE

// A simple listener that logs changes to an ad view.
#if GD_FIREBASE_ANDROID_IOS

/// A simple listener that logs changes to an ad view.
class LoggingAdViewListener : public admob::BannerView::Listener {
public:
	LoggingAdViewListener(GDAdMob *scene)
		: scene(scene) {}
	void OnPresentationStateChanged(
			firebase::admob::BannerView *adView,
			firebase::admob::BannerView::PresentationState state) override {
		LOGI("The ad view's PresentationState has changed to %d.", state);
	}
	void OnBoundingBoxChanged(firebase::admob::BannerView *adView,
			firebase::admob::BoundingBox box) override {
		LOGI("The ad view's BoundingBox has changed to (x: %d, y: %d, width: %d, height %d).",
				box.x, box.y, box.width, box.height);
	}

private:
	GDAdMob *scene;
};

/// A simple listener that logs changes to an InterstitialAd.
class LoggingInterstitialAdListener : public admob::InterstitialAd::Listener {
public:
	LoggingInterstitialAdListener(GDAdMob *scene)
		: scene(scene) {}
	void OnPresentationStateChanged(
			firebase::admob::InterstitialAd *interstitialAd,
			firebase::admob::InterstitialAd::PresentationState state) override {
		LOGI("InterstitialAd PresentationState has changed to %d.", state);
	}

private:
	GDAdMob *scene;
};

/// A simple listener that logs changes to rewarded video state.
class LoggingRewardedVideoListener : public admob::rewarded_video::Listener {
public:
	LoggingRewardedVideoListener(GDAdMob *scene)
		: scene(scene) {}
	void OnRewarded(admob::rewarded_video::RewardItem reward) override {
		LOGI("Rewarding user with %f %s.", reward.amount, reward.reward_type.c_str());
	}

	void OnPresentationStateChanged(admob::rewarded_video::PresentationState state) override {

		LOGI("Rewarded video PresentationState has changed to %d.", state);
	}

private:
	GDAdMob *scene;
};

/// This function is called when the Future for the last call to the ad view's
/// Initialize() method completes.
static void onAdViewInitializeCompletionCallback(const firebase::Future<void> &future, void *userData) {
	GDAdMob *scene = static_cast<GDAdMob *>(userData);
	if (future.error() == firebase::admob::kAdMobErrorNone) {
		LOGI("Initializing the ad view completed successfully.");
		scene->onAdViewInitialized();
	} else {
		LOGI("Initializing the ad view failed.");
		LOGI("ERROR: Action failed with error code %d and message \"%s\".", future.error(), future.error_message());
	}
}

static void onAdViewLoadAdCompletionCallback(const firebase::Future<void> &future, void *userData) {
	GDAdMob *scene = static_cast<GDAdMob *>(userData);
	if (future.error() == firebase::admob::kAdMobErrorNone) {
		LOGI("Loading the ad view completed successfully.");
	} else {
		LOGI("Loading the ad view failed.");
		LOGI("ERROR: Action failed with error code %d and message \"%s\".", future.error(), future.error_message());
	}
}

/// This function is called when the Future for the last call to the
/// InterstitialAds's Initialize() method completes.
static void onInterstitialAdInitializeCompletionCallback(const firebase::Future<void> &future, void *userData) {

	GDAdMob *scene = static_cast<GDAdMob *>(userData);
	if (future.error() == firebase::admob::kAdMobErrorNone) {
		LOGI("Initializing the interstitial ad completed successfully.");
		scene->onInterstitialInitialized();
	} else {
		LOGI("Initializing the interstitial ad failed.");
		LOGI("ERROR: Action failed with error code %d and message \"%s\".",
				future.error(), future.error_message());
	}
}

static void onInterstitialAdLoadAdCompletionCallback(const firebase::Future<void> &future, void *userData) {

	GDAdMob *scene = static_cast<GDAdMob *>(userData);
	if (future.error() == firebase::admob::kAdMobErrorNone) {
		LOGI("Loading the interstitial ad completed successfully.");
	} else {
		LOGI("Loading the interstitial ad failed.");
		LOGI("ERROR: Action failed with error code %d and message \"%s\".",
				future.error(), future.error_message());
	}
}

static void onRewardedVideoInitializeCompletionCallback(const firebase::Future<void> &future, void *userData) {
	GDAdMob *scene = static_cast<GDAdMob *>(userData);
	if (future.error() == firebase::admob::kAdMobErrorNone) {
		LOGI("Initializing rewarded video completed successfully.");
		scene->onRewardedVideoInitialized();
	} else {
		LOGI("Initializing rewarded video failed.");
		LOGI("ERROR: Action failed with error code %d and message \"%s\".",
				future.error(), future.error_message());
	}
}

static void onRewardedVideoLoadAdCompletionCallback(const firebase::Future<void> &future, void *userData) {

	GDAdMob *scene = static_cast<GDAdMob *>(userData);
	if (future.error() == firebase::admob::kAdMobErrorNone) {
		LOGI("Loading rewarded video completed successfully.");
	} else {
		LOGI("Loading rewarded video failed.");
		LOGI("ERROR: Action failed with error code %d and message \"%s\".",
				future.error(), future.error_message());

		// Rewarded Video returned an error. This might be because the developer did
		// not put their Rewarded Video ad unit into kRewardedVideoAdUnit above.
		LOGI("WARNING: Is your Rewarded Video ad unit ID correct?");
		LOGI("Ensure kRewardedVideoAdUnit is set to your own Rewarded Video ad unit ID.");
	}
}
#endif

GDAdMob *GDAdMob::mInstance = nullptr;

GDAdMob::GDAdMob() {
	k_AdMobAppID = "ca-app-pub-3940256099942544~3347511713";
	k_AdViewAdUnit = "ca-app-pub-3940256099942544/6300978111";
	k_InterstitialAdUnit = "ca-app-pub-3940256099942544/1033173712";
	k_RewardedVideoAdUnit = "ca-app-pub-3940256099942544/2888167318";
}

GDAdMob::~GDAdMob() {
#if GD_FIREBASE_ANDROID_IOS
	admob::rewarded_video::Destroy();
#endif
}

GDAdMob *GDAdMob::getInstance() {
	if (mInstance == nullptr) {
		mInstance = memnew(GDAdMob);
	}

	return mInstance;
}

#if GD_FIREBASE_ANDROID_IOS
void GDAdMob::init(::firebase::App *app, Dictionary p_config) {
	this->_app = app;

	k_AdMobAppID = p_config["AppId"].operator String().utf8();
	k_AdViewAdUnit = p_config["BannerAdId"].operator String().utf8();
	k_InterstitialAdUnit = p_config["InterstitialAdId"].operator String().utf8();
	k_RewardedVideoAdUnit = p_config["RewardedVideoAdId"].operator String().utf8();

	admob::Initialize(*_app, k_AdMobAppID);

	_adview_listener = new LoggingAdViewListener(this);
	_interstitial_listener = new LoggingInterstitialAdListener(this);
	_rewarded_ad_listener = new LoggingRewardedVideoListener(this);

	admob::rewarded_video::Initialize();

	_ad_view = new admob::BannerView();
	_interstitialAd = new admob::InterstitialAd();

	admob::AdSize adSize;
	adSize.ad_size_type = admob::kAdSizeStandard;
	adSize.width = k_AdViewWidth;
	adSize.height = k_AdViewHeight;

#ifdef ANDROID_ENABLED
	admob::AdParent parent = static_cast<admob::AdParent>(JNIHelper::getLayout());

	_ad_view->Initialize(parent, k_AdViewAdUnit, adSize);
	_interstitialAd->Initialize(JNIHelper::getActivity(), k_InterstitialAdUnit);
#endif

	/**
#ifdef IPHONE_ENABLED
	_ad_view->Initialize({Get Root Control View Here}, k_AdViewAdUnit, adSize);
	_interstitialAd->Initialize({Get Root Control View Here}, k_InterstitialAdUnit);
#endif
	**/

	_ad_view->InitializeLastResult().OnCompletion(onAdViewInitializeCompletionCallback, this);
	_interstitialAd->InitializeLastResult().OnCompletion(
			onInterstitialAdInitializeCompletionCallback, this);

	admob::rewarded_video::InitializeLastResult()
			.OnCompletion(onRewardedVideoInitializeCompletionCallback, this);

	LOGI("Initialized:Admob");
}

admob::AdRequest GDAdMob::createAdRequest() {
	admob::AdRequest request;
	// If the app is aware of the user's gender, it can be added to the targeting
	// information. Otherwise, "unknown" should be used.
	request.gender = admob::kGenderUnknown;

	// This value allows publishers to specify whether they would like the request
	// to be treated as child-directed for purposes of the Children’s Online
	// Privacy Protection Act (COPPA).
	// See http://business.ftc.gov/privacy-and-security/childrens-privacy.
	request.tagged_for_child_directed_treatment =
			admob::kChildDirectedTreatmentStateTagged;

	// The user's birthday, if known. Note that months are indexed from one.
	request.birthday_day = k_BirthdayDay;
	request.birthday_month = k_BirthdayMonth;
	request.birthday_year = k_BirthdayYear;

	// Additional keywords to be used in targeting.
	request.keyword_count = sizeof(k_Keywords) / sizeof(k_Keywords[0]);
	request.keywords = k_Keywords;

	static const admob::KeyValuePair kRequestExtras[] = {
		{ "the_name_of_an_extra", "the_value_for_that_extra" }
	};

	request.extras_count = sizeof(kRequestExtras) / sizeof(kRequestExtras[0]);
	request.extras = kRequestExtras;

	// This example uses ad units that are specially configured to return test ads
	// for every request. When using your own ad unit IDs, however, it's important
	// to register the device IDs associated with any devices that will be used to
	// test the app. This ensures that regardless of the ad unit ID, those
	// devices will always receive test ads in compliance with AdMob policy.
	//
	// Device IDs can be obtained by checking the logcat or the Xcode log while
	// debugging. They appear as a long string of hex characters.
	request.test_device_id_count =
			sizeof(k_TestDeviceIDs) / sizeof(k_TestDeviceIDs[0]);
	request.test_device_ids = k_TestDeviceIDs;

	return request;
}
#endif

void GDAdMob::onAdViewInitialized() {
#if GD_FIREBASE_ANDROID_IOS
	if (_ad_view->InitializeLastResult().status() == firebase::kFutureStatusComplete &&
			_ad_view->InitializeLastResult().error() == firebase::admob::kAdMobErrorNone) {

		LOGI("Setting the ad view listener.");

		_ad_view->SetListener(_adview_listener);

		LOGI("Loading the ad view.");

		admob::AdRequest ad_request = {};
		_ad_view->LoadAd(ad_request);
		_ad_view->LoadAdLastResult().OnCompletion(onAdViewLoadAdCompletionCallback, this);
	}
#endif
}

void GDAdMob::onInterstitialInitialized() {
#if GD_FIREBASE_ANDROID_IOS
	if (_interstitialAd->InitializeLastResult().status() == firebase::kFutureStatusComplete &&
			_interstitialAd->InitializeLastResult().error() == firebase::admob::kAdMobErrorNone) {

		LOGI("Setting the InterstitialAd listener.");

		_interstitialAd->SetListener(_interstitial_listener);

		LOGI("Loading the interstitial ad.");

		admob::AdRequest ad_request = {};
		_interstitialAd->LoadAd(ad_request);
		_interstitialAd->LoadAdLastResult().OnCompletion(
				onInterstitialAdLoadAdCompletionCallback, this);
	}
#endif
}

void GDAdMob::onRewardedVideoInitialized() {

}

void GDAdMob::createBanner() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDAdMob::createInterstitial() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDAdMob::createRewardedVideo() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

bool GDAdMob::isRewardedVideoLoaded() {
#if GD_FIREBASE_ANDROID_IOS
#endif
	//ObjectDB::get_instance(33)->call_deferred("", Variant());
	return false;
}

void GDAdMob::showRewardedVideo() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDAdMob::showBannedAd(bool p_show) {
#if GD_FIREBASE_ANDROID_IOS
	if (_ad_view->InitializeLastResult().status() == firebase::kFutureStatusComplete &&
			_ad_view->InitializeLastResult().error() == firebase::admob::kAdMobErrorNone) {

		if (p_show) {
			print_line("Showing Ad");
			_ad_view->Show();
		} else {
			_ad_view->Hide();
		}
	}
#endif
}

void GDAdMob::showInterstitialAd() {
#if GD_FIREBASE_ANDROID_IOS
	if (_interstitialAd->InitializeLastResult().status() == firebase::kFutureStatusComplete &&
			_interstitialAd->InitializeLastResult().error() == firebase::admob::kAdMobErrorNone) {

		_interstitialAd->Show();
	}
#endif
}

void GDAdMob::onRewardedVideoStatusChanged() {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

NS_GODOT_END
#endif
