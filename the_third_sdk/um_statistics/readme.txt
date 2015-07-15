1.安卓平台

android权限在AndroidManifest.xml添加下列四行：

<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE"></uses-permission>
<uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
<uses-permission android:name="android.permission.INTERNET"></uses-permission>
<uses-permission android:name="android.permission.READ_PHONE_STATE"></uses-permission>

在android的android.mk中添加android/umSDK.cpp路径


并在ecplise倒入a’droid/libUmSDK工程，在CrossApp工程入口java文件onCreate中添加AnalyticsHome.init(this);即可


2.iOS平台

添加依赖框架(Framework)和编译器选项
TARGETS-->Build Phases-->Link Binary With Libraries--> + -->libz.dylib

添加ios文件夹 与 umSDK.h到工程中

3.其他

添加umSDK.cpp 与umSDK.h 到工程中，保证可以编译通过



C++层

调用宏MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL(appkey, channel) 启动sdk



