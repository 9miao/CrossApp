package org.CrossApp.lib;

import java.util.List;
//import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;

public class AndroidNetWorkManager {
	private static Activity s_pActivity;
	//锟斤拷锟斤拷一锟斤拷WifiManager锟斤拷锟斤拷  
    private static WifiManager mWifiManager;  
    //锟斤拷锟斤拷一锟斤拷WifiInfo锟斤拷锟斤拷  
    private static WifiInfo mWifiInfo;  
    //扫锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷斜锟�  
    private static List<ScanResult> mWifiList;
    
  //锟斤拷锟斤拷锟斤拷锟斤拷锟叫憋拷  
    private static List<WifiConfiguration> mWifiConfigurations;  
    
    WifiLock mWifiLock;
    
	public static void setContext(Activity activity)
	{
		//取锟斤拷WifiManager锟斤拷锟斤拷  
        mWifiManager=(WifiManager) activity.getSystemService(Context.WIFI_SERVICE);  
        //取锟斤拷WifiInfo锟斤拷锟斤拷  
        mWifiInfo=mWifiManager.getConnectionInfo(); 
        
		s_pActivity = activity;
	}
	
	public static int getAPNType()
	{
		int netType = -1;  
		 
		ConnectivityManager connMgr = (ConnectivityManager) s_pActivity.getSystemService(Context.CONNECTIVITY_SERVICE); 
		
		NetworkInfo networkInfo = connMgr.getActiveNetworkInfo(); 
		 
		if(networkInfo==null){ 
		 
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
	
	public void openWifi()
	{  
        if(!mWifiManager.isWifiEnabled())
        {  
            mWifiManager.setWifiEnabled(true);  
        }  
    }  
    //锟截憋拷wifi  
    public void closeWifi(){  
        if(!mWifiManager.isWifiEnabled()){  
            mWifiManager.setWifiEnabled(false);  
        }  
    }  
     // 锟斤拷榈鼻�wifi状态    
    public int checkState()
    {    
        return mWifiManager.getWifiState();    
    }    
    //锟斤拷锟斤拷wifiLock  
    public void acquireWifiLock()
    {  
        mWifiLock.acquire();  
    }  
    //锟斤拷锟斤拷wifiLock  
    public void releaseWifiLock()
    {  
        //锟叫讹拷锟角凤拷锟斤拷锟斤拷  
        if(mWifiLock.isHeld()){  
            mWifiLock.acquire();  
        }  
    }  
    //锟斤拷锟斤拷一锟斤拷wifiLock  
    public void createWifiLock()
    {  
        mWifiLock=mWifiManager.createWifiLock("test");  
    }
    //锟矫碉拷锟斤拷锟矫好碉拷锟斤拷锟斤拷  
    public List<WifiConfiguration> getConfiguration()
    {  
        return mWifiConfigurations;  
    }  
    //指锟斤拷锟斤拷锟矫好碉拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�  
    public void connetionConfiguration(int index)
    {  
        if(index>mWifiConfigurations.size())
        {  
            return ;  
        }  
        //锟斤拷锟斤拷锟斤拷锟矫猴拷指锟斤拷ID锟斤拷锟斤拷锟斤拷  
        mWifiManager.enableNetwork(mWifiConfigurations.get(index).networkId, true);  
    }  
    public static void startScan()
    {  
        mWifiManager.startScan();  
        //锟矫碉拷扫锟斤拷锟斤拷  
        mWifiList=mWifiManager.getScanResults();  
        //锟矫碉拷锟斤拷锟矫好碉拷锟斤拷锟斤拷锟斤拷锟斤拷  
        mWifiConfigurations=mWifiManager.getConfiguredNetworks();  
    }  
    //锟矫碉拷锟斤拷锟斤拷锟叫憋拷  
    public static List<ScanResult> getWifiList()
    {  
        return mWifiList;  
    }
    
    public static WifiInfo getWifiConnectionInfo()
    {
    	WifiInfo mInfo = mWifiManager.getConnectionInfo();
    	return mInfo;
    }
    //锟介看扫锟斤拷锟斤拷  
    public StringBuffer lookUpScan()
    {  
        StringBuffer sb=new StringBuffer();  
        for(int i=0;i<mWifiList.size();i++)
        {  
            sb.append("Index_" + new Integer(i + 1).toString() + ":");  
             // 锟斤拷ScanResult锟斤拷息转锟斤拷锟斤拷一锟斤拷锟街凤拷锟斤拷锟斤拷    
            // 锟斤拷锟叫把帮拷锟斤拷锟斤拷BSSID锟斤拷SSID锟斤拷capabilities锟斤拷frequency锟斤拷level    
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
    
    //锟矫碉拷锟斤拷锟接碉拷ID  
    public int getNetWordId()
    {  
        return (mWifiInfo==null)?0:mWifiInfo.getNetworkId();  
    }  
    
    //锟矫碉拷wifiInfo锟斤拷锟斤拷锟斤拷锟斤拷息  
    public String getWifiInfo()
    {  
        return (mWifiInfo==null)?"NULL":mWifiInfo.toString();  
    }  
    //锟斤拷锟揭伙拷锟斤拷锟斤拷绮�锟斤拷锟斤拷  
    public void addNetWork(WifiConfiguration configuration)
    {  
        int wcgId=mWifiManager.addNetwork(configuration);  
        mWifiManager.enableNetwork(wcgId, true);  
    }  
    //锟较匡拷指锟斤拷ID锟斤拷锟斤拷锟斤拷  
    public void disConnectionWifi(int netId)
    {  
        mWifiManager.disableNetwork(netId);  
        mWifiManager.disconnect();  
    }  
}
