
package org.CrossApp.Demo;

import org.CrossApp.lib.Cocos2dxActivity;
import org.CrossApp.lib.Cocos2dxGLSurfaceView;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;

public class Demo extends Cocos2dxActivity{
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// HelloCpp should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }
    
    public void browseWebpage(String url)
    {
    	Uri uri=Uri.parse(url);
    	Intent intent=new Intent(Intent.ACTION_VIEW,uri);
    	this.startActivity(intent);
    }

    static {
    	try {
    		System.loadLibrary("CrossApp_cpp");    		
    	} catch(Exception e) {
    		e.printStackTrace(System.out);
    	}
        
    }     
}
