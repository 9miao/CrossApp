
package org.CrossApp.lib;

import java.nio.ByteBuffer;

import org.CrossApp.lib.CrossAppActivity;

import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import android.widget.FrameLayout;
import android.widget.RelativeLayout.LayoutParams;


public class CrossAppCamera
{
	
	static private android.hardware.Camera		  mCamera;
	static private SurfaceHolder mSurfaceHolder;
	static private byte[] data;
	static private int width;
	static private int height;
	static private Bitmap bmp;
	static private ByteBuffer imageData;
	
	private static native void onByte(byte[] buf, int wdith, int height);
	private static native void onByte2(byte[] buf, int lenght);
    static public void openCamera()
    {
    	CrossAppActivity.getContext().runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	SurfaceView surfaceView = new SurfaceView(CrossAppActivity.getContext());
            	mSurfaceHolder = surfaceView.getHolder();
        		mSurfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

        		FrameLayout layout = CrossAppActivity.getFrameLayout();
        		FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT) ; 
            	params.leftMargin = 100;  
            	params.topMargin = 100;
            	params.rightMargin = 100;
            	params.bottomMargin = 300;
            	layout.addView(surfaceView, params) ;
        		
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
        			    catch (Exception e)
        			    {
        			        // Camera is not available (in use or does not exist)
        			    }
        			}
        			
        			public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3)
        			{
        				
        				
        				mCamera.setDisplayOrientation(90);
        				// TODO Auto-generated method stub
        				android.hardware.Camera.Parameters mParameters = mCamera.getParameters();
        				

        				if(CrossAppActivity.getContext().getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) 
        				{ 
        					mParameters.set("orientation", "landscape");
        				} 

        				else 

        				{ 
        					mParameters.set("orientation", "portrait"); 
        				} 

        				
        				mCamera.setParameters(mParameters);
        				
        				try 
        				{
        					mCamera.setPreviewDisplay(mSurfaceHolder);
        				} 
        				catch (Exception e) 
        				{
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
        						
        						Parameters parameters=arg1.getParameters(); 
//        		                int imageFormat = parameters.getPreviewFormat();  
        		                width = parameters.getPreviewSize().width;  
        		                height = parameters.getPreviewSize().height;
        						data = arg0;
        						Log.e("xxxxxxxxxxx    ", "w = " + width + "   h = " + height);
        						
  
        						int frameSize = width * height;  
        				        int[] rgba = new int[frameSize];  
        				  
        				            for (int i = 0; i < height; i++)  
        				                for (int j = 0; j < width; j++) {  
        				                    int y = (0xff & ((int) data[i * width + j]));  
        				                    int u = (0xff & ((int) data[frameSize + (i >> 1) * width + (j & ~1) + 0]));  
        				                    int v = (0xff & ((int) data[frameSize + (i >> 1) * width + (j & ~1) + 1]));  
        				                    y = y < 16 ? 16 : y;  
        				  
        				                    int r = Math.round(1.164f * (y - 16) + 1.596f * (v - 128));  
        				                    int g = Math.round(1.164f * (y - 16) - 0.813f * (v - 128) - 0.391f * (u - 128));  
        				                    int b = Math.round(1.164f * (y - 16) + 2.018f * (u - 128));  
        				  
        				                    r = r < 0 ? 0 : (r > 255 ? 255 : r);  
        				                    g = g < 0 ? 0 : (g > 255 ? 255 : g);  
        				                    b = b < 0 ? 0 : (b > 255 ? 255 : b);  
        				  
        				                    rgba[i * width + j] = 0xff000000 + (b << 16) + (g << 8) + r;  
        				                }  
        				  
        				        bmp = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);  
        				        bmp.setPixels(rgba, 0 , width, 0, 0, width, height);  
        				        
        				        boolean avaliable = bmp == null || (bmp !=null && bmp.isRecycled()) ? false : true ; 
        						Log.e("xxxxxxxxxxx    ", "" + avaliable);
        						imageData = ByteBuffer.allocate(bmp.getRowBytes() * bmp.getHeight());
        						bmp.copyPixelsToBuffer(imageData);
        						
        						
        						CrossAppActivity.getContext().runOnGLThread(new Runnable() 
        	                	{
        	                        @Override
        	                        public void run()
        	                        {
        	                        	onByte(imageData.array(), bmp.getWidth(), bmp.getHeight());
//        	                        	imageData = null;
//        	                        	bmp = null;
        	                        }
        	                    });
        					}
        				});
        				
        				mCamera.startPreview();
        			}
        		});
            }
        });
	}   
}
