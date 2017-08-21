/** register_types.cpp **/

#include "register_types.h"
#include "object_type_db.h"
#include "globals.h"
#include "gd_firebase.h"

#ifdef ANDROID_ENABLED
#include "jnihelper.h"

USING_NS_GODOT;

extern "C" int common_main () {
	JavaVM *jvm;
	JNIEnv *env = ThreadAndroid::get_env();
	env->GetJavaVM(&jvm);

	JNIHelper::setJavaVM(jvm);

	jclass cls = env->FindClass("org/godotengine/godot/Godot");
	jmethodID getIns = env->GetStaticMethodID(cls,"getInstance","()Lorg/godotengine/godot/Godot;");

	if (getIns == NULL) {
		LOGI("Error::Method..!");
		return 0;
	} else {
		GDFireBase::instance = env->CallStaticObjectMethod(cls, getIns);
		JNIHelper::setClassLoaderFrom(GDFireBase::instance);
		return 1;
	}

	return 0;
}
#endif

void register_gdfirebase_types() {
	//ObjectTypeDB::register_type<godot::GDFireBase>();

#ifndef ANDROID_ENABLED
	if (common_main()) { LOGI("JNIHelper is initialized!"); }
	else { LOGI("JNIHelper unable find org.godotengine.godot.Godot instance!"); }
#endif

	Globals::get_singleton()->add_singleton(
	Globals::Singleton("GDFireBase", memnew(godot::GDFireBase)));
}

void unregister_gdfirebase_types() {
	//nothing to do here
}

