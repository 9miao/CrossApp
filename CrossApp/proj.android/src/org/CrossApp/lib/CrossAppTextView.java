
package org.CrossApp.lib;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Map.Entry;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Rect;
import android.os.Handler;
import android.os.Looper;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.ViewTreeObserver;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.RelativeLayout.LayoutParams;
import android.widget.TextView.OnEditorActionListener;

@SuppressLint("UseSparseArrays") public class CrossAppTextView
{
	private EditText textView = null; 
	private static FrameLayout layout = null;
	private static CrossAppActivity context = null;
	private static Handler handler = null;
	private static HashMap<Integer, CrossAppTextView> dict = null;
	private int mykey = -1;
	private ByteBuffer imageData = null;
	private Bitmap bmp = null;
	private int keyboardheight = 0;
	private int keyboardheightTemp = 0;
	private int leftMargin = 5;
	private int rightMargin = 5;
	private int inputType = (InputType.TYPE_TEXT_FLAG_MULTI_LINE | InputType.TYPE_CLASS_TEXT);
	private int fontSize = 20;
	private String placeHolder = "";
	private int placeHolderColor = Color.GRAY;
	private String textViewText = "";
	private int textViewTextColor = Color.BLACK;
	private int contentSizeW = 800;
	private int contentSizeH = 400;
	private boolean secureTextEntry = false;
	private boolean showClearButton = false;
	private int keyBoardReturnType = EditorInfo.IME_ACTION_NONE;
	private int gravity = (Gravity.LEFT | Gravity.TOP);
	private TextWatcher textWatcher = null;
	private OnEditorActionListener onEditorActionListener = null;
	private ViewTreeObserver.OnGlobalLayoutListener onGlobalLayoutListener = null;
	
	private boolean isSetText = false;
	private String  beforeTextString = "";
	private int selection = 0;
	
	private boolean isFocus = false;
	private boolean isFocusAction = false;
	
 	protected void finalize()
    {
		layout.removeView(textView);
     }
	
