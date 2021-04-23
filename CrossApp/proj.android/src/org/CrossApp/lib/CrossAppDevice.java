package org.CrossApp.lib;

import java.util.ArrayList;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import org.CrossApp.lib.CrossAppBattery;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.database.Cursor;
import android.net.Uri;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiInfo;
import android.view.WindowManager;

@SuppressLint("SimpleDateFormat")
public abstract class CrossAppDevice extends Activity {

    private static Activity s_pContext;

    private static CrossAppDevice s_pDevice;

    public static Uri photoUri;

    private static AlertDialog mDialog = null;

    static String s;

    native static void getWifiList(ArrayList<CrossAppCustomScanResult> s);

    public static List<ScanResult> list;

    public static ScanResult mScanResult;

    public static CrossAppCustomScanResult cScanResult;

    public static int currentBattery = 0;

    private static long lastUpdateTime;

    private static final int UPTATE_INTERVAL_TIME = 1000;

    private static float mInterval;

    private static int mIndex = 0;
    private static CrossAppBattery battery = null;

    public CrossAppDevice(final Activity context) {
        s_pDevice = this;

        setContext(context);

    }

    public static Activity getContext() {
        return CrossAppDevice.s_pContext;
    }

    public static CrossAppDevice getDevice() {
        return CrossAppDevice.s_pDevice;
    }

    public static void setContext(Activity s_pContext) {

        CrossAppDevice.s_pContext = s_pContext;

        IntentFilter intentFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);

        battery = new CrossAppBattery();

