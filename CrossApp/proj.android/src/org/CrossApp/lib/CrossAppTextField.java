
package org.CrossApp.lib;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;

import android.R.bool;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Rect;
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
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.RelativeLayout.LayoutParams;
import android.widget.TextView.OnEditorActionListener;

@SuppressLint("UseSparseArrays") public class CrossAppTextField
{
	private EditText textField = null; 
	private static FrameLayout layout = null;
	private static CrossAppActivity context = null;
	private static Handler handler = null;
	private static HashMap<Integer, CrossAppTextField> dict = null;
	private int mykey = -1;
	private ByteBuffer imageData = null;
	private Bitmap bmp = null;
	private Button 	clearButton = null;
	private int keyboardheight = 0;
	private int keyboardheightTemp = 0;
	private int leftMargin = 10;
	private int rightMargin = 10;
	private int inputType = InputType.TYPE_CLASS_TEXT;
	private int fontSize = 20;
	private String placeHolder = "";
	private int placeHolderColor = Color.GRAY;
	private String textFieldText = "";
	private int textFieldTextColor = Color.BLACK;
	private int contentSizeW = 800;
	private int contentSizeH = 200;
	private boolean secureTextEntry = false;
	private boolean showClearButton = false;
	private int keyBoardReturnType = EditorInfo.IME_ACTION_DONE;
	private int gravity = (Gravity.LEFT | Gravity.CENTER_VERTICAL);
	private TextWatcher textWatcher = null;
	private OnEditorActionListener onEditorActionListener = null;
	private ViewTreeObserver.OnGlobalLayoutListener onGlobalLayoutListener = null;
	
	private boolean isSetText = false;
	private String  beforeTextString = "";
	private int selection = 0;

