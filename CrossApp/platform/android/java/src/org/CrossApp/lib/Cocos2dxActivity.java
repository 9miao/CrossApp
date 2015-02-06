
package org.CrossApp.lib;

import org.CrossApp.lib.Cocos2dxHelper.Cocos2dxHelperListener;


import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.WindowManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.widget.FrameLayout;
import android.widget.Toast;
import org.CrossApp.lib.AndroidVolumeControl;
import org.CrossApp.lib.AndroidNetWorkManager;
@SuppressLint("HandlerLeak")
public abstract class Cocos2dxActivity extends Activity implements Cocos2dxHelperListener {
	// ===========================================================
	// Constants
	// ===========================================================

	private static final String TAG = Cocos2dxActivity.class.getSimpleName();

	// ===========================================================
	// Fields
	// ===========================================================
    private Cocos2dxWebViewHelper mWebViewHelper = null;
	private Cocos2dxGLSurfaceView mGLSurfaceView;
	private Cocos2dxHandler mHandler;
	private static Context sContext = null;
	AndroidNativeTool actAndroidNativeTool;
	AndroidVolumeControl androidVolumeControl;
	static FrameLayout frame;
	static View rootview;
	static int keyboardheight;
	static float densityDpi;
	public static int currentBattery=0;
	private static Activity activity;
	native static void KeyBoardHeightReturn(int height);
	public static Context getContext() {
		return sContext;
	}
	public static Handler mLightHandler;
	// ===========================================================
	// Constructors
	// ===========================================================
	
	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		sContext = this;
		activity =this;
		
		DisplayMetrics metric = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(metric);
        float density = metric.density;
        densityDpi = metric.densityDpi; 
		
    	this.mHandler = new Cocos2dxHandler(this);
    	actAndroidNativeTool = new AndroidNativeTool(this);
    	
