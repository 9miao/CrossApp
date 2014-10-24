package org.CrossApp.lib;

import java.util.Iterator;
import java.util.List;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.location.Criteria;
import android.location.GpsSatellite;
import android.location.GpsStatus;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;
import android.widget.Toast;

public class AndroidGPS {
	private static Activity s_pContext;
	public static LocationManager locationManager;
	public static String provider;
	public static void Init( final Activity context )
	{
		s_pContext = context;
		openGPSSettings();
	}
	public static void openGPSSettings()
	{
		locationManager = (LocationManager) s_pContext.getSystemService(Context.LOCATION_SERVICE);
        if (!locationManager.isProviderEnabled(android.location.LocationManager.GPS_PROVIDER)) {
        	 Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
             s_pContext.startActivityForResult(intent,10);
            return;
        }
       
    }
	
	public static final LocationListener locationListener = new LocationListener() { 
		@Override 
		public void onStatusChanged(String provider, int status, Bundle extras) { 
		} 
		@Override 
		public void onProviderEnabled(String provider) { 
		} 
		@Override 
		public void onProviderDisabled(String provider) { 
		} 
		@Override 
		public void onLocationChanged(Location location) { 
			Log.i("onLocationChanged", "come in");
            if (location != null)
            {
                Log.w("Location", "Current altitude = "
                        + location.getAltitude());
                Log.w("Location", "Current latitude = "
                        + location.getLatitude());
            }
            locationManager.removeUpdates(this);
            locationManager.setTestProviderEnabled(provider, false);
		}
	};
	static GpsStatus.Listener listener = new GpsStatus.Listener() {
        public void onGpsStatusChanged(int event) {
            switch (event) {
            //第一次定位
            case GpsStatus.GPS_EVENT_FIRST_FIX:
                break;
            //卫星状态改变
            case GpsStatus.GPS_EVENT_SATELLITE_STATUS:
                //获取当前状态
                GpsStatus gpsStatus=locationManager.getGpsStatus(null);
                //获取卫星颗数的默认最大值
                int maxSatellites = gpsStatus.getMaxSatellites();
                //创建一个迭代器保存所有卫星 
                Iterator<GpsSatellite> iters = gpsStatus.getSatellites().iterator();
                int count = 0;     
                while (iters.hasNext() && count <= maxSatellites) {     
                    GpsSatellite s = iters.next();     
                    count++;     
                }   
                System.out.println("搜索到："+count+"颗卫星");
                break;
            //定位启动
            case GpsStatus.GPS_EVENT_STARTED:
                break;
            //定位结束
            case GpsStatus.GPS_EVENT_STOPPED:
                break;
            }
        };
    };
	public static double[] getLocation()
    {
        Criteria criteria = new Criteria();
        criteria.setAccuracy(Criteria.ACCURACY_FINE); 
        criteria.setAltitudeRequired(false);
        criteria.setBearingRequired(false);
        criteria.setCostAllowed(true);
        criteria.setPowerRequirement(Criteria.POWER_LOW);
        
        
		String serviceName = Context.LOCATION_SERVICE;
        locationManager = (LocationManager) s_pContext.getSystemService(serviceName);

       // locationManager.setTestProviderEnabled("gps",true); 
        provider = locationManager.getBestProvider(criteria, true); 
       // locationManager.requestLocationUpdates(provider, 100 * 1000, 500,locationListener);
        Location location =  getLastKnownLocation();
        		//locationManager.getLastKnownLocation(provider);
       // locationManager.requestLocationUpdates(provider, 1000, (float) 1000.0, locationListener);
//        		// 
//        while(location  == null)  
//        {  
//        	locationManager.requestLocationUpdates("gps", 60000, 1, locationListener);  
//        } 
        
        if (location != null) {
        	return new double[]{location.getLatitude() , location.getLongitude() };
        	
		}else {
			
			return new double[]{0,0};
		}
        

    }
	
	private static Location getLastKnownLocation() {
	//	locationManager = (LocationManager) s_pContext.getSystemService(serviceName);
	    List<String> providers = locationManager.getProviders(true);
	    Location bestLocation = null;
	    for (String provider : providers) {
	        Location l = locationManager.getLastKnownLocation(provider);
	        if (l == null) {
	            continue;
	        }
	        if (bestLocation == null || l.getAccuracy() < bestLocation.getAccuracy()) {
	            // Found best last known location: %s", l);
	            bestLocation = l;
	        }
	    }
	    return bestLocation;
	}

	private static void updateToNewLocation(Location location) {

        if (location != null) {
            double  latitude = location.getLatitude();
            double longitude= location.getLongitude();
        } else {
        }

    }
}
