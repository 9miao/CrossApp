/*
 * Basic no frills app which integrates the ZBar barcode scanner with
 * the camera.
 *
 * Created by lisah0 on 2012-02-24
 */
package org.CrossApp.lib;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.os.Handler;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TextView;

import net.sourceforge.zbar.Config;
import net.sourceforge.zbar.Image;
import net.sourceforge.zbar.ImageScanner;
import net.sourceforge.zbar.Symbol;
import net.sourceforge.zbar.SymbolSet;

import org.CrossApp.lib.CrossAppActivity;
import org.CrossApp.lib.FinderView;

import java.io.IOException;
import java.io.InputStream;

/* Import ZBar Class files */

@SuppressLint("NewApi")
public class CameraView {
    private boolean isShowing;
    private CrossAppActivity activity;
    private Camera mCamera;
    private CameraPreview mPreview;
    private Handler autoFocusHandler;
    private FinderView finder_view;
    TextView scanText;
    Button scanButton;

    ImageScanner scanner;

    private boolean barcodeScanned = false;
    private boolean previewing = true;
    private View view;
    private long clickTime = 0;

    public CameraView(Context context) {

        // TODO Auto-generated constructor stub

        activity = (CrossAppActivity) context;

        autoFocusHandler = new Handler();
        mCamera = getCameraInstance();

        scanner = new ImageScanner();
        scanner.setConfig(0, Config.X_DENSITY, 3);
        scanner.setConfig(0, Config.Y_DENSITY, 3);

        mPreview = new CameraPreview(activity, mCamera, previewCb, autoFocusCB);

        view = activity.getLayoutInflater().inflate(R.layout.main, null);

        final FrameLayout preview = activity.getFrameLayout();
        mPreview.setVisibility(View.GONE);
        preview.addView(mPreview);
        preview.addView(view);
        isShowing = true;
//		mCamera.setPreviewCallback(previewCb);
//		mCamera.startPreview();
        previewing = true;
//		mCamera.autoFocus(autoFocusCB);
        // finder_view = (FinderView) view.findViewById(R.id.finder_view);
        ImageView ivBack = (ImageView) view.findViewById(R.id.iv_back);
        InputStream input = null;
        try {
            input = activity.getResources().getAssets()
                    .open("source_material/btn_left_white.png");
        } catch (IOException e) {
            e.printStackTrace();
        }// 图片
        Bitmap bm = BitmapFactory.decodeStream(input);
        ivBack.setImageBitmap(bm);
        ivBack.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View arg0) {
                back(false);
            }
        });
        clickTime = System.currentTimeMillis();
        view.setBackgroundColor(Color.parseColor("#000000"));
        TranslateAnimation animation0 = new TranslateAnimation(
                Animation.RELATIVE_TO_SELF, 0f, Animation.RELATIVE_TO_SELF, 0f,
                Animation.RELATIVE_TO_SELF, 1.0f, Animation.RELATIVE_TO_SELF,
                1f);
        animation0.setDuration(0);
        view.startAnimation(animation0);

        TranslateAnimation animation = new TranslateAnimation(
                Animation.RELATIVE_TO_SELF, 0f, Animation.RELATIVE_TO_SELF, 0f,
                Animation.RELATIVE_TO_SELF, 1.0f, Animation.RELATIVE_TO_SELF,
                0f);
        animation.setDuration(300);
        animation.setAnimationListener(new AnimationListener() {

            @Override
            public void onAnimationEnd(Animation arg0) {
                autoFocusHandler.post(new Runnable() {//需要借助handle否咋会出现最后一帧掉帧；
                    @Override
                    public void run() {
                        /**
                         * 要执行的操作
                         */
                        view.setBackgroundColor(Color.parseColor("#00000000"));
                        CrossAppActivity.getGLSurfaceView().setVisibility(
                                View.GONE);
                        mPreview.setVisibility(View.VISIBLE);
                    }
                });
            }

            @Override
            public void onAnimationRepeat(Animation arg0) {
                // TODO Auto-generated method stub

            }

            @Override
            public void onAnimationStart(Animation arg0) {
                // TODO Auto-generated method stub

            }
        });
        view.startAnimation(animation);
    }

    private static native void cameraClosed();

    public static void closed() {
        final CrossAppActivity context = CrossAppActivity.getContext();
        context.runOnGLThread(new Runnable() {
            @Override
            public void run() {
                cameraClosed();
            }
        });
    }


    public void back(boolean isSuccess) {
        if ((System.currentTimeMillis() - clickTime) < 1800&&!isSuccess) {
            return;
        }
        clickTime=System.currentTimeMillis();
        view.setBackgroundColor(Color.parseColor("#000000"));
        CrossAppActivity.getGLSurfaceView().setVisibility(View.VISIBLE);
        TranslateAnimation animation = new TranslateAnimation(
                Animation.RELATIVE_TO_SELF, 0f, Animation.RELATIVE_TO_SELF, 0f,
                Animation.RELATIVE_TO_SELF, 0f, Animation.RELATIVE_TO_SELF, 1f);
        animation.setDuration(300);
        animation.setAnimationListener(new AnimationListener() {

            @Override
            public void onAnimationEnd(Animation arg0) {
                // TODO Auto-generated method stub
                isShowing = false;
                CrossAppActivity.getFrameLayout().removeView(view);
            }

            @Override
            public void onAnimationRepeat(Animation arg0) {
                // TODO Auto-generated method stub

            }

            @Override
            public void onAnimationStart(Animation arg0) {
                // TODO Auto-generated method stub

                releaseCamera();
                CrossAppActivity.getFrameLayout().removeView(mPreview);
                final CrossAppActivity context = CrossAppActivity.getContext();
                context.runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        cameraClosed();
                    }

                });
            }
        });
        view.startAnimation(animation);
    }

    public boolean iShowing() {
        return isShowing;
    }

    public void onPause() {

        releaseCamera();
    }

    /** A safe way to get an instance of the Camera object. */
    public static Camera getCameraInstance() {
        Camera c = null;
        try {
            c = Camera.open();
            c.setDisplayOrientation(90);
        } catch (Exception e) {
        }
        return c;
    }

    private void releaseCamera() {
        if (mCamera != null) {
            previewing = false;
            mCamera.setPreviewCallback(null);
            mCamera.release();
            mCamera = null;
        }
    }

    private Runnable doAutoFocus = new Runnable() {
        public void run() {
            if (previewing)
                mCamera.autoFocus(autoFocusCB);
        }
    };

    private static native void resultScanning(String str);

    PreviewCallback previewCb = new PreviewCallback() {
        public void onPreviewFrame(byte[] data, Camera camera) {
            Parameters parameters = camera.getParameters();
            Size size = parameters.getPreviewSize();

            Image barcode = new Image(size.width, size.height, "Y800");
            barcode.setData(data);

            int result = scanner.scanImage(barcode);
            if (result != 0) {
                previewing = false;
                mCamera.setPreviewCallback(null);
                mCamera.stopPreview();

                SymbolSet syms = scanner.getResults();
                for (Symbol sym : syms) {
                    barcodeScanned = true;
                    final String str = sym.getData();
                    final CrossAppActivity context = CrossAppActivity
                            .getContext();
                    context.runOnGLThread(new Runnable() {
                        @Override
                        public void run() {
                            resultScanning(str);

                        }

                    });
                    back(true);

                }
            }
        }
    };

    // Mimic continuous auto-focusing
    AutoFocusCallback autoFocusCB = new AutoFocusCallback() {
        public void onAutoFocus(boolean success, Camera camera) {
            autoFocusHandler.postDelayed(doAutoFocus, 1000);
        }
    };
}
