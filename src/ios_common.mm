/**
 * Copyright (C) FrogSquare. All Rights Reserved.
 * This is part of: GodotFireBaseCPP (http://frogsquare.com).
 *
 * details of the licence is provided in file name `LICENCE.`
 *
 * @file gd_invite.h, 2017
 * @author Ramesh Mani Maran
 * @contact http://frogsquare.com/
 **/

#ifdef IPHONE_ENABLED
#include "ios_common.h"

IOSCommon *IOSCommon::mInstance = nullptr;

IOSCommon::IOSCommon() {}
IOSCommon::~IOSCommon() {}

IOSCommon *IOSCommon::getInstance() {
  if (mInstance == nullptr) {
    mInstance = memnew(IOSCommon);
  }

  return mInstance;
}

ViewController *IOSCommon::getViewController() {
  if (root_controller == nullptr) {
    root_controller =
        (ViewController *)((AppDelegate *)[[UIApplication sharedApplication]
                               delegate])
            .window.rootViewController;
  }

  return root_controller;
}
#endif
