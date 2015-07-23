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
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.provider.MediaStore.Images.ImageColumns;
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
		Intent getImage = new Intent(Intent.ACTION_OPEN_DOCUMENT); 
		getImage.addCategory(Intent.CATEGORY_OPENABLE);
        getImage.setType("image/*");  
        
        Intent wrapperIntent2 = Intent.createChooser(getImage, null);
        int selectedType = 0;
        if (type ==0) {
			selectedType = 0;
		}
        else {
			selectedType = 4;
		}
        s_pContext.startActivityForResult(getImage, selectedType);//3
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
		 Log.i("qiaoxin", "cropImageUri: !!!");
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
		Log.i("qiaoxin","filename:" + filename);
		photoUri = s_pContext.getContentResolver().insert(
		MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
		Log.i("qiaoxin","photoUri:" + photoUri);

		intent.putExtra(MediaStore.EXTRA_OUTPUT, photoUri);
		Log.i("qiaoxin","intent:" + intent);

		 s_pContext.startActivityForResult(intent, 1);
	}
	
    public void onActivityResult(int requestCode, int resultCode, Intent intent)
    {  
    	Log.i("qiaoxin", "onActivityResult:requestCode:"+requestCode+ " resultCode: " + resultCode + " Intent:  "+ intent);
        if (true)
        {  
            switch (requestCode) 
            {  
            case 4:  // Photo
            	Log.i("qiaoxin","requestCode:"+ requestCode);

            	final String uriStr = getPath(s_pContext, intent.getData());
            	Log.i("qiaoxin", "String uriStr:"+uriStr);
            	
            	String fileStr = getRealFilePath(s_pContext, intent.getData());
            	
            	Log.i("qiaoxin", "String fileStr:"+fileStr);
            	
                Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable() {
					@Override
					public void run() {
		                NativeReturn( uriStr , null );
					}
				});
                break;  
            case 1:
            	Log.i("qiaoxin","requestCode:"+ requestCode);

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

                final String path1 = cursor1.getString(column_index1);
                Log.i("qiaoxin","qiaoxin2"+ path1);

                Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable() {
					@Override
					public void run() {
		                NativeReturn( path1 , null );
					}
				});

            	break;
            case 2:
            case 3:
            	Log.i("qiaoxin","requestCode3:"+ requestCode);

            	Uri originalUri2;
            	if (intent != null && intent.getData() != null) 
            	{
            		originalUri2= intent.getData();  
            	}
            	else
            	{
            		originalUri2= photoUri;
            	}
                Log.i("qiaoxin","photoUri::"+ originalUri2);

                String[] proj = {MediaStore.Images.Media.DATA};
                
                Cursor cursor = s_pContext.getContentResolver().query(originalUri2,proj, null, null, null);

                int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);

                cursor.moveToFirst();

                final String path = cursor.getString(column_index);
                Log.i("qiaoxin","qiaoxin--path:"+ path);

                Cocos2dxGLSurfaceView.getInstance().queueEvent(new Runnable() {
					@Override
					public void run() {
						NativeReturn( path , null );
					}
				});
                
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
            		Log.i("qiaoxin","qiaoxin3");
            	}else {
            		cropImageUri(photoUri, 640, 640, 1);   
            		Log.i("qiaoxin","qiaoxin4");
				}

            	 
            	 
            	break;
 
            default:  
                break;  
            }  
        }
       
        
    }
    
    public static String getPath(final Context context, final Uri uri) {  
        Log.i("qiaoxin", "uri---:" + uri);
        final boolean isKitKat = Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT;  
      
        // DocumentProvider  
        if (isKitKat && DocumentsContract.isDocumentUri(context, uri)) {  
            // ExternalStorageProvider  
            if (isExternalStorageDocument(uri)) {  
                final String docId = DocumentsContract.getDocumentId(uri);  
                final String[] split = docId.split(":");  
                final String type = split[0];  
      
                if ("primary".equalsIgnoreCase(type)) {  
                    return Environment.getExternalStorageDirectory() + "/" + split[1];  
                }  
      
                // TODO handle non-primary volumes  
            }  
            // DownloadsProvider  
            else if (isDownloadsDocument(uri)) {  
      
                final String id = DocumentsContract.getDocumentId(uri);  
                final Uri contentUri = ContentUris.withAppendedId(  
                        Uri.parse("content://downloads/public_downloads"), Long.valueOf(id));  
      
                return getDataColumn(context, contentUri, null, null);  
            }  
            // MediaProvider  
            else if (isMediaDocument(uri)) {  
                final String docId = DocumentsContract.getDocumentId(uri);  
                final String[] split = docId.split(":");  
                final String type = split[0];  
      
                Uri contentUri = null;  
                if ("image".equals(type)) {  
                    contentUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;  
                } else if ("video".equals(type)) {  
                    contentUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;  
                } else if ("audio".equals(type)) {  
                    contentUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;  
                }  
      
                final String selection = "_id=?";  
                final String[] selectionArgs = new String[] {  
                        split[1]  
                };  
      
                return getDataColumn(context, contentUri, selection, selectionArgs);  
            }  
        }  
        // MediaStore (and general)  
        else if ("content".equalsIgnoreCase(uri.getScheme())) {  
      
            // Return the remote address  
            if (isGooglePhotosUri(uri))  
                return uri.getLastPathSegment();  
      
            return getDataColumn(context, uri, null, null);  
        }  
        // File  
        else if ("file".equalsIgnoreCase(uri.getScheme())) {  
            return uri.getPath();  
        }else{
            String[] projection = { MediaStore.Images.Media.DATA };
            Cursor cursor = context.getContentResolver().query(uri, projection, null, null, null);
            int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
            cursor.moveToFirst();
            return cursor.getString(column_index);
        }
      
        return null;  
    }  
      
    public static String getRealFilePath( final Context context, final Uri uri ) {
        if ( null == uri ) return null;
        final String scheme = uri.getScheme();
        String data = null;
        if ( scheme == null )
            data = uri.getPath();
        else if ( ContentResolver.SCHEME_FILE.equals( scheme ) ) {
            data = uri.getPath();
        } else if ( ContentResolver.SCHEME_CONTENT.equals( scheme ) ) {
            Cursor cursor = context.getContentResolver().query( uri, new String[] { ImageColumns.DATA }, null, null, null );
            if ( null != cursor ) {
                if ( cursor.moveToFirst() ) {
                    int index = cursor.getColumnIndex( ImageColumns.DATA );
                    if ( index > -1 ) {
                        data = cursor.getString( index );
                    }
                }
                cursor.close();
            }
        }
        return data;
    }
    
    /** 
     * Get the value of the data column for this Uri. This is useful for 
     * MediaStore Uris, and other file-based ContentProviders. 
     * 
     * @param context The context. 
     * @param uri The Uri to query. 
     * @param selection (Optional) Filter used in the query. 
     * @param selectionArgs (Optional) Selection arguments used in the query. 
     * @return The value of the _data column, which is typically a file path. 
     */  
    public static String getDataColumn(Context context, Uri uri, String selection,  
            String[] selectionArgs) {  
      
        Cursor cursor = null;  
        final String column = "_data";  
        final String[] projection = {  
                column  
        };  
      
        try {  
            cursor = context.getContentResolver().query(uri, projection, selection, selectionArgs,  
                    null);  
            if (cursor != null && cursor.moveToFirst()) {  
                final int index = cursor.getColumnIndexOrThrow(column);  
                return cursor.getString(index);  
            }  
        } finally {  
            if (cursor != null)  
                cursor.close();  
        }  
        return null;  
    }  
      
      
    /** 
     * @param uri The Uri to check. 
     * @return Whether the Uri authority is ExternalStorageProvider. 
     */  
    public static boolean isExternalStorageDocument(Uri uri) {  
        return "com.android.externalstorage.documents".equals(uri.getAuthority());  
    }  
      
    /** 
     * @param uri The Uri to check. 
     * @return Whether the Uri authority is DownloadsProvider. 
     */  
    public static boolean isDownloadsDocument(Uri uri) {  
        return "com.android.providers.downloads.documents".equals(uri.getAuthority());  
    }  
      
    /** 
     * @param uri The Uri to check. 
     * @return Whether the Uri authority is MediaProvider. 
     */  
    public static boolean isMediaDocument(Uri uri) {  
        return "com.android.providers.media.documents".equals(uri.getAuthority());  
    }  
      
    /** 
     * @param uri The Uri to check. 
     * @return Whether the Uri authority is Google Photos. 
     */  
    public static boolean isGooglePhotosUri(Uri uri) {  
        return "com.google.android.apps.photos.content".equals(uri.getAuthority());  
    }  
}
