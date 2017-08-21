/**
 * Copyright (C) FrogSquare. All Rights Reserved.
 * This is part of: godot-custom (http://frogsquare.com).
 *
 * details of the licence is provided in file name `LICENCE.`
 *
 * @file gd_remote_config.h, 2017
 * @author Ramesh Mani Maran
 * @contact http://frogsquare.com/
 **/

#ifndef GD_REMOTE_CONFIG_H
#define GD_REMOTE_CONFIG_H

#include <dictionary.h>
#include <gd_config.h>
#include <gd_macros.h>
#include <ustring.h>

#if GD_FIREBASE_REMOTECONFIG

#ifdef GD_FIREBASE_ANDROID_IOS
#include "firebase/app.h"
#include "firebase/remote_config.h"
#include "firebase/util.h"

#ifdef ANDROID_ENABLED
#include "platform/android/thread_jandroid.h"
#endif

namespace remote_config = ::firebase::remote_config;
#endif

NS_GODOT_BEGINE

class GDRemoteConfig {

public:
	GDRemoteConfig() {}
	virtual ~GDRemoteConfig() {}

#ifdef GD_FIREBASE_ANDROID_IOS
	void init(::firebase::App *app);
#endif

	void setDefaultsFile(const String filepath);
	void setDefaults(const Dictionary p_defaults);

	Variant getValue(const String key, const String p_namespace = "");

	static GDRemoteConfig *getInstance();

private:
	void fetchRemoteConfigs();

#ifdef GD_FIREBASE_ANDROID_IOS
	::firebase::App *_app;
#endif

	static GDRemoteConfig *mInstance;
};

NS_GODOT_END

#endif
#endif // GD_REMOTE_CONFIG_H
