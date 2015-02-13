package org.CrossApp.lib;

import java.util.List;

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

@SuppressLint("DefaultLocale")
public class AndroidNetWorkManager {
	private static Activity s_pActivity;
	//定义一个WifiManager对象  
    private static WifiManager mWifiManager;  
    //定义一个WifiInfo对象  
    private static WifiInfo mWifiInfo;  
    //扫描出的网络连接列表  
    private static List<ScanResult> mWifiList;
    
  //网络连接列表  
    private static List<WifiConfiguration> mWifiConfigurations;  
    
    WifiLock mWifiLock;
    
   
    
    
	public static void setContext(Activity activity)
	{
		//取得WifiManager对象  
        mWifiManager=(WifiManager) activity.getSystemService(Context.WIFI_SERVICE);  
        //取得WifiInfo对象  
        mWifiInfo=mWifiManager.getConnectionInfo(); 
        
        
		s_pActivity = activity;
	}
	public static int getAPNType(){
		
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
	
	public void openWifi(){  
        if(!mWifiManager.isWifiEnabled()){  
            mWifiManager.setWifiEnabled(true);  
        }  
    }  
    //关闭wifi  
    public void closeWifi(){  
        if(!mWifiManager.isWifiEnabled()){  
            mWifiManager.setWifiEnabled(false);  
        }  
    }  
     // 检查当前wifi状态    
    public int checkState() {    
        return mWifiManager.getWifiState();    
    }    
    //锁定wifiLock  
    public void acquireWifiLock(){  
        mWifiLock.acquire();  
    }  
    //解锁wifiLock  
    public void releaseWifiLock(){  
        //判断是否锁定  
        if(mWifiLock.isHeld()){  
            mWifiLock.acquire();  
        }  
    }  
    //创建一个wifiLock  
    public void createWifiLock(){  
        mWifiLock=mWifiManager.createWifiLock("test");  
    }
    //得到配置好的网络  
    public List<WifiConfiguration> getConfiguration(){  
        return mWifiConfigurations;  
    }  
    //指定配置好的网络进行连接  
    public void connetionConfiguration(int index){  
        if(index>mWifiConfigurations.size()){  
            return ;  
        }  
        //连接配置好指定ID的网络  
        mWifiManager.enableNetwork(mWifiConfigurations.get(index).networkId, true);  
    }  
    public static void startScan(){  
        mWifiManager.startScan();  
        //得到扫描结果  
        mWifiList=mWifiManager.getScanResults();  
        //得到配置好的网络连接  
        mWifiConfigurations=mWifiManager.getConfiguredNetworks();  
    }  
    //得到网络列表  
    public static List<ScanResult> getWifiList(){  
        return mWifiList;  
    }  
    public static WifiInfo getWifiConnectionInfo()
    {
    	WifiInfo mInfo = mWifiManager.getConnectionInfo();
    	return mInfo;
    }
    //查看扫描结果  
    public StringBuffer lookUpScan(){  
        StringBuffer sb=new StringBuffer();  
        for(int i=0;i<mWifiList.size();i++){  
            sb.append("Index_" + new Integer(i + 1).toString() + ":");  
             // 将ScanResult信息转换成一个字符串包    
            // 其中把包括：BSSID、SSID、capabilities、frequency、level    
            sb.append((mWifiList.get(i)).toString()).append("\n");  
        }  
        return sb;    
    }  
    public String getMacAddress(){  
        return (mWifiInfo==null)?"NULL":mWifiInfo.getMacAddress();  
    }  
    public String getBSSID(){  
        return (mWifiInfo==null)?"NULL":mWifiInfo.getBSSID();  
    }  
    public int getIpAddress(){  
        return (mWifiInfo==null)?0:mWifiInfo.getIpAddress();  
    }  
    //得到连接的ID  
    public int getNetWordId(){  
        return (mWifiInfo==null)?0:mWifiInfo.getNetworkId();  
    }  
    //得到wifiInfo的所有信息  
    public String getWifiInfo(){  
        return (mWifiInfo==null)?"NULL":mWifiInfo.toString();  
    }  
    //添加一个网络并连接  
    public void addNetWork(WifiConfiguration configuration){  
        int wcgId=mWifiManager.addNetwork(configuration);  
        mWifiManager.enableNetwork(wcgId, true);  
    }  
    //断开指定ID的网络  
    public void disConnectionWifi(int netId){  
        mWifiManager.disableNetwork(netId);  
        mWifiManager.disconnect();  
    }  
}
