
def can_build(plat):
	return False;
	#return (plat == "android" or plat == "x11" or plat == "iphone")

def configure(env):
	if env["platform"] == "android":
		firebase_sdk_libs = ''

		if env["android_arch"] == "armv6":
			firebase_sdk_libs = \
			'#modules/gdfirebase/firebase_cpp_sdk/libs/android/armeabi/c++/'
		elif env["android_arch"] == "armv7":
			firebase_sdk_libs = \
			'#modules/gdfirebase/firebase_cpp_sdk/libs/android/armeabi-v7a/c++/'
		elif env["android_arch"] == "x86":
			firebase_sdk_libs = \
			'#modules/gdfirebase/firebase_cpp_sdk/libs/android/x86/c++/'
		else:
			print("ERR: Arch not supported by godot..!");
			return;

		print(firebase_sdk_libs);

		env.Append(LIBPATH=[firebase_sdk_libs])
		env.Append(LIBS=['analytics', 'app'])

		env.android_add_maven_repository("url 'https://oss.sonatype.org/content/repositories/snapshots'")

		env.android_add_gradle_classpath("com.google.gms:google-services:3.0.0")
		env.android_add_gradle_plugin("com.google.gms.google-services")

		env.android_add_dependency("compile 'com.google.firebase:firebase-core:10.0.1'")
		env.android_add_dependency("compile 'com.google.android.gms:play-services-base:10.0.1'")

		env.android_add_dependency("compile 'commons-codec:commons-codec:1.10'")

		env.android_add_default_config("applicationId 'com.froglogics.dotsndots'")

	elif env["platform"] == "iphone":
		firebase_sdk_libs = ''

		if env["arch"] == "arm":
			firebase_sdk_libs = '#modules/gdfirebase/firebase_cpp_sdk/libs/ios/i386'
		elif env["arch"] == "arm64":
			firebase_sdk_libs = '#modules/gdfirebase/firebase_cpp_sdk/libs/ios/arm64'
		elif env["arch"] == "x86":
			firebase_sdk_libs = '#modules/gdfirebase/firebase_cpp_sdk/libs/ios/x86_64'
		else:
			print("ERR: Arch not supported by godot..!");
			return;

		# Not tested (Need Help)
		firebase_sdk_libs = '#modules/gdfirebase/firebase_cpp_sdk/libs/ios/arm64/lib'

		env.Append(LIBPATH=[firebase_sdk_libs])
		env.Append(LIBS=['analytics', 'app'])

		env.Append(LINKFLAGS=['-ObjC', '-framework', 'Firebase'])
		env.Append(FRAMEWORKPATH=['modules/gdfirebase/ios/lib'])
		
pass
