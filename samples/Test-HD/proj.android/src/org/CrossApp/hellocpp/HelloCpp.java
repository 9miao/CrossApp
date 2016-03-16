
package org.CrossApp.hellocpp;

import org.CrossApp.lib.CrossAppActivity;
import org.CrossApp.lib.CrossAppGLSurfaceView;

import android.os.Bundle;

public class HelloCpp extends CrossAppActivity{
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
	}

    public CrossAppGLSurfaceView onCreateView() {
    	CrossAppGLSurfaceView glSurfaceView = new CrossAppGLSurfaceView(this);
    	// HelloCpp should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("CrossApp_cpp");
    }     
}
