/**
 * Copyright (C) FrogSquare. All Rights Reserved.
 * This is part of: GodotFireBaseCPP (http://frogsquare.com).
 *
 * details of the licence is provided in file name `LICENCE.`
 *
 * @file gd_storage.cpp, 2017
 * @author Ramesh Mani Maran
 * @contact http://frogsquare.com/
 **/

#include "gd_storage.h"

NS_GODOT_BEGINE

GDStorage *GDStorage::mInstance = nullptr;
GDStorage *GDStorage::getInstance() {
	if (mInstance == nullptr) {
		mInstance = memnew(GDStorage);
	}

	return mInstance;
}

GDStorage::GDStorage() {

}

GDStorage::~GDStorage() {
#if GD_FIREBASE_ANDROID_IOS
    delete _app;
#endif
}

#if GD_FIREBASE_ANDROID_IOS
void GDStorage::init(::firebase::App *p_app) {
    this->_app = p_app;
}
#endif

void GDStorage::download() {
}

void GDStorage::upload() {
}

NS_GODOT_END
