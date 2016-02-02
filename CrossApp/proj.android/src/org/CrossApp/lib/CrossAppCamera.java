
package org.CrossApp.lib;

import org.CrossApp.lib.Cocos2dxActivity;

import android.content.res.Configuration;
import android.graphics.Color;
import android.hardware.Camera.PreviewCallback;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import android.widget.FrameLayout;
import android.widget.RelativeLayout.LayoutParams;


public class CrossAppCamera
{
	
	static private android.hardware.Camera		  mCamera;
	static private SurfaceHolder mSurfaceHolder;
	
    static public void openCamera()
    {
    	Cocos2dxActivity.getContext().runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	SurfaceView surfaceView = new SurfaceView(Cocos2dxActivity.getContext());
            	surfaceView.setBackgroundColor(Color.RED);
            	
        		FrameLayout layout = Cocos2dxActivity.getFrameLayout();
        		FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT) ; 
            	params.leftMargin = 100; 
            	params.topMargin = 100;
            	params.rightMargin = 100;
            	params.bottomMargin = 100;
            	layout.addView(surfaceView, params) ;
        		
        		
        		mSurfaceHolder = surfaceView.getHolder();
        		mSurfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        		
        		mSurfaceHolder.addCallback(new Callback() {
        			
        			public void surfaceDestroyed(SurfaceHolder arg0)
        			{
        				// TODO Auto-generated method stub
        				
        			}
        			
        			public void surfaceCreated(SurfaceHolder arg0)
        			{
        				// TODO Auto-generated method stub
        				try 
        				{
        			        mCamera = android.hardware.Camera.open(); // attempt to get a Camera instance
        			    }
        			    catch (Exception e){
        			        // Camera is not available (in use or does not exist)
        			    }
        			}
        			
        			public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3)
        			{
        				
        				
        				mCamera.setDisplayOrientation(90);
        				// TODO Auto-generated method stub
        				android.hardware.Camera.Parameters mParameters = mCamera.getParameters();
        				
        				
//        				List<Size> vSizes = mParameters.getSupportedPictureSizes();
//        				for (int i = 0; i < vSizes.size(); i++) {
//        					Size vSize = vSizes.get(i);
//        				}

        				if(Cocos2dxActivity.getContext().getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) 
        				{ 
        					mParameters.set("orientation", "landscape");
        				} 

        				else 

        				{ 
        					mParameters.set("orientation", "portrait"); 
        				} 

        				
        				mCamera.setParameters(mParameters);
        				
        				//显示
        				try {
        					mCamera.setPreviewDisplay(mSurfaceHolder);
        				} catch (Exception e) {
        					// TODO: handle exception
        					mCamera.release();
        					mCamera = null;
        				}
        				
        				mCamera.setPreviewCallback(new PreviewCallback()
        				{
        					
        					@Override
        					public void onPreviewFrame(byte[] arg0, android.hardware.Camera arg1)
        					{
        						// TODO Auto-generated method stub
        					}
        				});
        				mCamera.startPreview();

        			}
        		});
            }
        });
	}   
}