        s_pContext.registerReceiver(battery, intentFilter);
    }

    public static CrossAppBattery getCrossAppBattery() {
        return battery;
    }

    public static void ShowDlg(String[] args) {
        s_pContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mDialog.show();
            }
        });
    }

    public static String getSystemVersion() {
        return android.os.Build.VERSION.RELEASE;
    }

    public static String getAppVersion() {
        String versionName = "";

        try {

            PackageInfo info = s_pContext.getPackageManager().getPackageInfo(s_pContext.getPackageName(), 0);

            versionName = info.versionName;

        } catch (NameNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return versionName;
    }

    /*
     *  CrossAppPersonList
     * */
    public static void getPersonList() {
        s_pContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppPersonList.getPersonList();
            }
        });
    }

    public static void setIdleTimerDisabled(final int type) {
        s_pContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (type == 1) {
                    s_pContext.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
                } else {

                    s_pContext.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
                }
            }
        });
    }

    public static void CAImageCapture(final int type) {
        s_pContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppNativeTool.CAImageCapture(type);
            }
        });
    }

    public static void CAVideoCapture() {
        CrossAppNativeTool.CAVideoCapture();
    }

    public static void CAVideoAlbum() {
        CrossAppNativeTool.CAVideoAlbum();
    }

    public static void CAImageAlbum(final int type) {
        s_pContext.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppNativeTool.CAImageAlbum(type);
            }
        });
    }

    public static void browserOpenURL(final String url) {
        CrossAppNativeTool.browserOpenURL(url);
    }

    public static int getBatteryLevel() {
        return CrossAppBattery.getBatteryLevel();
    }

    public static int getAPNType() {
        return CrossAppNetWorkManager.getAPNType();
    }

    public static int isNetWorkAvailble() {
        return CrossAppNetWorkManager.isNetWorkAvailble();
    }

    // Gyroscope
    public static void enableGyroscope() {

        CrossAppHelper.setGyroscopeEnable(true);

        CrossAppHelper.getCrossAppGyroscope().enable();
    }

    public static void setGyroscopeInterval(float interval) {
        CrossAppHelper.getCrossAppGyroscope().setInterval(interval);
    }

    public static void disableGyroscope() {
        CrossAppHelper.setGyroscopeEnable(false);
        CrossAppHelper.getCrossAppGyroscope().disable();
    }


    public void onResume() {
        if (CrossAppHelper.isGyroscopeEnabled()) {
            CrossAppHelper.getCrossAppGyroscope().enable();
        }
    }

    public void onPause() {
        if (CrossAppHelper.isGyroscopeEnabled()) {
            CrossAppHelper.getCrossAppGyroscope().disable();
        }
    }

    public void initBlueTooth() {
        CrossAppBlueTooth.initBlueTooth();
    }


    public void setBlueToothActionType(int type) {
        CrossAppBlueTooth.setBlueToothActionType(type);
    }


    public static int getScreenBrightness() {
        return CrossAppNativeTool.getScreenBrightness();
    }

    public static void setScreenBrightness(int value) {
        CrossAppNativeTool.setScreenBrightness(value);
    }

    public static void getWifiList() {
        CrossAppNetWorkManager.setContext(CrossAppActivity.getContext());

        CrossAppNetWorkManager.startScan();

        list = CrossAppNetWorkManager.getWifiList();

        ArrayList<CrossAppCustomScanResult> cList = new ArrayList<CrossAppCustomScanResult>();


        if (list != null) {
            for (int i = 0; i < list.size(); i++) {
                mScanResult = list.get(i);

                cScanResult = new CrossAppCustomScanResult(mScanResult.SSID, mScanResult.BSSID, mScanResult.level);

                if (cScanResult != null) {
                    cList.add(cScanResult);
                }
            }
            if (cList.size() != 0) {
                getWifiList(cList);
            }
        }
    }

    public static CrossAppCustomScanResult getWifiConnectionInfo() {
        WifiInfo mWifiInfo = CrossAppNetWorkManager.getWifiConnectionInfo();

        CrossAppCustomScanResult connectionInfo = null;

        if (mWifiInfo != null) {
            connectionInfo = new CrossAppCustomScanResult(mWifiInfo.getSSID(), mWifiInfo.getBSSID(), 0);
        }

        return connectionInfo;
    }

    public static void setVolum(float sender, int type) {
        CrossAppVolumeControl.setVolum(sender, type);
    }

    public static float getVolum(int type) {
        return CrossAppVolumeControl.getVolum(type);
    }


    @TargetApi(16)
    public static void showNotification(String title, String content) {

        NotificationManager manager = (NotificationManager) s_pContext.getSystemService(Context.NOTIFICATION_SERVICE);

        ComponentName name = new ComponentName(s_pContext.getPackageName(), s_pContext.getPackageName() + "." + s_pContext.getLocalClassName());

        Intent intent = new Intent(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        intent.setClass(s_pContext, CrossAppActivity.class);
        intent.setPackage("com.tencent.mobileqq");
        intent.setAction(Intent.ACTION_MAIN);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
        intent.setComponent(name);

        PendingIntent pendingIntent = PendingIntent.getActivity(CrossAppActivity.getContext(), 0, intent, 0);

        Notification notification = new Notification.Builder(s_pContext)
                .setAutoCancel(true)
                .setSmallIcon(0x7f020000)
                .setTicker(title)
                .setContentTitle(title)
                .setContentText(content)
                .setContentIntent(pendingIntent)
                .setWhen(System.currentTimeMillis())
                .setOngoing(false)
                .build();

        notification.flags = Notification.FLAG_AUTO_CANCEL;
        notification.defaults = Notification.DEFAULT_SOUND;

        ++mIndex;

        manager.notify(mIndex, notification);
    }

    public static void sendLocalNotification(final String title, final String content, int leftMessage) {
        showNotification(title, content);
    }

    public static boolean isGooglePhotosUri(Uri uri) {
        return "com.google.android.apps.photos.content".equals(uri.getAuthority());
    }

    public static String getSaveImagePath() {
        return CrossAppNativeTool.getSaveImagePath();
    }


    public static void UpdateCamera(final String url) {
        CrossAppNativeTool.UpdateCamera(url);
    }

    /**
     * Get the value of the data column for this Uri. This is useful for
     * MediaStore Uris, and other file-based ContentProviders.
     *
     * @param context       The context.
     * @param uri           The Uri to query.
     * @param selection     (Optional) Filter used in the query.
     * @param selectionArgs (Optional) Selection arguments used in the query.
     * @return The value of the _data column, which is typically a file path.
     */
    public static String getDataColumn(Context context, Uri uri, String selection, String[] selectionArgs) {
        Cursor cursor = null;

        final String column = "_data";

        final String[] projection = {column};

        try {

            cursor = context.getContentResolver().query(uri, projection, selection, selectionArgs, null);

            if (cursor != null && cursor.moveToFirst()) {
                final int index = cursor.getColumnIndexOrThrow(column);

                return cursor.getString(index);
            }
        } finally {
            if (cursor != null)
                cursor.close();
        }
        return null;
    }


    /**
     * @param uri The Uri to check.
     * @return Whether the Uri authority is ExternalStorageProvider.
     */
    public static boolean isExternalStorageDocument(Uri uri) {
        return "com.android.externalstorage.documents".equals(uri.getAuthority());
    }

    /**
     * @param uri The Uri to check.
     * @return Whether the Uri authority is DownloadsProvider.
     */
    public static boolean isDownloadsDocument(Uri uri) {
        return "com.android.providers.downloads.documents".equals(uri.getAuthority());
    }

    /**
     * @param uri The Uri to check.
     * @return Whether the Uri authority is MediaProvider.
     */
    public static boolean isMediaDocument(Uri uri) {
        return "com.android.providers.media.documents".equals(uri.getAuthority());
    }

    /**
     * @param uri The Uri to check. 
     * @return Whether the Uri authority is Google Photos. 
     */

}
