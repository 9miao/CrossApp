package org.CrossApp.lib;

import android.R.integer;
import android.app.Activity;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.os.IBinder;

public class AndroidVolumeControl extends Service{

	public static Activity s_pContext;
	public static AudioManager audioManager = null;
	public static void setContext(Context context)
	{
		s_pContext =(Activity) context;
		audioManager = (AudioManager)s_pContext.getSystemService(s_pContext.AUDIO_SERVICE);
	}

	public static float getCallVolum(int ringtype)
	{
		int max = 0;
		float current =0.0f;
		int type = 0;
		switch (ringtype) 
		{
		case 0:
			type = AudioManager.STREAM_MUSIC;
			break;
		case 1:
			type = AudioManager.STREAM_SYSTEM;
			break;
		case 2:
			type = AudioManager.STREAM_RING;
			break;
		case 3:
			type = AudioManager.STREAM_VOICE_CALL;
			break;
		case 4:
			type = AudioManager.STREAM_ALARM;
			break;
		case 5:
			type = AudioManager.STREAM_NOTIFICATION;
			break;	
		default:
			break;
		}
		max = audioManager.getStreamMaxVolume(type );

	    current = (float)audioManager.getStreamVolume( AudioManager.STREAM_VOICE_CALL )/(float)max;
		return current;
	}
	
	public static void setVolum(float sender,int type)
	{
		int ringType=0;
		switch (type) 
		{
		case 0:
			ringType = AudioManager.STREAM_MUSIC;
			break;
		case 1 :
			ringType = AudioManager.STREAM_SYSTEM;
			break;
		case 2 :
			ringType = AudioManager.STREAM_RING;
			break;
		case 3 :
			ringType = AudioManager.STREAM_VOICE_CALL;
			break;
		case 4 :
			ringType = AudioManager.STREAM_ALARM;
			break;
		default:
			ringType = AudioManager.STREAM_NOTIFICATION;
			break;
		}
		int max = audioManager.getStreamMaxVolume(ringType );
		int value = (int) ((float)max*sender);
		audioManager.setStreamVolume(ringType,value, 0);
	}

	@Override
	public IBinder onBind(Intent intent) 
	{
		// TODO Auto-generated method stub
		return null;
	}
}
