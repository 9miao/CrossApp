
package org.CrossApp.lib;

import java.io.UnsupportedEncodingException;

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

		this.mText = s.toString();
	}

	@Override
	public void beforeTextChanged(final CharSequence pCharSequence, final int start, final int count, final int after) {

		Log.d(TAG, "beforeTextChanged(" + pCharSequence + ")start: " + start + ",count: " + count + ",after: " + after);

	}

	@Override
	public void onTextChanged(final CharSequence pCharSequence, final int start, final int before, final int count) {
		
		Log.d(TAG, "onTextChanged(" + pCharSequence + ")start: " + start + ",before: " + before + ",count: " + count+" inputLen="+pCharSequence.length());

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
		String tmp =  subReplace;
		if(start < subReplace.length()) {
			tmp =  subReplace.substring(start);
		}
 		final char[] chars = tmp.toCharArray();

 		int codePoint;
 		boolean isSurrogatePair;
 		// check every char in the input text
 		for (int i = 0; i < chars.length; i++) {
 			// if the char is a leading part of a surrogate pair (Unicode)
 			if (Character.isHighSurrogate(chars[i])) {
 				// just ignore it and wait for the trailing part
 				continue;
 			}
 			// if the char is a trailing part of a surrogate pair (Unicode)
 			else if (Character.isLowSurrogate(chars[i])) {
 				// if the char and its predecessor are indeed a valid surrogate pair
 				if (i > 0 && Character.isSurrogatePair(chars[i-1], chars[i])) {
 					// get the Unicode code point for the surrogate pair
 					codePoint = Character.toCodePoint(chars[i-1], chars[i]);
 					// remember that we have a surrogate pair here (which counts as two characters)
 					isSurrogatePair = true;
 				}
 				// if the char and its predecessor are not actually a valid surrogate pair
 				else {
 					// just ignore it
 					continue;
 				}
 			}
 			// if the char is not part of a surrogate pair, i.e. a simple char from the BMP
 			else {
 				// get the Unicode code point by simply casting the char to int
 				codePoint = (int) chars[i];
 				// remember that we have a BMP symbol here (which counts as a single character)
 				isSurrogatePair = false;
 			}
 			String bytes = "";
 			for(byte b: subReplace.getBytes()){
 				bytes += Integer.toHexString(b);
 			}
 			Log.d(TAG, "pCharSequence =0x"+ Integer.toHexString(codePoint)+" len="+subReplace.length()+" len2="+bytes);
 		}
 		
 		this.mCocos2dxGLSurfaceView.willInsertText(start, subReplace, (subReplace.length() == 0 && before == 2) ? 1 : before, count);
	}

	
	@Override
	public boolean onEditorAction(final TextView pTextView, final int pActionID, final KeyEvent pKeyEvent) 
	{
		if (pKeyEvent == null)
			return true;
		
		if (pActionID == EditorInfo.IME_ACTION_DONE) 
		{
			this.mCocos2dxGLSurfaceView.queueEvent(new Runnable() {
				@Override
				public void run() {
					KeyBoardReturnCallBack();
				}
			});
            return true;
		}

		if (pActionID == EditorInfo.IME_ACTION_SEARCH)
        {
			this.mCocos2dxGLSurfaceView.queueEvent(new Runnable() {
				@Override
				public void run() {
					KeyBoardReturnCallBack();
				}
			});
            return true;
		}

		if (pActionID == EditorInfo.IME_ACTION_SEND)
		{
			this.mCocos2dxGLSurfaceView.queueEvent(new Runnable() {
				@Override
				public void run() {
					KeyBoardReturnCallBack();
				}
			});
            return true;
		}
        
        if (this.mCocos2dxGLSurfaceView.getCocos2dxEditText() != pTextView)
        {
            return false;
        }
        
        if(pKeyEvent.getAction() == KeyEvent.ACTION_DOWN)
        {
            return true;
        }
        
        if (pActionID == EditorInfo.IME_ACTION_UNSPECIFIED)
        {
        	this.mCocos2dxGLSurfaceView.queueEvent(new Runnable() {
				@Override
				public void run() {
					mCocos2dxGLSurfaceView.insertText("\n");
				}
			});
            return true;
        }
		return true;
	}

}