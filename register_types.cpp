/** register_types.cpp **/
#include <version_generated.gen.h>

#include "register_types.h"
#include "gd_firebase.h"
#include "gd_firestore.h"

#if VERSION_MAJOR == 3
#include <core/class_db.h>
#include <core/engine.h>
#else
#include "object_type_db.h"
#include "globals.h"
#endif

#ifdef ANDROID_ENABLED
#include "jnihelper.h"
#include <assert.h>

USING_NS_GODOT;

extern "C" int common_main() {
	JavaVM *jvm;
	JNIEnv *env = ThreadAndroid::get_env();
	env->GetJavaVM(&jvm);

	JNIHelper::setJavaVM(jvm);

	jclass cls = env->FindClass("org/godotengine/godot/Godot");
	jmethodID getIns = env->GetStaticMethodID(cls, "getInstance", "()Lorg/godotengine/godot/Godot;");

	if (getIns == NULL) {
		LOGI("Error::Method..!");
		return 0;
	} else {
		GDFireBase::instance = env->CallStaticObjectMethod(cls, getIns);
		JNIHelper::setClassLoaderFrom(GDFireBase::instance);
		JNIHelper::setLayout(JNIHelper::getField(cls, GDFireBase::instance, "layout"));
		return 1;
	}

	return 0;
}
#endif

void register_gdfirebase_types() {
#ifdef ANDROID_ENABLED
	if (common_main()) {
		LOGI("JNIHelper is initialized!");
	} else {
		LOGI("JNIHelper: unable to find org.godotengine.godot.Godot instance!");
	}
#endif

#if VERSION_MAJOR == 3
    ClassDB::register_class<godot::GDFireBase>();
    ClassDB::register_class<godot::GDFirestore>();
#else
	ObjectTypeDB::register_type<godot::GDFireBase>();
	ObjectTypeDB::register_type<godot::GDFirestore>();
#endif
}

void unregister_gdfirebase_types() {
	//nothing to do here
}
