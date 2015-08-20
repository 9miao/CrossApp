
package org.CrossApp.lib;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.concurrent.Callable;

import org.CrossApp.lib.Cocos2dxHelper.Cocos2dxHelperListener;

import android.R.integer;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Rect;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiInfo;
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

import android.text.ClipboardManager;
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
	public static Cocos2dxRenderer mCocos2dxRenderer;
	private static Context sContext = null;
	AndroidNativeTool actAndroidNativeTool;
	AndroidVolumeControl androidVolumeControl;
	static FrameLayout frame;
	static View rootview;
	public static int keyboardheight;
	public static int currentBattery=0;
	private static Activity activity;
	private static Cocos2dxActivity cocos2dxActivity;
	native static void KeyBoardHeightReturn(int height);
	native static void getWifiList(ArrayList<CustomScanResult> s);
	public static List<ScanResult> list;
	public static ScanResult mScanResult;
	public static CustomScanResult cScanResult;
	private static BluetoothAdapter mAdapter = null;
    private final int REQUEST_OPEN_BT_CODE = 1;
    private final int REQUEST_DISCOVERY_BT_CODE = 2;
    native static void returnBlueToothState(int state);
    native static void returnDiscoveryDevice(AndroidBlueTooth sender);
    native static void returnStartedDiscoveryDevice();
    native static void returnFinfishedDiscoveryDevice();
    public static Handler msHandler;
	public static Cocos2dxActivity getContext() {
		return cocos2dxActivity;
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
		cocos2dxActivity = this;

    	this.mHandler = new Cocos2dxHandler(this);
    	actAndroidNativeTool = new AndroidNativeTool(this);
    	AndroidVolumeControl.setContext(sContext);
    	AndroidPersonList.Init(this);

    	this.init();
    	rootview = this.getWindow().getDecorView();
		Cocos2dxHelper.init(this, this);
		exeHandler();
		AndroidNetWorkManager.setContext(this);

		 if(mWebViewHelper == null)
		 {
			 mWebViewHelper = new Cocos2dxWebViewHelper(frame);
		 }
		 if (savedInstanceState != null && savedInstanceState.containsKey("WEBVIEW"))
		 {
			 mWebViewHelper = new Cocos2dxWebViewHelper(frame);
			 String[] strs = savedInstanceState.getStringArray("WEBVIEW");
			 mWebViewHelper.setAllWebviews(strs);
			 savedInstanceState.clear();
		 }
		 IntentFilter intentFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);

	     BatteryReceiver batteryReceiver = new BatteryReceiver();

	     registerReceiver(batteryReceiver, intentFilter);

	}

