package org.CrossApp.lib;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class CrossAppBattery extends BroadcastReceiver {

	public static int currentBattery=0;
	
	@Override
	public void onReceive(Context content, Intent intent) {
		
		   if(Intent.ACTION_BATTERY_CHANGED.equals(intent.getAction())){

               int level = intent.getIntExtra("level", 0);

               int scale = intent.getIntExtra("scale", 100);

               currentBattery =level*100/ scale;
           }
		
	}
	

	public static int getBatteryLevel()
	{
		return currentBattery;
	}

}
