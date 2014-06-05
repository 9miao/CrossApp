package org.cocos2dx.lib;

import java.io.File;
import java.io.FileNotFoundException;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.R.string;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.ContentResolver;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;

@SuppressLint("SdCardPath")
public class AndroidNativeTool
{
	private static AlertDialog mDialog = null;
	private static Activity s_pContext;
	native static void NativeReturn( String arg1 , Object arg2 );
	
	public AndroidNativeTool( final Activity context )
	{
		s_pContext = context;
		final CharSequence[] items = { "拍照", "录像","视频","相册" }; 
		mDialog = new AlertDialog.Builder(context).setTitle("选择图片").setItems(items,   
    	new DialogInterface.OnClickListener() {   
        public void onClick(DialogInterface dialog,int item)
        {   
	        switch( item )
	        {
	        case 0:
	        	CAImageCapture();
	            break;
	        case 1:
	        	CAVideoCapture();
	            break;
	        case 2:
	        	CAVideoAlbum();
	        	break;
	        case 3:
	        	CAImageAlbum();
	            break;
	        }
        } } ).create();
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
	public static void CAImageCapture()
	{
		File sdDir = null; 
		boolean sdCardExist = Environment.getExternalStorageState() 
				.equals(android.os.Environment.MEDIA_MOUNTED); //判断sd卡是否存在 
	    if (sdCardExist) 
		{ 
	    	sdDir = Environment.getExternalStorageDirectory();//获取跟目录 
		} 
		System.out.println("getExternalStorageDirectory(): "+sdDir.toString()); 

		
		
		//必须确保文件夹路径存在，否则拍照后无法完成回调
		String mediapathString =sdDir.toString();
		
		System.out.println(mediapathString);
		s=mediapathString+"/img.jpg";
		File vFile = new File(s);
		
		if(!vFile.exists())
		{
			File vDirPath = vFile.getParentFile(); //new File(vFile.getParent());
			vDirPath.mkdirs();
		}
		Uri uri = Uri.fromFile(vFile);
		Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		intent.putExtra(MediaStore.EXTRA_OUTPUT, uri);//
		s_pContext.startActivityForResult(intent,0);
//		Intent getImageByCamera= new Intent("android.media.action.IMAGE_CAPTURE");     
//		s_pContext.startActivityForResult(getImageByCamera,0);
	}
	public static void CAVideoCapture()
	{
		Intent getImageByCamera2= new Intent("android.media.action.VIDEO_CAPTURE");     
		s_pContext.startActivityForResult(getImageByCamera2, 1);  
	}
	public static void CAVideoAlbum()
	{
		Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
    	intent.setType("video/*"); //String VIDEO_UNSPECIFIED = "video/*";
    	Intent wrapperIntent = Intent.createChooser(intent, null);
    	s_pContext.startActivityForResult(wrapperIntent, 2);
	}
	public static void CAImageAlbum()
	{
		Intent getImage = new Intent(Intent.ACTION_GET_CONTENT);   
		
        //getImage.addCategory(Intent.CATEGORY_OPENABLE);   
        getImage.setType("image/*");  
        
        Intent wrapperIntent2 = Intent.createChooser(getImage, null);
        s_pContext.startActivityForResult(wrapperIntent2, 3);
	}
    public void onActivityResult(int requestCode, int resultCode, Intent intent)
    {  
    	System.out.println("~~~||||");
    	ContentResolver resolver = s_pContext.getContentResolver();   
    	System.out.println(resultCode);
        if (resultCode == -1) {  
            switch (requestCode) {  
            case 2:
            case 3:  // Photo
                //获得图片的uri   
            	//runNativeCallback(requestCode,resultCode,intent);
                Uri originalUri = intent.getData();  
                
                
                String[] proj = {MediaStore.Images.Media.DATA};

                

                //好像是android多媒体数据库的封装接口，具体的看Android文档

                Cursor cursor = s_pContext.managedQuery(originalUri, proj, null, null, null); 
                		//

                //按我个人理解 这个是获得用户选择的图片的索引值

                int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);

                //将光标移至开头 ，这个很重要，不小心很容易引起越界

                cursor.moveToFirst();

                //最后根据索引值获取图片路径

                String path = cursor.getString(column_index);
                /*Bitmap bitmap = null;  
                try {  
                    Bitmap originalBitmap = BitmapFactory.decodeStream(resolver.openInputStream(originalUri));  
                } catch (FileNotFoundException e) {  
                    e.printStackTrace();  
                }*/
                System.out.println(path);
                NativeReturn( path , null );
                break;  
            case 1:
            case 0:  // camera image
                /*Bundle extras = intent.getExtras();   
                Bitmap originalBitmap1 = (Bitmap) extras.get("data");  */
     
                NativeReturn( s , null );
                break;  
            default:  
                break;  
            }  
        }
    }
}
