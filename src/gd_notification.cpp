/**
 * Copyright (C) FrogSquare. All Rights Reserved.
 * This is part of: GodotFireBaseCPP (http://frogsquare.com).
 *
 * details of the licence is provided in file name `LICENCE.`
 *
 * @file gd_notification.cpp, 2017
 * @author Ramesh Mani Maran
 * @contact http://frogsquare.com/
 **/

#include "gd_notification.h"

NS_GODOT_BEGINE

#if GD_FIREBASE_ANDROID_IOS
class MessageListener : public messaging::Listener {

public:
	MessageListener(GDNotification *scene)
		: scene(scene) {
	}

	void OnMessage(const messaging::Message &p_message) {
		LOGI("FROM, %s", p_message.from.c_str());
		LOGI("Message ID: %s", p_message.message_id.c_str());
	}

	void OnTokenReceived(const char *p_token) {
		LOGI("Updating Messaging Token, %s", p_token);
		scene->setToken(p_token);
	}

private:
	GDNotification *scene;
};
#endif

GDNotification *GDNotification::mInstance = nullptr;
GDNotification *GDNotification::getInstance() {
	if (mInstance == nullptr) {
		mInstance = memnew(GDNotification);
	}

	return mInstance;
}

GDNotification::GDNotification() {
	_token = "";
}

GDNotification::~GDNotification() {
#if GD_FIREBASE_ANDROID_IOS
	delete _app;
#endif
}

#if GD_FIREBASE_ANDROID_IOS
void GDNotification::init(firebase::App *p_app) {
	this->_app = p_app;

	_message_listener = new MessageListener(this);
	messaging::Initialize(*_app, _message_listener);

	LOGI("Initialized:Notification/Messaging");
}
#endif

void GDNotification::notifyInMin(String p_message, int p_mins) {
#if GD_FIREBASE_ANDROID_IOS
#endif
}

void GDNotification::subscribeToTopic(String p_topic) {
#if GD_FIREBASE_ANDROID_IOS
	ERR_FAIL_COND(this->_app == nullptr)

	messaging::Subscribe(p_topic.utf8().get_data());
#endif
}

String GDNotification::getToken() {
#if GD_FIREBASE_ANDROID_IOS
#endif
	return _token;
}

NS_GODOT_END
