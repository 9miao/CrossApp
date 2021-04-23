package org.CrossApp.lib;

import android.content.Context;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.view.Display;
import android.view.Surface;
import android.view.WindowManager;

public class CrossAppGyroscope  implements SensorEventListener{
	
    private SensorManager sensorManager;
    
    private final Context mContext;
    
	private final SensorManager mSensorManager;
	
	private final Sensor mGyroscope;
	
	private float mInterval = 0.001f;
	
	private long lastUpdateTime;
	
	private static final int UPTATE_INTERVAL_TIME = 1000;
	
	private final int mNaturalOrientation;
	
	public CrossAppGyroscope(final Context pContext) {
		
		this.mContext = pContext;

		this.mSensorManager = (SensorManager) this.mContext.getSystemService(Context.SENSOR_SERVICE);
		
		this.mGyroscope = this.mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
		
		final Display display = ((WindowManager) this.mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		this.mNaturalOrientation = display.getOrientation();
	}
    
    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy)
    {
    }

    
    @Override
	public void onSensorChanged(final SensorEvent pSensorEvent) 
    {
		if (pSensorEvent.sensor.getType() != Sensor.TYPE_ACCELEROMETER) {
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
				
		float x = pSensorEvent.values[0];
		float y = pSensorEvent.values[1];
		final float z = pSensorEvent.values[2];

		/*
		 * Because the axes are not swapped when the device's screen orientation
		 * changes. So we should swap it here. In tablets such as Motorola Xoom,
		 * the default orientation is landscape, so should consider this.
		 */
		final int orientation = this.mContext.getResources().getConfiguration().orientation;

		if ((orientation == Configuration.ORIENTATION_LANDSCAPE) && (this.mNaturalOrientation != Surface.ROTATION_0)) {
			final float tmp = x;
			x = -y;
			y = tmp;
		} else if ((orientation == Configuration.ORIENTATION_PORTRAIT) && (this.mNaturalOrientation != Surface.ROTATION_0)) {
			final float tmp = x;
			x = y;
			y = -tmp;
		}		
		
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
