package org.CrossApp.lib;

public class CrossAppLocationInfo {
	
	private String sLongitude;
	private String sLatitude;
	private String sAltitude;
	private String sSpeed;
	private String sBearing;

	
	public CrossAppLocationInfo(String longitude,String latitude,String altitude,String speed,String bearing)
	{
		sLongitude = longitude;
		sLatitude = latitude;
		sAltitude = altitude;
		sSpeed = speed;
		sBearing = bearing;
	}
	
	public String getLongitude()
	{
		return sLongitude;
	}
	
	public String getLatitude()
	{
		return sLatitude;
	}
	
	public String getAltitude()
	{
		return sAltitude;
	}
	
	public String getSpeed()
	{
		return sSpeed;
	}
	
	public String getBearing()
	{
		return sBearing;
	}
}
