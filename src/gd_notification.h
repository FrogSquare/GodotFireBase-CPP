/**
 * Copyright (C) FrogSquare. All Rights Reserved.
 * This is part of: GodotFireBaseCPP (http://frogsquare.com).
 *
 * details of the licence is provided in file name `LICENCE.`
 *
 * @file gd_notification.h, 2017
 * @author Ramesh Mani Maran
 * @contact http://frogsquare.com/
 **/

#ifndef GD_NOTIFICATION_H
#define GD_NOTIFICATION_H

#include "utils.h"

#if GD_FIREBASE_ANDROID_IOS
#include "firebase/app.h"
#include "firebase/messaging.h"

namespace messaging = ::firebase::messaging;
#endif

NS_GODOT_BEGINE

class MessageListener;

class GDNotification {
public:
	GDNotification();
	virtual ~GDNotification();

	static GDNotification *getInstance();

#if GD_FIREBASE_ANDROID_IOS
	void init(::firebase::App *p_app);
#endif

	void notifyInMin(String p_message, int p_mins);
	void subscribeToTopic(String p_topic);
	String getToken();

	inline void setToken(const char *p_tok) { this->_token = p_tok; }

private:
#if GD_FIREBASE_ANDROID_IOS
	::firebase::App *_app;
#endif

	MessageListener *_message_listener;

	static GDNotification *mInstance;
	String _token;
};

NS_GODOT_END

#endif // GD_NOTIFICATION_H
