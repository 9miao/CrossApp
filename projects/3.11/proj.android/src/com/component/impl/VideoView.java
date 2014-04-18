package com.component.impl;

import java.io.IOException;


import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.net.Uri;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

public class VideoView extends SurfaceView implements SurfaceHolder.Callback,
		View.OnTouchListener, MediaPlayer.OnPreparedListener,
		MediaPlayer.OnErrorListener, MediaPlayer.OnInfoListener,
		MediaPlayer.OnCompletionListener {
	private static final String TAG = "VideoView";

	private MediaPlayer mPlayer;
	private Activity gameActivity;
	private Uri resUri;
	private AssetFileDescriptor fd;
	private boolean surfaceCreated;
	private OnFinishListener onFinishListener;
	private int posttion;

	private boolean skipable;

	@SuppressWarnings("deprecation")
	public VideoView(Activity context) {
		super(context);

		this.gameActivity = context;

		final SurfaceHolder holder = getHolder();
		holder.addCallback(this);
		holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		setOnTouchListener(this);

		mPlayer = new MediaPlayer();
		// mPlayer.setDisplay(getHolder());
		mPlayer.setScreenOnWhilePlaying(true);

		mPlayer.setOnPreparedListener(this);
		mPlayer.setOnCompletionListener(this);
		mPlayer.setOnErrorListener(this);
		mPlayer.setOnInfoListener(this);
	}

	public VideoView setOnFinishListener(OnFinishListener onFinishListener) {
		this.onFinishListener = onFinishListener;

		return this;
	}

	public void setVideo(Uri resUri) {
		try {
			this.resUri = resUri;
			mPlayer.setDataSource(gameActivity, resUri);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void setVideo(AssetFileDescriptor fd) {
		try {
			this.fd = fd;
			mPlayer.setDataSource(fd.getFileDescriptor(), fd.getStartOffset(),
					fd.getLength());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
	}

	@Override
	public void surfaceCreated(final SurfaceHolder holder) {
		try {
			Log.i(TAG, "surfaceCreated");
			surfaceCreated = true;
			mPlayer.setDisplay(holder);
//			mPlayer.prepare();
			mPlayer.prepareAsync();
		} catch (Exception e) {
		}
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		Log.i(TAG, "surfaceDestroyed");
		surfaceCreated = false;
		if (mPlayer != null) {
			mPlayer.stop();
			mPlayer.reset();
		}
	}

	@Override
	public void onPrepared(MediaPlayer player) {
		Log.i(TAG, "onPrepared");

		int wWidth = getWidth();
		int wHeight = getHeight();

		// 强制屏幕比率
		int vWidth = 640;// mPlayer.getVideoWidth();
		int vHeight = 960;// mPlayer.getVideoHeight();

		float wRatio = (float) vWidth / (float) wWidth;
		float hRatio = (float) vHeight / (float) wHeight;
		float ratio = Math.max(wRatio, hRatio);
		vWidth = (int) Math.ceil((float) vWidth / ratio);
		vHeight = (int) Math.ceil((float) vHeight / ratio);

		getHolder().setFixedSize(vWidth, vHeight);

		Log.d("vHeight:" + Integer.toString(vHeight),
				"vWidth:" + Integer.toString(vWidth));

		VideoViewCtrl.getInstance().centerView(this, vWidth, vHeight, 0, 0);

		mPlayer.seekTo(posttion);
		mPlayer.start();
	}

	private void dispose() {
		mPlayer.release();
		mPlayer = null;
		resUri = null;
		if (fd != null) {
			try {
				fd.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
			fd = null;
		}
	}

	@Override
	public void onCompletion(MediaPlayer mp) {
		Log.i(TAG, "onCompletion");

		dispose();

		if (onFinishListener != null)
			onFinishListener.onVideoFinish();
	}

	@Override
	public boolean onInfo(MediaPlayer mp, int what, int extra) {
		return true;
	}

	@Override
	public boolean onError(MediaPlayer mp, int what, int extra) {
		return false;
	}

	@Override
	public boolean onTouch(View v, MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			if (skipable)
				stop();
		}
		return true;
	}

	public void stop() {
		mPlayer.stop();
		posttion = 0;
		dispose();
		if (onFinishListener != null)
			onFinishListener.onVideoFinish();
	}

	public void pause() {
		mPlayer.pause();
		posttion = mPlayer.getCurrentPosition();
	}

	public void resume() {
		if (surfaceCreated) {
			mPlayer.seekTo(posttion);
			mPlayer.start();
		} else {
			try {
				if (resUri != null){
					mPlayer.setDataSource(gameActivity, resUri);
					mPlayer.seekTo(posttion);
					}
				else if (fd != null) {
					mPlayer.setDataSource(fd.getFileDescriptor(),
							fd.getStartOffset(), fd.getLength());
					mPlayer.seekTo(posttion);
				}
			} catch (Exception e) {
			}
		}
	}

	public void setSkipable(boolean value) {
		this.skipable = value;
	}

	public interface OnFinishListener {
		public void onVideoFinish();
	}
}
