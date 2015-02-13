
package org.CrossApp.lib;

import android.R.integer;
import android.content.Context;
import android.graphics.Rect;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Message;
import android.text.InputType;
import android.text.Selection;
import android.text.Spannable;
import android.text.method.PasswordTransformationMethod;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;

public class Cocos2dxGLSurfaceView extends GLSurfaceView {
	// ===========================================================
	// Constants
	// ===========================================================

	private static final String TAG = Cocos2dxGLSurfaceView.class.getSimpleName();

	private final static int HANDLER_OPEN_IME_KEYBOARD = 2;
	private final static int HANDLER_CLOSE_IME_KEYBOARD = 3;

	private final static int KEY_BOARD_TYPE_NORMAL=10;
	private final static int KEY_BOARD_TYPE_NUMBER=11;
	private final static int KEY_BOARD_TYPE_ALPHABET=12;
	
	private final static int KEY_BOARD_RETURNTYPE_DONE=21;
	private final static int KEY_BOARD_RETURNTYPE_SEARCH=22;
	private final static int KEY_BOARD_RETURNTYPE_SEND=23;
	private final static int RESET_SELECTION_POSITION =24;
	private final static int RESET_TEXT=13;

	private final static int SET_CURSOR_POS = 33;

	// ===========================================================
	// Fields
	// ===========================================================

	// TODO Static handler -> Potential leak!
	private static Handler sHandler;

	private static Cocos2dxGLSurfaceView mCocos2dxGLSurfaceView;
	private static Cocos2dxTextInputWraper sCocos2dxTextInputWraper;

	private Cocos2dxRenderer mCocos2dxRenderer;
	private Cocos2dxEditText mCocos2dxEditText;
	private static Cocos2dxEditText text;
	// ===========================================================
	// Constructors
	// ===========================================================

	public Cocos2dxGLSurfaceView(final Context context) {
		super(context);

		this.initView();
	}

	public Cocos2dxGLSurfaceView(final Context context, final AttributeSet attrs) {
		super(context, attrs);
		
		this.initView();
	}

