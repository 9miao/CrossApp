
package org.CrossApp.lib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.content.pm.ActivityInfo;
import android.opengl.GLSurfaceView;

public class CrossAppRenderer implements GLSurfaceView.Renderer {
	// ===========================================================
	// Constants
	// ===========================================================

	private final static long NANOSECONDSPERSECOND = 1000000000L;
	private final static long NANOSECONDSPERMICROSECOND = 1000000;

	private static long sAnimationInterval = (long) (1.0 / 60 * CrossAppRenderer.NANOSECONDSPERSECOND);

	// ===========================================================
	// Fields
	// ===========================================================

	private long mLastTickInNanoSeconds;
	private CrossAppGLSurfaceView mGLSurfaceView;
	private int mScreenWidth;
	private int mScreenHeight;

	// ===========================================================
	// Constructors
	// ===========================================================

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	public static void setAnimationInterval(final double pAnimationInterval) {
		CrossAppRenderer.sAnimationInterval = (long) (pAnimationInterval * CrossAppRenderer.NANOSECONDSPERSECOND);
	}

	public void setScreenWidthAndHeight(final int pSurfaceWidth, final int pSurfaceHeight) {
		this.mScreenWidth = pSurfaceWidth;
		this.mScreenHeight = pSurfaceHeight;
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void onSurfaceCreated(final GL10 pGL10, final EGLConfig pEGLConfig) 
	{
		final int orientation;
		if(((CrossAppActivity)CrossAppActivity.getContext()).getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT)
        {
        	orientation = 1;
        }
        else if(((CrossAppActivity)CrossAppActivity.getContext()).getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE) 
        {
        	orientation = 2;
        }
        else
        {
        	orientation = 1;
        }
		
		CrossAppRenderer.nativeChangedOrientation(orientation);
		this.crossAppRendererNativeInit();
		
		this.mLastTickInNanoSeconds = System.nanoTime();
	}
	
	public void crossAppRendererNativeInit()
	{
		CrossAppRenderer.nativeInit(this.mScreenWidth, this.mScreenHeight);
	}

	@Override
	public void onSurfaceChanged(final GL10 pGL10, final int pWidth, final int pHeight)
	{

	}

	@Override
	public void onDrawFrame(final GL10 gl) 
	{
		CrossAppRenderer.nativeRender();
	}

	// ===========================================================
	// Methods
	// ===========================================================

	private static native void nativeTouchesBegin(final int pID, final float pX, final float pY);
	private static native void nativeTouchesEnd(final int pID, final float pX, final float pY);
	private static native void nativeTouchesMove(final int[] pIDs, final float[] pXs, final float[] pYs);
	private static native void nativeTouchesCancel(final int[] pIDs, final float[] pXs, final float[] pYs);
	private static native boolean nativeKeyDown(final int pKeyCode);
	private static native void nativeRender();
	private static native void nativeInit(final int pWidth, final int pHeight);
	public static native void nativeChanged(final int pWidth, final int pHeight);
	public static native void nativeChangedOrientation(final int type);
	private static native void nativeOnPause();
	private static native void nativeOnResume();
	private static native void nativeCloseKeyPad();
	private static native void nativeOpenKeyPad();

	public void handleActionDown(final int pID, final float pX, final float pY) {
		CrossAppRenderer.nativeTouchesBegin(pID, pX, pY);
	}

	public void handleActionUp(final int pID, final float pX, final float pY) {
		CrossAppRenderer.nativeTouchesEnd(pID, pX, pY);
	}

	public void handleActionCancel(final int[] pIDs, final float[] pXs, final float[] pYs) {
		CrossAppRenderer.nativeTouchesCancel(pIDs, pXs, pYs);
	}

	public void handleActionMove(final int[] pIDs, final float[] pXs, final float[] pYs) {
		CrossAppRenderer.nativeTouchesMove(pIDs, pXs, pYs);
	}

	public void handleKeyDown(final int pKeyCode) {
		CrossAppActivity.getContext().runOnGLThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	CrossAppRenderer.nativeKeyDown(pKeyCode);
            }
        });
	}

	public void handleOnPause() {
		CrossAppRenderer.nativeOnPause();
	}
	
	public void handleCloseKeyPad()
	{
		CrossAppRenderer.nativeCloseKeyPad();
	}

	public void handleOpenKeyPad()
	{
		CrossAppRenderer.nativeOpenKeyPad();
	}
	
	public void handleOnResume() {
		CrossAppRenderer.nativeOnResume();
	}

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
