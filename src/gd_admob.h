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

#ifndef GD_ADMOB_H
#define GD_ADMOB_H

#include "reference.h"
#include "utils.h"

#ifdef GD_FIREBASE_ADMOB

#if GD_FIREBASE_ANDROID_IOS
#include "firebase/admob.h"
#include "firebase/app.h"

#include "firebase/admob/banner_view.h"
#include "firebase/admob/interstitial_ad.h"
#include "firebase/admob/rewarded_video.h"

namespace admob = ::firebase::admob;
#endif

NS_GODOT_BEGINE

class LoggingAdViewListener;
class LoggingInterstitialAdListener;
class LoggingRewardedVideoListener;

// The ad view's ad size.
static const int k_AdViewWidth = 320;
static const int k_AdViewHeight = 50;

// Sample keywords to use in making the request.
static const char *k_Keywords[] = { "AdMob", "C++", "Fun" };

// Sample test device IDs to use in making the request.
static const char *k_TestDeviceIDs[] = {
	"2077ef9a63d2b398840261c8221a0c9b",
	"098fe087d987c9a878965454a65654d7"
};

// Sample birthday value to use in making the request.
static const int k_BirthdayDay = 10;
static const int k_BirthdayMonth = 11;
static const int k_BirthdayYear = 1976;

class GDAdMob {
public:
	GDAdMob();

#if GD_FIREBASE_ANDROID_IOS
	void init(::firebase::App *app);
	admob::AdRequest createAdRequest();
#endif

	void createBanner();
	void createInterstitial();
	void createRewardedVideo();

	bool isRewardedVideoLoaded();

	void showRewardedVideo();
	void showBannedAd(bool p_show);
	void showInterstitialAd();

	void onRewardedVideoStatusChanged();

	void onAdViewInitialized();
	void onInterstitialInitialized();

	static GDAdMob *getInstance();

private:
#if GD_FIREBASE_ANDROID_IOS
	::firebase::App *_app;
	::firebase::admob::BannerView *_ad_view;
	::firebase::admob::InterstitialAd *_interstitialAd;
#endif
	static GDAdMob *mInstance;

	bool interstitialAdShown = false;
	bool rewardedVideoAdShown = false;

	LoggingAdViewListener *_adview_listener;
	LoggingInterstitialAdListener *_interstitial_listener;
	LoggingRewardedVideoListener *_rewarded_ad_listener;

	const char *k_AdMobAppID;
	const char *k_AdViewAdUnit;
	const char *k_InterstitialAdUnit;
	const char *k_RewardedVideoAdUnit;
};

NS_GODOT_END

#endif
#endif // GD_ADMOB_H
