
import sys
import json
import os
import re
import shutil

from colors import *

# Set your Android app ID
p_app_id = "games.misu.greatalchemist"
p_firebase_sdk = "/firebase_cpp_sdk/libs"

# Update this to customize the module
_config = {
"Analytics"      : True,
"AdMob"          : True,
"Invites"        : False,
"RemoteConfig"   : True,
"Notification"   : True,
"Storage"        : True,
"Firestore"      : True,

"Authentication" : True,
"AuthGoogle"     : True,
"AuthFacebook"   : True,
"AuthTwitter"    : True
}

cpp_defines = ['GD_FIREBASE_ANALYTICS']
linkflags = ['app', 'analytics']

def can_build(plat):
    return (plat == "android" or plat == "x11" or plat == "iphone")

def configure(env):
    cur_dir = os.path.dirname(os.path.abspath(__file__))
    if _config["AdMob"]:
        if env["platform"] == "android":
            env.android_add_dependency("compile 'com.google.firebase:firebase-ads:15.0.1'")

        cpp_defines.append('GD_FIREBASE_ADMOB')
        linkflags.append('admob')

    if _config["RemoteConfig"]:
        if env["platform"] == "android":
            env.android_add_dependency("compile 'com.google.firebase:firebase-config:16.0.0'")

        cpp_defines.append('GD_FIREBASE_REMOTECONFIG')
        linkflags.append('remote_config')

    if _config["Notification"]:
        if env["platform"] == "android":
            env.android_add_dependency("compile 'com.google.firebase:firebase-messaging:17.3.0'")
            env.android_add_dependency("compile 'com.google.firebase.messaging.cpp:firebase_messaging_cpp@aar'")
            env.android_add_dependency("compile 'com.firebase:firebase-jobdispatcher:0.8.5'")

        cpp_defines.append('GD_FIREBASE_NOTIFICATION')
        linkflags.append('messaging')

    if _config["Invites"]:
        if env["platform"] == "android":
            env.android_add_dependency("compile 'com.google.firebase:firebase-invites:16.0.3'")

        cpp_defines.append('GD_FIREBASE_INVITES')
        linkflags.append('invites')

    if _config["Storage"]:
        if env["platform"] == "android":
            env.android_add_dependency("compile 'com.google.firebase:firebase-storage:16.0.1'")

        cpp_defines.append('GD_FIREBASE_STORAGE')
        linkflags.append('storage')

    if _config["Authentication"]:
        if env["platform"] == "android":
            env.android_add_dependency("compile 'com.google.firebase:firebase-auth:16.0.3'")
            
        cpp_defines.append('GD_FIREBASE_AUTHENTICATION')
        linkflags.append('auth')

    env.Append(CPPDEFINES=cpp_defines);

    if env["platform"] == "android":
        env.android_add_maven_repository("url 'https://maven.google.com'")
        env.android_add_maven_repository("url 'https://oss.sonatype.org/content/repositories/snapshots'")

        env.android_add_flat_dir(cur_dir + p_firebase_sdk + "/android/")
        env.android_add_gradle_classpath("com.google.gms:google-services:4.1.0")
        env.android_add_gradle_plugin("com.google.gms.google-services")

        env.android_add_dependency("compile 'com.google.firebase:firebase-core:16.0.1'")
        env.android_add_dependency("compile 'com.google.firebase:firebase-analytics:16.0.1'")
        env.android_add_dependency("compile 'com.google.android.gms:play-services-base:15.0.1'")
        env.android_add_dependency("compile 'commons-codec:commons-codec:1.10'")

        env.android_add_default_config("applicationId '"+ p_app_id +"'")

        ARCH = "armeabi" 
        if env["android_arch"] == "armv7": ARCH = "armeabi-v7a"
        elif env["android_arch"] == "x86": ARCH = "x86"
        else:
            print("ERR: Arch not supported by godot..!")
            return

        firebase_sdk_libs = cur_dir + p_firebase_sdk + '/android/'+ ARCH +'/c++/'

        _linkflags = []
        for l in linkflags:
            _linkflags.append("firebase_" + l)
            pass

        print(firebase_sdk_libs)
        print(GREEN + "FireBase: " + RESET + "[" + _linkflags[1:-1] + "]")

        env.Append(LIBPATH=[firebase_sdk_libs])
        env.Append(LIBS=_linkflags)
    elif env["platform"] == "iphone":
        ARCH = 'i386'
        if env["arch"] == "arm64": ARCH = "arm64"
        elif env["arch"] == "x86": ARCH = "x86_64"
        else:
            print("ERR: Arch not supported by godot..!")
            return

        firebase_sdk_libs = cur_dir + p_firebase_sdk + '/ios/'+ ARCH

        # Not tested (Need Help)
        env.Append(LIBPATH=[firebase_sdk_libs])
        env.Append(LIBS=linkflags)

        env.Append(LINKFLAGS=['-ObjC', '-framework', 'Firebase', '-framework', 'FirebaseAdmob'])
        env.Append(FRAMEWORKPATH=['modules/gdfirebase/frameworks/ios/'+ARCH])

    pass

