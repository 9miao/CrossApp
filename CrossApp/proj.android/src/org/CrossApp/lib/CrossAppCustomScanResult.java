package org.CrossApp.lib;

public class CrossAppCustomScanResult {
	private String ssid;
	private String mac;
	private int level;
	public CrossAppCustomScanResult()
	{
		
	}
	public CrossAppCustomScanResult(String ssid,String mac,int level)
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
