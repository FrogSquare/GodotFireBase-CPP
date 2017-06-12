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

#ifndef __GD_JNIHELPER_H__
#define __GD_JNIHELPER_H__

#include <jni.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "math/vector3.h"

#include "variant.h"
#include "gd_config.h"

NS_GODOT_BEGINE

typedef struct JniMethodInfo {
	JNIEnv *env;
	jclass classID;
	jmethodID methodID;
} JniMethodInfo;

struct jvalret {
	jobject obj;
	jvalue val;

	jvalret() { obj=NULL; }
};


class JNIHelper {
public:
	static void setJavaVM(JavaVM *javaVM);
	static JavaVM *getJavaVM();
	static JNIEnv *getEnv();
	static jobject getActivity();

	static bool setClassLoaderFrom(jobject activityInstance);
	static bool getStaticMethodInfo(
	JniMethodInfo &methodInfo, const char *className, const char *methodName, const char *paramCode);

	static bool getMethodInfo(
	JniMethodInfo &methodInfo, const char *className, const char *methodName, const char *paramCode);

	static jmethodID loadclassMethod_methodID;
	static jobject classLoader;
	static std::function<void()> classloaderCallback;

	static jvalret varientToJvalue(JNIEnv *env, Variant::Type p_type, const Variant arg, bool force_jobject = false);
	static Variant JObjectToVarient (JNIEnv * env, jobject obj);

	template <typename... Ts>
	static void CallStaticVoidMethod(const std::string &className,
					 const std::string &methodName,
					 Ts... xs) {

		JniMethodInfo t;
		std::string signature = "(" + std::string(getJNISignature(xs...)) + ")V";
		if (JNIHelper::getStaticMethodInfo (t, className.c_str(), methodName.c_str(), signature.c_str())) {
			t.env->CallStaticVoidMethod(t.classID, t.methodID, convert(t, xs)...);
			t.env->DeleteLocalRef(t.classID);
			deleteLocalRefs(t.env);
		} else {
			LOGI("Error here..!");
		}
	}
private:
	static JNIEnv *cacheEnv(JavaVM* jvm);

	static bool getMethodInfo_DefaultClassLoader(
	JniMethodInfo &methdInfo, const char *className, const char *methodName, const char *paramCode);

	static JavaVM *_psJavaVM;
	static jobject _activity;

	static jstring convert(JniMethodInfo &t, Variant x);

	template<typename T>
	static T convert (JniMethodInfo &t, T x) {
		return x;
	}

	static std::unordered_map<JNIEnv*, std::vector<jobject>> localRefs;
	static void deleteLocalRefs(JNIEnv *env);

	static std::string getJNISignature() {
		return "";
	}

	static std::string getJNISignature (bool) {
		return "Z";
	}

	static std::string getJNISignature (char) {
		return "C";
	}

	static std::string getJNISignature(short) {
		return "S";
	}

	static std::string getJNISignature(int) {
		return "I";
	}

	static std::string getJNISignature(long) {
		return "J";
	}

	static std::string getJNISignature(float) {
		return "F";
	}

	static  std::string getJNISignature(double) {
		return "D";
	}

	static std::string getJNISignature(const char*) {
		return "Ljava/lang/String;";
	}

	static std::string getJNISignature(const std::string&) {
		return "Ljava/lang/String;";
	}

	template <typename T>
	static std::string getJNISignature(T x) {
		// This template should never be instantiated
		static_assert(sizeof(x) == 0, "Unsupported argument type");
		return "";
	}

	template <typename T, typename... Ts>
	static std::string getJNISignature(T x, Ts... xs) {
		return getJNISignature(x) + getJNISignature(xs...);
	}

};

NS_GODOT_END

#endif // JNIHELPER_H
