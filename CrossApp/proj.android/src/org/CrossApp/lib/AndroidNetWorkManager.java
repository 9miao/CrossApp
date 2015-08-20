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
	//����һ��WifiManager����  
    private static WifiManager mWifiManager;  
    //����һ��WifiInfo����  
    private static WifiInfo mWifiInfo;  
    //ɨ��������������б�  
    private static List<ScanResult> mWifiList;
    
  //���������б�  
    private static List<WifiConfiguration> mWifiConfigurations;  
    
    WifiLock mWifiLock;
    
	public static void setContext(Activity activity)
	{
		//ȡ��WifiManager����  
        mWifiManager=(WifiManager) activity.getSystemService(Context.WIFI_SERVICE);  
        //ȡ��WifiInfo����  
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
    //�ر�wifi  
    public void closeWifi(){  
        if(!mWifiManager.isWifiEnabled()){  
            mWifiManager.setWifiEnabled(false);  
        }  
    }  
     // ��鵱ǰwifi״̬    
    public int checkState()
    {    
        return mWifiManager.getWifiState();    
    }    
    //����wifiLock  
    public void acquireWifiLock()
    {  
        mWifiLock.acquire();  
    }  
    //����wifiLock  
    public void releaseWifiLock()
    {  
        //�ж��Ƿ�����  
        if(mWifiLock.isHeld()){  
            mWifiLock.acquire();  
        }  
    }  
    //����һ��wifiLock  
    public void createWifiLock()
    {  
        mWifiLock=mWifiManager.createWifiLock("test");  
    }
    //�õ����úõ�����  
    public List<WifiConfiguration> getConfiguration()
    {  
        return mWifiConfigurations;  
    }  
    //ָ�����úõ������������  
    public void connetionConfiguration(int index)
    {  
        if(index>mWifiConfigurations.size())
        {  
            return ;  
        }  
        //�������ú�ָ��ID������  
        mWifiManager.enableNetwork(mWifiConfigurations.get(index).networkId, true);  
    }  
    public static void startScan()
    {  
        mWifiManager.startScan();  
        //�õ�ɨ����  
        mWifiList=mWifiManager.getScanResults();  
        //�õ����úõ���������  
        mWifiConfigurations=mWifiManager.getConfiguredNetworks();  
    }  
    //�õ������б�  
    public static List<ScanResult> getWifiList()
    {  
        return mWifiList;  
    }
    
    public static WifiInfo getWifiConnectionInfo()
    {
    	WifiInfo mInfo = mWifiManager.getConnectionInfo();
    	return mInfo;
    }
    //�鿴ɨ����  
    public StringBuffer lookUpScan()
    {  
        StringBuffer sb=new StringBuffer();  
        for(int i=0;i<mWifiList.size();i++)
        {  
            sb.append("Index_" + new Integer(i + 1).toString() + ":");  
             // ��ScanResult��Ϣת����һ���ַ�����    
            // ���аѰ�����BSSID��SSID��capabilities��frequency��level    
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
    
    //�õ����ӵ�ID  
    public int getNetWordId()
    {  
        return (mWifiInfo==null)?0:mWifiInfo.getNetworkId();  
    }  
    
    //�õ�wifiInfo��������Ϣ  
    public String getWifiInfo()
    {  
        return (mWifiInfo==null)?"NULL":mWifiInfo.toString();  
    }  
    //���һ�����粢����  
    public void addNetWork(WifiConfiguration configuration)
    {  
        int wcgId=mWifiManager.addNetwork(configuration);  
        mWifiManager.enableNetwork(wcgId, true);  
    }  
    //�Ͽ�ָ��ID������  
    public void disConnectionWifi(int netId)
    {  
        mWifiManager.disableNetwork(netId);  
        mWifiManager.disconnect();  
    }  
}