//	@Override
//	protected void onDestroy() 
//	{
//		super.onDestroy();
//		unregisterReceiver(BluetoothReciever) ; 
//		unregisterReceiver(BTDiscoveryReceiver) ; 
//	}
	@Override
	protected void onSaveInstanceState(Bundle outState) {
		
		outState.putStringArray("WEBVIEW", mWebViewHelper.getAllWebviews());
		super.onSaveInstanceState(outState);
	}


	
	class BatteryReceiver extends BroadcastReceiver{

        @Override
        public void onReceive(Context context, Intent intent) {

            if(Intent.ACTION_BATTERY_CHANGED.equals(intent.getAction())){

                int level = intent.getIntExtra("level", 0);

                int scale = intent.getIntExtra("scale", 100);

                currentBattery =level*100/ scale;
            }
        }
    }


	public void initBlueTooth()
	{
		mAdapter = BluetoothAdapter.getDefaultAdapter();

		IntentFilter bluetoothFilter = new IntentFilter();
        bluetoothFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
        bluetoothFilter.addAction(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED);
        registerReceiver(BluetoothReciever, bluetoothFilter);


        IntentFilter btDiscoveryFilter = new IntentFilter();
        btDiscoveryFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        btDiscoveryFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        btDiscoveryFilter.addAction(BluetoothDevice.ACTION_FOUND);
        btDiscoveryFilter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
        this.registerReceiver(BTDiscoveryReceiver, btDiscoveryFilter);

        Set<BluetoothDevice> bts = mAdapter.getBondedDevices();
        Iterator<BluetoothDevice> iterator  = bts.iterator();
        while(iterator.hasNext())
        {
            BluetoothDevice bd = iterator.next() ;
            Log.i(TAG , " Name : " + bd.getName() + " Address : "+ bd.getAddress() ); ;
            Log.i(TAG, "Device class" + bd.getBluetoothClass());
        }

        BluetoothDevice findDevice =  mAdapter.getRemoteDevice("00:11:22:33:AA:BB");

        Log.i(TAG , "findDevice Name : " + findDevice.getName() + "  findDevice Address : "+ findDevice.getAddress() ); ;
        Log.i(TAG , "findDevice class" + findDevice.getBluetoothClass());
	}

	public static void getWifiList()
	{
		AndroidNetWorkManager.setContext(activity);
		AndroidNetWorkManager.startScan();
		list = AndroidNetWorkManager.getWifiList();
		ArrayList<CustomScanResult> cList = new ArrayList<CustomScanResult>();
		if(list!=null){
            for(int i=0;i<list.size();i++){
                //�õ�ɨ����
                mScanResult=list.get(i);
                cScanResult = new CustomScanResult(mScanResult.SSID, mScanResult.BSSID, mScanResult.level);
                if (cScanResult!=null) {
                	cList.add(cScanResult);
				}

            }
            if (cList!=null)
            {
            	getWifiList(cList);
            }
		}
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
            	ClipboardManager clipboard =  (ClipboardManager)sContext.getSystemService(Context.CLIPBOARD_SERVICE);
                clipboard.getText();
                return clipboard.getText().toString();
            }
        };
        try {
            return Cocos2dxWebViewHelper.callInMainThread(callable);
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
                Log.v(TAG, "### Bluetooth State has changed ##");

                int btState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                        BluetoothAdapter.STATE_OFF);

                printBTState(btState);
            }
            else if(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED.equals(intent.getAction()))
            {
                Log.v(TAG, "### ACTION_SCAN_MODE_CHANGED##");
                int cur_mode_state = intent.getIntExtra(BluetoothAdapter.EXTRA_SCAN_MODE, BluetoothAdapter.SCAN_MODE_NONE);
                int previous_mode_state = intent.getIntExtra(BluetoothAdapter.EXTRA_PREVIOUS_SCAN_MODE, BluetoothAdapter.SCAN_MODE_NONE);

                Log.v(TAG, "### cur_mode_state ##" + cur_mode_state + " ~~ previous_mode_state" + previous_mode_state);

            }
        }

    };

    public void setBlueToothActionType(int type)
    {
    	boolean wasBtOpened = mAdapter.isEnabled();
    	switch (type) {
		case 0:
			boolean result = mAdapter.enable();
			if(result)
				returnBlueToothState(0);//����������ɹ�
			else if(wasBtOpened)
				returnBlueToothState(1);//�����Ѿ�����
			else
			{
				returnBlueToothState(2);//������ʧ��
			}
			break;

		case 1:
			boolean result1 = mAdapter.disable();
			if(result1)
				returnBlueToothState(3);//�����رղ����ɹ�
			else if(!wasBtOpened)
				returnBlueToothState(4);//�����Ѿ��ر�
			else
				returnBlueToothState(5);//�����ر�ʧ��.
			break;
		case 2:
			if (!wasBtOpened)
			{
				Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
				startActivityForResult(intent, REQUEST_OPEN_BT_CODE);
			}
			else
				returnBlueToothState(1);//�����Ѿ���
			break;
		case 3:
			if (!mAdapter.isDiscovering()){
                Log.i(TAG, "btCancelDiscovery ### the bluetooth dont't discovering");
                mAdapter.startDiscovery();
            }
            else
                toast("�������������豸��");
			break;
		case 4:
			if (mAdapter.isDiscovering()){
                Log.i(TAG, "btCancelDiscovery ### the bluetooth is isDiscovering");
                mAdapter.cancelDiscovery();
            }
			break;
		case 5:
            Intent discoveryintent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
            discoveryintent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
            startActivityForResult(discoveryintent, REQUEST_DISCOVERY_BT_CODE);
			break;
		default:
			break;
		}
    }
    //����ɨ��ʱ�Ĺ㲥������
    public BroadcastReceiver BTDiscoveryReceiver = new BroadcastReceiver()
    {
        @Override
        public void onReceive(Context context, Intent intent)
        {
            // TODO Auto-generated method stub
            if (BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(intent.getAction()))
            {
                Log.v(TAG, "### BT ACTION_DISCOVERY_STARTED ##");
                returnStartedDiscoveryDevice();
            }
            else if(BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(intent.getAction()))
            {
                Log.v(TAG, "### BT ACTION_DISCOVERY_FINISHED ##");
                returnFinfishedDiscoveryDevice();
            }
            else if(BluetoothDevice.ACTION_FOUND.equals(intent.getAction()))
            {
                Log.v(TAG, "### BT BluetoothDevice.ACTION_FOUND ##");
                BluetoothDevice btDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if(btDevice != null)
                {
                	Log.v(TAG , "Name : " + btDevice.getName() + " Address: " + btDevice.getAddress());
                	AndroidBlueTooth mAndroidBlueTooth = new AndroidBlueTooth(btDevice.getAddress(),btDevice.getName());
                	returnDiscoveryDevice(mAndroidBlueTooth);
                }
            }
            else if(BluetoothDevice.ACTION_BOND_STATE_CHANGED.equals(intent.getAction()))
            {
                Log.v(TAG, "### BT ACTION_BOND_STATE_CHANGED ##");

                int cur_bond_state = intent.getIntExtra(BluetoothDevice.EXTRA_BOND_STATE, BluetoothDevice.BOND_NONE);
                int previous_bond_state = intent.getIntExtra(BluetoothDevice.EXTRA_PREVIOUS_BOND_STATE, BluetoothDevice.BOND_NONE);
                Log.v(TAG, "### cur_bond_state ##" + cur_bond_state + " ~~ previous_bond_state" + previous_bond_state);
            }
        }

    };

    private void printBTState(int btState)
    {
        switch (btState)
        {
            case BluetoothAdapter.STATE_OFF:
                toast("����״̬:�ѹر�");
                Log.v(TAG, "BT State �� BluetoothAdapter.STATE_OFF ###");
                break;
            case BluetoothAdapter.STATE_TURNING_OFF:
                toast("����״̬:���ڹر�");
                Log.v(TAG, "BT State :  BluetoothAdapter.STATE_TURNING_OFF ###");
                break;
            case BluetoothAdapter.STATE_TURNING_ON:
                toast("����״̬:���ڴ�");
                Log.v(TAG, "BT State ��BluetoothAdapter.STATE_TURNING_ON ###");
                break;
            case BluetoothAdapter.STATE_ON:
                toast("����״̬:�Ѵ�");
                Log.v(TAG, "BT State ��BluetoothAdapter.STATE_ON ###");
                break;
            default:
                break;
        }
    }

    private void toast(String str)
    {
    	System.out.println(str);

        //Toast.makeText(Cocos2dxActivity.this, str, Toast.LENGTH_SHORT).show();
    }

	public static CustomScanResult getWifiConnectionInfo()
	{
		WifiInfo mWifiInfo = AndroidNetWorkManager.getWifiConnectionInfo();
		CustomScanResult connectionInfo = null;
		if(mWifiInfo!=null)
		{
			connectionInfo = new CustomScanResult(mWifiInfo.getSSID(), mWifiInfo.getBSSID(), 0);
		}
		return connectionInfo;
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
        mCocos2dxRenderer = new Cocos2dxRenderer();
        this.mGLSurfaceView.setCocos2dxRenderer(mCocos2dxRenderer);
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
                System.out.println(r.toString());

                int screenHeight = rootview.getRootView().getHeight();
                keyboardheight =screenHeight- r.bottom;
                System.out.println(keyboardheight);
                
                frame.getViewTreeObserver().removeGlobalOnLayoutListener(this);

                cocos2dxActivity.mGLSurfaceView.queueEvent(new Runnable()
                {
					@Override
					public void run() {
						if (keyboardheight!=0)
		        		{
		        			KeyBoardHeightReturn(keyboardheight);
		        			mCocos2dxRenderer.handleOpenKeyPad();
		        			System.out.println("handleOpenKeyPad");
		        		}
		                else
		                {
		                	mCocos2dxRenderer.handleCloseKeyPad();
		                	System.out.println("handleCloseKeyPad");
						}
					}
				});
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

   public static void setkeyboardHeight(int height){
       if(height == 0){
           if(keyboardheight > 0){
               KeyBoardHeightReturn(keyboardheight);
           }
       } else {
           KeyBoardHeightReturn(height);
       }
   }

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
