package com.component.impl;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.AbsoluteLayout;

import com.component.impl.VideoView.OnFinishListener;

public class VideoViewCtrl {

	private VideoView video = null;
	private static AbsoluteLayout layout = null;
	private static Activity context;
	private static VideoViewCtrl instance = null;
	private boolean finished = false;

	private Display display;

	public VideoViewCtrl() {
		// TODO Auto-generated constructor stub
	}

	public static VideoViewCtrl getInstance() {
		if (instance == null)
			instance = new VideoViewCtrl();
		return instance;
	}

	public static void init(Activity activity) {
		VideoViewCtrl.context = activity;
		layout = new AbsoluteLayout(context);

		DisplayMetrics metrics = new DisplayMetrics();
		context.getWindowManager().getDefaultDisplay().getMetrics(metrics);
		int width = metrics.widthPixels;
		int height = metrics.heightPixels;
		int margin = 0;
		if ((float) width / height > 2.0 / 3.0) {
			// 左右留边
			margin = (int) ((width - ((2.0 * height) / 3.0)) / 2.0);
			layout.setPadding(margin, 0, margin, 0);
		} else if ((float) width / height < 2.0 / 3.0) {
			// 上下留边
			margin = (int) ((height - ((3.0 * width) / 2.0)) / 2.0);
			layout.setPadding(0, margin, 0, margin);
		}

		context.addContentView(layout, new LayoutParams(
				LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
	}

	public void play(final String filename, final boolean skipable) {
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					finished = false;
					video = new VideoView(context);
					video.setSkipable(skipable);
					AssetFileDescriptor afd = context.getAssets().openFd(
							filename);
					video.setVideo(afd);

					layout.addView(video);
					video.setZOrderMediaOverlay(true);

					video.setOnFinishListener(new OnFinishListener() {
						@Override
						public void onVideoFinish() {
							layout.removeView(video);
							// context.
							finished = true;
							video = null;
						}
					});

				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	public void stop() {
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if (video != null)
						video.stop();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	public void pause() {
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if (video != null)
						video.pause();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	public void resume() {
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					if (video != null)
						video.resume();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	public void setSkipable(final boolean value) {
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				if (video != null)
					video.setSkipable(value);
			}
		});
	}

	public boolean finished() {
		if (finished) {
			finished = false;
			return true;
		}
		return false;
	}

	// 将视图居中显示
	public void centerView(final View view, final int vWidth,
			final int vHeight, final int x, final int y) {
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				try {
					LayoutParams laParams = (LayoutParams) view
							.getLayoutParams();
					AbsoluteLayout.LayoutParams newLaParams = new AbsoluteLayout.LayoutParams(
							laParams.width, laParams.height, x, y);
					view.setLayoutParams(newLaParams);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
}
