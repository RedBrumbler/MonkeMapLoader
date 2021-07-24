& ./build.ps1

& adb push libs/arm64-v8a/libmonkemaploader.so /sdcard/Android/data/com.AnotherAxiom.GorillaTag/files/mods/libmonkemaploader.so
Wait-Sleep -Milliseconds 500
& adb shell am force-stop com.AnotherAxiom.GorillaTag
& adb shell am start com.AnotherAxiom.GorillaTag/com.unity3d.player.UnityPlayerActivity
& ./log.ps1
