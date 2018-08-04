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

#include "gd_firestore.h"

NS_GODOT_BEGINE

GDFirestore *GDFirestore::mInstance = nullptr;
GDFirestore *GDFirestore::getInstance() {
    if (mInstance == nullptr) {
		mInstance = memnew(GDFirestore);
    }

    return mInstance;
}

#if GD_FIREBASE_ANDROID_IOS
void GDFirestore::init(::firebase::App *p_app) {
    this->_app = p_app;

    download = memnew(HTTPRequest);
    add_child(download);

    download->connect("request_completed", this, "_http_download_completed");  
}
#endif

void GDFirestore::load_collection(String p_col_name, int callback_id) {
#if GD_FIREBASE_ANDROID_IOS
    // https://firestore.googleapis.com/v1beta1/projects/dots--dots-40748713/databases/(default)/documents/Events/
    String url = FIREBASE_URL;

    Dictionary vals;
    vals["PROJECT_ID"] = _project_id;
    vals["COLLECTION_DOCUMENT"] = p_col_name;

    host = url.format(vals);
    make_request();
#endif
}

void GDFirestore::load_document(String p_col_name, String p_doc_name, int callback_id) {
#if GD_FIREBASE_ANDROID_IOS
    // https://firestore.googleapis.com/v1beta1/projects/dots--dots-40748713/databases/(default)/documents/Events/Event_1
    String url = FIREBASE_URL;

    Dictionary vals;
    vals["PROJECT_ID"] = _project_id;
    vals["COLLECTION_DOCUMENT"] = p_col_name + "/" + p_doc_name;

    host = url.format(vals);
    make_request();
#endif
}

void GDFirestore::add_document(String p_name, Dictionary p_dict) {

}

void GDFirestore::set_data(String p_col_name, String p_doc_name, Dictionary p_dict) {

}

void GDFirestore::make_request() {
    download->cancel_request();

    Error err = download->request(host);
    if (err != OK) {
        // "Error making request";
    } else {
        // set_process(true);
    }
}

void GDFirestore::_http_download_completed(int p_status, int p_code, const StringArray &headers, const ByteArray &p_data) {
#if GD_FIREBASE_ANDROID_IOS
    String error_text;

    if (p_status == HTTPRequest::RESULT_SUCCESS && p_code < HTTPClient::RESPONSE_BAD_REQUEST) {
        String str = Utils::bytearray_to_string(p_data);
        LOGI(str.utf8());
    }
#endif
}

void GDFirestore::_bind_methods() {
    ObjectTypeDB::bind_method("load_collection", &GDFirestore::load_collection);
    ObjectTypeDB::bind_method("load_document", &GDFirestore::load_document);
    ObjectTypeDB::bind_method("add_document", &GDFirestore::add_document);
    ObjectTypeDB::bind_method("set_data", &GDFirestore::set_data);

    ObjectTypeDB::bind_method("_http_download_completed", &GDFirestore::_http_download_completed);
}

NS_GODOT_END
