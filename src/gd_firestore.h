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

#ifndef GD_FIRESTORE_H
#define GD_FIRESTORE_H

#include "utils.h"
#include "scene/main/http_request.h"

#if GD_FIREBASE_ANDROID_IOS

#include "firebase/app.h"

#ifdef IPHONE_ENABLED
//#include "ios_common.h"
#endif

#endif

#define FIREBASE_URL "https://firestore.googleapis.com/v1beta1/projects/{PROJECT_ID}/databases/(default)/documents/{COLLECTION_DOCUMENT}"

NS_GODOT_BEGINE

class GDFirestore : public Node {
    OBJ_TYPE(GDFirestore, Node)

public:
	GDFirestore() {}
	virtual ~GDFirestore() {}

	static GDFirestore *getInstance();

#if GD_FIREBASE_ANDROID_IOS
	void init(::firebase::App *p_app);
#endif

    void make_request();
    void _http_download_completed(int p_status, int p_code, const StringArray &headers, const ByteArray &p_data);

    void load_collection(String p_name, int callback_id);
    void load_document(String p_name, String p_doc_name, int callback_id);

    void add_document(String p_name, Dictionary p_dict);
    void set_data(String p_col_name, String p_doc_name, Dictionary p_dict);

protected:
    static void _bind_methods();

private:
#if GD_FIREBASE_ANDROID_IOS
	::firebase::App *_app;
#endif

    String _project_id;

    String host;
    HTTPRequest *download;

	static GDFirestore *mInstance;
};

NS_GODOT_END

#endif // GD_STORAGE_H
