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

#include "gd_config.h"
#include "reference.h"

#ifdef GD_FIREBASE_ADMOB

#include "firebase/admob.h"
#include "firebase/app.h"

NS_GODOT_BEGINE

#ifdef GD_FIREBASE_ANDROID_IOS
const char *k_AdMobAppID = "ca-app-pub-3940256099942544~3347511713";
#else
const char *k_AdMobAppID = "";
#endif

// These ad units are configured to always serve test ads.
#ifdef GD_FIREBASE_ANDROID_IOS
const char *k_AdViewAdUnit = "ca-app-pub-3940256099942544/6300978111";
const char *k_InterstitialAdUnit = "ca-app-pub-3940256099942544/1033173712";
const char *k_RewardedVideoAdUnit = "ca-app-pub-3940256099942544/2888167318";
#else
const char *k_AdViewAdUnit = "";
const char *k_InterstitialAdUnit = "";
const char *k_RewardedVideoAdUnit = "";
#endif

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

	void createBanner();
	void createInterstitial();
	void createRewardedVideo();

	bool isRewardedVideoLoaded();

	void showRewardedVideo();
	void showBannedAd();
	void showInterstitialAd();

	void onRewardedVideoStatusChanged();

	static GDAdMob *getInstance();

private:
	static GDAdMob *mInstance;

	bool interstitialAdShown = false;
	bool rewardedVideoAdShown = false;
};

NS_GODOT_END

#endif
#endif // GD_ADMOB_H
