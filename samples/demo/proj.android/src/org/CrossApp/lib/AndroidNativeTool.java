package org.CrossApp.lib;

import java.io.File;
import java.io.FileNotFoundException;

import org.CrossApp.lib.Cocos2dxGLSurfaceView;

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
		final CharSequence[] items = { "����", "¼��","��Ƶ","���" }; 
		mDialog = new AlertDialog.Builder(context).setTitle("ѡ��ͼƬ").setItems(items,   
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
				.equals(android.os.Environment.MEDIA_MOUNTED); //�ж�sd���Ƿ���� 
	    if (sdCardExist) 
		{ 
	    	sdDir = Environment.getExternalStorageDirectory();//��ȡ��Ŀ¼ 
		} 
		System.out.println("getExternalStorageDirectory(): "+sdDir.toString()); 

		
		
		//����ȷ���ļ���·�����ڣ��������պ��޷���ɻص�
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
                //���ͼƬ��uri   
            	//runNativeCallback(requestCode,resultCode,intent);
                Uri originalUri = intent.getData();  
                
                
                String[] proj = {MediaStore.Images.Media.DATA};

                

                //������android��ý����ݿ�ķ�װ�ӿڣ�����Ŀ�Android�ĵ�

                Cursor cursor = s_pContext.managedQuery(originalUri, proj, null, null, null); 
                		//

                //���Ҹ������ ����ǻ���û�ѡ���ͼƬ������ֵ

                int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);

                //�����������ͷ ���������Ҫ����С�ĺ���������Խ��

                cursor.moveToFirst();

                //���������ֵ��ȡͼƬ·��

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
