

package org.CrossApp.lib;

import java.lang.ref.WeakReference;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.widget.Toast;

public class Cocos2dxHandler extends Handler {
	// ===========================================================
	// Constants
	// ===========================================================
	public final static int HANDLER_SHOW_DIALOG = 1;
	public final static int HANDLER_SHOW_EDITBOX_DIALOG = 2;
	public final static int HANDLER_SHOW_TOAST = 3;
	// ===========================================================
	// Fields
	// ===========================================================
	private WeakReference<Cocos2dxActivity> mActivity;
	
	// ===========================================================
	// Constructors
	// ===========================================================
	public Cocos2dxHandler(Cocos2dxActivity activity) {
		this.mActivity = new WeakReference<Cocos2dxActivity>(activity);
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

	public void handleMessage(Message msg) {
		switch (msg.what) {
		case Cocos2dxHandler.HANDLER_SHOW_DIALOG:
			showDialog(msg);
			break;
		case Cocos2dxHandler.HANDLER_SHOW_EDITBOX_DIALOG:
			showEditBoxDialog(msg);
			break;

		case Cocos2dxHandler.HANDLER_SHOW_TOAST:
			showCAToast(msg);
			break;
		}
	}
	
	private void showDialog(Message msg) {
		Cocos2dxActivity theActivity = this.mActivity.get();
		DialogMessage dialogMessage = (DialogMessage)msg.obj;
		new AlertDialog.Builder(theActivity)
		.setTitle(dialogMessage.titile)
		.setMessage(dialogMessage.message)
		.setPositiveButton("Ok", 
				new DialogInterface.OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub
						
					}
				}).create().show();
	}
	
	class ToastShowRunnable implements Runnable
	{
		private Toast m_toast = null;
		public ToastShowRunnable(Toast _t)
		{
			m_toast = _t;
		}
		public void run() {
			m_toast.show();
		}
	}
    
	private void showCAToast(Message msg) {
		final Cocos2dxActivity theActivity = this.mActivity.get();
		ToastMessage toastMessage = (ToastMessage)msg.obj;
		new Handler(Looper.getMainLooper()).post(
				new ToastShowRunnable(
						Toast.makeText(theActivity,
								       toastMessage.message,
								       toastMessage._flag==0 ? Toast.LENGTH_SHORT:Toast.LENGTH_LONG)));
		
  		// toast.setGravity(Gravity.CENTER, 0, 0);
	}
	
	private void showEditBoxDialog(Message msg) {
		EditBoxMessage editBoxMessage = (EditBoxMessage)msg.obj;
		new Cocos2dxEditBoxDialog(this.mActivity.get(),
				editBoxMessage.title,
				editBoxMessage.content,
				editBoxMessage.inputMode,
				editBoxMessage.inputFlag,
				editBoxMessage.returnType,
				editBoxMessage.maxLength).show();
	}
	
	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
	
	public static class DialogMessage {
		public String titile;
		public String message;
		
		public DialogMessage(String title, String message) {
			this.titile = title;
			this.message = message;
		}
	}
	
	public static class ToastMessage {
		public String message;
		public int _flag;
		public ToastMessage(String message, int flag) {
			this.message = message;
			this._flag    = flag;
		}
	}
	
	public static class EditBoxMessage {
		public String title;
		public String content;
		public int inputMode;
		public int inputFlag;
		public int returnType;
		public int maxLength;
		
		public EditBoxMessage(String title, String content, int inputMode, int inputFlag, int returnType, int maxLength){
			this.content = content;
			this.title = title;
			this.inputMode = inputMode;
			this.inputFlag = inputFlag;
			this.returnType = returnType;
			this.maxLength = maxLength;
		}
	}
}