	private boolean isShowKey = false;
	private boolean isKeyAction = false;
	
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
    		context =  (CrossAppActivity)CrossAppActivity.getContext();
    	}
		
		if (layout == null)
    	{
    		layout = CrossAppActivity.getFrameLayout();
    	}
	}
	
	public static void updateImage()
	{
		Set<Integer> keys = (Set<Integer>) dict.keySet() ; 
		Iterator<Integer> iterator = keys.iterator() ; 
		while (iterator.hasNext())
		{
			Integer key = iterator.next();
			CrossAppTextField textField = dict.get(key);
			textField.getImage();
		}
	}
	
	public static void reload()
	{
		handler = new Handler(Looper.myLooper());
		context =  (CrossAppActivity)CrossAppActivity.getContext();
		layout = CrossAppActivity.getFrameLayout();
		
		Set<Integer> keys = (Set<Integer>) dict.keySet() ; 
		Iterator<Integer> iterator = keys.iterator() ; 
		while (iterator.hasNext())
		{
			Integer key = iterator.next();
			CrossAppTextField textField = dict.get(key);
			textField.initWithTextField(key);
		}
	}
	
	//keyBoard return call back
	private static native void keyBoardReturnCallBack(int key);
	private static native boolean textChange(int key,String before,String change,int arg0,int arg1);
	private static native void text(int key, byte[] text, int lenght);
    public void init(int key)
    {
    	mykey = key;
    	dict.put(key, this);
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	initWithTextField(mykey);
            }
        });
    }
  
    
    //keyboard height
    

    private static native void keyBoardHeightReturn(int key,int height);
    private static native void resignFirstResponder(int key);
    public int getKeyBoardHeight()
    {
    	onGlobalLayoutListener = new ViewTreeObserver.OnGlobalLayoutListener() 
    	{

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
//            				if (keyboardheight<1) {
//    							//show
//            					Log.d("android", "show board");
//    						}
//            				Log.d("android", "call c++");
            				
            				//keyBoardReturn
            				if (isKeyAction)
            				{
            					context.runOnGLThread(new Runnable() 
                            	{
                                    @Override
                                    public void run()
                                    {
                                    	keyBoardHeightReturn(mykey, keyboardheightTemp);
                                    }
                                });
            					isKeyAction = false;
            				}
            			}
            		});
    			}
                keyboardheight = keyboardheightTemp;
            }
        };
    	layout.getViewTreeObserver().addOnGlobalLayoutListener(onGlobalLayoutListener);

		return keyboardheight;

    }

    public void setFontSize(final int size) 
    {
		context.runOnUiThread(new Runnable()
		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				fontSize = size;
				textField.setTextSize(size);
			}
		});
	}
    //placeholder text
    public void setTextFieldPlacHolder(final String text) 
    {
		context.runOnUiThread(new Runnable()
		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				placeHolder = text;
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
				textFieldText = text;
				textField.setText(text);
				isSetText = false;
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
				placeHolderColor = color;
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
			public void run()
			{
				// TODO Auto-generated method stub
				textFieldTextColor = color;
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
			public void run() 
			{
				// TODO Auto-generated method stub
				switch (var)
				{
				case 0:
					//center
					gravity = (Gravity.LEFT | Gravity.CENTER_VERTICAL);
					break;
				case 1:
					//left
					gravity = (Gravity.CENTER | Gravity.CENTER_VERTICAL);
					break;
				case 2:
					//right
					gravity = (Gravity.RIGHT | Gravity.CENTER_VERTICAL);
					break;
				default:
					break;
				}
				textField.setGravity(gravity);
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
				
				switch (type) 
				{
				case 0:
					keyBoardReturnType = EditorInfo.IME_ACTION_DONE;
					break;
				case 1:
					keyBoardReturnType = EditorInfo.IME_ACTION_GO;
					break;
				case 2:
					keyBoardReturnType = EditorInfo.IME_ACTION_NEXT;
					break;
				case 3:
					keyBoardReturnType = EditorInfo.IME_ACTION_SEARCH;
					break;
				case 4:
					keyBoardReturnType = EditorInfo.IME_ACTION_SEND;
					break;
				default:
					keyBoardReturnType = EditorInfo.IME_ACTION_DONE;
					break;
				}
				textField.setImeOptions(keyBoardReturnType);
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
            	showClearButton = true;
            	
            	clearButton = new Button(context);
            	clearButton.setBackgroundColor(0);
//            	clearButton.setHighlightColor(Color.YELLOW);
            	
            	FrameLayout.LayoutParams btnParams = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT);
            	btnParams.width = 20;
            	btnParams.height = 20;
            	btnParams.rightMargin = -1000;
            	btnParams.topMargin = -1000;
            	layout.addView(clearButton, btnParams) ;
            	
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
    	contentSizeW = width;
    	contentSizeH = height;
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)textField.getLayoutParams(); 
            	params.width = contentSizeW;
            	params.height = contentSizeH;
            	textField.setLayoutParams(params);
            }
        });
    }
    
    public void setSecureTextEntry(int var)
    {
    	if (var == 0)
    	{
    		secureTextEntry = false;
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
    		secureTextEntry = true;
    		context.runOnUiThread(new Runnable() 
        	{
                @Override
                public void run()
                {
                	if (inputType == InputType.TYPE_CLASS_NUMBER)
                	{
                		textField.setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_VARIATION_PASSWORD);
                	}
                	else
                	{
                		textField.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
                	}
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
    	CrossAppActivity.setSingleTextField(this);
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	isShowKey = true;
            	isKeyAction = true;
            	//show
              	InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE); 
        		imm.toggleSoftInput(0, InputMethodManager.HIDE_NOT_ALWAYS);
        		textField.requestFocus();
        		Editable etext = textField.getText();
            	textField.setSelection(etext.length());
            	
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
    	CrossAppActivity.setSingleTextField(null);
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	isShowKey = false;
            	isKeyAction = true;
            	//show
            	if (clearButton != null)
            	{
            		clearButton.setVisibility(View.GONE);
            		textField.setPadding(leftMargin, 0, 10, 0);
            	}
            	textField.setSelection(0);
            	InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE);  
            	imm.hideSoftInputFromWindow(textField.getWindowToken(), 0);
        		textField.clearFocus();
        		
        		FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)textField.getLayoutParams(); 
            	params.leftMargin = -10000; 
            	params.topMargin = 0;
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
    
    public void removeThis()
    {
    	textField.removeTextChangedListener(textWatcher);
    	layout.getViewTreeObserver().removeOnGlobalLayoutListener(onGlobalLayoutListener);
    	layout.removeView(textField);
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
		final CrossAppTextField var = dict.get(key);
		if (var != null)
		{
			context.runOnUiThread(new Runnable() 
	    	{
	            @Override
	            public void run()
	            {
	            	var.removeThis();
	            	dict.remove(key);
	            }
	        });
		}
		
	}
	
	public void initWithTextField(int key)
	{
		if (textField != null)
		{
			layout.removeView(textField);
		}
		
    	textField = new EditText(context) ; 
    	textField.setMaxLines(1);
    	textField.setSingleLine(true); 
    	textField.setGravity(gravity);
    	textField.setBackgroundColor(0);
		textField.setFocusable(true);
		textField.setDrawingCacheEnabled(true);
		textField.setTextSize(fontSize);
		textField.setInputType(inputType);
		textField.setHint(placeHolder);
		textField.setHintTextColor(placeHolderColor);
		textField.setText(textFieldText);
		textField.setTextColor(textFieldTextColor);
		textField.setImeOptions(keyBoardReturnType);
		
    	FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT) ; 
    	params.leftMargin = -10000; 
    	params.topMargin = 0;
    	params.width = contentSizeW;
    	params.height = contentSizeH;
    	layout.addView(textField, params) ;
    	
    	textField.setPadding(leftMargin, 0, rightMargin, 0) ;
    	
    	if	(secureTextEntry == true)
    	{
    		setSecureTextEntry(1);
    	}
		
    	if (showClearButton == true)
    	{
    		showClearButton();
    	}

    	textWatcher = new TextWatcher()
    	{
			@Override
			public void onTextChanged(CharSequence arg0, int arg1, int arg2, int arg3)
			{
				// TODO Auto-generated method stub

				String string = arg0.toString();
				
				String  changedText = "";
				if (arg3 > 0) 
				{
					changedText = string.substring(arg1, arg1 + arg3);
				}
				else 
				{
					changedText = "";
				}

				if (!textChange(mykey, beforeTextString, changedText, arg1, arg2))
				{
					if (isSetText == false)
					{
						isSetText = true;
						textField.setText(beforeTextString);
						textField.setSelection(selection);
						isSetText = false;
					}
					
				}
				else
				{
					if (isSetText == false)
					{
						isSetText = true;
						textField.setText(string);
						textField.setSelection(selection - arg2 + arg3);
					}
					ByteBuffer textBuffer = ByteBuffer.wrap(textField.getText().toString().getBytes());
					text(mykey, textBuffer.array(), textBuffer.array().length);
					isSetText = false;
				}
			}
			
			@Override
			public void beforeTextChanged(CharSequence arg0, int arg1, int arg2,
					int arg3)
			{
				if (isSetText)
				{
					return;
				}
				// TODO Auto-generated method stub
				beforeTextString = arg0.toString();
				selection = textField.getSelectionStart();  
			}
			
			@Override
			public void afterTextChanged(Editable arg0)
			{
				if (isSetText)
				{
					return;
				}
				// TODO Auto-generated method stub
			}
		};
		textField.addTextChangedListener(textWatcher);

		
		onEditorActionListener = new OnEditorActionListener() {
			
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
		};    	
    	textField.setOnEditorActionListener(onEditorActionListener);
    	
		getKeyBoardHeight();
	}
	
	
	public void resume() 
	{
		TimerTask task = new TimerTask()
		{    
			public void run()
			{    
				context.runOnGLThread(new Runnable() 
            	{
                    @Override
                    public void run()
                    {
                    	resignFirstResponder(mykey);
                    }
                });
			}    
		};  
		
		Timer timer = new Timer();  
		timer.schedule(task, (long) 100);
	}
}
