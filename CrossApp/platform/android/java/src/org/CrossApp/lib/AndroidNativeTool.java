package org.CrossApp.lib;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;

import org.CrossApp.lib.Cocos2dxGLSurfaceView;

import android.R.integer;
import android.R.string;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.MediaStore.Images.Media;
import android.provider.Settings;
import android.provider.Settings.SettingNotFoundException;
import android.text.format.DateFormat;
import android.util.Log;
import android.widget.Toast;

@SuppressLint("SdCardPath")
public class AndroidNativeTool
{
	private static AlertDialog mDialog = null;
	private static Activity s_pContext;
	native static void NativeReturn( String arg1 , Object arg2 );
	
	public AndroidNativeTool( final Activity context )
	{
		s_pContext = context;
	}
	
	public static void ShowDlg( String[] args )
    {
		s_pContext.runOnUiThread(new Runnable() {
            @Override
            public void run()
            {
            	mDialog.show();
            }
        });
     }
	
	static String s;
	public static Uri photoUri;
	public static void CAImageCapture(int type)
	{
		Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		SimpleDateFormat timeStampFormat = new SimpleDateFormat(
				"yyyy_MM_dd_HH_mm_ss");
				String filename = timeStampFormat.format(new Date(0));
				ContentValues values = new ContentValues();
				values.put(Media.TITLE, filename);

				photoUri = s_pContext.getContentResolver().insert(
				MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);

				intent.putExtra(MediaStore.EXTRA_OUTPUT, photoUri);
			
				int selectedType = 0;
		        if (type ==0) {
					selectedType = 0;
				}
		        else {
					selectedType = 3;
				}
			
		s_pContext.startActivityForResult(intent,selectedType);
	}
	
	public static void CAVideoCapture()
	{
		Intent getImageByCamera2= new Intent("android.media.action.VIDEO_CAPTURE");     
		s_pContext.startActivityForResult(getImageByCamera2, 1);  
	}
	
	public static void CAVideoAlbum()
	{
		Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
    	intent.setType("video/*");
    	Intent wrapperIntent = Intent.createChooser(intent, null);
    	s_pContext.startActivityForResult(wrapperIntent, 2);
	}
	
	public static void CAImageAlbum(int type)
	{
		Intent getImage = new Intent(Intent.ACTION_GET_CONTENT);   
		
        getImage.setType("image/*");  
        
        Intent wrapperIntent2 = Intent.createChooser(getImage, null);
        int selectedType = 0;
        if (type ==0) {
			selectedType = 0;
		}
        else {
			selectedType = 3;
		}
        s_pContext.startActivityForResult(wrapperIntent2, selectedType);//3
	}
	
	public static int getScreenBrightness() 
	{
	    int value = 0;
	    ContentResolver cr = s_pContext.getContentResolver();
	    try 
	    {
	    	
	        value = Settings.System.getInt(cr, Settings.System.SCREEN_BRIGHTNESS);
	    } 
	    catch (SettingNotFoundException e)
	    {
	        
	    }
	    return value;
	}

	public static void setScreenBrightness( int value) 
	{
		 
	   Cocos2dxActivity mActivity = (Cocos2dxActivity)s_pContext;
	   mActivity.mLightHandler.sendEmptyMessage(value);
	}
	
	public static void browserOpenURL(final String url)
	{
		s_pContext.runOnUiThread(new Runnable()
		{
			public void run() 
			{
				try
				{
					if (s_pContext == null)
						return;
					Intent intent = new Intent();
					intent.setAction("android.intent.action.VIEW");
					Uri content_url = Uri.parse(url);
					intent.setData(content_url);
					s_pContext.startActivity(intent);
				} 
				catch (Exception e) 
				{
					Log.d("CrossApp", "browserOpenURL", e);
				}
			}
		});
	}
	
	private void cropImageUri(Uri uri, int outputX, int outputY, int requestCode)
	{
		 Intent intent = new Intent("com.android.camera.action.CROP");

		 intent.setDataAndType(uri, "image/*");
		 intent.putExtra("crop", "true");
		 intent.putExtra("aspectX", 1);
		 intent.putExtra("aspectY", 1);
		 intent.putExtra("outputX", outputX);
		 intent.putExtra("outputY", outputY);
		 intent.putExtra("scale", true);
		 intent.putExtra(MediaStore.EXTRA_OUTPUT, uri);
		 intent.putExtra("return-data", false);
		 intent.putExtra("outputFormat", Bitmap.CompressFormat.JPEG.toString());
		 intent.putExtra("noFaceDetection", true); // no face detection
		 SimpleDateFormat timeStampFormat = new SimpleDateFormat(
					"yyyy_MM_dd_HH_mm_ss");
		String filename = timeStampFormat.format(new Date(0));
		ContentValues values = new ContentValues();
		values.put(Media.TITLE, filename);

		photoUri = s_pContext.getContentResolver().insert(
		MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);

		intent.putExtra(MediaStore.EXTRA_OUTPUT, photoUri);
		 s_pContext.startActivityForResult(intent, 1);
	}
	
    public void onActivityResult(int requestCode, int resultCode, Intent intent)
    {  
        if (resultCode == -1)
        {  
            switch (requestCode) 
            {  
            case 2:
            case 3:  // Photo
                Uri originalUri = intent.getData();  

                String[] proj = {MediaStore.Images.Media.DATA};
                
                Cursor cursor = s_pContext.managedQuery(originalUri, proj, null, null, null); 

                int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);

                cursor.moveToFirst();

                String path = cursor.getString(column_index);

                NativeReturn( path , null );
                break;  
            case 1:
            	Uri originalUri1;
            	if (intent != null && intent.getData() != null) 
            	{
            		originalUri1= intent.getData();  
            	}
            	else
            	{
            		originalUri1= photoUri;
            	}
            	
            	//1414136613714.jpg
                String[] proj1 = {MediaStore.Images.Media.DATA};

                Cursor cursor1 = s_pContext.managedQuery(originalUri1, proj1, null, null, null); 

                int column_index1 = cursor1.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);

                cursor1.moveToFirst();

                String path1 = cursor1.getString(column_index1);

                NativeReturn( path1 , null );

            	break;
            case 0: 
            	 Uri takePhoto;
            	 
            	// Uri photoUri = s_pContext. getContentResolver().insert(
            		//	 MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);

            			// intent.putExtra(MediaStore.EXTRA_OUTPUT, photoUri);
            	if (intent != null && intent.getData() != null) 
            	{
            		takePhoto= intent.getData();  
            		cropImageUri(takePhoto, 640, 640, 1);
            	}else {
            		cropImageUri(photoUri, 640, 640, 1);
				}
            	
            	 
            	 
            	break;
 
            default:  
                break;  
            }  
        }
    }
}
