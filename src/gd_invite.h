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

#ifndef GD_INVITE_H
#define GD_INVITE_H

#if GD_FIREBASE_INVITES

#include "utils.h"

#if GD_FIREBASE_ANDROID_IOS
#include "firebase/app.h"
#include "firebase/invites.h"

namespace invites = ::firebase::invites;

void ConversionFinished(const firebase::Future<void> &future_result, void *user_data);
#endif

NS_GODOT_BEGINE

class InviteListener;

class GDInvite {
public:
	GDInvite();
	virtual ~GDInvite();

	static GDInvite *getInstance();

#if GD_FIREBASE_ANDROID_IOS
	void init(::firebase::App *p_app);
#endif

	void invite(String p_message);
	void invite(String p_message, String p_deeplink);

	void update();

private:
#if GD_FIREBASE_ANDROID_IOS
	::firebase::App *_app;
#endif

	InviteListener *_invite_listener;
	static GDInvite *mInstance;

	bool _invite_sent;
    bool _initialized;
};

NS_GODOT_END

#endif
#endif // GD_INVITE_H
