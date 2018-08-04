/**
 * Copyright (C) FrogSquare. All Rights Reserved.
 * This is part of: GodotFireBaseCPP (http://frogsquare.com).
 *
 * details of the licence is provided in file name `LICENCE.`
 *
 * @file ios_common.h, 2017
 * @author Ramesh Mani Maran
 * @contact http://frogsquare.com/
 **/

#ifdef IPHONE_ENABLED

#ifndef IOS_COMMON_H
#define IOS_COMMON_H

#import "app_delegate.h"

#include "utils.h"

class IOSCommon {
public:
	IOSCommon();
	virtual ~IOSCommon();

	ViewController *getViewController();

private:
	ViewController *root_controller;

	static IOSCommon *mInstance;
};

#endif // IOS_COMMON_H

#endif
