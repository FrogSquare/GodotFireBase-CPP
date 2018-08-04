/**
 * Copyright (C) FrogSquare. All Rights Reserved.
 * This is part of: GodotFireBaseCPP (http://frogsquare.com).
 *
 * details of the licence is provided in file name `LICENCE.`
 *
 * @file gd_storage.h, 2017
 * @author Ramesh Mani Maran
 * @contact http://frogsquare.com/
 **/

#ifndef GD_STORAGE_H
#define GD_STORAGE_H

#include "utils.h"

#if GD_FIREBASE_ANDROID_IOS
#include "firebase/app.h"
#include "firebase/storage.h"

namespace messaging = ::firebase::storage;
#endif

NS_GODOT_BEGINE

class GDStorage {
public:
	GDStorage();
	virtual ~GDStorage();

	static GDStorage *getInstance();

#if GD_FIREBASE_ANDROID_IOS
	void init(::firebase::App *p_app);
#endif

	void upload();
	void download();

private:
#if GD_FIREBASE_ANDROID_IOS
	::firebase::App *_app;
#endif

	static GDStorage *mInstance;
};

NS_GODOT_END

#endif // GD_STORAGE_H
