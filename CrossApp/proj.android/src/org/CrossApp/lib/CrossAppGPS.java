package org.CrossApp.lib;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONArray;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.location.Address;
import android.location.Criteria;
import android.location.Geocoder;
import android.location.GpsSatellite;
import android.location.GpsStatus;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.LocationProvider;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;
import android.widget.Toast;

public class CrossAppGPS {
	
	private static final Context AndroidGPS = null;
	private static Activity s_pContext;
	public static LocationManager locationManager;
	public static String provider;
	private static final long serialVersionUID = -4582739827003032383L;   
	static Intent intent ;
	public static void Init( final Activity context )
	{
		s_pContext = context;
		openGPSSettings();	
	}
	
	public static void openGPSSettings()
	{

		locationManager = (LocationManager) s_pContext.getSystemService(Context.LOCATION_SERVICE);

        if (!locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
        	 intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
             s_pContext.startActivityForResult(intent,10);
            return;
        }    
    }
	
	public static  LocationListener locationListener = new LocationListener() { 

		@Override 
		public void onStatusChanged(String provider, int status, Bundle extras)
		{ 
		} 
		
		@Override 
		public void onProviderEnabled(String provider) 
		{ 
		} 
		
		@Override 
		public void onProviderDisabled(String provider)
		{ 
		} 
		
		@Override 
		public void onLocationChanged(Location location) 
		{ 
            if (location != null)
            {
            	returnLocationInfo(getLocationInfo(location));
            }    	  	
		}
	};
	
    
   static GpsStatus.Listener listener = new GpsStatus.Listener() {
        public void onGpsStatusChanged(int event) {
            switch (event) {

            case GpsStatus.GPS_EVENT_SATELLITE_STATUS:
                Log.i("log", "staus");

                GpsStatus gpsStatus=locationManager.getGpsStatus(null);

                int maxSatellites = gpsStatus.getMaxSatellites();

                Iterator<GpsSatellite> iters = gpsStatus.getSatellites().iterator();
                int count = 0;     
                while (iters.hasNext() && count <= maxSatellites) {     
                    GpsSatellite s = iters.next();     
                    count++;     
                }   
                break;
            case GpsStatus.GPS_EVENT_STARTED:
                Log.i("log", "start");
                break;    
            case GpsStatus.GPS_EVENT_STOPPED:
                Log.i("log", "stop");
                break;
            }
        };
    };
    
	public static void getLocation()
    {
		Criteria criteria= new Criteria();
        criteria.setAccuracy(Criteria.ACCURACY_FINE); 
        criteria.setAltitudeRequired(false);
        criteria.setBearingRequired(false);
        criteria.setCostAllowed(true);
        criteria.setBearingAccuracy(1);
        criteria.setPowerRequirement(Criteria.POWER_LOW);
        
        
		String serviceName = Context.LOCATION_SERVICE;
        locationManager = (LocationManager) s_pContext.getSystemService(serviceName);

        provider = locationManager.getBestProvider(criteria, true); 
        Location location =  getLastKnownLocation();

		s_pContext.runOnUiThread(new Runnable()
		{
		
			@Override
			public void run() {
				locationManager.addGpsStatusListener(listener);
		        
				locationManager.requestLocationUpdates(provider, 6000,1, locationListener);			
			}
		});
		
		if(location != null)
		{
			returnLocationInfo(getLocationInfo(location));
		}
       
    }
	
	private static Location getLastKnownLocation()
	{
	    List<String> providers = locationManager.getProviders(true);
	    Location bestLocation = null;
	    for (String provider : providers)
	    {
	        Location l = locationManager.getLastKnownLocation(provider);
	        if (l == null)
	        {
	        	 Log.i("location providers", "providersnull");
	            continue;
	        }
	        if (bestLocation == null || l.getAccuracy() < bestLocation.getAccuracy())
	        {
	            // Found best last known location: %s", l);
	            bestLocation = l;
	            break;
	        }
	    }

	    return bestLocation;
	}

	private static void updateToNewLocation(Location location)
	{
        if (location != null)
        {
        	returnLocationInfo(getLocationInfo(location));
        }
    }
	
	
	private static CrossAppLocationInfo getLocationInfo(Location location)
	{
    	String longitude = Double.toString(location.getLongitude());
    	String latitude = Double.toString(location.getLatitude());
    	String altitude = Double.toString(location.getAltitude()); 	
    	String speed = Float.toString(location.getSpeed());
    	String bearing = Float.toString(location.getBearing());
    	
    	CrossAppLocationInfo mInfo = new CrossAppLocationInfo(longitude,latitude,altitude,speed,bearing);

    	return mInfo;
	}
	
	public static void stopUpdateLocation()
	{
		locationManager.removeUpdates(locationListener);
		locationManager.removeGpsStatusListener(listener);
	}
	
	public static native void returnLocationInfo(CrossAppLocationInfo info);
	
}
