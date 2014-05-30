package org.cocos2dx.lib;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.provider.Settings;
import android.widget.Toast;

public class AndroidGPS {
	private static Activity s_pContext;
	public static void Init( final Activity context )
	{
		s_pContext = context;
		openGPSSettings();
	}
	public static void openGPSSettings()
	{
        LocationManager alm = (LocationManager) s_pContext.getSystemService(Context.LOCATION_SERVICE);
        if (alm.isProviderEnabled(android.location.LocationManager.GPS_PROVIDER)) {
            Toast.makeText(s_pContext, "GPS模块正常", Toast.LENGTH_SHORT).show();
        	 System.out.println("sdfsdfdsf");
            return;
        }
        System.out.println("sdfsdfds11f");
        Toast.makeText(s_pContext, "请开启GPS！", Toast.LENGTH_SHORT).show();
        Intent intent = new Intent(Settings.ACTION_SECURITY_SETTINGS);
        s_pContext.startActivityForResult(intent,10); //此为设置完成后返回到获取界面
    }
	public static double[] getLocation()
    {
        // 获取位置管理服务
		
        LocationManager locationManager;
        String serviceName = Context.LOCATION_SERVICE;
        locationManager = (LocationManager) s_pContext.getSystemService(serviceName);
        // 查找到服务信息
        Criteria criteria = new Criteria();
        criteria.setAccuracy(Criteria.ACCURACY_FINE); // 高精度
        criteria.setAltitudeRequired(false);
        criteria.setBearingRequired(false);
        criteria.setCostAllowed(true);
        criteria.setPowerRequirement(Criteria.POWER_LOW); // 低功耗
       
        String provider = locationManager.getBestProvider(criteria, true); // 获取GPS信息
        System.out.println(provider);
        Location location = locationManager.getLastKnownLocation(provider); // 通过GPS获取位置
        updateToNewLocation(location);
        // 设置监听器，自动更新的最小时间为间隔N秒(1秒为1*1000，这样写主要为了方便)或最小位移变化超过N米
//        locationManager.requestLocationUpdates(provider, 100 * 1000, 500,
//                locationListener);
     
        if ( location == null ) return null;
        	
        AlertDialog.Builder dialog=new AlertDialog.Builder(s_pContext);
        String str = "x"+location.getLatitude() + " ,y="+location.getLongitude();
         dialog.setTitle(str);
         dialog.show();
        return new double[]{ location.getLatitude() , location.getLongitude() };
    }
	private static final LocationListener locationListener = new LocationListener() { 
		@Override 
		public void onStatusChanged(String provider, int status, Bundle extras) { 
		} 
		@Override 
		public void onProviderEnabled(String provider) { 
		} 
		@Override 
		public void onProviderDisabled(String provider) { 
		} 
		//当位置变化时触发 
		@Override 
		public void onLocationChanged(Location location) { 
		//使用新的location更新TextView显示 
		//updateToNewLocation(location); 
		}
		}; 
	private static void updateToNewLocation(Location location) {

        if (location != null) {
            double  latitude = location.getLatitude();
            double longitude= location.getLongitude();
            //tv1.setText("维度：" +  latitude+ "\n经度" + longitude);
        } else {
            //tv1.setText("无法获取地理信息");
        }

    }
}
