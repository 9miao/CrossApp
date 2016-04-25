
package org.CrossApp.lib;

import java.util.concurrent.Callable;
import org.CrossApp.lib.CrossAppHelper.CrossAppHelperListener;
import android.annotation.SuppressLint;
import android.app.ActionBar.LayoutParams;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.WindowManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.widget.FrameLayout;
import android.widget.Toast;
import org.CrossApp.lib.CrossAppVolumeControl;
import org.CrossApp.lib.CrossAppNetWorkManager;
import org.CrossApp.lib.CrossAppDevice;

@SuppressLint("HandlerLeak")
public abstract class CrossAppActivity extends Activity implements CrossAppHelperListener {
	// ===========================================================
	// Constants
	// ===========================================================

	private static final String TAG = CrossAppActivity.class.getSimpleName();

	// ===========================================================
	// Fields
	// ===========================================================
    private CrossAppWebViewHelper mWebViewHelper = null;
    
	private CrossAppGLSurfaceView mGLSurfaceView;
	
	private CrossAppHandler mHandler;
	
	private static CrossAppRenderer mCrossAppRenderer;
	
    public static Handler msHandler;
   	
    public static Handler mLightHandler;
    
	private static CrossAppActivity s_pActivity;
	
	private static FrameLayout frame;
	
	private static View rootview;
	
	private int screenWidth = 0;
	
	private int screenHeight = 0;
	
	private int orientation = 0;
	
	private static BluetoothAdapter mAdapter = null;
	
    private final int REQUEST_OPEN_BT_CODE = 1;
    
    private final int REQUEST_DISCOVERY_BT_CODE = 2;
    
    native static void returnBlueToothState(int state);
    
    native static void returnDiscoveryDevice(CrossAppBlueTooth sender);
    
    native static void returnStartedDiscoveryDevice();
    
    native static void returnFinfishedDiscoveryDevice();
    
    public static CrossAppTextField _sTextField = null;
    
    public static CrossAppTextView _sTextView = null;
    
    public static CrossAppNativeTool nativeTool;
    
    public static CrossAppDevice actAppDevice;
    
    public static void setSingleTextField(CrossAppTextField text) 
    {
		_sTextField = text;
	}
    
	public static void setSingleTextView(CrossAppTextView text) 
	{
		_sTextView = text;
	}
	
	public static CrossAppActivity getContext()
	{
		return CrossAppActivity.s_pActivity;
	}
	
	public static FrameLayout getFrameLayout()
	{
		return CrossAppActivity.frame;
	}
	

	// ===========================================================
	// Constructors
	// ===========================================================

	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		s_pActivity = this;

    	this.mHandler = new CrossAppHandler(this);

    	CrossAppVolumeControl.setContext(s_pActivity);
    	
    	CrossAppPersonList.Init(this);
    	
		CrossAppHelper.init(this, this);

		CrossAppNetWorkManager.setContext(this);
		
		 actAppDevice = new CrossAppDevice(this) {};
		
		nativeTool = new CrossAppNativeTool(this);

    	this.init();

		this.exeHandler();

		rootview = this.getWindow().getDecorView();
		
		 if(savedInstanceState == null)
		 {
			mWebViewHelper = new CrossAppWebViewHelper(frame);
			
			CrossAppTextField.initWithHandler();
			
			CrossAppTextView.initWithHandler();
			
		 }
		 else if (savedInstanceState != null)
		 {
			 CrossAppTextField.reload();
			 CrossAppTextView.reload();
			 if (savedInstanceState.containsKey("WEBVIEW"))
			 {
				 mWebViewHelper = new CrossAppWebViewHelper(frame);
				 String[] strs = savedInstanceState.getStringArray("WEBVIEW");
				 mWebViewHelper.setAllWebviews(strs);
			 }
			 savedInstanceState.clear();
		 }
		 
		this.mHandler = new CrossAppHandler(this);
		
		exeHandler();
		
		CrossAppNetWorkManager.setContext(this);
		
	    CrossAppVolumeControl.setContext(s_pActivity);
		
	    CrossAppPersonList.Init(this);
		
