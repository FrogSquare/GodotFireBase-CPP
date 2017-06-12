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

NS_GODOT_BEGINE

class GDAdMob : public Reference {
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

	void onPause();
	void onResume();
	void onStart();
	void onStop();
};

NS_GODOT_END

#endif // GD_ADMOB_H
