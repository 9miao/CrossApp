package com.component.impl;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import android.Manifest;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.telephony.TelephonyManager;
import android.util.Log;

public class PhoneInfo {

	private static final String TAG = PhoneInfo.class.getSimpleName();
	private static final String FILE_MEMORY = "/proc/meminfo";
	private static final String FILE_CPU = "/proc/cpuinfo";

	public String mIMEI = null;
	public int mPhoneType = 0;
	public String mSysVersion = null;
	public String mNetWorkCountryIso = null;
	public String mNetWorkOperator = null;
	public String mNetWorkOperatorName = null;
	public int mNetWorkType = 0;
	public boolean mIsOnLine = false;
	public String mConnectTypeName = null;
	public long mFreeMem = 0;
	public long mTotalMem = 0;
	public String mMem = null;
	public String mCupInfo = null;
	public String mProductName = null;
	public String mModelName = null;
	public String mManufacturerName = null;
	public String mIP = null;
	public String mISP = null;
	public String mDeviceVersion = null;

	private static PhoneInfo instance;
	private static Activity context;

	public static PhoneInfo getInstance() {
		if (instance == null)
			instance = new PhoneInfo();
		return instance;
	}

	public PhoneInfo() {
		init();
	}

	public static void init(Activity activity) {
		PhoneInfo.context = activity;
	}