		IntentFilter intentFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
	}

	@Override
	protected void onResume() 
	{
		super.onResume();
		if (_sTextField != null) 
		{
			_sTextField.resume();
		}
		
		if (_sTextView != null)
		{
			_sTextView.resume();
		}

		CrossAppHelper.onResume();
		this.mGLSurfaceView.onResume();
		if (CrossAppGPS.locationManager!=null)
		{
			CrossAppGPS.locationManager.requestLocationUpdates(CrossAppGPS.locationManager.GPS_PROVIDER, 1000, 1, CrossAppGPS.locationListener);
		}
	}

	@Override
	protected void onPause() {
		super.onPause();

		CrossAppHelper.onPause();
		this.mGLSurfaceView.onPause();
		if (CrossAppGPS.locationManager!=null)
		{
			CrossAppGPS.locationManager.removeUpdates(CrossAppGPS.locationListener);
		}
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		
		outState.putStringArray("WEBVIEW", mWebViewHelper.getAllWebviews());
		super.onSaveInstanceState(outState);
	}

    public void setPasteBoardStr(String sender)
    {
        Message msg=new Message();
        msg.obj = sender;
        msg.what = 0;
        msHandler.sendMessage(msg);
    }

    public String getPasteBoardStr()
    {
		Callable<String> callable = new Callable<String>() 
		{
            @Override
            public String call() throws Exception 
            {
            	ClipboardManager clipboard =  (ClipboardManager)s_pActivity.getSystemService(Context.CLIPBOARD_SERVICE);
            	
                clipboard.getText();
                
                return clipboard.getText().toString();
            }
        };
        try {
            return CrossAppWebViewHelper.callInMainThread(callable);
        } catch (Exception e) {
        }
		return "";
    }

	public BroadcastReceiver BluetoothReciever = new BroadcastReceiver()
    {
        @Override
        public void onReceive(Context context, Intent intent)
        {
            // TODO Auto-generated method stub
            if (BluetoothAdapter.ACTION_STATE_CHANGED.equals(intent.getAction()))
            {

                int btState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                        BluetoothAdapter.STATE_OFF);

                printBTState(btState);
            }
            else if(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED.equals(intent.getAction()))
            {
                int cur_mode_state = intent.getIntExtra(BluetoothAdapter.EXTRA_SCAN_MODE, BluetoothAdapter.SCAN_MODE_NONE);
                int previous_mode_state = intent.getIntExtra(BluetoothAdapter.EXTRA_PREVIOUS_SCAN_MODE, BluetoothAdapter.SCAN_MODE_NONE);

            }
        }

    };
    
    public BroadcastReceiver BTDiscoveryReceiver = new BroadcastReceiver()
    {
        @Override
        public void onReceive(Context context, Intent intent)
        {
            // TODO Auto-generated method stub
            if (BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(intent.getAction()))
            {
                returnStartedDiscoveryDevice();
            }
            else if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(intent.getAction()))
            {
                returnFinfishedDiscoveryDevice();
            }
            else if(BluetoothDevice.ACTION_FOUND.equals(intent.getAction()))
            {
                BluetoothDevice btDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if(btDevice != null)
                {
                	CrossAppBlueTooth mAndroidBlueTooth = new CrossAppBlueTooth(btDevice.getAddress(),btDevice.getName());
                	returnDiscoveryDevice(mAndroidBlueTooth);
                }
            }
            else if(BluetoothDevice.ACTION_BOND_STATE_CHANGED.equals(intent.getAction()))
            {
                int cur_bond_state = intent.getIntExtra(BluetoothDevice.EXTRA_BOND_STATE, BluetoothDevice.BOND_NONE);
                int previous_bond_state = intent.getIntExtra(BluetoothDevice.EXTRA_PREVIOUS_BOND_STATE, BluetoothDevice.BOND_NONE);
            }
        }

    };

    private void printBTState(int btState)
    {
        switch (btState)
        {
            case BluetoothAdapter.STATE_OFF:
                break;
            case BluetoothAdapter.STATE_TURNING_OFF:
                break;
            case BluetoothAdapter.STATE_TURNING_ON:
                break;
            case BluetoothAdapter.STATE_ON:
                break;
            default:
                break;
        }
    }

	 public void onActivityResult(int requestCode, int resultCode, Intent intent)
	 {
		 nativeTool.onActivityResult(requestCode, resultCode, intent);
	 }
	 
	public static void CAImageAlbum(int type)
	{
		CrossAppNativeTool.CAImageAlbum(type);
	}
		
	// ===========================================================
	// Getter & Setter
	// ===========================================================

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	public static void setScreenBrightness(int value) {
		try {
			//System.putInt(s_pActivity.getContentResolver(),android.provider.Settings.System.SCREEN_BRIGHTNESS,value);
			WindowManager.LayoutParams lp = s_pActivity.getWindow().getAttributes();
			lp.screenBrightness = (value<=0?1:value) / 255f;
			s_pActivity.getWindow().setAttributes(lp);
		} catch (Exception e) {
			Toast.makeText(s_pActivity,"error",Toast.LENGTH_SHORT).show();
		}
	}
	
	private void exeHandler(){
		if(mLightHandler ==null){
			mLightHandler = new Handler(){

				 @Override
				public void handleMessage(Message msg) {
					int value = msg.what;
					 WindowManager.LayoutParams lp = s_pActivity.getWindow().getAttributes();
					 lp.screenBrightness = value/255.0f;
					 s_pActivity.getWindow().setAttributes(lp);
				}
			 };
		 }
        if(msHandler ==null){
            msHandler = new Handler(){

                @Override
                public void handleMessage(Message msg) {
                    String value = (String)msg.obj;
                    int what = msg.what;
                    ClipboardManager cmb = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
                    if(what == 0)
                    {

                        cmb.setText(value);
                    }
                }
            };
        }
	}
	
	public static void startUpdatingLocation() {
		CrossAppGPS.Init(s_pActivity);
		CrossAppGPS.getLocation();
	}
	
	
	@Override
	public void showDialog(final String pTitle, final String pMessage) {
		Message msg = new Message();
		msg.what = CrossAppHandler.HANDLER_SHOW_DIALOG;
		msg.obj = new CrossAppHandler.DialogMessage(pTitle, pMessage);
		this.mHandler.sendMessage(msg);
	}

	@Override
	public void showEditTextDialog(final String pTitle, final String pContent, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength) {
		Message msg = new Message();
		msg.what = CrossAppHandler.HANDLER_SHOW_EDITBOX_DIALOG;
		msg.obj = new CrossAppHandler.EditBoxMessage(pTitle, pContent, pInputMode, pInputFlag, pReturnType, pMaxLength);
		this.mHandler.sendMessage(msg);
	}

	@Override
	public void runOnGLThread(final Runnable pRunnable) {
		this.mGLSurfaceView.queueEvent(pRunnable);
	}

	// ===========================================================
	// Methods
	// ===========================================================
