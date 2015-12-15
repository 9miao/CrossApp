
package org.CrossApp.lib;

import java.nio.ByteBuffer;
import java.util.HashMap;
import org.CrossApp.lib.DrawableShapUtil;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.RelativeLayout.LayoutParams;

public class CrossAppTextField
{
	private EditText textField = null; 
	private static FrameLayout layout = null;
	private static Cocos2dxActivity context = null;
	private static Handler handler = null;
	private static HashMap<Integer, CrossAppTextField> dict = null;
	private int mykey = -1;
	private ByteBuffer imageData = null;
	private Bitmap bmp = null;
	protected void finalize()
    {
		layout.removeView(textField);
     }
	
	public static void initWithHandler()
	{
		if (handler == null)
    	{
    		handler = new Handler(Looper.myLooper());
    	}
		
		if (dict == null)
    	{
    		dict = new HashMap<Integer, CrossAppTextField>();
    	}
		
		if (context == null)
    	{
    		context =  (Cocos2dxActivity)Cocos2dxActivity.getContext();
    	}
		
		if (layout == null)
    	{
    		ViewGroup.LayoutParams framelayout_params =
                    new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
                                               ViewGroup.LayoutParams.FILL_PARENT);
    		layout = (FrameLayout) context.getWindow().getDecorView() ;
    		layout.setLayoutParams(framelayout_params);
    	}
	}
	
    public void init(int key)
    {
    	
    	
    	mykey = key;
    	dict.put(key, this);

    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	//���濮����
		    	textField = new EditText(context) ; 
		    	textField.setPadding(5, 5, 5, 5) ;
		    	textField.setTextSize(14);
		    	textField.setMaxLines(1) ;
		    	
		    	//娣诲����板��灞�涓�
				FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT) ; 
		    	params.leftMargin = -1000; 
		    	params.topMargin = -1000;
		    	params.width = 800;
		    	params.height = 200;
		    	layout.addView(textField, params) ;

		    	//���������瑙���������惧舰
				Drawable backgound = DrawableShapUtil.getCorlorRectAngleContaionColor(context, Color.WHITE, 10, new Rect()) ;
				textField.setBackgroundDrawable(backgound) ;
				textField.setFocusable(true);
				textField.setDrawingCacheEnabled(true);
		        
//		      //娣诲��杈���ユ��瀛����������,涓�涓���规��浠ｈ〃涓���������跺��
//		        textField.addTextChangedListener(new TextWatcher() 
//		        		{
//		      			@Override
//		      			public void onTextChanged(CharSequence s, int start, int before, int count) {
//		      				// TODO Auto-generated method stub
//		      			}
//		      			@Override
//		      			public void beforeTextChanged(CharSequence s, int start, int count,
//		      					int after) {
//		      			}
//		      			
//		      			@Override
//		      			public void afterTextChanged(Editable s) {
//		      			}
//		      		}) ; 
            }
        });
    }
    
	public void setTextFieldPoint(final int x, final int y)
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)textField.getLayoutParams(); 
            	params.leftMargin = x; 
            	params.topMargin = y + 75;
            	textField.setLayoutParams(params);
            }
        });
    }
    
    public void setTextFieldSize(final int width, final int height)
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)textField.getLayoutParams(); 
            	params.width = width;
            	params.height = height;
            	textField.setLayoutParams(params);
            }
        });
    }
    
    private static native void onByte(int key, byte[] buf, int wdith, int height);
    
    public void getImage()
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	bmp = textField.getDrawingCache();
            	if (bmp != null)
            	{
            		imageData = ByteBuffer.allocate(bmp.getRowBytes() * bmp.getHeight());
            		bmp.copyPixelsToBuffer(imageData);
            		
            		context.runOnGLThread(new Runnable() 
                	{
                        @Override
                        public void run()
                        {
                        	onByte(mykey, imageData.array(), bmp.getWidth(), bmp.getHeight());
                        }
                    });
            		
            		
            	}
            }
        });
    }
    
    public void becomeFirstResponder()
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE); 
        		imm.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS);
        		textField.requestFocus();
            }
        });
    }
    
    public void resignFirstResponder()
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE); 
            	imm.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS);
        		textField.clearFocus();
            }
        });
    }
    
	static public CrossAppTextField getTextField(final int key)
	{
		CrossAppTextField var = dict.get(key);
		if (var != null) return var;
		return null;
		
	}
	
	static public void createTextField(final int key)
	{
		CrossAppTextField text = new CrossAppTextField();
		text.init(key);
	}
	
	static public void removeTextField(final int key) 
	{
		dict.remove(key);
	}
}
