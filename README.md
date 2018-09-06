# GDFireBase-CPP
Godot FireBase module based on `C++ SDK`

This is not complete so using this module is not recommended, 

# Clone
```
git clone https://github.com/FrogSquare/GDFireBase-CPP <GODOT-ROOT>/modules/gdfirebas
```

# How To
First you need to download `FireBase` c++ sdk from [Here](https://firebase.google.com/docs/cpp/setup) and extract that under `<GODOT-ROOT>/modules/gdfirebase/` as `firebase_cpp_sdk`

# GDScript - creating a GDFireBase instance;
```
var firebase = null

func _ready():
    firebase = GDFireBase.new()
```

# GodotFireBase: copy `godot-firebase-config.json` to your projects root directord.
GodotFireBase config file, By default every feature is disabled.

```
{
    "AdMob" : true,
    "Authentication" : true,
    "Invites" : true,
    "RemoteConfig" : true,
    "Notification" : true,
    "Storage" : true,
    "Firestore" : true,

    "AuthConf" :
    {
        "Google" : true,
        "Twitter" : true,
        "Facebook" : true,
        "FacebookAppId" : "1234566789875"
    },

    "Ads" :
    {
        "AppId": "YOUR_APP_ID_HERE",

        "BannerAd" : true,
        "BannerGravity" : "BOTTOM",
        "BannerAdId" : "",

        "InterstitialAd" : true,
        "InterstitialAdId" : "",

        "RewardedVideoAd" : true,
        "RewardedVideoAdId" : "",

        "TestAds" : false
    }
}
```

And initializing Firebase, to initialize with `Dictionary`
```
    firebase.init(conf_dictionary, get_instance_ID())
```

or by using a `.json` file

```
    firebase.init_with_file("res://godot-firebase-config.json", get_instance_ID())
```

# Build Error
```
The library com.google.android.gms:play-services-measurement-base is being requested by various other libraries at [[15.0.4,15.0.4], [16.0.2,16.0.2]], but resolves to 16.0.2. Disable the plugin and check your dependencies tree using ./gradlew :app:dependencies
```

To fix the error add the following to `platform/android/build.gradle.template`
```
com.google.gms.googleservices.GoogleServicesPlugin.config.disableVersionCheck = true
```