public void init()
	{
		FrameLayout framelayout = new FrameLayout(this);
		framelayout.layout(0, 0, 0, 0);
		
        this.frame = framelayout;
        this.mGLSurfaceView = this.onCreateView();
		
        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
        params.leftMargin = 0; 
    	params.rightMargin = 0;
    	params.topMargin = 0;
    	params.bottomMargin = 0;
    	
        framelayout.addView(this.mGLSurfaceView, params);

        if (isAndroidEmulator())
        {
           this.mGLSurfaceView.setEGLConfigChooser(8 , 8, 8, 8, 16, 0);
        }
        
        this.mCrossAppRenderer = new CrossAppRenderer();
        
        this.mGLSurfaceView.setCrossAppRenderer(mCrossAppRenderer);
        
        this.setContentView(framelayout);
        
	}
	
	public static int dip2px(Context context, float dpValue) {
	     final float scale = context.getResources().getDisplayMetrics().density;
	     return (int) (dpValue * scale + 0.5f);
	}

    public CrossAppGLSurfaceView onCreateView() {
    	return new CrossAppGLSurfaceView(this);
    }
	
    public int getStatusBarHeight() 
    {
    	  WindowManager.LayoutParams attrs = getWindow().getAttributes();
    	  if((attrs.flags & WindowManager.LayoutParams.FLAG_FULLSCREEN) == WindowManager.LayoutParams.FLAG_FULLSCREEN)
    	  {
    		  return 0;
    	  }
    	  int result = 0;
    	  int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
    	  if (resourceId > 0) {
    	      result = getResources().getDimensionPixelSize(resourceId);
    	  }
    	  return result;
    }
	
		public void onConfigurationChanged(Configuration newConfiguration)
	{
		super.onConfigurationChanged(newConfiguration);
		
		final DisplayMetrics displayMetrics = new DisplayMetrics();
		this.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
		this.screenWidth = displayMetrics.widthPixels;
		this.screenHeight = displayMetrics.heightPixels - this.getStatusBarHeight();
		
		mCrossAppRenderer.setScreenWidthAndHeight(screenWidth, screenHeight);

		if(newConfiguration.orientation == Configuration.ORIENTATION_PORTRAIT)
        {
        	orientation = 1;
        }
        else if(newConfiguration.orientation == Configuration.ORIENTATION_LANDSCAPE) 
        {
        	orientation = 2;
        }
        
        this.runOnGLThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	
            	CrossAppRenderer.nativeChanged(screenWidth, screenHeight);
            	CrossAppRenderer.nativeChangedOrientation(orientation);
            	
            	CrossAppTextField.updateImage();
         		CrossAppTextView.updateImage();
            	
            	s_pActivity.runOnUiThread(new Runnable() 
             	{
                     @Override
                     public void run()
                     {
                 		Bundle savedInstanceState = new Bundle();
                 		savedInstanceState.putStringArray("WEBVIEW", mWebViewHelper.getAllWebviews());;
                 		mWebViewHelper = new CrossAppWebViewHelper(frame);
                 		String[] strs = savedInstanceState.getStringArray("WEBVIEW");
                 		mWebViewHelper.setAllWebviews(strs);
                 		savedInstanceState.clear();
                     }
                 });
            }
        });  
	}
	
   private final static boolean isAndroidEmulator() {
	   
      String model = Build.MODEL;
	  
      String product = Build.PRODUCT;
	  
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
