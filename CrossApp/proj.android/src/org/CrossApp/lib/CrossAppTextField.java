
package org.CrossApp.lib;

import java.nio.ByteBuffer;
import java.util.HashMap;
import org.CrossApp.lib.DrawableShapUtil;

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
import android.text.InputType;
import android.text.TextWatcher;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.View.OnClickListener;
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
	private Button 	clearButton = null;
	private boolean isClearBtn = false;
	private int keyboardheight = 0;
	private int keyboardheightTemp = 0;
	
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
	
	//keyBoard return call back
	private static native void keyBoardReturnCallBack(int key);
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
		    	textField.setPadding(10, 0, 10, 0) ;
		    	textField.setTextSize(14);
		    	textField.setMaxLines(1) ;
		    	textField.setSingleLine(true); 
		    	textField.setGravity(Gravity.CENTER_VERTICAL);
		    	
		    	
		    	textField.addTextChangedListener(new TextWatcher() {
					
					@Override
					public void onTextChanged(CharSequence arg0, int arg1, int arg2, int arg3) {
						// TODO Auto-generated method stub
						String string = "onTextChanged:     "+arg0.toString() + ":"+arg1 +":"+ arg2 +":"+ arg3;
						Log.d("android", string);
					}
					
					@Override
					public void beforeTextChanged(CharSequence arg0, int arg1, int arg2,
							int arg3) {
						// TODO Auto-generated method stub
						String string = "beforeTextChanged:     "+arg0.toString() + ":"+arg1 +":"+ arg2 +":"+ arg3;
						Log.d("android", string);
					}
					
					@Override
					public void afterTextChanged(Editable arg0) {
						// TODO Auto-generated method stub
						Log.d("android", "afterTextChanged:     "+arg0.toString());
					}
				});
		    	


		    	textField.setOnEditorActionListener(new OnEditorActionListener() {
					
					@Override
					public boolean onEditorAction(TextView arg0, int arg1, KeyEvent arg2) {
						// TODO Auto-generated method stub
						keyBoardReturnCallBack(mykey);
						return true;
					}
				});
		    		    	
		    	
		    	
		    	
		    	
		    	//娣诲����板��灞�涓�
				FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT) ; 
		    	params.leftMargin = -1000; 
		    	params.topMargin = -1000;
		    	params.width = 800;
		    	params.height = 200;
		    	layout.addView(textField, params) ;

		    	//���������瑙���������惧舰
				textField.setBackgroundColor(0);
				textField.setFocusable(true);
				textField.setDrawingCacheEnabled(true);
		        
				
				getKeyBoardHeight();
            }
        });
    }
    
    //keyboard height
    

    private static native void keyBoardHeightReturn(int key,int height);
    private static native void keyBoardShow(int key);
    private static native void keyBoardHide(int key);
    public int getKeyBoardHeight()
    {
		layout.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {

        @Override
        public void onGlobalLayout() {
        	
            // TODO Auto-generated method stub
            Rect r = new Rect();
            layout.getWindowVisibleDisplayFrame(r);

            int screenHeight = layout.getRootView().getHeight();
            
            keyboardheightTemp = screenHeight- r.bottom;
            if (keyboardheightTemp!=keyboardheight) {
            	context.runOnUiThread(new Runnable() {
        			
        			@Override
        			public void run() {
        				// TODO Auto-generated method stub
//        				if (keyboardheightTemp<1) {
//							//hide
//        					Log.d("android", "hide board");
//						}
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

    //placeholder text
    public void setTextFieldPlacHolder(final String text) {
		context.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textField.setHint(text);
			}
		});
	}
    public void setTextFieldText(final String text) {
    		context.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textField.setText(text);
			}
		});
	}
    public void setFontSize(final int size) {
		context.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textField.setTextSize(size);
			}
		});
	}
  //placeholder color
    public void setTextFieldPlacHolderColor(final int color) {
		context.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textField.setHintTextColor(color);
			}
		});
	}
    //textfield color 
    public void setTextFieldTextColor(final int color) {
		context.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textField.setTextColor(color);
			}
		});
	}
    
    //keyboard type
    public void setKeyBoardType(final int type) {
		context.runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				switch (type) {
				case 0:
					//default
					textField.setInputType(InputType.TYPE_NULL);
					break;
				case 1:
					//NumbersAndPunctuation
					textField.setInputType(InputType.TYPE_NUMBER_VARIATION_NORMAL);
					break;
				case 2:
					//URL
					textField.setInputType(InputType.TYPE_TEXT_VARIATION_URI);
					break;
				case 3:
					//NumberPad
					textField.setInputType(InputType.TYPE_CLASS_NUMBER);
					break;
				case 4:
					//PhonePad
					textField.setInputType(InputType.TYPE_CLASS_PHONE);
					break;
				case 5:
					//NamePhonePad
					textField.setInputType(InputType.TYPE_TEXT_VARIATION_PERSON_NAME);
					break;
				case 6:
					//EmailAddress
					textField.setInputType(InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS);
					break;
				default:
					break;
				}
			}
		});
	}
    
    //text field return type
    public void setKeyBoardReturnType(final int type) {
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				String string = type + "";
				Log.d("android", string);
				
				switch (type) {
				case 0:
					textField.setImeOptions(EditorInfo.IME_ACTION_DONE);
					Log.d("android", "00000");
					break;
					
				case 1:
					textField.setImeOptions(EditorInfo.IME_ACTION_GO);
					Log.d("android", "11111");
					break;
				case 2:
					textField.setImeOptions(EditorInfo.IME_ACTION_NEXT);
					Log.d("android", "22222");
					break;
				case 3:
					textField.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
					Log.d("android", "33333");
					break;
				case 4:
					textField.setImeOptions(EditorInfo.IME_ACTION_SEND);
					Log.d("android", "44444");
					break;

				default:
					textField.setImeOptions(EditorInfo.IME_ACTION_DONE);
					Log.d("android", "55555");
					break;
				}
			}
		});
    }
    
    //margins right length
    public void setMarginsDis(final int left,final int right,final int top,final int bottom) {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	textField.setPadding(left, 0, right, 0);
            }
        });
	}
    
    //margins left image
    public void setMarginLeftImage(final String filePath) {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	
            }
        });
	}
  //margins right image
    public void setMarginRightImage(final String filePath) {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	
            }
        });
	}
  //clearButton
    public void setClearBtn(final int flag) {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	isClearBtn = (flag>0)?true:false;
            	if (isClearBtn&&(clearButton==null)) {
					clearButton = new Button(context);
	            	clearButton.setBackgroundColor(0);
	            	
	            	FrameLayout.LayoutParams btnParams = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
	            	btnParams.width = 0;
	            	btnParams.height = 0;
	            	btnParams.rightMargin = -1000;
	            	btnParams.topMargin = -1000;
	            	layout.addView(clearButton,btnParams) ;
	            	
	            	clearButton.setOnClickListener(new View.OnClickListener() {
	                     public void onClick(View v) {
	                     	textField.setText("");
	                   }
	                });

				}else{
					layout.removeView(clearButton);
					clearButton = null;
				}
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
            	
            	
            	
            	if (clearButton!=null) {
					FrameLayout.LayoutParams btnParams = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
					btnParams.width = textField.getPaddingRight();
					btnParams.height = params.height;
					btnParams.leftMargin = params.leftMargin + params.width - btnParams.width;
					btnParams.topMargin = params.topMargin;
	            	clearButton.setLayoutParams(btnParams);
				}
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
