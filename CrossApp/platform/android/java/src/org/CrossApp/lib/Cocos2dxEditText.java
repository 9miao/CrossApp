
package org.CrossApp.lib;

import android.content.Context;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.widget.EditText;

public class Cocos2dxEditText extends EditText {
	// ===========================================================
	// Constants
	// ===========================================================

	// ===========================================================
	// Fields
	// ===========================================================

	private Cocos2dxGLSurfaceView mCocos2dxGLSurfaceView;

	// ===========================================================
	// Constructors
	// ===========================================================

	public Cocos2dxEditText(final Context context) {
		super(context);
	}

	public Cocos2dxEditText(final Context context, final AttributeSet attrs) {
		super(context, attrs);
	}

	public Cocos2dxEditText(final Context context, final AttributeSet attrs, final int defStyle) {
		super(context, attrs, defStyle);
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	public void setCocos2dxGLSurfaceView(final Cocos2dxGLSurfaceView pCocos2dxGLSurfaceView) {
		this.mCocos2dxGLSurfaceView = pCocos2dxGLSurfaceView;
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public boolean onKeyDown(final int pKeyCode, final KeyEvent pKeyEvent) {
		super.onKeyDown(pKeyCode, pKeyEvent);

		/* Let GlSurfaceView get focus if back key is input. */
		if (pKeyCode == KeyEvent.KEYCODE_BACK || pKeyCode == KeyEvent.KEYCODE_MENU
				|| pKeyCode == KeyEvent.KEYCODE_DPAD_LEFT || pKeyCode == KeyEvent.KEYCODE_DPAD_RIGHT) {
			this.mCocos2dxGLSurfaceView.requestFocus();
			this.mCocos2dxGLSurfaceView.onKeyDown(pKeyCode, pKeyEvent);
		}

		return true;
	}

	// ===========================================================
	// Methods
	// ===========================================================

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
