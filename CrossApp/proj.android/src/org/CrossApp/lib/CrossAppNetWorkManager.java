package org.CrossApp.lib;

import java.util.List;

import android.R.integer;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.util.Log;

@SuppressLint("DefaultLocale")
public class CrossAppNetWorkManager {

    public static final String TAG = "CrossAppNetWorkManager" ;

    private static Activity s_pActivity;

    private static WifiManager mWifiManager;

    private static WifiInfo mWifiInfo;

    private static List<ScanResult> mWifiList;

    private static List<WifiConfiguration> mWifiConfigurations;

    WifiLock mWifiLock;

    public static void setContext(Activity activity)
    {
        mWifiManager=(WifiManager) activity.getSystemService(Context.WIFI_SERVICE);

        mWifiInfo=mWifiManager.getConnectionInfo();

        s_pActivity = activity;
    }

    public static int getAPNType()
    {
        int netType = -1;

        ConnectivityManager connMgr = (ConnectivityManager) s_pActivity.getSystemService(Context.CONNECTIVITY_SERVICE);

        NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();

        if(networkInfo==null)
        {
            return netType;
        }

        int nType = networkInfo.getType();

        if(nType==ConnectivityManager.TYPE_MOBILE)
        {
            if(networkInfo.getExtraInfo().toLowerCase().equals("cmnet"))
            {
                netType = 2;
            }
            else
            {
                netType = 3;
            }
        }
        else if(nType==ConnectivityManager.TYPE_WIFI)
        {
            netType = 1;
        }
        return netType;
    }

    public static int isNetWorkAvailble()
    {
        int type = 0;
        ConnectivityManager connManager = (ConnectivityManager) s_pActivity.getSystemService(Context.CONNECTIVITY_SERVICE);

        // 获取代表联网状态的NetWorkInfo对象
        NetworkInfo networkInfo = connManager.getActiveNetworkInfo();
        if(networkInfo == null)
        {
            return type;
        }

        // 获取当前的网络连接是否可用
        boolean available = networkInfo.isAvailable();
        if(available)
        {
            type = 1;
        }

        return type;
    }

    public void openWifi()
    {
        if(!mWifiManager.isWifiEnabled())
        {
            mWifiManager.setWifiEnabled(true);
        }
    }

    public void closeWifi(){
        if(!mWifiManager.isWifiEnabled()){
            mWifiManager.setWifiEnabled(false);
        }
    }

    public int checkState()
    {
        return mWifiManager.getWifiState();
    }

    public void acquireWifiLock()
    {
        mWifiLock.acquire();
    }

    public void releaseWifiLock()
    {
        if(mWifiLock.isHeld()){
            mWifiLock.acquire();
        }
    }

    public void createWifiLock()
    {
        mWifiLock=mWifiManager.createWifiLock("test");
    }

    public List<WifiConfiguration> getConfiguration()
    {
        return mWifiConfigurations;
    }

    public void connetionConfiguration(int index)
    {
        if(index>mWifiConfigurations.size())
        {
            return ;
        }

        mWifiManager.enableNetwork(mWifiConfigurations.get(index).networkId, true);
    }
    public static void startScan()
    {
        mWifiManager.startScan();

        mWifiList=mWifiManager.getScanResults();

        mWifiConfigurations=mWifiManager.getConfiguredNetworks();
    }

    public static List<ScanResult> getWifiList()
    {
        return mWifiList;
    }

    public static WifiInfo getWifiConnectionInfo()
    {
        WifiInfo mInfo = mWifiManager.getConnectionInfo();
        return mInfo;
    }

    public StringBuffer lookUpScan()
    {
        StringBuffer sb=new StringBuffer();

        for(int i=0;i<mWifiList.size();i++)
        {
            sb.append("Index_" + new Integer(i + 1).toString() + ":");

            sb.append((mWifiList.get(i)).toString()).append("\n");
        }
        return sb;
    }

    public String getMacAddress()
    {
        return (mWifiInfo==null)?"NULL":mWifiInfo.getMacAddress();
    }

    public String getBSSID()
    {
        return (mWifiInfo==null)?"NULL":mWifiInfo.getBSSID();
    }

    public int getIpAddress()
    {
        return (mWifiInfo==null)?0:mWifiInfo.getIpAddress();
    }

    public int getNetWordId()
    {
        Log.i(TAG,"networid :"+mWifiInfo.getNetworkId());

        return (mWifiInfo==null)?0:mWifiInfo.getNetworkId();
    }

    public String getWifiInfo()
    {
        return (mWifiInfo==null)?"NULL":mWifiInfo.toString();
    }

    public void addNetWork(WifiConfiguration configuration)
    {
        int wcgId=mWifiManager.addNetwork(configuration);

        mWifiManager.enableNetwork(wcgId, true);
    }

    public void disConnectionWifi(int netId)
    {
        mWifiManager.disableNetwork(netId);

        mWifiManager.disconnect();
    }
}
