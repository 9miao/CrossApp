
package org.CrossApp.lib;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

public class Cocos2dxRenderer implements GLSurfaceView.Renderer {
	// ===========================================================
	// Constants
	// ===========================================================

	private final static long NANOSECONDSPERSECOND = 1000000000L;
	private final static long NANOSECONDSPERMICROSECOND = 1000000;

	private static long sAnimationInterval = (long) (1.0 / 60 * Cocos2dxRenderer.NANOSECONDSPERSECOND);

	// ===========================================================
	// Fields
	// ===========================================================

	private long mLastTickInNanoSeconds;
	private int mScreenWidth;
	private int mScreenHeight;

	// ===========================================================
	// Constructors
	// ===========================================================

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	public static void setAnimationInterval(final double pAnimationInterval) {
		Cocos2dxRenderer.sAnimationInterval = (long) (pAnimationInterval * Cocos2dxRenderer.NANOSECONDSPERSECOND);
	}

	public void setScreenWidthAndHeight(final int pSurfaceWidth, final int pSurfaceHeight) {
		this.mScreenWidth = pSurfaceWidth;
		this.mScreenHeight = pSurfaceHeight;
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void onSurfaceCreated(final GL10 pGL10, final EGLConfig pEGLConfig) {
		Cocos2dxRenderer.nativeInit(this.mScreenWidth, this.mScreenHeight);
		this.mLastTickInNanoSeconds = System.nanoTime();
	}

	@Override
	public void onSurfaceChanged(final GL10 pGL10, final int pWidth, final int pHeight) {
	}

	@Override
	public void onDrawFrame(final GL10 gl) {
		/*
		 * FPS controlling algorithm is not accurate, and it will slow down FPS
		 * on some devices. So comment FPS controlling code.
		 */
		
		/*
		final long nowInNanoSeconds = System.nanoTime();
		final long interval = nowInNanoSeconds - this.mLastTickInNanoSeconds;
		*/

		// should render a frame when onDrawFrame() is called or there is a
		// "ghost"
		Cocos2dxRenderer.nativeRender();

		/*
		// fps controlling
		if (interval < Cocos2dxRenderer.sAnimationInterval) {
			try {
				// because we render it before, so we should sleep twice time interval
				Thread.sleep((Cocos2dxRenderer.sAnimationInterval - interval) / Cocos2dxRenderer.NANOSECONDSPERMICROSECOND);
			} catch (final Exception e) {
			}
		}

		this.mLastTickInNanoSeconds = nowInNanoSeconds;
		*/
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
	private static native void nativeOnPause();
	private static native void nativeOnResume();

	public void handleActionDown(final int pID, final float pX, final float pY) {
		Cocos2dxRenderer.nativeTouchesBegin(pID, pX, pY);
	}

	public void handleActionUp(final int pID, final float pX, final float pY) {
		Cocos2dxRenderer.nativeTouchesEnd(pID, pX, pY);
	}

	public void handleActionCancel(final int[] pIDs, final float[] pXs, final float[] pYs) {
		Cocos2dxRenderer.nativeTouchesCancel(pIDs, pXs, pYs);
	}

	public void handleActionMove(final int[] pIDs, final float[] pXs, final float[] pYs) {
		Cocos2dxRenderer.nativeTouchesMove(pIDs, pXs, pYs);
	}

	public void handleKeyDown(final int pKeyCode) {
		Cocos2dxRenderer.nativeKeyDown(pKeyCode);
	}

	public void handleOnPause() {
		Cocos2dxRenderer.nativeOnPause();
	}

	public void handleOnResume() {
		Cocos2dxRenderer.nativeOnResume();
	}

	private static native void nativeInsertText(final String pText);
	private static native void nativeAndroidWillInsertText(final int start,final String pString,final int before,final int count);
	private static native void nativeDeleteBackward();
	private static native String nativeGetContentText();
	private static native int nativeGetCursorPos();
	private static native int[] nativeGetCharRange();

	public void handleInsertText(final String pText) {
		Cocos2dxRenderer.nativeInsertText(pText);
	}

	public void handleWillInsertText(final int start,final String pString,final int before,final int count) {
		Cocos2dxRenderer.nativeAndroidWillInsertText(start,pString,before,count);
	}
	
	public void handleDeleteBackward() {
		Cocos2dxRenderer.nativeDeleteBackward();
	}

	public String getContentText() {
		return Cocos2dxRenderer.nativeGetContentText();
	}

	public int getCursorPos() {
		return Cocos2dxRenderer.nativeGetCursorPos();
	}

	public int[] getCharRange() {
		return Cocos2dxRenderer.nativeGetCharRange();
	}

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
