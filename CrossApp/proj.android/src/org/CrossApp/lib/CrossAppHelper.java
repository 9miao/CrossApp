
package org.CrossApp.lib;

import java.io.UnsupportedEncodingException;
import java.util.Locale;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetManager;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.StateListDrawable;
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

	private static CrossAppMusic sCrossAppMusic;
	private static CrossAppSound sCrossAppound;
	private static AssetManager sAssetManager;
	private static CrossAppGyroscope sCrossAppGyroscope;
	private static boolean bGyroscope;
	private static String sPackageName;
	private static String sFileDirectory;
	private static Context sContext = null;
	private static CrossAppHelperListener sCrossAppHelperListener;
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

		CrossAppHelper.setCrossAppGyroscope(new CrossAppGyroscope(pContext));
		CrossAppHelper.sCrossAppMusic = new CrossAppMusic(pContext);
		CrossAppHelper.sCrossAppound = new CrossAppSound(pContext);
		CrossAppHelper.sAssetManager = pContext.getAssets();
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

	public static void enableGyroscopr() {
		CrossAppHelper.setGyroscopeEnable(true);
		CrossAppHelper.getCrossAppGyroscope().enable();
	}

	public static void setGyroscoprInterval(float interval) {
		CrossAppHelper.getCrossAppGyroscope().setInterval(interval);
	}

	public static void disableGyroscopr() {
		CrossAppHelper.setGyroscopeEnable(false);
		CrossAppHelper.getCrossAppGyroscope().disable();
	}


	public static void onResume() {

		if(CrossAppHelper.isGyroscopeEnabled())
		{
			CrossAppHelper.getCrossAppGyroscope().enable();
		}
	}

	public static void onPause() {

		if(CrossAppHelper.isGyroscopeEnabled())
		{
			CrossAppHelper.getCrossAppGyroscope().disable();
		}
	}

	public static void preloadBackgroundMusic(final String pPath) {
		CrossAppHelper.sCrossAppMusic.preloadBackgroundMusic(pPath);
	}

	public static void playBackgroundMusic(final String pPath, final boolean isLoop) {
		CrossAppHelper.sCrossAppMusic.playBackgroundMusic(pPath, isLoop);
	}

	public static void resumeBackgroundMusic() {
		CrossAppHelper.sCrossAppMusic.resumeBackgroundMusic();
	}

	public static void pauseBackgroundMusic() {
		CrossAppHelper.sCrossAppMusic.pauseBackgroundMusic();
	}

	public static void stopBackgroundMusic() {
		CrossAppHelper.sCrossAppMusic.stopBackgroundMusic();
	}

	public static void rewindBackgroundMusic() {
		CrossAppHelper.sCrossAppMusic.rewindBackgroundMusic();
	}

	public static boolean isBackgroundMusicPlaying() {
		return CrossAppHelper.sCrossAppMusic.isBackgroundMusicPlaying();
	}

	public static float getBackgroundMusicVolume() {
		return CrossAppHelper.sCrossAppMusic.getBackgroundVolume();
	}

	public static void setBackgroundMusicVolume(final float volume) {
		CrossAppHelper.sCrossAppMusic.setBackgroundVolume(volume);
	}

	public static void preloadEffect(final String path) {
		CrossAppHelper.sCrossAppound.preloadEffect(path);
	}

	public static int playEffect(final String path, final boolean isLoop) {
		return CrossAppHelper.sCrossAppound.playEffect(path, isLoop);
	}

	public static void resumeEffect(final int soundId) {
		CrossAppHelper.sCrossAppound.resumeEffect(soundId);
	}

	public static void pauseEffect(final int soundId) {
		CrossAppHelper.sCrossAppound.pauseEffect(soundId);
	}

	public static void stopEffect(final int soundId) {
		CrossAppHelper.sCrossAppound.stopEffect(soundId);
	}

	public static float getEffectsVolume() {
		return CrossAppHelper.sCrossAppound.getEffectsVolume();
	}

	public static void setEffectsVolume(final float volume) {
		CrossAppHelper.sCrossAppound.setEffectsVolume(volume);
	}

	public static void unloadEffect(final String path) {
		CrossAppHelper.sCrossAppound.unloadEffect(path);
	}

	public static void pauseAllEffects() {
		CrossAppHelper.sCrossAppound.pauseAllEffects();
	}

	public static void resumeAllEffects() {
		CrossAppHelper.sCrossAppound.resumeAllEffects();
	}

	public static void stopAllEffects() {
		CrossAppHelper.sCrossAppound.stopAllEffects();
	}

	public static void end() {
		CrossAppHelper.sCrossAppMusic.end();
		CrossAppHelper.sCrossAppound.end();
	}


	public static void terminateProcess() {
		android.os.Process.killProcess(android.os.Process.myPid());
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

	public static int dip2px(Context context, float dpValue) {
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (dpValue * scale + 0.5f);
	}

	public static int px2dip(Context context, float pxValue) {
		final float scale = context.getResources().getDisplayMetrics().density;
		return (int) (pxValue / scale + 0.5f);
	}


	/**
	 * 生成正方形圆角的shape
	 * @param context
	 * @param colorresid
	 * @param ridous
	 * @return
	 */
	public static Drawable getCornerRectAngle(Context context ,int colorresid,int ridous,Rect rect){
		return getCorlorRectAngleContaionColor(context, context.getResources().getColor(colorresid), ridous, rect) ;
	}

	public static Drawable getCorlorRectAngle(Context context ,String colorHex,int ridous,Rect rect){
		return getCorlorRectAngleContaionColor(context, Color.parseColor(colorHex), ridous, rect) ;
	}

	public static Drawable getCorlorRectAngleContaionColor(Context context ,int color,int ridous,Rect rect){
		GradientDrawable mGradientDrawable = new GradientDrawable() ;
		mGradientDrawable.setCornerRadius(ridous);
		mGradientDrawable.setBounds(rect== null ? new Rect() : rect) ;
		mGradientDrawable.setColor(color);
		return mGradientDrawable ;
	}

	/**
	 * @param context
	 * @param colorHex
	 * @param ridous
	 * @param rect
	 * @param which  0 : top-left , 1 : top-right , 2 : bottom-right , 3 : bottom-left
	 * @return
	 */
	public static Drawable getColorRectAngleInCorner(Context context , String colorHex  , int ridous , Rect rect , int which){

		int l = which == 0 ? ridous : 0 ;
		int t = which  ==1 ? ridous : 0  ;
		int r = which == 2 ? ridous : 0 ;
		int b = which == 3 ? ridous : 0  ;

		float[] radii = new float[]{l,l ,t , t , r , t, b , b};

		GradientDrawable drawable = new GradientDrawable();
		drawable.setCornerRadii(radii);
		drawable.setBounds(rect== null ? new Rect() : rect);
		drawable.setColor(Color.parseColor(colorHex));

		return drawable ;
	}

	/**
	 * 代码生成Selector
	 * @param context
	 * @param normal
	 * @param pressed
	 * @return
	 */
	public static StateListDrawable setSelector(Context context,Drawable normal ,Drawable pressed){
		StateListDrawable bg=new StateListDrawable();
		bg.addState(new int[] { android.R.attr.state_pressed, android.R.attr.state_enabled }, pressed);
		bg.addState(new int[] { android.R.attr.state_enabled }, normal);
		bg.addState(new int[] {}, normal);
		return bg;
	}

	/**
	 * 得到某种颜色的矩形
	 * @param context
	 * @param colorresid
	 * @return
	 */
	public static Drawable getRectAngle(Context context ,int colorresid,Rect rect){
		GradientDrawable mGradientDrawable = new GradientDrawable() ;
		mGradientDrawable.setAlpha(100);
		mGradientDrawable.setBounds(rect) ;
		mGradientDrawable.setColor(context.getResources().getColor(colorresid));
		return mGradientDrawable ;
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


	// Gyroscope
	public static boolean isGyroscopeEnabled() {
		return bGyroscope;
	}

	public static void setGyroscopeEnabled(boolean gyroscopeEnabled) {
		CrossAppHelper.bGyroscope = gyroscopeEnabled;
	}

	public static void setGyroscopeEnable(boolean gyroEnable) {
		CrossAppHelper.bGyroscope = gyroEnable;
	}

	public static CrossAppGyroscope getCrossAppGyroscope() {
		return sCrossAppGyroscope;
	}

	public static void setCrossAppGyroscope(CrossAppGyroscope gyroscope) {
		CrossAppHelper.sCrossAppGyroscope = gyroscope;
	}

	public static interface CrossAppHelperListener
	{
		public void runOnGLThread(final Runnable pRunnable);
	}
}
