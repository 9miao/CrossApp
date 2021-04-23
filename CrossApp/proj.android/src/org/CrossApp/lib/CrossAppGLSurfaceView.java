
package org.CrossApp.lib;

import android.app.ActionBar.LayoutParams;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.widget.FrameLayout;

public class CrossAppGLSurfaceView extends GLSurfaceView {
	// ===========================================================
	// Constants
	// ===========================================================

	private static final String TAG = CrossAppGLSurfaceView.class.getSimpleName();

	private static CrossAppGLSurfaceView mGLSurfaceView;

	private CrossAppRenderer mRenderer;
	// ===========================================================
	// Constructors
	// ===========================================================

	public CrossAppRenderer getRenderer()
	{
		return mRenderer;
	}
	
	public CrossAppGLSurfaceView(final Context context)
	{
		super(context);

		this.initView();
	}

	public CrossAppGLSurfaceView(final Context context, final AttributeSet attrs) 
	{
		super(context, attrs);
		
		this.initView();
	}

	protected void initView() 
	{
		this.setEGLContextClientVersion(2);
		this.setFocusableInTouchMode(true);
		CrossAppGLSurfaceView.mGLSurfaceView = this;
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================


	public static CrossAppGLSurfaceView getInstance()
	{
		return mGLSurfaceView;
	}

	public static void queueGyroscope(final float x, final float y, final float z, final float timestamp)
	{
		mGLSurfaceView.queueEvent(new Runnable()
		{
			@Override
			public void run() 
			{
				CrossAppGyroscope.onGyroSensorChanged(x,y,z,timestamp);
			}
		});
	}
	
	public void setCrossAppRenderer(final CrossAppRenderer renderer)
	{
		this.mRenderer = renderer;
		this.setRenderer(this.mRenderer);
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void onResume() 
	{
		super.onResume();
		
		this.setRenderMode(RENDERMODE_CONTINUOUSLY);
		
		this.queueEvent(new Runnable() 
		{
			@Override
			public void run()
			{
				CrossAppGLSurfaceView.this.mRenderer.handleOnResume();
			}
		});
	}

	@Override
	public void onPause() 
	{
		this.queueEvent(new Runnable()
		{
			@Override
			public void run()
			{
				CrossAppGLSurfaceView.this.mRenderer.handleOnPause();
			}
		});
		
		this.setRenderMode(RENDERMODE_WHEN_DIRTY);
		
		//super.onPause();
	}

	@Override
	public boolean onTouchEvent(final MotionEvent pMotionEvent)
	{
		// these data are used in ACTION_MOVE and ACTION_CANCEL
		final int pointerNumber = pMotionEvent.getPointerCount();
		final int[] ids = new int[pointerNumber];
		final float[] xs = new float[pointerNumber];
		final float[] ys = new float[pointerNumber];

		for (int i = 0; i < pointerNumber; i++)
		{
			ids[i] = pMotionEvent.getPointerId(i);
			xs[i] = pMotionEvent.getX(i);
			ys[i] = pMotionEvent.getY(i);
		}
		switch (pMotionEvent.getAction() & MotionEvent.ACTION_MASK) 
		{
			case MotionEvent.ACTION_POINTER_DOWN:
				final int indexPointerDown = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
				final int idPointerDown = pMotionEvent.getPointerId(indexPointerDown);
				final float xPointerDown = pMotionEvent.getX(indexPointerDown);
				final float yPointerDown = pMotionEvent.getY(indexPointerDown);

				this.queueEvent(new Runnable() 
				{
					@Override
					public void run() 
					{
						CrossAppGLSurfaceView.this.mRenderer.handleActionDown(idPointerDown, xPointerDown, yPointerDown);
						
					}
				});
				break;

			case MotionEvent.ACTION_DOWN:
				// there are only one finger on the screen
				final int idDown = pMotionEvent.getPointerId(0);
				final float xDown = xs[0];
				final float yDown = ys[0];

				this.queueEvent(new Runnable() 
				{
					@Override
					public void run() 
					{
						CrossAppGLSurfaceView.this.mRenderer.handleActionDown(idDown, xDown, yDown);
					}
				});
				break;

			case MotionEvent.ACTION_MOVE:
				this.queueEvent(new Runnable() 
				{
					@Override
					public void run()
					{
						CrossAppGLSurfaceView.this.mRenderer.handleActionMove(ids, xs, ys);
					}
				});
				break;

			case MotionEvent.ACTION_POINTER_UP:
				final int indexPointUp = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
				final int idPointerUp = pMotionEvent.getPointerId(indexPointUp);
				final float xPointerUp = pMotionEvent.getX(indexPointUp);
				final float yPointerUp = pMotionEvent.getY(indexPointUp);

				this.queueEvent(new Runnable() 
				{
					@Override
					public void run()
					{
						CrossAppGLSurfaceView.this.mRenderer.handleActionUp(idPointerUp, xPointerUp, yPointerUp);
					}
				});
				break;

			case MotionEvent.ACTION_UP:
				// there are only one finger on the screen
				final int idUp = pMotionEvent.getPointerId(0);
				final float xUp = xs[0];
				final float yUp = ys[0];

				this.queueEvent(new Runnable()
				{
					@Override
					public void run() 
					{
						CrossAppGLSurfaceView.this.mRenderer.handleActionUp(idUp, xUp, yUp);
					}
				});
				break;

			case MotionEvent.ACTION_CANCEL:
				this.queueEvent(new Runnable()
				{
					@Override
					public void run() 
					{
						CrossAppGLSurfaceView.this.mRenderer.handleActionCancel(ids, xs, ys);
					}
				});
				break;
		}
		
        
		return true;
	}
	/*
	 * This function is called before CrossAppRenderer.nativeInit(), so the
	 * width and height is correct.
	 */
	@Override
	protected void onSizeChanged(final int pNewSurfaceWidth, final int pNewSurfaceHeight, final int pOldSurfaceWidth, final int pOldSurfaceHeight) 
	{
		
        if(!this.isInEditMode())
        {
        	FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
 	        params.leftMargin = 0; 
 	        params.topMargin = 0;
            params.rightMargin = 0;
            params.bottomMargin = 0;
// 	    	params.width = pNewSurfaceWidth;
// 	    	params.height = pNewSurfaceHeight;
            setLayoutParams(params);
        	mRenderer.setScreenWidthAndHeight(pNewSurfaceWidth, pNewSurfaceHeight);
        	
            this.queueEvent(new Runnable() 
	    	{
	            @Override
	            public void run()
	            {
	            	CrossAppRenderer.nativeChanged(pNewSurfaceWidth, pNewSurfaceHeight);
	            }
	        }); 
        }
	}

	@Override
	public boolean onKeyDown(final int pKeyCode, final KeyEvent pKeyEvent)
	{
		switch (pKeyCode) {
			case KeyEvent.KEYCODE_BACK:
			case KeyEvent.KEYCODE_MENU:
				this.queueEvent(new Runnable()
				{
					@Override
					public void run() 
					{
						CrossAppGLSurfaceView.this.mRenderer.handleKeyDown(pKeyCode);
					}
				});
				return true;
			case KeyEvent.KEYCODE_DPAD_LEFT:
				this.queueEvent(new Runnable()
				{
					@Override
					public void run() 
					{
						CrossAppGLSurfaceView.this.mRenderer.handleKeyDown(pKeyCode);
					}
				});
				return true;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
				this.queueEvent(new Runnable()
				{
					@Override
					public void run() 
					{
						CrossAppGLSurfaceView.this.mRenderer.handleKeyDown(pKeyCode);
					}
				});
				return true;
			default:
				return super.onKeyDown(pKeyCode, pKeyEvent);
		}
	}

	// ===========================================================
	// Methods
	// ===========================================================

	private static void dumpMotionEvent(final MotionEvent event)
	{
		final String names[] = { "DOWN", "UP", "MOVE", "CANCEL", "OUTSIDE", "POINTER_DOWN", "POINTER_UP", "7?", "8?", "9?" };
		final StringBuilder sb = new StringBuilder();
		final int action = event.getAction();
		final int actionCode = action & MotionEvent.ACTION_MASK;
		sb.append("event ACTION_").append(names[actionCode]);
		
		if (actionCode == MotionEvent.ACTION_POINTER_DOWN || actionCode == MotionEvent.ACTION_POINTER_UP)
		{
			sb.append("(pid ").append(action >> MotionEvent.ACTION_POINTER_ID_SHIFT);
			sb.append(")");
		}
		sb.append("[");
		
		for (int i = 0; i < event.getPointerCount(); i++) 
		{
			sb.append("#").append(i);
			sb.append("(pid ").append(event.getPointerId(i));
			sb.append(")=").append((int) event.getX(i));
			sb.append(",").append((int) event.getY(i));
			if (i + 1 < event.getPointerCount())
			{
				sb.append(";");
			}
		}
		
		sb.append("]");
	}
}