	protected void initView() {
		this.setEGLContextClientVersion(2);
		this.setFocusableInTouchMode(true);

		Cocos2dxGLSurfaceView.mCocos2dxGLSurfaceView = this;
		Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper = new Cocos2dxTextInputWraper(this);

		Cocos2dxGLSurfaceView.sHandler = new Handler() {
			@Override
			public void handleMessage(final Message msg) {
				switch (msg.what) {
					case HANDLER_OPEN_IME_KEYBOARD:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus()) {
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.removeTextChangedListener(Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper);
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setText("");
							text = Cocos2dxGLSurfaceView.this.mCocos2dxEditText;
							final String text = (String) msg.obj;
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setText(text);
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setSelection(msg.arg1);
							Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper.setOriginText(text);
						//	
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setSelection(Cocos2dxGLSurfaceView.this.mCocos2dxEditText.getText().length());
//							CharSequence text1 = Cocos2dxGLSurfaceView.this.mCocos2dxEditText.getText();
//							
//							if (text1 instanceof Spannable) {
//							     Spannable spanText = (Spannable)text1;
//							     Selection.setSelection(spanText, text1.length());
//							 }
							//
							//InputType.TYPE_CLASS_NUMBER
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.addTextChangedListener(Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper);
							final InputMethodManager imm = (InputMethodManager) Cocos2dxGLSurfaceView.mCocos2dxGLSurfaceView.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
							imm.showSoftInput(Cocos2dxGLSurfaceView.this.mCocos2dxEditText, 0);
							Log.d("GLSurfaceView", "showSoftInput");
							
						}
						break;
					case SET_CURSOR_POS:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText) {
							text = Cocos2dxGLSurfaceView.this.mCocos2dxEditText;
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setSelection(msg.arg1);
						}
						break;
					case HANDLER_CLOSE_IME_KEYBOARD:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText) {
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.removeTextChangedListener(Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper);
							final InputMethodManager imm = (InputMethodManager) Cocos2dxGLSurfaceView.mCocos2dxGLSurfaceView.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
							imm.hideSoftInputFromWindow(Cocos2dxGLSurfaceView.this.mCocos2dxEditText.getWindowToken(), 0);
							Cocos2dxGLSurfaceView.this.requestFocus();
							Log.d("GLSurfaceView", "HideSoftInput");
						}
						break;
					case KEY_BOARD_TYPE_NORMAL:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus()) {
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setInputType(InputType.TYPE_CLASS_TEXT|InputType.TYPE_TEXT_FLAG_CAP_WORDS);
						}
						break;
					case KEY_BOARD_TYPE_NUMBER:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus()) {
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setInputType(InputType.TYPE_CLASS_NUMBER|InputType.TYPE_NUMBER_FLAG_DECIMAL);
						}
						break;
					case KEY_BOARD_TYPE_ALPHABET:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus()) {
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setInputType(InputType.TYPE_CLASS_TEXT|InputType.TYPE_TEXT_FLAG_CAP_CHARACTERS);
						}
						break;
					case RESET_TEXT:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus()) {
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.removeTextChangedListener(Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper);
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setText("");
							text = Cocos2dxGLSurfaceView.this.mCocos2dxEditText;
							final String text = (String) msg.obj;
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.append(text);
							Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper.setOriginText(text);
							
							//
							//InputType.TYPE_CLASS_NUMBER
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.addTextChangedListener(Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper);
						}
						break;
					case KEY_BOARD_RETURNTYPE_DONE:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus())
						{
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setImeOptions(EditorInfo.IME_ACTION_DONE);
						}
						break;
					case KEY_BOARD_RETURNTYPE_SEARCH:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus())
						{
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
						}
						break;
					case KEY_BOARD_RETURNTYPE_SEND:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus())
						{
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setImeOptions(EditorInfo.IME_ACTION_SEND);
						}
						break;
					case RESET_SELECTION_POSITION:
						if (Cocos2dxGLSurfaceView.this.mCocos2dxEditText !=null) 
						{
							int a= (int)msg.arg1;
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setSelection(a);
						}
						break;
				}
			}
		};
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================


	public static Cocos2dxGLSurfaceView getInstance() {
		return mCocos2dxGLSurfaceView;
	}

	public void setKeyBoardType(int type) {
		final Message msg = new Message();
		msg.what = type;
		Cocos2dxGLSurfaceView.sHandler.sendMessage(msg);
	}

	public void setKeyBoardRetrunType(int type) {
		final Message msg = new Message();
		msg.what = type;
		Cocos2dxGLSurfaceView.sHandler.sendMessage(msg);
	}

	public void setCursorPos(int pos) {
		final Message msg = new Message();
		msg.what = SET_CURSOR_POS;
		msg.arg1 = pos;
		Cocos2dxGLSurfaceView.sHandler.sendMessage(msg);
	}

	public static void queueAccelerometer(final float x, final float y, final float z, final long timestamp) {
		mCocos2dxGLSurfaceView.queueEvent(new Runnable() {
			@Override
			public void run() {
				Cocos2dxAccelerometer.onSensorChanged(x, y, z, timestamp);
			}
		});
	}

	public void setCocos2dxRenderer(final Cocos2dxRenderer renderer) {
		this.mCocos2dxRenderer = renderer;
		this.setRenderer(this.mCocos2dxRenderer);
	}
	public void changeSelectionPosition(int pos)
	{
		
		final Message msg = new Message();
		msg.what = Cocos2dxGLSurfaceView.RESET_SELECTION_POSITION;
		msg.arg1 =pos;
		Cocos2dxGLSurfaceView.sHandler.sendMessage(msg);


	}
	public String getContentText() {
		return this.mCocos2dxRenderer.getContentText();
	}

	public Cocos2dxEditText getCocos2dxEditText() {
		return this.mCocos2dxEditText;
	}

	public void setCocos2dxEditText(final Cocos2dxEditText pCocos2dxEditText) {
		this.mCocos2dxEditText = pCocos2dxEditText;
		
		if (null != this.mCocos2dxEditText && null != Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper) {
			this.mCocos2dxEditText.setOnEditorActionListener(Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper);
			this.mCocos2dxEditText.setCocos2dxGLSurfaceView(this);
			this.requestFocus();
		}
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void onResume() {
		super.onResume();
		
		this.setRenderMode(RENDERMODE_CONTINUOUSLY);
		
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleOnResume();
			}
		});
	}

	@Override
	public void onPause() {
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleOnPause();
			}
		});
		
		this.setRenderMode(RENDERMODE_WHEN_DIRTY);
		
		//super.onPause();
	}

	@Override
	public boolean onTouchEvent(final MotionEvent pMotionEvent) {
		// these data are used in ACTION_MOVE and ACTION_CANCEL
		final int pointerNumber = pMotionEvent.getPointerCount();
		final int[] ids = new int[pointerNumber];
		final float[] xs = new float[pointerNumber];
		final float[] ys = new float[pointerNumber];

		for (int i = 0; i < pointerNumber; i++) {
			ids[i] = pMotionEvent.getPointerId(i);
			xs[i] = pMotionEvent.getX(i);
			ys[i] = pMotionEvent.getY(i);
		}
		switch (pMotionEvent.getAction() & MotionEvent.ACTION_MASK) {
			case MotionEvent.ACTION_POINTER_DOWN:
				final int indexPointerDown = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
				final int idPointerDown = pMotionEvent.getPointerId(indexPointerDown);
				final float xPointerDown = pMotionEvent.getX(indexPointerDown);
				final float yPointerDown = pMotionEvent.getY(indexPointerDown);

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionDown(idPointerDown, xPointerDown, yPointerDown);
						
					}
				});
				break;

			case MotionEvent.ACTION_DOWN:
				// there are only one finger on the screen
				final int idDown = pMotionEvent.getPointerId(0);
				final float xDown = xs[0];
				final float yDown = ys[0];

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionDown(idDown, xDown, yDown);
					}
				});
				break;

			case MotionEvent.ACTION_MOVE:
				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionMove(ids, xs, ys);
					}
				});
				break;

			case MotionEvent.ACTION_POINTER_UP:
				final int indexPointUp = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
				final int idPointerUp = pMotionEvent.getPointerId(indexPointUp);
				final float xPointerUp = pMotionEvent.getX(indexPointUp);
				final float yPointerUp = pMotionEvent.getY(indexPointUp);

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionUp(idPointerUp, xPointerUp, yPointerUp);
					}
				});
				break;

			case MotionEvent.ACTION_UP:
				// there are only one finger on the screen
				final int idUp = pMotionEvent.getPointerId(0);
				final float xUp = xs[0];
				final float yUp = ys[0];

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionUp(idUp, xUp, yUp);
					}
				});
				break;

			case MotionEvent.ACTION_CANCEL:
				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionCancel(ids, xs, ys);
					}
				});
				break;
		}

        /*
		if (BuildConfig.DEBUG) {
			Cocos2dxGLSurfaceView.dumpMotionEvent(pMotionEvent);
		}
		*/
		return true;
	}

	/*
	 * This function is called before Cocos2dxRenderer.nativeInit(), so the
	 * width and height is correct.
	 */
	@Override
	protected void onSizeChanged(final int pNewSurfaceWidth, final int pNewSurfaceHeight, final int pOldSurfaceWidth, final int pOldSurfaceHeight) {
        if(!this.isInEditMode()) {
            Log.e("SUN", "SurfaceView onSizeChanged ..."+pNewSurfaceWidth+"."+pNewSurfaceHeight+" old "+pOldSurfaceWidth+"."+pOldSurfaceHeight);
            
            ViewGroup.LayoutParams lp = getLayoutParams();
            lp.width = pNewSurfaceWidth;
            lp.height = pNewSurfaceHeight;
            setLayoutParams(lp);
            this.mCocos2dxRenderer.setScreenWidthAndHeight(pNewSurfaceWidth, pNewSurfaceHeight);
            this.mCocos2dxRenderer.handleOnResume();
        }
	}

	@Override
	public boolean onKeyDown(final int pKeyCode, final KeyEvent pKeyEvent) {
		switch (pKeyCode) {
			case KeyEvent.KEYCODE_BACK:
			case KeyEvent.KEYCODE_MENU:
				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleKeyDown(pKeyCode);
					}
				});
				return true;
			case KeyEvent.KEYCODE_DPAD_LEFT:
				if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus()) {
					Cocos2dxGLSurfaceView.text = Cocos2dxGLSurfaceView.this.mCocos2dxEditText;
					this.queueEvent(new Runnable() {
						@Override
						public void run() {
							Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleKeyDown(pKeyCode);
						}
					});
				}
				return true;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
				if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus()) {
					Cocos2dxGLSurfaceView.text = Cocos2dxGLSurfaceView.this.mCocos2dxEditText;
					this.queueEvent(new Runnable() {
						@Override
						public void run() {
							Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleKeyDown(pKeyCode);
						}
					});
				}
				return true;
			default:
				return super.onKeyDown(pKeyCode, pKeyEvent);
		}
	}

	// ===========================================================
	// Methods
	// ===========================================================

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================

	public static void openIMEKeyboard() {
		final Message msg = new Message();
		msg.what = Cocos2dxGLSurfaceView.HANDLER_OPEN_IME_KEYBOARD;
		msg.obj = Cocos2dxGLSurfaceView.mCocos2dxGLSurfaceView.getContentText();
		msg.arg1 = Cocos2dxGLSurfaceView.mCocos2dxGLSurfaceView.mCocos2dxRenderer.getCursorPos();
		Cocos2dxGLSurfaceView.sHandler.sendMessage(msg);
	}

	public static void closeIMEKeyboard() {
		final Message msg = new Message();
		msg.what = Cocos2dxGLSurfaceView.HANDLER_CLOSE_IME_KEYBOARD;
		Cocos2dxGLSurfaceView.sHandler.sendMessage(msg);
	}

	public void insertText(final String pText) {
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleInsertText(pText);
			}
		});
	}
	public void willInsertText(final int start,final String pString,final int before,final int count) {
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleWillInsertText(start,pString,before,count);


			}
		});
	}
	public void deleteBackward() {
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleDeleteBackward();
			}
		});
	}

	private static void dumpMotionEvent(final MotionEvent event) {
		final String names[] = { "DOWN", "UP", "MOVE", "CANCEL", "OUTSIDE", "POINTER_DOWN", "POINTER_UP", "7?", "8?", "9?" };
		final StringBuilder sb = new StringBuilder();
		final int action = event.getAction();
		final int actionCode = action & MotionEvent.ACTION_MASK;
		sb.append("event ACTION_").append(names[actionCode]);
		if (actionCode == MotionEvent.ACTION_POINTER_DOWN || actionCode == MotionEvent.ACTION_POINTER_UP) {
			sb.append("(pid ").append(action >> MotionEvent.ACTION_POINTER_ID_SHIFT);
			sb.append(")");
		}
		sb.append("[");
		for (int i = 0; i < event.getPointerCount(); i++) {
			sb.append("#").append(i);
			sb.append("(pid ").append(event.getPointerId(i));
			sb.append(")=").append((int) event.getX(i));
			sb.append(",").append((int) event.getY(i));
			if (i + 1 < event.getPointerCount()) {
				sb.append(";");
			}
		}
		sb.append("]");
		Log.d(Cocos2dxGLSurfaceView.TAG, sb.toString());
	}
}
