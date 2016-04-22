
package org.CrossApp.lib;

import java.io.UnsupportedEncodingException;
import java.util.Locale;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetManager;
import android.os.Build;
import android.util.DisplayMetrics;
import android.view.Display;
import android.view.WindowManager;

public class CrossAppHelper {
	// ===========================================================
	// Constants
	// ===========================================================
	private static final String PREFS_NAME = "CrossAppPrefsFile";

	// ===========================================================
	// Fields
	// ===========================================================

	private static CrossAppMusic sCocos2dMusic;
	private static CrossAppSound sCocos2dSound;
	private static AssetManager sAssetManager;
	private static CrossAppAccelerometer sCrossAppAccelerometer;
	private static boolean sAccelerometerEnabled;
	private static String sPackageName;
	private static String sFileDirectory;
	private static Context sContext = null;
	private static CrossAppHelperListener sCrossAppHelperListener;
    private static CrossAppSDL sSDL;
	// ===========================================================
	// Constructors
	// ===========================================================

	public static void init(final Context pContext, final CrossAppHelperListener pCrossAppHelperListener) {
		final ApplicationInfo applicationInfo = pContext.getApplicationInfo();
		
		CrossAppHelper.sContext = pContext;
		CrossAppHelper.sCrossAppHelperListener = pCrossAppHelperListener;

		CrossAppHelper.sPackageName = applicationInfo.packageName;
		CrossAppHelper.sFileDirectory = pContext.getFilesDir().getAbsolutePath();
		CrossAppHelper.nativeSetApkPath(applicationInfo.sourceDir);

		CrossAppHelper.setsCrossAppAccelerometer(new CrossAppAccelerometer(pContext));
		CrossAppHelper.sCocos2dMusic = new CrossAppMusic(pContext);
		CrossAppHelper.sCocos2dSound = new CrossAppSound(pContext);
		CrossAppHelper.sAssetManager = pContext.getAssets();
		CrossAppHelper.sSDL = new CrossAppSDL(pContext);
		CrossAppBitmap.setContext(pContext);
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	// ===========================================================
	// Methods
	// ===========================================================

	private static native void nativeSetApkPath(final String pApkPath);

	private static native void nativeSetEditTextDialogResult(final byte[] pBytes);

	public static String getCrossAppPackageName() {
		return CrossAppHelper.sPackageName;
	}

	public static String getCrossAppWritablePath() {
		return CrossAppHelper.sFileDirectory;
	}

	public static String getCurrentLanguage() {
		return Locale.getDefault().getLanguage();
	}
	
	public static String getDeviceModel(){
		return Build.MODEL;
    }

	public static AssetManager getAssetManager() {
		return CrossAppHelper.sAssetManager;
	}

	public static void enableAccelerometer() {
		CrossAppHelper.setsAccelerometerEnabled(true);
		CrossAppHelper.getsCrossAppAccelerometer().enable();
	}


	public static void setAccelerometerInterval(float interval) {
		CrossAppHelper.getsCrossAppAccelerometer().setInterval(interval);
	}

	public static void disableAccelerometer() {
		CrossAppHelper.setsAccelerometerEnabled(false);
		CrossAppHelper.getsCrossAppAccelerometer().disable();
	}
	
	public static void onResume() {
		if (CrossAppHelper.issAccelerometerEnabled()) {
			CrossAppHelper.getsCrossAppAccelerometer().enable();
		}
	}

	public static void onPause() {
		if (CrossAppHelper.issAccelerometerEnabled()) {
			CrossAppHelper.getsCrossAppAccelerometer().disable();
		}
	}
	
	public static void preloadBackgroundMusic(final String pPath) {
		CrossAppHelper.sCocos2dMusic.preloadBackgroundMusic(pPath);
	}

	public static void playBackgroundMusic(final String pPath, final boolean isLoop) {
		CrossAppHelper.sCocos2dMusic.playBackgroundMusic(pPath, isLoop);
	}

	public static void resumeBackgroundMusic() {
		CrossAppHelper.sCocos2dMusic.resumeBackgroundMusic();
	}

	public static void pauseBackgroundMusic() {
		CrossAppHelper.sCocos2dMusic.pauseBackgroundMusic();
	}

	public static void stopBackgroundMusic() {
		CrossAppHelper.sCocos2dMusic.stopBackgroundMusic();
	}

	public static void rewindBackgroundMusic() {
		CrossAppHelper.sCocos2dMusic.rewindBackgroundMusic();
	}

	public static boolean isBackgroundMusicPlaying() {
		return CrossAppHelper.sCocos2dMusic.isBackgroundMusicPlaying();
	}

	public static float getBackgroundMusicVolume() {
		return CrossAppHelper.sCocos2dMusic.getBackgroundVolume();
	}

	public static void setBackgroundMusicVolume(final float volume) {
		CrossAppHelper.sCocos2dMusic.setBackgroundVolume(volume);
	}

	public static void preloadEffect(final String path) {
		CrossAppHelper.sCocos2dSound.preloadEffect(path);
	}

	public static int playEffect(final String path, final boolean isLoop) {
		return CrossAppHelper.sCocos2dSound.playEffect(path, isLoop);
	}

	public static void resumeEffect(final int soundId) {
		CrossAppHelper.sCocos2dSound.resumeEffect(soundId);
	}

	public static void pauseEffect(final int soundId) {
		CrossAppHelper.sCocos2dSound.pauseEffect(soundId);
	}

	public static void stopEffect(final int soundId) {
		CrossAppHelper.sCocos2dSound.stopEffect(soundId);
	}

	public static float getEffectsVolume() {
		return CrossAppHelper.sCocos2dSound.getEffectsVolume();
	}

	public static void setEffectsVolume(final float volume) {
		CrossAppHelper.sCocos2dSound.setEffectsVolume(volume);
	}

	public static void unloadEffect(final String path) {
		CrossAppHelper.sCocos2dSound.unloadEffect(path);
	}

	public static void pauseAllEffects() {
		CrossAppHelper.sCocos2dSound.pauseAllEffects();
	}

	public static void resumeAllEffects() {
		CrossAppHelper.sCocos2dSound.resumeAllEffects();
	}

	public static void stopAllEffects() {
		CrossAppHelper.sCocos2dSound.stopAllEffects();
	}

	public static void end() {
		CrossAppHelper.sCocos2dMusic.end();
		CrossAppHelper.sCocos2dSound.end();
	}


	public static void terminateProcess() {
		android.os.Process.killProcess(android.os.Process.myPid());
	}

	private static void showDialog(final String pTitle, final String pMessage) {
		CrossAppHelper.sCrossAppHelperListener.showDialog(pTitle, pMessage);
	}

	private static void showEditTextDialog(final String pTitle, final String pMessage, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength) {
		CrossAppHelper.sCrossAppHelperListener.showEditTextDialog(pTitle, pMessage, pInputMode, pInputFlag, pReturnType, pMaxLength);
	}

	public static void setEditTextDialogResult(final String pResult) {
		try {
			final byte[] bytesUTF8 = pResult.getBytes("UTF8");

			CrossAppHelper.sCrossAppHelperListener.runOnGLThread(new Runnable() {
				@Override
				public void run() {
					CrossAppHelper.nativeSetEditTextDialogResult(bytesUTF8);
				}
			});
		} catch (UnsupportedEncodingException pUnsupportedEncodingException) {
			/* Nothing. */
		}
	}

    public static float getDPI()
    {
		if (sContext != null)
		{
			DisplayMetrics metrics = new DisplayMetrics();
			WindowManager wm = ((Activity)sContext).getWindowManager();
			if (wm != null)
			{
				Display d = wm.getDefaultDisplay();
				if (d != null)
				{
					d.getMetrics(metrics);
					return (metrics.density * 160.0f);
				}
			}
		}
		return -1.0f;
    }
    
    // ===========================================================
 	// Functions for CCUserDefault
 	// ===========================================================
    
    public static boolean getBoolForKey(String key, boolean defaultValue) {
    	SharedPreferences settings = ((Activity)sContext).getSharedPreferences(CrossAppHelper.PREFS_NAME, 0);
    	return settings.getBoolean(key, defaultValue);
    }
    
    public static int getIntegerForKey(String key, int defaultValue) {
    	SharedPreferences settings = ((Activity)sContext).getSharedPreferences(CrossAppHelper.PREFS_NAME, 0);
    	return settings.getInt(key, defaultValue);
    }
    
    public static float getFloatForKey(String key, float defaultValue) {
    	SharedPreferences settings = ((Activity)sContext).getSharedPreferences(CrossAppHelper.PREFS_NAME, 0);
    	return settings.getFloat(key, defaultValue);
    }
    
    public static double getDoubleForKey(String key, double defaultValue) {
    	// SharedPreferences doesn't support saving double value
    	SharedPreferences settings = ((Activity)sContext).getSharedPreferences(CrossAppHelper.PREFS_NAME, 0);
    	return settings.getFloat(key, (float)defaultValue);
    }
    
    public static String getStringForKey(String key, String defaultValue) {
    	SharedPreferences settings = ((Activity)sContext).getSharedPreferences(CrossAppHelper.PREFS_NAME, 0);
    	return settings.getString(key, defaultValue);
    }
    
    public static void setBoolForKey(String key, boolean value) {
    	SharedPreferences settings = ((Activity)sContext).getSharedPreferences(CrossAppHelper.PREFS_NAME, 0);
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putBoolean(key, value);
    	editor.commit();
    }
    
    public static void setIntegerForKey(String key, int value) {
    	SharedPreferences settings = ((Activity)sContext).getSharedPreferences(CrossAppHelper.PREFS_NAME, 0);
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putInt(key, value);
    	editor.commit();
    }
    
    public static void setFloatForKey(String key, float value) {
    	SharedPreferences settings = ((Activity)sContext).getSharedPreferences(CrossAppHelper.PREFS_NAME, 0);
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putFloat(key, value);
    	editor.commit();
    }
    
    public static void setDoubleForKey(String key, double value) {
    	// SharedPreferences doesn't support recording double value
    	SharedPreferences settings = ((Activity)sContext).getSharedPreferences(CrossAppHelper.PREFS_NAME, 0);
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putFloat(key, (float)value);
    	editor.commit();
    }
    
    public static void setStringForKey(String key, String value) {
    	SharedPreferences settings = ((Activity)sContext).getSharedPreferences(CrossAppHelper.PREFS_NAME, 0);
    	SharedPreferences.Editor editor = settings.edit();
    	editor.putString(key, value);
    	editor.commit();
    }
	
	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================

	public static boolean issAccelerometerEnabled() {
		return sAccelerometerEnabled;
	}

	public static void setsAccelerometerEnabled(boolean sAccelerometerEnabled) {
		CrossAppHelper.sAccelerometerEnabled = sAccelerometerEnabled;
	}

	public static CrossAppAccelerometer getsCrossAppAccelerometer() {
		return sCrossAppAccelerometer;
	}

	public static void setsCrossAppAccelerometer(CrossAppAccelerometer sCrossAppAccelerometer) {
		CrossAppHelper.sCrossAppAccelerometer = sCrossAppAccelerometer;
	}

	public static interface CrossAppHelperListener {
		public void showDialog(final String pTitle, final String pMessage);
		public void showEditTextDialog(final String pTitle, final String pMessage, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength);

		public void runOnGLThread(final Runnable pRunnable);
	}
}
