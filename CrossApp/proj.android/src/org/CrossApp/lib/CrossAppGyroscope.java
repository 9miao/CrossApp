package org.CrossApp.lib;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;
import android.view.Display;


public class CrossAppGyroscope  implements SensorEventListener{
	
    private SensorManager sensorManager;
    
    private final Context mContext;
    
	private final SensorManager mSensorManager;
	
	private final Sensor mGyroscope;
	
	private float mInterval = 0.001f;
	
	private long lastUpdateTime;
	
	private static final int UPTATE_INTERVAL_TIME = 1000;
	
	public CrossAppGyroscope(final Context pContext) {
		
		this.mContext = pContext;

		this.mSensorManager = (SensorManager) this.mContext.getSystemService(Context.SENSOR_SERVICE);
		
		this.mGyroscope = this.mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
	}
    


    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    public void onSensorChanged (SensorEvent event) {
    	
		if (event.sensor.getType() != Sensor.TYPE_GYROSCOPE) {
			return;
		}

		// The current time
		long currentUpdateTime = System.currentTimeMillis();
		
		// Two detection time interval
		long timeInterval = currentUpdateTime - lastUpdateTime;
		
		// Determine whether detection time interval has been reached
		if (timeInterval < UPTATE_INTERVAL_TIME*mInterval)		
			return;
		
		// The time into the last time
		lastUpdateTime = currentUpdateTime;
    	
		float x = event.values[0];
		float y = event.values[1];
		float z = event.values[2];

		CrossAppGLSurfaceView.queueGyroscope(x, y, z, mInterval);
	}

    
	public void enable() {
		this.mSensorManager.registerListener(this, this.mGyroscope, SensorManager.SENSOR_DELAY_GAME);
	}

    public void setInterval(float interval) 
    {
    	mInterval = interval;
    	
        // Honeycomb version is 11
        if(android.os.Build.VERSION.SDK_INT < 11) 
        {
	    this.mSensorManager.registerListener(this, this.mGyroscope, SensorManager.SENSOR_DELAY_GAME);
        } 
        else
        {
	    //convert seconds to microseconds
	    this.mSensorManager.registerListener(this, this.mGyroscope, (int)(interval*100000));
        }
	}
      
	public void disable() {
		this.mSensorManager.unregisterListener(this);
	}
  
	public static native void onGyroSensorChanged(final float pX, final float pY, final float pZ, float pTimestamp);
}
