/**
 * Copyright (C) FrogSquare. All Rights Reserved.
 * This is part of: GodotFireBaseCPP (http://frogsquare.com).
 *
 * details of the licence is provided in file name `LICENCE.`
 *
 * @file gd_invite.cpp, 2017
 * @author Ramesh Mani Maran
 * @contact http://frogsquare.com/
 **/

#include "gd_invite.h"

#if GD_FIREBASE_INVITES

NS_GODOT_BEGINE

#if GD_FIREBASE_ANDROID_IOS
void ConversionFinished(const firebase::Future<void> &future_result, void *user_data) {
	if (future_result.status() == firebase::kFutureStatusInvalid) {
		LOGI("ConvertInvitation: Invalid, sorry!");
	} else if (future_result.status() == firebase::kFutureStatusComplete) {
		LOGI("ConvertInvitation: Complete!");
		if (future_result.error() != 0) {
			LOGI("ConvertInvitation: Error %d: %s", future_result.error(),
			                                		future_result.error_message());
		} else {
			LOGI("ConvertInvitation: Successfully converted invitation");
		}
	}
}

class InviteListener : public firebase::invites::Listener {
public:
      void OnInviteReceived(const char* invitation_id,
                                const char* deep_link,
                                bool is_strong_match) override {     

		if (invitation_id != nullptr) {
			LOGI("InviteReceived: Got invitation ID: %s", invitation_id);

			// We got an invitation ID, so let's try and convert it.
			LOGI("ConvertInvitation: Converting invitation %s", invitation_id);

			invites::ConvertInvitation(invitation_id).OnCompletion(ConversionFinished, nullptr);
		}

		if (deep_link != nullptr) {
			LOGI("InviteReceived: Got deep link: %s", deep_link);
		}
	}

	void OnInviteNotReceived() override {
		LOGI("InviteReceived: No invitation ID or deep link, confirmed.");
	}

	void OnErrorReceived(int error_code, const char *error_message) override {
		LOGI("Error (%d) on received invite: %s", error_code, error_message);
	}
};
#endif

GDInvite *GDInvite::mInstance = nullptr;
GDInvite::GDInvite() {
	_invite_sent = false;
}

GDInvite::~GDInvite() {
}

GDInvite *GDInvite::getInstance() {
	if (mInstance == nullptr) {
		mInstance = memnew(GDInvite);
	}

	return mInstance;
}

#if GD_FIREBASE_ANDROID_IOS
void GDInvite::init(firebase::App *p_app) {
	this->_app = p_app;
    this->_initialized = false;

	_invite_listener = new InviteListener();

	::firebase::InitResult result = invites::Initialize(*p_app);
	if (result == ::firebase::kInitResultSuccess) {
		invites::SetListener(_invite_listener);
        _initialized = true;

		LOGI("Initialized:Invites");
	} else {
		LOGI("Initializing:Invites:Failed");
	}

	LOGI("Initializing:Invites");
}
#endif

void GDInvite::invite(String p_message) {
#if GD_FIREBASE_ANDROID_IOS
    if (not _initialized) {
        LOGI("Invites:NotInitialized:Yet");
        return;
    }

	::firebase::invites::Invite invite;

	invite.title_text = "Invites Test App";
	invite.message_text = "Please try my app! It's awesome.";
	invite.call_to_action_text = "Download it for FREE";

	::firebase::invites::SendInvite(invite);
	_invite_sent = true;

    update();
#endif
}

void GDInvite::invite(String p_message, String p_deeplink) {
#if GD_FIREBASE_ANDROID_IOS
    if (not _initialized) {
        LOGI("Invites:NotInitialized:Yet");
        return;
    }

	::firebase::invites::Invite invite;

	invite.title_text = "Invites Test App";
	invite.message_text = "Please try my app! It's awesome.";
	invite.call_to_action_text = "Download it for FREE";
	invite.deep_link_url = p_deeplink.utf8();

	::firebase::invites::SendInvite(invite);
	_invite_sent = true;

    update();
#endif
}

void GDInvite::update() {
#if GD_FIREBASE_ANDROID_IOS
	if (_invite_sent) {
		firebase::Future<invites::SendInviteResult> future = invites::SendInviteLastResult();
		if (future.status() == firebase::kFutureStatusComplete) {
			if (future.error() == 0) {
				const invites::SendInviteResult &result = *future.result();
				if (result.invitation_ids.size() > 0) {
					// One or more invitations were sent. You can log the invitation IDs
					// here for analytics purposes, as they will be the same on the
					// receiving side.
					LOGI("Invite sent successfully!");
				} else {
					// Zero invitations were sent. This tells us that the user canceled
					// sending invitations.
					LOGI("Invite canceled.");
				}
			} else {
				// error() is nonzero, which means an error occurred. You can check
				// future_result.error_message() for more information.
				LOGI("Error sending the invite. (Error %i: \"%s\")",
						future.error(), future.error_message());
			}

			_invite_sent = false;
		} else {
			// The SendInvite() operation has not completed yet, which means the
			// Invites client UI is still on screen. Check the status() again soon.
		}
	}
#endif
}

NS_GODOT_END

#endif
