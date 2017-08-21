/**
 * Copyright (C) FrogSquare. All Rights Reserved.
 * This is part of: godot-custom (http://frogsquare.com).
 *
 * details of the licence is provided in file name `LICENCE.`
 *
 * @file gd_remote_config.cpp, 2017
 * @author Ramesh Mani Maran
 * @contact http://frogsquare.com/
 **/

#include <gd_remote_config.h>

#ifdef ANDROID_ENABLED
#ifdef GD_FIREBASE_DEBUG
#include <sys/prctl.h>
#endif

#include "jnihelper.h"
#endif

#if GD_FIREBASE_REMOTECONFIG

NS_GODOT_BEGINE

GDRemoteConfig *GDRemoteConfig::mInstance = nullptr;

GDRemoteConfig *GDRemoteConfig::getInstance() {
	if (mInstance == nullptr) {
		mInstance = new GDRemoteConfig();
	}

	return mInstance;
}

#ifdef GD_FIREBASE_ANDROID_IOS
void GDRemoteConfig::init(firebase::App *app) {
	this->_app = app;

	remote_config::Initialize(*_app);
	fetchRemoteConfigs();
}
#endif

void GDRemoteConfig::setDefaultsFile(String filepath) {
#ifdef GD_FIREBASE_ANDROID_IOS
//remote_config::ConfigSetting;
#endif
}

void GDRemoteConfig::setDefaults(const Dictionary p_defaults) {
#ifdef GD_FIREBASE_ANDROID_IOS
	remote_config::ConfigKeyValueVariant *_defaults =
			new remote_config::ConfigKeyValueVariant[p_defaults.size()];

	for (unsigned int i = 0; i < p_defaults.size(); i++) {
		remote_config::ConfigKeyValueVariant _key_val;
		_key_val.key = (const char *)p_defaults.keys()[i].operator String().c_str();
		_key_val.value = (const char *)p_defaults[p_defaults.keys()[i]].operator String().c_str();

		_defaults[i] = _key_val;
	}

	remote_config::SetDefaults(_defaults, p_defaults.size());
	delete[] _defaults;
#endif
}

void GDRemoteConfig::fetchRemoteConfigs() {
#ifdef GD_FIREBASE_ANDROID_IOS
	const int cacheExpire = 3000;
	remote_config::Fetch(cacheExpire)
			.OnCompletion([](const firebase::Future<void> &call, void *user_data) {

				if (call.error() == 0) {
					remote_config::ActivateFetched();
				} else {
					// Print Error here..!
				}
			},
					nullptr);
#endif
}

Variant GDRemoteConfig::getValue(const String key, const String p_namespace) {
	Variant ret;
#ifdef GD_FIREBASE_ANDROID_IOS
	const char *_key = (const char *)key.c_str();
	const char *_namespace = (const char *)p_namespace.c_str();

	if (p_namespace != "") {
		ret = remote_config::GetString(_key, _namespace).c_str();
	} else {
		ret = remote_config::GetString(_key).c_str();
	}

#endif
	return ret;
}

NS_GODOT_END

#endif
