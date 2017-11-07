
# Update this to customize the module
_config = {
"Analytics"      : True,
"AdMob"          : True,
"Invites"        : True,
"RemoteConfig"   : True,
"Notification"   : True,
"Storage"        : True,
"Firestore"      : False,

"Authentication" : True,
"AuthGoogle"     : True,
"AuthFacebook"   : True,
"AuthTwitter"    : True
}

linkflags = ['app', 'analytics']

def can_build(plat):
    #return False;
    return (plat == "android" or plat == "x11" or plat == "iphone")

def configure(env):
    if env["platform"] == "android" or env["platform"] == "iphone":
        if _config["AdMob"]:
            if env["platform"] == "android":
                env.android_add_dependency("compile 'com.google.firebase:firebase-ads:10.0.1'")

            linkflags.append('admob')

        if _config["RemoteConfig"]:
            if env["platform"] == "android":
                env.android_add_dependency("compile 'com.google.firebase:firebase-config:10.0.1'")

            linkflags.append('remote_config')

        if _config["Notification"]:
            if env["platform"] == "android":
                env.android_add_dependency("compile 'com.google.firebase:firebase-messaging:10.0.1'")
                env.android_add_dependency("compile 'com.firebase:firebase-jobdispatcher:0.5.2'")

            linkflags.append('messaging')

        if _config["Invites"]:
            if env["platform"] == "android":
                env.android_add_dependency("compile 'com.google.firebase:firebase-invites:10.0.1'")

            linkflags.append('invites')

        if _config["Storage"]:
            if env["platform"] == "android":
                env.android_add_dependency("compile 'com.google.firebase:firebase-storage:10.0.1'")

            linkflags.append('storage')

    if env["platform"] == "android":
        env.android_add_maven_repository("url 'https://oss.sonatype.org/content/repositories/snapshots'")
        env.android_add_flat_dir("#modules/gdfirebase/firebase_cpp_sdk/libs/android/")
        env.android_add_gradle_classpath("com.google.gms:google-services:3.0.0")
        env.android_add_gradle_plugin("com.google.gms.google-services")

        env.android_add_dependency("compile 'com.google.firebase:firebase-core:10.0.1'")
        env.android_add_dependency("compile 'com.google.android.gms:play-services-base:10.0.1'")
        env.android_add_dependency("compile 'commons-codec:commons-codec:1.10'")

        env.android_add_default_config("applicationId 'com.froglogics.dotsndots'")

        ARCH = "armeabi" 
        if env["android_arch"] == "armv7": ARCH = "armeabi-v7a"
        elif env["android_arch"] == "x86": ARCH = "x86"
        else:
            print("ERR: Arch not supported by godot..!")
            return

        firebase_sdk_libs = '#modules/gdfirebase/firebase_cpp_sdk/libs/android/'+ ARCH +'/c++/'

        env.Append(LIBPATH=[firebase_sdk_libs])
        env.Append(LIBS=linkflags)
    elif env["platform"] == "iphone":
        ARCH = 'i386'
        if env["arch"] == "arm64": ARCH = "arm64"
        elif env["arch"] == "x86": ARCH = "x86_64"
        else:
            print("ERR: Arch not supported by godot..!")
            return

        firebase_sdk_libs = '#modules/gdfirebase/firebase_cpp_sdk/libs/ios/' + ARCH

        # Not tested (Need Help)
        env.Append(LIBPATH=[firebase_sdk_libs])
        env.Append(LIBS=linkflags)

        env.Append(LINKFLAGS=['-ObjC', '-framework', 'Firebase', '-framework', 'FirebaseAdmob'])
        env.Append(FRAMEWORKPATH=['modules/gdfirebase/frameworks/ios/'+ARCH])
