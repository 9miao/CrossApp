
package org.CrossApp.lib;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Timer;
import java.util.TimerTask;

import android.R.bool;
import android.R.integer;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnKeyListener;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.os.Looper;
import android.text.Editable;
import android.text.InputFilter;
import android.text.InputType;
import android.text.TextWatcher;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.View.OnClickListener;
import android.view.ViewDebug.IntToString;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.webkit.WebView.FindListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.RelativeLayout.LayoutParams;
import android.widget.TextView.OnEditorActionListener;

public class CrossAppTextView
{
	private EditText textView = null; 
	private static FrameLayout layout = null;
	private static Cocos2dxActivity context = null;
	private static Handler handler = null;
	private static HashMap<Integer, CrossAppTextView> dict = null;
	private int mykey = -1;
	private ByteBuffer imageData = null;
	private Bitmap bmp = null;
	private int keyboardheight = 0;
	private int keyboardheightTemp = 0;
	private int leftMargin = 0;
	private int rightMargin = 0;
	private int inputType = 1;
	
	//代理回调需要
	private boolean isSetText = false;
	private String  beforeTextString = "";
	private String  changedTextString = "";
	private String  currTextString = "";
	private int _arg1 = 0;
	private int _arg2 = 0;
	private int _arg3 = 0;
	
	//是否弹出键盘
	private boolean isShowKey = false;
	
	
 	protected void finalize()
    {
		layout.removeView(textView);
     }
	
	public static void initWithHandler()
	{
		if (handler == null)
    	{
    		handler = new Handler(Looper.myLooper());
    	}
		
		if (dict == null)
    	{
    		dict = new HashMap<Integer, CrossAppTextView>();
    	}
		
		if (context == null)
    	{
    		context =  (Cocos2dxActivity)Cocos2dxActivity.getContext();
    	}
		
		if (layout == null)
    	{
    		layout = Cocos2dxActivity.getFrameLayout();
    	}
	}
	
	//keyBoard return call back
	private static native void keyBoardReturnCallBack(int key);
	private static native void textChange(int key,String before,String change,int arg0,int arg1,int arg2);
	private static native void text(int key, String text);
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
		    	textView = new EditText(context) ; 
		    	textView.setPadding(10, 0, 10, 0) ;
		    	textView.setGravity(Gravity.LEFT | Gravity.TOP);
		    	
		    	textView.addTextChangedListener(new TextWatcher() {
					
					@Override
					public void onTextChanged(CharSequence arg0, int arg1, int arg2, int arg3)
					{//起始位置， 删除长度，增加长度
						// TODO Auto-generated method stub
						if (isSetText)
						{
							isSetText = false;
							return;
						}
						
						String string = arg0.toString();
						if (arg2>arg3) 
						{
							changedTextString = beforeTextString.substring(arg1, arg1+arg2);
						}
						else
						{
							changedTextString = string.substring(arg1, arg1+arg3);
						}
						
						_arg1 = arg1;
						_arg2 = arg2;
						_arg3 = arg3;
						
						context.runOnUiThread(new Runnable()
						{
							
							@Override
							public void run()
							{
								text(mykey, textView.getText().toString());
								textChange(mykey, beforeTextString, changedTextString, _arg1, _arg2, _arg3);
							}
							
						});
						
						
						
					}
					
					@Override
					public void beforeTextChanged(CharSequence arg0, int arg1, int arg2,
							int arg3)
					{
						// TODO Auto-generated method stub
						beforeTextString = arg0.toString();
					}
					
					@Override
					public void afterTextChanged(Editable arg0)
					{
						// TODO Auto-generated method stub
					}
				});
		    	


		    	textView.setOnEditorActionListener(new OnEditorActionListener() {
					
					@Override
					public boolean onEditorAction(TextView arg0, int arg1, KeyEvent arg2) {
						// TODO Auto-generated method stub
	            		context.runOnGLThread(new Runnable() 
	                	{
	                        @Override
	                        public void run()
	                        {
	                        	keyBoardReturnCallBack(mykey);
	                        }
	                    });
						
						return true;
					}
				});
		    		    	