	// 设备版本号
	public String getIMEI() {
		try {
			TelephonyManager manager = (TelephonyManager) context
					.getSystemService(Activity.TELEPHONY_SERVICE);
			return manager.getDeviceId();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public int getPhoneType() {
		try {
			TelephonyManager manager = (TelephonyManager) context
					.getSystemService(Activity.TELEPHONY_SERVICE);
			return manager.getPhoneType();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}

	// ISP
	public String getISP() {
		try {

			TelephonyManager manager = (TelephonyManager) context
					.getSystemService(Activity.TELEPHONY_SERVICE);

			return manager.getSimOperatorName();
		} catch (Exception e) {
			Log.d("qhero", "get isp error!!", e);
		}

		return null;
	}

	// DeviceVersion
	public String getDeviceVersion() {
		try {
			TelephonyManager manager = (TelephonyManager) context
					.getSystemService(Activity.TELEPHONY_SERVICE);
			return manager.getDeviceSoftwareVersion();
		} catch (Exception e) {
			Log.d("qhero", "get device version error!!", e);
		}
		return null;
	}

	// 系统版本号
	public String getSysVersion() {
		PackageManager packageManager = context.getPackageManager();
		PackageInfo packInfo = null;
		try {
			packInfo = packageManager.getPackageInfo(context.getPackageName(),
					0);
			return "Android:" + packInfo.versionName;
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return null;
	}

	public String getNetWorkCountryIso() {
		try {
			TelephonyManager manager = (TelephonyManager) context
					.getSystemService(Activity.TELEPHONY_SERVICE);
			return manager.getNetworkCountryIso();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public String getNetWorkOperator() {
		try {
			TelephonyManager manager = (TelephonyManager) context
					.getSystemService(Activity.TELEPHONY_SERVICE);
			return manager.getNetworkOperator();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public String getNetWorkOperatorName() {
		try {
			TelephonyManager manager = (TelephonyManager) context
					.getSystemService(Activity.TELEPHONY_SERVICE);
			return manager.getNetworkOperatorName();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public int getNetworkType() {
		try {
			TelephonyManager manager = (TelephonyManager) context
					.getSystemService(Activity.TELEPHONY_SERVICE);
			return manager.getNetworkType();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}

	public boolean isOnline() {
		ConnectivityManager manager = (ConnectivityManager) context
				.getSystemService(Activity.CONNECTIVITY_SERVICE);
		NetworkInfo info = manager.getActiveNetworkInfo();
		if (info != null && info.isConnected()) {
			return true;
		}
		return false;
	}

	public String getConnectTypeName() {
		if (!isOnline()) {
			return "OFFLINE";

		}
		ConnectivityManager manager = (ConnectivityManager) context
				.getSystemService(Activity.CONNECTIVITY_SERVICE);
		NetworkInfo info = manager.getActiveNetworkInfo();
		if (info != null) {
			return info.getTypeName();
		} else {
			return "OFFLINE";
		}
	}

	// 可用内存
	public long getFreeMem() {
		ActivityManager manager = (ActivityManager) context
				.getSystemService(Activity.ACTIVITY_SERVICE);
		MemoryInfo info = new MemoryInfo();
		manager.getMemoryInfo(info);
		long free = info.availMem / 1024 / 1024;
		return free;
	}

	// 总内存
	public long getTotalMem() {
		try {
			FileReader fr = new FileReader(FILE_MEMORY);
			BufferedReader br = new BufferedReader(fr);
			String text = br.readLine();
			String[] array = text.split("\\s+");
			return Long.valueOf(array[1]) / 1024;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return 0;
	}

	public String getMem() {
		return Long.toString(mTotalMem) + "M";
	}

	// cpu信息
	public String getCpuInfo() {
		try {
			FileReader fr = new FileReader(FILE_CPU);
			BufferedReader br = new BufferedReader(fr);
			String text = br.readLine();
			String[] array = text.split(":\\s+", 2);
			// for (int i = 0; i < array.length; i++) {
			// Log.w(TAG, " .....  " + array[i]);
			// }
			// Log.w(TAG, text);
			return array[1];
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}

	public String getProductName() {
		return Build.PRODUCT;
	}

	public String getModelName() {
		return Build.MODEL;
	}

	public String getManufacturerName() {
		return Build.MANUFACTURER;
	}

	// 取得IP地址
	public String getIPAddress() {
		//这段代码有问题，光取wifi的IP是没有用的，没考虑移动网络的情况，而且这只能取得本地IP，毫无意义 by Clark 2013/11/11
		// WifiManager wifiMgr = (WifiManager) context
		// .getSystemService(Context.WIFI_SERVICE);
		// if (!wifiMgr.isWifiEnabled()) {
		// wifiMgr.setWifiEnabled(true);
		// }
		// WifiInfo wifiInfo = wifiMgr.getConnectionInfo();
		// int ip = wifiInfo.getIpAddress();
		// return intToIP(ip);
		return "0.0.0.0";
	}

	private String intToIP(final int i) {
		return (i & 0xFF) + "." + ((i >> 8) & 0xFF) + "." + ((i >> 16) & 0xFF)
				+ "." + (i >> 24 & 0xFF);
	}

	private void init() {
		mIMEI = getIMEI();
		mPhoneType = getPhoneType();
		mSysVersion = getSysVersion();
		mNetWorkCountryIso = getNetWorkCountryIso();
		mNetWorkOperator = getNetWorkOperator();
		mNetWorkOperatorName = getNetWorkOperatorName();
		mNetWorkType = getNetworkType();
		mIsOnLine = isOnline();
		mConnectTypeName = getConnectTypeName();
		mFreeMem = getFreeMem();
		mTotalMem = getTotalMem();
		mCupInfo = getCpuInfo();
		mProductName = getProductName();
		mModelName = getModelName();
		mManufacturerName = getManufacturerName();
		mIP = getIPAddress();
		mISP = getISP();
		mDeviceVersion = getDeviceVersion();
	}

	@Override
	public String toString() {
		StringBuilder builder = new StringBuilder();
		builder.append("IMEI : " + mIMEI + "\n");
		builder.append("mPhoneType : " + mPhoneType + "\n");
		builder.append("mSysVersion : " + mSysVersion + "\n");
		builder.append("mNetWorkCountryIso : " + mNetWorkCountryIso + "\n");
		builder.append("mNetWorkOperator : " + mNetWorkOperator + "\n");
		builder.append("mNetWorkOperatorName : " + mNetWorkOperatorName + "\n");
		builder.append("mNetWorkType : " + mNetWorkType + "\n");
		builder.append("mIsOnLine : " + mIsOnLine + "\n");
		builder.append("mConnectTypeName : " + mConnectTypeName + "\n");
		builder.append("mFreeMem : " + mMem + "M\n");
		builder.append("mTotalMem : " + mTotalMem + "M\n");
		builder.append("mCupInfo : " + mCupInfo + "\n");
		builder.append("mProductName : " + mProductName + "\n");
		builder.append("mModelName : " + mModelName + "\n");
		builder.append("mISP : " + mISP + "\n");
		builder.append("mManufacturerName : " + mManufacturerName + "\n");
		builder.append("ip : " + mIP + "\n");
		return builder.toString();
	}
}