	public static void initWithHandler()
	{
		if (dict == null)
    	{
    		dict = new HashMap<Integer, CrossAppTextView>();
    	}
		
		CrossAppTextView.reload();
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
			CrossAppTextView textView = dict.get(key);
			textView.initWithTextView(key);
		}
	}
	
	public static boolean isShowKeyboard()
	{
		boolean showKeyboard = false;
		Iterator<Entry<Integer, CrossAppTextView>> iter = dict.entrySet().iterator();
		while (iter.hasNext())
		{
			HashMap.Entry entry = (HashMap.Entry) iter.next();
			CrossAppTextView val = (CrossAppTextView)entry.getValue();
			if (val.textView!=null && val.textView.isFocused())
			{
				showKeyboard = true;
				break;
			}
		}
		return showKeyboard;
	}
	
	//keyBoard return call back
	private static native void keyBoardReturnCallBack(int key);
	private static native boolean textChange(int key,String before,String change,int arg0,int arg1);
	private static native void text(int key, byte[] text, int lenght);
    public void init(int key)
    {
    	mykey = key;
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	initWithTextView(mykey);
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
            				if (keyboardheightTemp < 1 && isFocus == true)
            				{
    							//hide
            					isFocus = false;
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
            				if (isFocusAction)
            				{
            					context.runOnGLThread(new Runnable() 
                            	{
                                    @Override
                                    public void run()
                                    {
                                    	keyBoardHeightReturn(mykey, keyboardheightTemp);
                                    }
                                });
            					isFocusAction = false;
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
				textView.setTextSize(size);
			}
		});
	}

    public void setTextViewText(final String text) 
    {
    		context.runOnUiThread(new Runnable()
    		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				isSetText = true;
				textViewText = text;
				textView.setText(text);
				isSetText = false;
			}
		});
	}
    
    //textView color 
    public void setTextViewTextColor(final int color)
    {
		context.runOnUiThread(new Runnable()
		{
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				textViewTextColor = color;
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
			public void run() 
			{
				// TODO Auto-generated method stub
				switch (var)
				{
				case 0:
					//center
					gravity = (Gravity.LEFT | Gravity.TOP);
					break;
				case 1:
					//left
					gravity = (Gravity.CENTER | Gravity.TOP);
					break;
				case 2:
					//right
					gravity = (Gravity.RIGHT | Gravity.TOP);
					break;
				default:
					break;
				}
				textView.setGravity(gravity);
			}
		});
	}
    
    public void setKeyBoardReturnType(final int type)
    {
		context.runOnUiThread(new Runnable() 
		{
			@Override
			public void run() {
				// TODO Auto-generated method stub
				String string = type + "";
				switch (type) {
				case 0:
					inputType = (InputType.TYPE_TEXT_FLAG_MULTI_LINE | InputType.TYPE_CLASS_TEXT);
					keyBoardReturnType = EditorInfo.IME_ACTION_NONE;
					break;
				case 1:
					inputType = (InputType.TYPE_TEXT_FLAG_MULTI_LINE | InputType.TYPE_CLASS_TEXT);
					keyBoardReturnType = EditorInfo.IME_ACTION_DONE;
					break;
				case 2:
					inputType = (InputType.TYPE_TEXT_FLAG_MULTI_LINE | InputType.TYPE_CLASS_TEXT);
					keyBoardReturnType = EditorInfo.IME_ACTION_SEND;
					break;
				case 3:
					inputType = (InputType.TYPE_TEXT_FLAG_MULTI_LINE | InputType.TYPE_CLASS_TEXT);
					keyBoardReturnType = EditorInfo.IME_ACTION_NEXT;
					break;
				default:
					break;
				}
				textView.setInputType(inputType);
				textView.setImeOptions(keyBoardReturnType);
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
            	textView.requestLayout();
            }
        });
    }
    
    public void setTextViewSize(final int width, final int height)
    {
    	contentSizeW = width;
    	contentSizeH = height;
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)textView.getLayoutParams(); 
            	params.width = width;
            	params.height = height;
            	textView.setLayoutParams(params);
            	textView.requestLayout();
            	
            	TimerTask task = new TimerTask()
        		{    
        			public void run()
        			{    
        				getImage();
        			}    
        		};  
        		
        		Timer timer = new Timer();  
        		timer.schedule(task, (long) 100);
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
            	if (bmp != null && imageData == null)
            	{
            		imageData = ByteBuffer.allocate(bmp.getRowBytes() * bmp.getHeight());
            		bmp.copyPixelsToBuffer(imageData);
            		
            		context.runOnGLThread(new Runnable() 
                	{
                        @Override
                        public void run()
                        {
                        	onByte(mykey, imageData.array(), bmp.getWidth(), bmp.getHeight());
                        	imageData = null;
                        }
                    });
            	}
            }
        });
    }
    
    private static native void hideImageView(int key);
    
    public void becomeFirstResponder()
    {
    	CrossAppActivity.setSingleTextView(this);
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {
            	isFocus = true;
            	isFocusAction = true;
            	
            	//show
        		textView.requestFocus();
        		Editable etext = textView.getText();
            	textView.setSelection(etext.length());
        		
            	TimerTask task = new TimerTask()
        		{    
        			public void run()
        			{    
        				if (CrossAppTextField.isShowKeyboard() || CrossAppTextView.isShowKeyboard())
        				{
        					InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE);
        					imm.showSoftInput(textView, 0);  
        				}
        			}    
        		};  
        		Timer timer = new Timer();  
        		timer.schedule(task, (long) 20);
            	
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
    
    private static native void showImageView(int key);
    
    public void resignFirstResponder()
    {
    	CrossAppActivity.setSingleTextView(null);
    	context.runOnUiThread(new Runnable() 
    	{
            @Override
            public void run()
            {        
            	isFocus = false;
            	isFocusAction = true;
            	
            	textView.clearFocus();
        		
        		TimerTask task = new TimerTask()
        		{    
        			public void run()
        			{    
        				if (!CrossAppTextField.isShowKeyboard() && !CrossAppTextView.isShowKeyboard())
        				{
        					InputMethodManager imm = (InputMethodManager)context.getSystemService(Context.INPUT_METHOD_SERVICE);
        					imm.hideSoftInputFromWindow(textView.getWindowToken(), 0);
        				}
        			}    
        		};  
        		Timer timer = new Timer();  
        		timer.schedule(task, (long) 20);
        		
        		FrameLayout.LayoutParams params = (FrameLayout.LayoutParams)textView.getLayoutParams(); 
        		params.leftMargin = -10000; 
            	params.topMargin = 0;
            	textView.setLayoutParams(params);
        		
            	bmp = textView.getDrawingCache();
            	if (bmp != null && imageData == null)
            	{
            		imageData = ByteBuffer.allocate(bmp.getRowBytes() * bmp.getHeight());
            		bmp.copyPixelsToBuffer(imageData);
            		
            		context.runOnGLThread(new Runnable() 
                	{
                        @Override
                        public void run()
                        {
                        	onByte(mykey, imageData.array(), bmp.getWidth(), bmp.getHeight());
                        	showImageView(mykey);
                        	imageData = null;
                        }
                    });
            	} 
            }
        });
    }
    
    @TargetApi(16)
    public void removeThis()
    {
    	textView.removeTextChangedListener(textWatcher);
    	layout.getViewTreeObserver().removeOnGlobalLayoutListener(onGlobalLayoutListener);
    	layout.removeView(textView);
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
		dict.put(key, text);
		text.init(key);
	}
	
	static public void removeTextView(final int key) 
	{
		final CrossAppTextView var = dict.get(key);
		dict.remove(key);
		if (var != null)
		{
			context.runOnUiThread(new Runnable() 
	    	{
	            @Override
	            public void run()
	            {
	            	var.removeThis();
	            }
	        });
		}
	}

	public void initWithTextView(int key)
	{
		if (textView != null)
		{
			layout.removeView(textView);
			textView = null;
		}
		
		textView = new EditText(context) ; 
		textView.setSingleLine(false);  
		textView.setGravity(gravity);
		textView.setBackgroundColor(0);
    	textView.setFocusable(true);
    	textView.setDrawingCacheEnabled(true);
    	textView.setTextSize(fontSize);
		textView.setInputType(inputType);
//		textView.setHint(placeHolder);
//		textView.setHintTextColor(placeHolderColor);
		textView.setText(textViewText);
		textView.setTextColor(textViewTextColor);
		textView.setImeOptions(keyBoardReturnType);
		
    	FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT) ; 
    	params.leftMargin = -10000; 
    	params.topMargin = 0;
    	params.width = contentSizeW;
    	params.height = contentSizeH;
    	layout.addView(textView, params) ;


    	textWatcher = new TextWatcher()
    	{
    		@Override
			public void onTextChanged(CharSequence arg0, int arg1, int arg2, int arg3)
			{
				// TODO Auto-generated method stub
				if (isSetText)
				{
					return;
				}

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
					isSetText = true;
					textView.setText(beforeTextString);
					textView.setSelection(selection);
					isSetText = false;
				}
				else
				{
					isSetText = true;
					textView.setText(string);
					textView.setSelection(string!=null && string.length() > 0 ? selection - arg2 + arg3 : 0);

					final ByteBuffer textBuffer = ByteBuffer.wrap(textView.getText().toString().getBytes());
					context.runOnGLThread(new Runnable() 
                	{
                        @Override
                        public void run()
                        {
                        	text(mykey, textBuffer.array(), textBuffer.array().length);
                        }
                    });
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
				selection = textView.getSelectionStart();
			}
			
			@Override
			public void afterTextChanged(Editable arg0)
			{
				// TODO Auto-generated method stub
			}
		};
		textView.addTextChangedListener(textWatcher);

		
		onEditorActionListener = new OnEditorActionListener() {
			
			@Override
			public boolean onEditorAction(TextView arg0, int arg1, KeyEvent arg2) {
				// TODO Auto-generated method stub
				if (keyBoardReturnType != EditorInfo.IME_ACTION_NONE && arg2.getAction() == KeyEvent.ACTION_DOWN)
				{
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
        		return false;
			}
		};    	
    	textView.setOnEditorActionListener(onEditorActionListener);
    	
		getKeyBoardHeight();
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
