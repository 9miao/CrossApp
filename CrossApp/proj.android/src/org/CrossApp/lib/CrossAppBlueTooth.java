package org.CrossApp.lib;

import java.util.Iterator;
import java.util.Set;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.util.Log;

public class CrossAppBlueTooth {
	private String maddress;
	
	private String mname;
	
	private static BluetoothAdapter mAdapter = null;
	
    native static void returnBlueToothState(int state);
    
    native static void returnDiscoveryDevice(CrossAppBlueTooth sender);
    
    native static void returnStartedDiscoveryDevice();
    
    native static void returnFinfishedDiscoveryDevice();
    
    private final static int REQUEST_OPEN_BT_CODE = 1;
    
    private final static int REQUEST_DISCOVERY_BT_CODE = 2;
    
	public CrossAppBlueTooth()
	{		
	}
	
	public CrossAppBlueTooth(String address,String name)
	{
		maddress = address;
		mname = name;
	}
	
	public String getAddress()
	{
		return maddress;
	}
	
	public String getName()
	{
		return mname;
	}
	

	public static void initBlueTooth()
	{
		mAdapter = BluetoothAdapter.getDefaultAdapter();

		IntentFilter bluetoothFilter = new IntentFilter();
        bluetoothFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
        bluetoothFilter.addAction(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED);
        CrossAppActivity.getContext().registerReceiver(BluetoothReciever, bluetoothFilter);

        IntentFilter btDiscoveryFilter = new IntentFilter();
        btDiscoveryFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        btDiscoveryFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        btDiscoveryFilter.addAction(BluetoothDevice.ACTION_FOUND);
        btDiscoveryFilter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
        CrossAppActivity.getContext().registerReceiver(BTDiscoveryReceiver, btDiscoveryFilter);

        Set<BluetoothDevice> bts = mAdapter.getBondedDevices();
        Iterator<BluetoothDevice> iterator  = bts.iterator();
        while(iterator.hasNext())
        {
            BluetoothDevice bd = iterator.next() ;
        }

        BluetoothDevice findDevice =  mAdapter.getRemoteDevice("00:11:22:33:AA:BB");

	}
	
    public static BroadcastReceiver BTDiscoveryReceiver = new BroadcastReceiver()
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
    
    public static void setBlueToothActionType(int type)
    {    	
    	boolean wasBtOpened = mAdapter.isEnabled();
    	switch (type) {
		case 0:
			if(mAdapter!=null)
			{
				Log.i("1", "madapter: not null");
			}
			
			boolean result = mAdapter.enable();
			Log.i("1", "madapter: not null");
			if(result)
			{
				returnBlueToothState(0);	//闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹风兘鏁撻弬銈嗗闁跨喐鏋婚幏閿嬫闁跨噦鎷�			
			}
			else if(wasBtOpened)
			{
				returnBlueToothState(1);	//闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔虹崵閹惧懏瀚归柨鐔告灮閹风兘鏁撻弬銈嗗
			}
			else
			{
				returnBlueToothState(2);	//闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹峰嘲銇戦柨鐔告灮閹凤拷			
			}
			break;

		case 1:
			boolean result1 = mAdapter.disable();
			if(result1)
				returnBlueToothState(3);//闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔稿焻闂傤叀顕滈幏鐑芥晸閺傘倖瀚归柨鐔哄嵆閻у憡瀚�			else if(!wasBtOpened)
				returnBlueToothState(4);//闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔虹崵閹惧懏瀚归柨鐔稿焻閹插瀚�			else
				returnBlueToothState(5);//闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔稿焻閹插瀚规径閬嶆晸閺傘倖瀚�
			break;
		case 2:
			if (!wasBtOpened)
			{
				Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
				CrossAppActivity.getContext().startActivityForResult(intent, REQUEST_OPEN_BT_CODE);
			}
			else
				returnBlueToothState(1);//闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔虹崵閹惧懏瀚归柨鐔告灮閹凤拷			break;
		case 3:
			if (!mAdapter.isDiscovering()){
                mAdapter.startDiscovery();
            }
			break;
		case 4:
			if (mAdapter.isDiscovering()){
                mAdapter.cancelDiscovery();
            }
			break;
		case 5:
            Intent discoveryintent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
            discoveryintent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
            CrossAppActivity.getContext().startActivityForResult(discoveryintent, REQUEST_DISCOVERY_BT_CODE);
			break;
		default:
			break;
		}
    }
    
	public static BroadcastReceiver BluetoothReciever = new BroadcastReceiver()
    {
        @Override
        public void onReceive(Context context, Intent intent)
        {
            // TODO Auto-generated method stub
            if (BluetoothAdapter.ACTION_STATE_CHANGED.equals(intent.getAction()))
            {
                int btState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,BluetoothAdapter.STATE_OFF);

                printBTState(btState);
            }
            else if(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED.equals(intent.getAction()))
            {
                int cur_mode_state = intent.getIntExtra(BluetoothAdapter.EXTRA_SCAN_MODE, BluetoothAdapter.SCAN_MODE_NONE);
                int previous_mode_state = intent.getIntExtra(BluetoothAdapter.EXTRA_PREVIOUS_SCAN_MODE, BluetoothAdapter.SCAN_MODE_NONE);
            }
        }

    };
    
    private static void printBTState(int btState)
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
	
}

