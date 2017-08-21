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

#ifdef ANDROID_ENABLED
#include "jnihelper.h"
#endif

#if GD_FIREBASE_ADMOB
NS_GODOT_BEGINE

// A simple listener that logs changes to an ad view.
class LoggingAdViewListener : public firebase::admob::BannerView::Listener {
public:
	LoggingAdViewListener(GDAdMob *scene)
		: _scene(scene) {}

	void OnPresentationStateChanged(firebase::admob::BannerView *adView,
			firebase::admob::BannerView::PresentationState state) override {

		printf("The ad view's PresentationState has changed to %d\n", state);
	}

	void OnBoundingBoxChanged(firebase::admob::BannerView *adView,
			firebase::admob::BoundingBox box) override {

		printf("The ad BoundingBox changed (x: %d, y: %d, width: %d, height: %d).",
				box.x, box.y, box.width, box.height);
	}

private:
	GDAdMob *_scene;
};

GDAdMob *GDAdMob::mInstance = nullptr;

GDAdMob::GDAdMob() {
}

GDAdMob *GDAdMob::getInstance() {
	if (mInstance == nullptr) {
		mInstance = memnew(GDAdMob);
	}

	return mInstance;
}

void GDAdMob::createBanner() {
#ifdef GD_FIREBASE_ANDROID_IOS
#endif
}

void GDAdMob::createInterstitial() {
#ifdef GD_FIREBASE_ANDROID_IOS
#endif
}

void GDAdMob::createRewardedVideo() {
#ifdef GD_FIREBASE_ANDROID_IOS
#endif
}

bool GDAdMob::isRewardedVideoLoaded() {
#ifdef GD_FIREBASE_ANDROID_IOS
#endif

	return false;
}

void GDAdMob::showRewardedVideo() {
#ifdef GD_FIREBASE_ANDROID_IOS
#endif
}

void GDAdMob::showBannedAd() {
#ifdef GD_FIREBASE_ANDROID_IOS
#endif
}

void GDAdMob::showInterstitialAd() {
#ifdef GD_FIREBASE_ANDROID_IOS
#endif
}

void GDAdMob::onRewardedVideoStatusChanged(){
#ifdef GD_FIREBASE_ANDROID_IOS
#endif
}

NS_GODOT_END
#endif
