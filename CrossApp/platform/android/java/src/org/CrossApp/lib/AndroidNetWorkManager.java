package org.CrossApp.lib;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;

@SuppressLint("DefaultLocale")
public class AndroidNetWorkManager {
	private static Activity s_pActivity;
	
	public static void setContext(Activity activity)
	{
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
}
