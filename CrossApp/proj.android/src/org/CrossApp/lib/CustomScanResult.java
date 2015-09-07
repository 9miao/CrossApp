package org.CrossApp.lib;

import android.R.integer;


public class CustomScanResult {
	private String ssid;
	private String mac;
	private int level;
	public CustomScanResult()
	{
		
	}
	public CustomScanResult(String ssid,String mac,int level)
	{
		this.level=level;
		this.ssid = ssid;
		this.mac = mac;
	}
	public int getlevel()
	{
		return level;
	}
	public String getmac()
	{
		return mac;
	}
	public String getssid()
	{
		return ssid;
	}
	
}