    	AndroidPersonList.Init(this);
    	AndroidNetWorkManager.setContext(this);
    	this.init();
    	rootview = this.getWindow().getDecorView();
		Cocos2dxHelper.init(this, this);
		exeHandler();
		AndroidVolumeControl.setContext(sContext);
		 if(mWebViewHelper == null)
		 {
			 mWebViewHelper = new Cocos2dxWebViewHelper(frame);
		 }
		 IntentFilter intentFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);  

	     BatteryReceiver batteryReceiver = new BatteryReceiver();  

	     registerReceiver(batteryReceiver, intentFilter);
	     
	}
	
	class BatteryReceiver extends BroadcastReceiver{  
		  
        @Override  
        public void onReceive(Context context, Intent intent) {  
            // TODO Auto-generated method stub  
            //�ж����Ƿ���Ϊ�����仯��Broadcast Action  
            if(Intent.ACTION_BATTERY_CHANGED.equals(intent.getAction())){  
                //��ȡ��ǰ����  
                int level = intent.getIntExtra("level", 0);  
                //�������̶ܿ�  
                int scale = intent.getIntExtra("scale", 100);  
                //����ת�ɰٷֱ�    
                currentBattery =level*100/ scale;
            }  
        }  
          
    }
	
	public static int getBatteryLevel()
	{
		return currentBattery;
	}
	
	 public void onActivityResult(int requestCode, int resultCode, Intent intent)
	 {
		 actAndroidNativeTool.onActivityResult(requestCode, resultCode, intent);
		 
	 }
	// ===========================================================
	// Getter & Setter
	// ===========================================================

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================
	public static float getDensitDpi() {
		return densityDpi;
	}
	public static void setScreenBrightness(int value) {
		try {
			// System.putInt(s_pContext.getContentResolver(),android.provider.Settings.System.SCREEN_BRIGHTNESS,value); 
			WindowManager.LayoutParams lp = activity.getWindow().getAttributes(); 
			lp.screenBrightness = (value<=0?1:value) / 255f;
			activity.getWindow().setAttributes(lp);
		} catch (Exception e) {
			Toast.makeText(activity,"error",Toast.LENGTH_SHORT).show();
		} 
	}
	private void exeHandler(){
		if(mLightHandler ==null){
			mLightHandler = new Handler(){
				 @Override
				public void handleMessage(Message msg) {
					int value = msg.what;
					 WindowManager.LayoutParams lp = (Cocos2dxActivity.activity).getWindow().getAttributes();  
					 lp.screenBrightness = value/255.0f;
				        (Cocos2dxActivity.activity).getWindow().setAttributes(lp);  
				}
			 };
		 }
	}
	public static void startGps() {
		AndroidGPS.Init(activity);
		
	}
	@Override
	protected void onResume() {
		super.onResume();

		Cocos2dxHelper.onResume();
		this.mGLSurfaceView.onResume();
		if (AndroidGPS.locationManager!=null) 
		{
			AndroidGPS.locationManager.requestLocationUpdates(AndroidGPS.locationManager.GPS_PROVIDER, 1000, 1, AndroidGPS.locationListener);
		}
	}

	@Override
	protected void onPause() {
		super.onPause();

		Cocos2dxHelper.onPause();
		this.mGLSurfaceView.onPause();
		if (AndroidGPS.locationManager!=null) 
		{
			AndroidGPS.locationManager.removeUpdates(AndroidGPS.locationListener);
		}
	}

	@Override
	public void showDialog(final String pTitle, final String pMessage) {
		Message msg = new Message();
		msg.what = Cocos2dxHandler.HANDLER_SHOW_DIALOG;
		msg.obj = new Cocos2dxHandler.DialogMessage(pTitle, pMessage);
		this.mHandler.sendMessage(msg);
	}

	@Override
	public void showEditTextDialog(final String pTitle, final String pContent, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength) { 
		Message msg = new Message();
		msg.what = Cocos2dxHandler.HANDLER_SHOW_EDITBOX_DIALOG;
		msg.obj = new Cocos2dxHandler.EditBoxMessage(pTitle, pContent, pInputMode, pInputFlag, pReturnType, pMaxLength);
		this.mHandler.sendMessage(msg);
	}
	
	@Override
	public void runOnGLThread(final Runnable pRunnable) {
		this.mGLSurfaceView.queueEvent(pRunnable);
	}

	// ===========================================================
	// Methods
	// ===========================================================
	public void init() {
		
    	// FrameLayout
        ViewGroup.LayoutParams framelayout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                                       ViewGroup.LayoutParams.FILL_PARENT);
        FrameLayout framelayout = new FrameLayout(this);
        framelayout.setLayoutParams(framelayout_params);
        frame = framelayout;
        // Cocos2dxEditText layout
        ViewGroup.LayoutParams edittext_layout_params =
            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                                       ViewGroup.LayoutParams.WRAP_CONTENT);
        Cocos2dxEditText edittext = new Cocos2dxEditText(this);
        edittext.setLayoutParams(edittext_layout_params);

        // ...add to FrameLayout
        framelayout.addView(edittext);

        // Cocos2dxGLSurfaceView
        this.mGLSurfaceView = this.onCreateView();

        // ...add to FrameLayout
        framelayout.addView(this.mGLSurfaceView);

        // Switch to supported OpenGL (ARGB888) mode on emulator
        if (isAndroidEmulator())
           this.mGLSurfaceView.setEGLConfigChooser(8 , 8, 8, 8, 16, 0);

        this.mGLSurfaceView.setCocos2dxRenderer(new Cocos2dxRenderer());
        this.mGLSurfaceView.setCocos2dxEditText(edittext);

        getKeyBoardHeight();
        // Set framelayout as the content view
		setContentView(framelayout);
	}
	public static int getKeyBoardHeight()
	{
		frame.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {

            @Override
            public void onGlobalLayout() {
                // TODO Auto-generated method stub
                Rect r = new Rect();

                rootview.getWindowVisibleDisplayFrame(r);

                Rect frame = new Rect();
                rootview.getWindowVisibleDisplayFrame(frame);
                int statusBarHeight = frame.top;
                
                int screenHeight = rootview.getRootView().getHeight();
                keyboardheight =screenHeight- (r.bottom - (r.top-statusBarHeight));
                if (keyboardheight!=0)
        		{
        			KeyBoardHeightReturn(keyboardheight);
        		}
                //boolean visible = heightDiff > screenHeight / 3;
            }
        });
		
		return keyboardheight;
		
	}
	
	public static int dip2px(Context context, float dpValue) {  
	     final float scale = context.getResources().getDisplayMetrics().density;  
	     return (int) (dpValue * scale + 0.5f);  
	}  
	  
    public Cocos2dxGLSurfaceView onCreateView() {
    	return new Cocos2dxGLSurfaceView(this);
    }

   private final static boolean isAndroidEmulator() {
      String model = Build.MODEL;
      Log.d(TAG, "model=" + model);
      String product = Build.PRODUCT;
      Log.d(TAG, "product=" + product);
      boolean isEmulator = false;
      if (product != null) {
         isEmulator = product.equals("sdk") || product.contains("_sdk") || product.contains("sdk_");
      }
      Log.d(TAG, "isEmulator=" + isEmulator);
      return isEmulator;
   }

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
