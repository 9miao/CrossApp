
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
	private int keyboardheight = 0;
	private int keyboardheightTemp = 0;
	private int leftMargin = 0;
	private int rightMargin = 0;
	private int inputType = 1;
	
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
		    	textField = new EditText(context) ; 
		    	textField.setPadding(10, 0, 10, 0) ;
		    	textField.setTextSize(14);
		    	textField.setMaxLines(1) ;
		    	textField.setSingleLine(true); 
		    	textField.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
		    	
		    	textField.addTextChangedListener(new TextWatcher() {
					
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
								text(mykey, textField.getText().toString());
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

    //placeholder text
    public void setTextFieldPlacHolder(final String text) 
    {
		context.runOnUiThread(new Runnable()
		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textField.setHint(text);
			}
		});
	}
    public void setTextFieldText(final String text) 
    {
    		context.runOnUiThread(new Runnable()
    		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				isSetText = true;
				textField.setText(text);
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
				textField.setTextSize(size);
			}
		});
	}
  //placeholder color
    public void setTextFieldPlacHolderColor(final int color)
    {
		context.runOnUiThread(new Runnable()
		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textField.setHintTextColor(color);
			}
		});
	}
    //textfield color 
    public void setTextFieldTextColor(final int color)
    {
		context.runOnUiThread(new Runnable()
		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textField.setTextColor(color);
			}
		});
	}
    
    //keyboard type
    public void setKeyBoardType(final int type)
    {
		context.runOnUiThread(new Runnable()
		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				switch (type) {
				case 0:
					//default
					inputType = InputType.TYPE_CLASS_TEXT;
					break;
				case 1:
					//NumbersAndPunctuation
					inputType = InputType.TYPE_NUMBER_VARIATION_NORMAL;
					break;
				case 2:
					//URL
					inputType = InputType.TYPE_TEXT_VARIATION_URI;
					break;
				case 3:
					//NumberPad
					inputType = InputType.TYPE_CLASS_NUMBER;
					break;
				case 4:
					//PhonePad
					inputType = InputType.TYPE_CLASS_PHONE;
					break;
				case 5:
					//NamePhonePad
					inputType = InputType.TYPE_TEXT_VARIATION_PERSON_NAME;
					break;
				case 6:
					//EmailAddress
					inputType = InputType.TYPE_TEXT_VARIATION_EMAIL_ADDRESS;
					break;
				default:
					break;
				}
				textField.setInputType(inputType);
			}
		});
	}
    
    //textField Algin
    public void setTextFieldAlgin(final int var)
    {
    	context.runOnUiThread(new Runnable() 
    	{
			@Override
			public void run() {
				// TODO Auto-generated method stub
				switch (var) {
				case 0:
					//center
					textField.setGravity(Gravity.CENTER_VERTICAL);
					break;
				case 1:
					//left
					textField.setGravity(Gravity.LEFT|Gravity.CENTER_VERTICAL);
					break;
				case 2:
					//right
					textField.setGravity(Gravity.RIGHT|Gravity.CENTER_VERTICAL);
					break;
				default:
					break;
				}
			}
		});
	}
    
    //text field return type
    public void setKeyBoardReturnType(final int type)
    {
		context.runOnUiThread(new Runnable() 
		{
			@Override
			public void run() {
				// TODO Auto-generated method stub
				String string = type + "";
				Log.d("android", string);
				
				switch (type) {
				case 0:
					textField.setImeOptions(EditorInfo.IME_ACTION_DONE);
					break;
				case 1:
					textField.setImeOptions(EditorInfo.IME_ACTION_GO);
					break;
				case 2:
					textField.setImeOptions(EditorInfo.IME_ACTION_NEXT);
					break;
				case 3:
					textField.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
					break;
				case 4:
					textField.setImeOptions(EditorInfo.IME_ACTION_SEND);
					break;
				default:
					textField.setImeOptions(EditorInfo.IME_ACTION_DONE);
					break;
				}
			}
		});
    }
    
    //margins right length
    public void setMarginsDis(final int left,final int right,final int top,final int bottom) 
    {
    	
    	leftMargin = left;
    	rightMargin = right;
    	
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
    public void setMarginLeftImage(final String filePath)
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	
            }
        });
	}
  //margins right image
    public void setMarginRightImage(final String filePath) 
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	
            }
        });
	}
  //clearButton
    public void showClearButton()
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	clearButton = new Button(context);
            	clearButton.setBackgroundColor(0);
//            	clearButton.setHighlightColor(Color.YELLOW);
            	
            	FrameLayout.LayoutParams btnParams = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
            	btnParams.width = 20;
            	btnParams.height = 20;
            	btnParams.rightMargin = -1000;
            	btnParams.topMargin = -1000;
            	layout.addView(clearButton,btnParams) ;
            	
            	clearButton.setVisibility(View.GONE);
            	
            	clearButton.setOnClickListener(new View.OnClickListener()
            	{
                     public void onClick(View v) {
                     	textField.setText("");
                   }
                });
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
            	params.topMargin = y;
            	textField.setLayoutParams(params);
            	
            	if (clearButton != null) 
            	{
					FrameLayout.LayoutParams btnParams = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
					btnParams.width = params.height;
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
    
    public void setSecureTextEntry(int var)
    {
    	if (var == 0)
    	{
    		context.runOnUiThread(new Runnable() 
        	{
                @Override
                public void run()
                {
                	textField.setInputType(inputType);
                }
            });
    	}
    	else
    	{
    		context.runOnUiThread(new Runnable() 
        	{
                @Override
                public void run()
                {
                	textField.setInputType(InputType.TYPE_NUMBER_VARIATION_PASSWORD);
                }
            });
    	}
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
    
    private static native void hideImageView(int key);
    
    public void becomeFirstResponder()
    {
    	isShowKey = true;
    	Cocos2dxActivity.setSingleTextField(this);
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	//show
              	InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE); 
        		imm.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS);
        		textField.requestFocus();
        		
        		if (clearButton != null)
            	{
    				clearButton.setVisibility(View.VISIBLE);
    				textField.setPadding(leftMargin, 0, rightMargin, 0);
            	}
        		
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
    	Cocos2dxActivity.setSingleTextField(null);
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	//show
            	if (clearButton != null)
            	{
            		clearButton.setVisibility(View.GONE);
            		textField.setPadding(leftMargin, 0, 10, 0);
            	}
            	
            	Editable etext = textField.getText();
            	textField.setSelection(etext.length());
            	InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE);  
            	imm.hideSoftInputFromWindow(textField.getWindowToken(), 0);
        		textField.clearFocus();
        		
        		FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)textField.getLayoutParams(); 
            	params.leftMargin = -1000; 
            	params.topMargin = -1000;
            	textField.setLayoutParams(params);
        		
            	TimerTask task = new TimerTask()
            	{    
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
            	};  
            		
            	Timer timer = new Timer();  
            	timer.schedule(task, (long) 50);
				
            }
        });
    }
    
    public void setMaxLenght(final int var)
    {
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	textField.setFilters(new InputFilter[]{new InputFilter.LengthFilter(var)});
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
