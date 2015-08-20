package org.CrossApp.lib;

public class AndroidBlueTooth {
	private String maddress;
	
	private String mname;
	
	public AndroidBlueTooth()
	{
		
	}
	
	public AndroidBlueTooth(String address,String name)
	{
		maddress = address;
		mname = name;
	}
	
	public String getAddress()
	{
		return maddress;
	}
	
	public String getName()
	{
		return mname;
	}
	
}