		    	//娣诲����板��灞�涓�
				FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT) ; 
		    	params.leftMargin = -1000; 
		    	params.topMargin = -1000;
		    	params.width = 800;
		    	params.height = 200;
		    	layout.addView(textView, params) ;

		    	//���������瑙���������惧舰
				textView.setBackgroundColor(0);
				textView.setFocusable(true);
				textView.setDrawingCacheEnabled(true);
		        
				
				getKeyBoardHeight();
            }
        });
    }
    
    //keyboard height
    

    private static native void keyBoardHeightReturn(int key,int height);
    private static native void resignFirstResponder(int key);
    public int getKeyBoardHeight()
    {
		layout.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {

        @Override
        public void onGlobalLayout() 
        {
        	
            // TODO Auto-generated method stub
            Rect r = new Rect();
            layout.getWindowVisibleDisplayFrame(r);

            int screenHeight = layout.getRootView().getHeight();
            
            keyboardheightTemp = screenHeight- r.bottom;
            if (keyboardheightTemp!=keyboardheight) {
            	context.runOnUiThread(new Runnable() 
            	{
        			
        			@Override
        			public void run() 
        			{
        				// TODO Auto-generated method stub
        				if (keyboardheightTemp < 1 && isShowKey == true)
        				{
							//hide
        					isShowKey = false;
        					context.runOnGLThread(new Runnable() 
                        	{
                                @Override
                                public void run()
                                {
                                	resignFirstResponder(mykey);
                                }
                            });
        					
						}
//        				if (keyboardheight<1) {
//							//show
//        					Log.d("android", "show board");
//						}
//        				Log.d("android", "call c++");
        				
        				//keyBoardReturn
        				keyBoardHeightReturn(mykey, keyboardheightTemp);
        			}
        		});
			}
            keyboardheight = keyboardheightTemp;
        }
    });

	return keyboardheight;

}

    public void setTextViewText(final String text) 
    {
    		context.runOnUiThread(new Runnable()
    		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				isSetText = true;
				textView.setText(text);
			}
		});
	}
    
    public void setFontSize(final int size) 
    {
		context.runOnUiThread(new Runnable()
		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textView.setTextSize(size);
			}
		});
	}

    //textView color 
    public void setTextViewextColor(final int color)
    {
		context.runOnUiThread(new Runnable()
		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textView.setTextColor(color);
			}
		});
	}
    

    //textView Algin
    public void setTextViewAlgin(final int var)
    {
    	context.runOnUiThread(new Runnable() 
    	{
			@Override
			public void run() {
				// TODO Auto-generated method stub
				switch (var) {
				case 0:
					//left
					textView.setGravity(Gravity.LEFT|Gravity.CENTER_VERTICAL);
					
					break;
				case 1:
					//center
					textView.setGravity(Gravity.CENTER_VERTICAL);
					break;
				case 2:
					//right
					textView.setGravity(Gravity.RIGHT|Gravity.CENTER_VERTICAL);
					break;
				default:
					break;
				}
			}
		});
	}
    

	public void setTextViewPoint(final int x, final int y)
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)textView.getLayoutParams(); 
            	params.leftMargin = x; 
            	params.topMargin = y;
            	textView.setLayoutParams(params);
            }
        });
    }
    
    public void setTextViewSize(final int width, final int height)
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)textView.getLayoutParams(); 
            	params.width = width;
            	params.height = height;
            	textView.setLayoutParams(params);
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
            	bmp = textView.getDrawingCache();
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
    
    private static native void hideImageView(int key);
    
    public void becomeFirstResponder()
    {
    	isShowKey = true;
    	Cocos2dxActivity.setSingleTextView(this);
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	//show
              	InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE); 
        		imm.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS);
        		textView.requestFocus();
        		Editable etext = textView.getText();
            	textView.setSelection(etext.length());
        		
        		context.runOnGLThread(new Runnable() 
            	{
                    @Override
                    public void run()
                    {
                    	hideImageView(mykey);
                    }
                });
            }
        });
    }
    
    public void resignFirstResponder()
    {
    	isShowKey = false;
    	Cocos2dxActivity.setSingleTextView(null);
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {           	
            	textView.setSelection(0);
            	InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE);  
            	imm.hideSoftInputFromWindow(textView.getWindowToken(), 0);
        		textView.clearFocus();
        		
        		FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)textView.getLayoutParams(); 
            	params.leftMargin = -1000; 
            	params.topMargin = -1000;
            	textView.setLayoutParams(params);
        		
            	TimerTask task = new TimerTask()
            	{    
            		public void run()
            		{    
            			bmp = textView.getDrawingCache();
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
            	};  
            		
            	Timer timer = new Timer();  
            	timer.schedule(task, (long) 50);
				
            }
        });
    }
    

	static public CrossAppTextView getTextView(final int key)
	{
		CrossAppTextView var = dict.get(key);
		if (var != null) return var;
		return null;
		
	}
	
	static public void createTextView(final int key)
	{
		CrossAppTextView text = new CrossAppTextView();
		text.init(key);
	}
	
	static public void removeTextView(final int key) 
	{
		CrossAppTextView var = dict.get(key);
		if (var != null)
		{
			
		}
		dict.remove(key);
	}
	
	public void resume() 
	{
		TimerTask task = new TimerTask()
		{    
			public void run()
			{    
				resignFirstResponder(mykey);
			}    
		};  
		
		Timer timer = new Timer();  
		timer.schedule(task, (long) 100);
	}
}
