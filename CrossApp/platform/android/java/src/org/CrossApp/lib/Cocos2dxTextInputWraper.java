
package org.CrossApp.lib;

import android.app.Activity;
import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

public class Cocos2dxTextInputWraper implements TextWatcher, OnEditorActionListener {
	// ===========================================================
	// Constants
	// ===========================================================

	private static final String TAG = Cocos2dxTextInputWraper.class.getSimpleName();

	// ===========================================================
	// Fields
	// ===========================================================

	private final Cocos2dxGLSurfaceView mCocos2dxGLSurfaceView;
	private String mText;
	private String mOriginText;
	native static void KeyBoardReturnCallBack();
	// ===========================================================
	// Constructors
	// ===========================================================

	public Cocos2dxTextInputWraper(final Cocos2dxGLSurfaceView pCocos2dxGLSurfaceView) {
		this.mCocos2dxGLSurfaceView = pCocos2dxGLSurfaceView;
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	private boolean isFullScreenEdit() {
		final TextView textField = this.mCocos2dxGLSurfaceView.getCocos2dxEditText();
		final InputMethodManager imm = (InputMethodManager) textField.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
		return imm.isFullscreenMode();
				//isFullscreenMode();
	}

	public void setOriginText(final String pOriginText) {
		this.mOriginText = pOriginText;
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void afterTextChanged(final Editable s) {
	
		Log.d(TAG, "afterTextChanged: " + s.toString());
		
		if (this.isFullScreenEdit()) 
		{
			return;
		}
//		int nModified = s.length() - this.mText.length();
//		if (nModified > 0) {
//			final String insertText = s.subSequence(this.mText.length(), s.length()).toString();
//		
//			//this.mCocos2dxGLSurfaceView.insertText(insertText);
//			/*
//			if (BuildConfig.DEBUG) {
//				Log.d(TAG, "insertText(" + insertText + ")");
//			}
//			*/ 
//		} else {
//			for (; nModified < 0; ++nModified) {
//				//Tthis.mCocos2dxGLSurfaceView.deleteBackward();
//				/*
//				if (BuildConfig.DEBUG) {
//					Log.d(TAG, "deleteBackward");
//				}
//				*/
//			}
//		}
//		this.mText = s.toString();
	}

	@Override
	public void beforeTextChanged(final CharSequence pCharSequence, final int start, final int count, final int after) {
		/*
		if (BuildConfig.DEBUG) {
			Log.d(TAG, "beforeTextChanged(" + pCharSequence + ")start: " + start + ",count: " + count + ",after: " + after);
		}
		*/
		Log.d(TAG, "beforeTextChanged(" + pCharSequence + ")start: " + start + ",count: " + count + ",after: " + after);
		//System.out.println(pCharSequence.toString());
		this.mText = pCharSequence.toString();
	}

	@Override
	public void onTextChanged(final CharSequence pCharSequence, final int start, final int before, final int count) {
		
		Log.d(TAG, "onTextChanged(" + pCharSequence + ")start: " + start + ",before: " + before + ",count: " + count);

		String subReplace = null;
		
		try {
			CharSequence subSeq = pCharSequence.subSequence(start, start+count);
			if (subSeq != null) {
				subReplace = subSeq.toString();
			} else {
				subReplace = "";
			}
		} catch (IndexOutOfBoundsException e) {
			Log.d(TAG, e.toString());
			
			return;
		}
		
		this.mCocos2dxGLSurfaceView.willInsertText(start, subReplace, before, count);
	}

	
	@Override
	public boolean onEditorAction(final TextView pTextView, final int pActionID, final KeyEvent pKeyEvent) 
	{
		
		
		
		
		if (pActionID == EditorInfo.IME_ACTION_DONE) 
		{
			KeyBoardReturnCallBack();
            return true;
		}
		if (pActionID == EditorInfo.IME_ACTION_SEARCH)
        {
			KeyBoardReturnCallBack();
            return true;
		}
		if (pActionID == EditorInfo.IME_ACTION_SEND)
		{
			KeyBoardReturnCallBack();
            return true;
		}
        
        if (this.mCocos2dxGLSurfaceView.getCocos2dxEditText() != pTextView)
        {
            return false;
        }
        
        if(pKeyEvent.getAction() == KeyEvent.ACTION_DOWN)
        {
            return false;
        }
        if (pActionID == EditorInfo.IME_ACTION_UNSPECIFIED)
        {
            this.mCocos2dxGLSurfaceView.insertText("\n");
            return true;
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