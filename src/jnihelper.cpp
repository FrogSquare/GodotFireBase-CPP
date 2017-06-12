/**
 * Copyright 2017 RameshRavone. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#include "jnihelper.h"
#include <android/log.h>
#include <string.h>
#include <pthread.h>

#include "print_string.h"

#define LOG_TAG "JNIHelper"

static pthread_key_t g_key;

jclass _getClassID(const char *className) {
	if (className == nullptr) {
		return nullptr;
	}

	JNIEnv *env = godot::JNIHelper::getEnv ();

	jstring _jstrClassName = env->NewStringUTF(className);
	jclass _clazz = (jclass) env->CallObjectMethod(
				godot::JNIHelper::classLoader,
				godot::JNIHelper::loadclassMethod_methodID,
				_jstrClassName);

	if (_clazz == nullptr) {
		LOGI("ClassLoader failed to find class of %s", className);
		env->ExceptionClear();
	}

	env->DeleteLocalRef(_jstrClassName);
	return _clazz;
}

String _getClassName(JNIEnv * env, jclass cls, bool* array) {

	jclass cclass = env->FindClass("java/lang/Class");
	jmethodID getName = env->GetMethodID(cclass, "getName", "()Ljava/lang/String;");
	jstring clsName=(jstring) env->CallObjectMethod(cls, getName);

	if (array) {
		jmethodID isArray = env->GetMethodID(cclass, "isArray", "()Z");
		jboolean isarr = env->CallBooleanMethod(cls, isArray);
		(*array) = isarr ? true : false;
	}
	String name = env->GetStringUTFChars( clsName, NULL );
	env->DeleteLocalRef(clsName);

	return name;
}

void _detachCurrentThread(void* a) {
	godot::JNIHelper::getJavaVM()->DetachCurrentThread();
}

NS_GODOT_BEGINE

JavaVM *JNIHelper::_psJavaVM = nullptr;
jmethodID JNIHelper::loadclassMethod_methodID = nullptr;
jobject JNIHelper::classLoader = nullptr;
std::function<void()> JNIHelper::classloaderCallback = nullptr;

jobject JNIHelper::_activity = nullptr;
std::unordered_map<JNIEnv*, std::vector<jobject>> JNIHelper::localRefs;

JavaVM *JNIHelper::getJavaVM () {
	pthread_t thisthread = pthread_self();
	LOGD("JniHelper::getJavaVM(), pthread_self() = %ld", thisthread);
	return _psJavaVM;
}

void JNIHelper::setJavaVM (JavaVM *javaVM) {
	pthread_t thisthread = pthread_self();
	LOGD("JniHelper::setJavaVM(%p), pthread_self() = %ld", javaVM, thisthread);
	_psJavaVM = javaVM;

	pthread_key_create(&g_key, _detachCurrentThread);
}

JNIEnv *JNIHelper::cacheEnv (JavaVM *jvm) {
	JNIEnv *_env = nullptr;
	// get jni environment

	LOGI("Working..!");
	jint ret = jvm->GetEnv((void**)&_env, JNI_VERSION_1_6);

	switch (ret) {
	case JNI_OK:
		pthread_setspecific(g_key, _env);
		return _env;
	case JNI_EDETACHED:
		if (jvm->AttachCurrentThread(&_env, nullptr) < 0) {
			LOGE("Failed to get the environment using AttachCurrentThread()");
			return nullptr;
		} else {
			// Success : Attached and obtained JNIEnv!
			pthread_setspecific(g_key, _env);
			return _env;
		}
	case JNI_EVERSION:
		// Cannot recover from this error
		LOGE("JNI interface version 1.4 not supported");
	default:
		LOGE("Failed to get the environment using GetEnv()");
		return nullptr;
	}
	LOGI("Working.. Done");
}

JNIEnv *JNIHelper::getEnv () {
	JNIEnv *_env = (JNIEnv*) pthread_getspecific(g_key);

	if (_env == nullptr) {
		_env = JNIHelper::cacheEnv (_psJavaVM);
	}
	return _env;
}

jobject JNIHelper::getActivity () {
	return _activity;
}

bool JNIHelper::setClassLoaderFrom (jobject activityinstance) {
	JniMethodInfo _getclassloaderMethod;

	if (!JNIHelper::getMethodInfo_DefaultClassLoader (_getclassloaderMethod,
							  "android/content/Context",
							  "getClassLoader",
							  "()Ljava/lang/ClassLoader;")) {
		return false;
	}

	jobject _c = JNIHelper::getEnv ()->CallObjectMethod(activityinstance,
							     _getclassloaderMethod.methodID);

	if (_c == nullptr) {
		return false;
	}

	JniMethodInfo _m;
	if (!JNIHelper::getMethodInfo_DefaultClassLoader (_m,
							  "java/lang/ClassLoader",
							  "loadClass",
							  "(Ljava/lang/String;)Ljava/lang/Class;")) {
		return false;
	}

	JNIHelper::classLoader = JNIHelper::getEnv ()->NewGlobalRef(_c);
	JNIHelper::loadclassMethod_methodID = _m.methodID;
	JNIHelper::_activity = JNIHelper::getEnv ()->NewGlobalRef(activityinstance);

	if (JNIHelper::classloaderCallback != nullptr) {
		JNIHelper::classloaderCallback();
	}

	return true;
}

bool JNIHelper::getStaticMethodInfo (JniMethodInfo &methodInfo, const char *className, const char *methodName, const char *paramCode) {
	if (className == nullptr || methodName == nullptr || paramCode == nullptr) {
		return false;
	}

	JNIEnv *env = JNIHelper::getEnv ();

	if (!env) {
		LOGE("Failed to get JNIEnv");
		return false;
	}

	jclass classID = _getClassID (className);
	if (!classID) {
		LOGE("Failed to find class %s", className);
		env->ExceptionClear();
		return false;
	}

	jmethodID methodID = env->GetStaticMethodID(classID, methodName, paramCode);

	if (!methodID) {
		LOGE("Failed to find static method id of %s", methodName);
		env->ExceptionClear();
		return false;
	}

	methodInfo.classID = classID;
	methodInfo.env = env;
	methodInfo.methodID = methodID;

	return true;
}

bool JNIHelper::getMethodInfo_DefaultClassLoader (JniMethodInfo &methodInfo, const char *className, const char *methodName, const char *paramCode) {
	if (className == nullptr || methodName == nullptr || paramCode == nullptr) {
		return false;
	}

	JNIEnv *env = JNIHelper::getEnv ();
	if (!env) {
		return false;
	}

	jclass classID = env->FindClass (className);
	if (!classID) {
		LOGE("Failed to find class %s", className);
		env->ExceptionClear();
		return false;
	}

	jmethodID methodID = env->GetMethodID(classID, methodName, paramCode);
	if (!methodID) {
		LOGE("Failed to find static method id of %s", methodName);
		env->ExceptionClear();
		return false;
	}

	methodInfo.classID = classID;
	methodInfo.env = env;
	methodInfo.methodID = methodID;

	return true;
}

bool JNIHelper::getMethodInfo (JniMethodInfo &methodInfo, const char *className, const char *methodName, const char *paramCode) {
	if (className == nullptr || methodName == nullptr || paramCode == nullptr) {
		return false;
	}

	JNIEnv *env = JNIHelper::getEnv ();
	if (!env) {
		return false;
	}

	jclass classID = _getClassID (className);
	if (!classID) {
		LOGE("Failed to find class %s", className);
		env->ExceptionClear();
		return false;
	}

	jmethodID methodID = env->GetMethodID(classID, methodName, paramCode);
	if (!methodID) {
		LOGE("Failed to find static method id of %s", methodName);
		env->ExceptionClear();
		return false;
	}

	methodInfo.classID = classID;
	methodInfo.env = env;
	methodInfo.methodID = methodID;

	return true;
}

jstring JNIHelper::convert (JniMethodInfo &t, Variant x) {
	jvalret ret = varientToJvalue (t.env, x.get_type(), x);
	localRefs[t.env].push_back(ret.obj);

	return (jstring)ret.obj;
}

void JNIHelper::deleteLocalRefs (JNIEnv *env) {
	if (!env) { return; }

	for (const auto& ref : localRefs[env]) {
		env->DeleteLocalRef(ref);
	}

	localRefs[env].clear();
}

jvalret JNIHelper::varientToJvalue (JNIEnv *env, Variant::Type p_type, const Variant arg, bool force_jobject) {
	const Variant *p_arg = &arg;
	jvalret v;

	switch(p_type) {

		case Variant::BOOL: {

			if (force_jobject) {
				jclass bclass = env->FindClass("java/lang/Boolean");
				jmethodID ctor = env->GetMethodID(bclass, "<init>", "(Z)V");
				jvalue val;
				val.z = (bool)(*p_arg);
				jobject obj = env->NewObjectA(bclass, ctor, &val);
				v.val.l = obj;
				v.obj=obj;
				env->DeleteLocalRef(bclass);
			} else {
				v.val.z=*p_arg;

			};
		} break;
		case Variant::INT: {

			if (force_jobject) {

				jclass bclass = env->FindClass("java/lang/Integer");
				jmethodID ctor = env->GetMethodID(bclass, "<init>", "(I)V");
				jvalue val;
				val.i = (int)(*p_arg);
				jobject obj = env->NewObjectA(bclass, ctor, &val);
				v.val.l = obj;
				v.obj=obj;
				env->DeleteLocalRef(bclass);

			} else {
				v.val.i=*p_arg;

			};
		} break;
		case Variant::REAL: {

			if (force_jobject) {

				jclass bclass = env->FindClass("java/lang/Double");
				jmethodID ctor = env->GetMethodID(bclass, "<init>", "(D)V");
				jvalue val;
				val.d = (double)(*p_arg);
				jobject obj = env->NewObjectA(bclass, ctor, &val);
				v.val.l = obj;
				v.obj=obj;
				env->DeleteLocalRef(bclass);

			} else {
				v.val.f=*p_arg;
			};
		} break;
		case Variant::STRING: {

			String s = *p_arg;
			jstring jStr = env->NewStringUTF(s.utf8().get_data());
			v.val.l=jStr;
			v.obj=jStr;
		} break;
		case Variant::STRING_ARRAY: {

			DVector<String> sarray = *p_arg;
			jobjectArray arr = env->NewObjectArray(sarray.size(),env->FindClass("java/lang/String"),env->NewStringUTF(""));

			for(int j=0;j<sarray.size();j++) {

				jstring str = env->NewStringUTF( sarray[j].utf8().get_data() );
				env->SetObjectArrayElement(arr,j,str);
				env->DeleteLocalRef(str);
			}
			v.val.l=arr;
			v.obj=arr;

		} break;

		case Variant::DICTIONARY: {

			Dictionary dict = *p_arg;
			jclass dclass = env->FindClass("org/godotengine/godot/Dictionary");
			jmethodID ctor = env->GetMethodID(dclass, "<init>", "()V");
			jobject jdict = env->NewObject(dclass, ctor);

			Array keys = dict.keys();

			jobjectArray jkeys = env->NewObjectArray(keys.size(), env->FindClass("java/lang/String"), env->NewStringUTF(""));
			for (int j=0; j<keys.size(); j++) {
				jstring str = env->NewStringUTF(String(keys[j]).utf8().get_data());
				env->SetObjectArrayElement(jkeys, j, str);
				env->DeleteLocalRef(str);
			};

			jmethodID set_keys = env->GetMethodID(dclass, "set_keys", "([Ljava/lang/String;)V");
			jvalue val;
			val.l = jkeys;
			env->CallVoidMethodA(jdict, set_keys, &val);
			env->DeleteLocalRef(jkeys);

			jobjectArray jvalues = env->NewObjectArray(keys.size(), env->FindClass("java/lang/Object"), NULL);

			for (int j=0; j<keys.size(); j++) {
				Variant var = dict[keys[j]];
				jvalret v = varientToJvalue (env, var.get_type(), var, true);
				env->SetObjectArrayElement(jvalues, j, v.val.l);
				if (v.obj) {
					env->DeleteLocalRef(v.obj);
				}
			};

			jmethodID set_values = env->GetMethodID(dclass, "set_values", "([Ljava/lang/Object;)V");
			val.l = jvalues;
			env->CallVoidMethodA(jdict, set_values, &val);
			env->DeleteLocalRef(jvalues);
			env->DeleteLocalRef(dclass);

			v.val.l = jdict;
			v.obj=jdict;
		} break;

		case Variant::INT_ARRAY: {

			DVector<int> array = *p_arg;
			jintArray arr = env->NewIntArray(array.size());
			DVector<int>::Read r = array.read();
			env->SetIntArrayRegion(arr,0,array.size(),r.ptr());
			v.val.l=arr;
			v.obj=arr;

		} break;
		case Variant::RAW_ARRAY: {
			DVector<uint8_t> array = *p_arg;
			jbyteArray arr = env->NewByteArray(array.size());
			DVector<uint8_t>::Read r = array.read();
			env->SetByteArrayRegion(arr,0,array.size(),reinterpret_cast<const signed char*>(r.ptr()));
			v.val.l=arr;
			v.obj=arr;

		} break;
		case Variant::REAL_ARRAY: {

			DVector<float> array = *p_arg;
			jfloatArray arr = env->NewFloatArray(array.size());
			DVector<float>::Read r = array.read();
			env->SetFloatArrayRegion(arr,0,array.size(),r.ptr());
			v.val.l=arr;
			v.obj=arr;

		} break;
		default: {

			v.val.i = 0;
		} break;

	}
	return v;
}

Variant JNIHelper::JObjectToVarient (JNIEnv * env, jobject obj) {

	if (obj == NULL) {
		return Variant();
	}

	jclass c = env->GetObjectClass(obj);
	bool array;
	String name = _getClassName (env, c, &array);
	//print_line("name is " + name + ", array "+Variant(array));

	print_line("ARGNAME: "+name);
	if (name == "java.lang.String") {

		return String::utf8(env->GetStringUTFChars( (jstring)obj, NULL ));
	};


	if (name == "[Ljava.lang.String;") {

		jobjectArray arr = (jobjectArray)obj;
		int stringCount = env->GetArrayLength(arr);
		//print_line("String array! " + String::num(stringCount));
		DVector<String> sarr;

		for (int i=0; i<stringCount; i++) {
			jstring string = (jstring) env->GetObjectArrayElement(arr, i);
			sarr.push_back(String::utf8(env->GetStringUTFChars(string, NULL)));
			env->DeleteLocalRef(string);

		}

		return sarr;
	};

	if (name == "java.lang.Boolean") {

		jmethodID boolValue = env->GetMethodID(c, "booleanValue", "()Z");
		bool ret = env->CallBooleanMethod(obj, boolValue);
		return ret;
	};

	if (name == "java.lang.Integer") {

		jclass nclass = env->FindClass("java/lang/Number");
		jmethodID intValue = env->GetMethodID(nclass, "intValue", "()I");
		int ret = env->CallIntMethod(obj, intValue);
		return ret;
	};

	if (name == "[I") {

		jintArray arr = (jintArray)obj;
		int fCount = env->GetArrayLength(arr);
		DVector<int> sarr;
		sarr.resize(fCount);

		DVector<int>::Write w = sarr.write();
		env->GetIntArrayRegion(arr,0,fCount,w.ptr());
		w = DVector<int>::Write();
		return sarr;
	};

	if (name == "[B") {

		jbyteArray arr = (jbyteArray)obj;
		int fCount = env->GetArrayLength(arr);
		DVector<uint8_t> sarr;
		sarr.resize(fCount);

		DVector<uint8_t>::Write w = sarr.write();
		env->GetByteArrayRegion(arr,0,fCount,reinterpret_cast<signed char*>(w.ptr()));
		w = DVector<uint8_t>::Write();
		return sarr;
	};

	if (name == "java.lang.Float" || name == "java.lang.Double") {

		jclass nclass = env->FindClass("java/lang/Number");
		jmethodID doubleValue = env->GetMethodID(nclass, "doubleValue", "()D");
		double ret = env->CallDoubleMethod(obj, doubleValue);
		return ret;
	};

	if (name == "[D") {

		jdoubleArray arr = (jdoubleArray)obj;
		int fCount = env->GetArrayLength(arr);
		RealArray sarr;
		sarr.resize(fCount);

		RealArray::Write w = sarr.write();

		for (int i=0; i<fCount; i++) {

			double n;
			env->GetDoubleArrayRegion(arr, i, 1, &n);
			w.ptr()[i] = n;

		};
		return sarr;
	};

	if (name == "[F") {

		jfloatArray arr = (jfloatArray)obj;
		int fCount = env->GetArrayLength(arr);
		RealArray sarr;
		sarr.resize(fCount);


		RealArray::Write w = sarr.write();

		for (int i=0; i<fCount; i++) {

			float n;
			env->GetFloatArrayRegion(arr, i, 1, &n);
			w.ptr()[i] = n;

		};
		return sarr;
	};


	if (name == "[Ljava.lang.Object;") {

		jobjectArray arr = (jobjectArray)obj;
		int objCount = env->GetArrayLength(arr);
		Array varr(true);

		for (int i=0; i<objCount; i++) {
			jobject jobj = env->GetObjectArrayElement(arr, i);
			Variant v = JObjectToVarient (env, jobj);
			varr.push_back(v);
			env->DeleteLocalRef(jobj);

		}

		return varr;
	};

	if (name == "java.util.HashMap" || name == "org.godotengine.godot.Dictionary") {

		Dictionary ret(true);
		jclass oclass = c;
		jmethodID get_keys = env->GetMethodID(oclass, "get_keys", "()[Ljava/lang/String;");
		jobjectArray arr = (jobjectArray)env->CallObjectMethod(obj, get_keys);

		StringArray keys = JObjectToVarient (env, arr);
		env->DeleteLocalRef(arr);

		jmethodID get_values = env->GetMethodID(oclass, "get_values", "()[Ljava/lang/Object;");
		arr = (jobjectArray)env->CallObjectMethod(obj, get_values);

		Array vals = JObjectToVarient (env, arr);
		env->DeleteLocalRef(arr);

		//print_line("adding " + String::num(keys.size()) + " to Dictionary!");
		for (int i=0; i<keys.size(); i++) {

			ret[keys[i]] = vals[i];
		};


		return ret;
	};

	env->DeleteLocalRef(c);

	return Variant();
}

NS_GODOT_END

