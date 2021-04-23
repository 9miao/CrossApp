
package org.CrossApp.lib;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;

import org.CrossApp.lib.CrossAppHelper.CrossAppHelperListener;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.ActionBar.LayoutParams;
import android.app.Activity;
import android.app.Dialog;
import android.content.ClipboardManager;
import android.content.ContentResolver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.util.Log;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.Toast;


import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.FragmentActivity;

import com.tencent.smtt.sdk.ValueCallback;
import com.tencent.smtt.sdk.WebChromeClient;
import com.tencent.smtt.sdk.WebView;

@SuppressLint("HandlerLeak")
public abstract class CrossAppActivity extends FragmentActivity implements CrossAppHelperListener {
    // ===========================================================
    // Constants
    // ===========================================================

    private static final String TAG = CrossAppActivity.class.getSimpleName();

    // ===========================================================
    // Fields
    // ===========================================================


    private static CrossAppActivity s_pActivity;

    private CrossAppGLSurfaceView mGLSurfaceView;

    private CrossAppWebViewHelper mWebViewHelper = null;

    private static CrossAppRenderer mCrossAppRenderer;

    public static Handler msHandler;

    public static Handler mLightHandler;

    private static FrameLayout frame;

    public static CameraView cameraView = null;

    public static CrossAppTextField _sTextField = null;

    public static CrossAppTextView _sTextView = null;

    public static CrossAppNativeTool nativeTool;
    public static CrossAppDevice crossAppDevice;

    public static void setSingleTextField(CrossAppTextField text) {
        _sTextField = text;
    }

    public static void setSingleTextView(CrossAppTextView text) {
        _sTextView = text;
    }

    public static CrossAppActivity getContext() {
        return CrossAppActivity.s_pActivity;
    }

    public static CrossAppGLSurfaceView getGLSurfaceView() {
        return s_pActivity.mGLSurfaceView;
    }

    public static FrameLayout getFrameLayout() {
        return CrossAppActivity.frame;
    }

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        s_pActivity = this;

        CrossAppVolumeControl.setContext(s_pActivity);

        CrossAppPersonList.Init(this);

        CrossAppHelper.init(this, this);

        CrossAppNetWorkManager.setContext(this);

        CrossAppAlertView.checkAliveAlertView();

        crossAppDevice = new CrossAppDevice(this) {
        };

        nativeTool = new CrossAppNativeTool(this);

        this.init();

        this.exeHandler();

        if (savedInstanceState == null) {
            mWebViewHelper = new CrossAppWebViewHelper(frame);

            CrossAppTextField.initWithHandler();
            CrossAppTextView.initWithHandler();
        } else if (savedInstanceState != null) {
            CrossAppTextField.reload();

            CrossAppTextView.reload();

            if (savedInstanceState.containsKey("WEBVIEW")) {
                mWebViewHelper = new CrossAppWebViewHelper(frame);

                String[] strs = savedInstanceState.getStringArray("WEBVIEW");

                mWebViewHelper.setAllWebviews(strs);
            }
            savedInstanceState.clear();
        }

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (crossAppDevice != null && crossAppDevice.getCrossAppBattery() != null) {
            CrossAppBattery battery = crossAppDevice.getCrossAppBattery();
            this.unregisterReceiver(battery);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();

        if (_sTextField != null) {
            _sTextField.resume();
        }

        if (_sTextView != null) {
            _sTextView.resume();
        }

        CrossAppHelper.onResume();
        this.mGLSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();

        CrossAppHelper.onPause();
        this.mGLSurfaceView.onPause();

        if (cameraView != null) {
            cameraView.back(false);
        }
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {

        outState.putStringArray("WEBVIEW", mWebViewHelper.getAllWebviews());
        super.onSaveInstanceState(outState);
    }

    @Override
    public boolean onKeyDown(final int pKeyCode, final KeyEvent pKeyEvent) {
        if ((pKeyCode == KeyEvent.KEYCODE_BACK && cameraView != null && cameraView.iShowing())) {
            cameraView.back(false);
            return false;
        } else {
            return CrossAppGLSurfaceView.getInstance().onKeyDown(pKeyCode, pKeyEvent);
        }
    }

    public void setPasteBoardStr(String sender) {
        Message msg = new Message();
        msg.obj = sender;
        msg.what = 0;
        msHandler.sendMessage(msg);
    }

    public String getPasteBoardStr() {
        Callable<String> callable = new Callable<String>() {
            @SuppressLint("NewApi")
            @Override
            public String call() throws Exception {
                ClipboardManager clipboard = (ClipboardManager) s_pActivity.getSystemService(Context.CLIPBOARD_SERVICE);

                clipboard.getText();

                return clipboard.getText().toString();
            }
        };
        try {
            return CrossAppWebViewHelper.callInMainThread(callable);
        } catch (Exception e) {
        }
        return "";
    }

    private void toast(String str) {
        System.out.println(str);

        Toast.makeText(CrossAppActivity.this, str, Toast.LENGTH_SHORT).show();
    }

    public void onActivityResult(int requestCode, int resultCode, Intent intent) {
        super.onActivityResult(requestCode, resultCode, intent);
        nativeTool.onActivityResult(requestCode, resultCode, intent);
    }

    @TargetApi(11)
    private void exeHandler() {
        if (mLightHandler == null) {
            mLightHandler = new Handler() {

                @Override
                public void handleMessage(Message msg) {
                    int value = msg.what;
                    WindowManager.LayoutParams lp = s_pActivity.getWindow().getAttributes();
                    lp.screenBrightness = value / 255.0f;
                    s_pActivity.getWindow().setAttributes(lp);

                    ContentResolver resolver = s_pActivity.getContentResolver();
                    saveBrightness(resolver, value);
                }
            };
        }
        if (msHandler == null) {
            msHandler = new Handler() {

                @Override
                public void handleMessage(Message msg) {
                    String value = (String) msg.obj;
                    int what = msg.what;
                    ClipboardManager cmb = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
                    if (what == 0) {

                        cmb.setText(value);
                    }
                }
            };
        }
    }

    public static void saveBrightness(ContentResolver resolver, int brightness) {
        Uri uri = android.provider.Settings.System.getUriFor("screen_brightness");
        android.provider.Settings.System.putInt(resolver, "screen_brightness", brightness);
        resolver.notifyChange(uri, null);
    }

    @Override
    public void runOnGLThread(final Runnable pRunnable) {
        this.mGLSurfaceView.queueEvent(pRunnable);
    }

    // ===========================================================
    // Methods
    // ===========================================================
    @SuppressWarnings("deprecation")
    public void init() {
        FrameLayout framelayout = new FrameLayout(this);
        framelayout.layout(0, 0, 0, 0);

        this.frame = framelayout;
        this.mGLSurfaceView = this.onCreateView();

        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
        params.leftMargin = 0;
        params.rightMargin = 0;
        params.topMargin = 0;
        params.bottomMargin = 0;

        framelayout.addView(this.mGLSurfaceView, params);

        if (isAndroidEmulator()) {
            this.mGLSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        }

        this.mCrossAppRenderer = new CrossAppRenderer();

        this.mGLSurfaceView.setCrossAppRenderer(mCrossAppRenderer);

        this.setContentView(framelayout);

    }

    public static int dip2px(Context context, float dpValue) {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (dpValue * scale + 0.5f);
    }

    public static int px2dip(Context context, float pxValue) {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (pxValue / scale + 0.5f);
    }


    public CrossAppGLSurfaceView onCreateView() {
        return new CrossAppGLSurfaceView(this);
    }

    public int getStatusBarHeight() {
        WindowManager.LayoutParams attrs = getWindow().getAttributes();
        if ((attrs.flags & WindowManager.LayoutParams.FLAG_FULLSCREEN) == WindowManager.LayoutParams.FLAG_FULLSCREEN) {
            return 0;
        }
        int result = 0;
        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            result = getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }


    private final static boolean isAndroidEmulator() {

        String product = Build.PRODUCT;

        boolean isEmulator = false;

        if (product != null) {
            isEmulator = product.equals("sdk") || product.contains("_sdk") || product.contains("sdk_");
        }

        Log.d(TAG, "isEmulator=" + isEmulator);

        return isEmulator;
    }


    public CrossAppWebView.OnValueCallbackListenner getOnValueCallbackListenner() {
        return onValueCallbackListenner;
    }

    ;
    private ValueCallback valueCallback;
    private ValueCallback<Uri> valueCallbackUri;
    private ValueCallback<Uri[]> valueCallbackUriArry;

    public ValueCallback getValueCallback() {
        return valueCallback;
    }

    public void setValueCallback(ValueCallback valueCallback) {
        this.valueCallback = valueCallback;
    }

    public ValueCallback<Uri> getValueCallbackUri() {
        return valueCallbackUri;
    }

    public void setValueCallbackUri(ValueCallback<Uri> valueCallback) {
        this.valueCallbackUri = valueCallback;
    }

    public ValueCallback<Uri[]> getValueCallbackUriArry() {
        return valueCallbackUriArry;
    }

    public void setValueCallbackUriArry(ValueCallback<Uri[]> valueCallback) {
        this.valueCallbackUriArry = valueCallback;
    }

    private CrossAppWebView.OnValueCallbackListenner onValueCallbackListenner = new CrossAppWebView.OnValueCallbackListenner() {
        @Override
        public void OnValueCallback(ValueCallback<Uri> valueCallback) {
            Log.d("ceshi", "回调1");
            showBottomDialog();
            setValueCallback(valueCallback);
        }

        @Override
        public void OnValueCallback(ValueCallback valueCallback, String acceptType) {
            Log.d("ceshi", "回调2");
            showBottomDialog();
            setValueCallbackUri(valueCallback);

        }

        @Override
        public void OnValueCallback(ValueCallback<Uri> valueCallback, String acceptType, String capture) {
            Log.d("ceshi", "回调3");
            showBottomDialog();
            setValueCallbackUri(valueCallback);
        }

        @Override
        public void OnValueCallback(WebView webView, ValueCallback<Uri[]> filePathCallback, WebChromeClient.FileChooserParams fileChooserParams) {
            Log.d("ceshi", "回调4");
            showBottomDialog();
            setValueCallbackUriArry(filePathCallback);
        }
    };


    boolean isvalid = false;

    private void showBottomDialog() {

        isvalid = false;

        final Dialog dialog = new Dialog(CrossAppActivity.getContext(), R.style.ActionSheet);
        LayoutInflater inflater = (LayoutInflater) CrossAppActivity.getContext()
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        View mDlgCallView = inflater.inflate(R.layout.dlg_actionsheet, null);
        final int cFullFillWidth = 10000;
        mDlgCallView.setMinimumWidth(cFullFillWidth);
        TextView tv_camera_txt = (TextView) mDlgCallView
                .findViewById(R.id.tv_camera_txt);
        TextView tv_album_txt = (TextView) mDlgCallView
                .findViewById(R.id.tv_album_txt);

        TextView cancel_txt = (TextView) mDlgCallView
                .findViewById(R.id.cancel_txt);

        tv_camera_txt.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                isvalid = true;
                nativeTool.CAImageCapture(3);
                dialog.dismiss();
            }
        });
        tv_album_txt.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                isvalid = true;
                nativeTool.CAWebViewImageAlbum(5);
                dialog.dismiss();
            }
        });

        dialog.setOnDismissListener(new DialogInterface.OnDismissListener() {
            @Override
            public void onDismiss(DialogInterface dialogInterface) {
                if (!isvalid) {
                    cancelCallback();
                }
                Log.d("ceshi", "xiaoshi");
            }
        });

        cancel_txt.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Log.d("ceshi", "点击取消");
                dialog.dismiss();
            }
        });

        Window w = dialog.getWindow();
        WindowManager.LayoutParams lp = w.getAttributes();
        lp.x = 0;

        final int cMakeBottom = -1000;
        lp.y = cMakeBottom;
        lp.gravity = Gravity.BOTTOM;
        dialog.onWindowAttributesChanged(lp);
        dialog.setCanceledOnTouchOutside(true);
        dialog.setCancelable(true);
        dialog.setContentView(mDlgCallView);
        dialog.show();
    }

    public void cancelCallback() {
        if (getValueCallback() != null) {
            getValueCallback().onReceiveValue(null);
        }
        if (getValueCallbackUri() != null) {
            getValueCallbackUri().onReceiveValue(null);
        }
        if (getValueCallbackUriArry() != null) {
            getValueCallbackUriArry().onReceiveValue(null);
        }
        if (getValueNativeCallback() != null) {
            getValueNativeCallback().onReceiveValue(null);
        }
        if (getValueNativeCallbackUri() != null) {
            getValueNativeCallbackUri().onReceiveValue(null);
        }
        if (getValueNativeCallbackUriArry() != null) {
            getValueNativeCallbackUriArry().onReceiveValue(null);
        }
        setValueCallback(null);
        setValueCallbackUri(null);
        setValueCallbackUriArry(null);
        setValueNativeCallback(null);
        setValueNativeCallbackUri(null);
        setValueNativeCallbackUriArry(null);
    }

    // 二维码相关
    public static void showQRCodeView() {

        final CrossAppActivity context = CrossAppActivity.getContext();
        context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                ArrayList<String> types = new ArrayList<String>();
                types.add("相机");
                CAPermissionsManager.request(types, new CAPermissionsManager.CAPermissionsManagerCallBack() {
                    @Override
                    public void onCallBack(Boolean isSucess) {
                        if (isSucess) {
                            cameraView = new CameraView(context);
                        }
                    }
                });
            }
        });
    }

    public CrossAppWebViewNative.OnValueCallbackListenner getOnValueNativeCallbackListenner() {
        return onNativeValueCallbackListenner;
    }

    ;
    private android.webkit.ValueCallback valueNativeCallback;
    private android.webkit.ValueCallback<Uri> valueNativeCallbackUri;
    private android.webkit.ValueCallback<Uri[]> valueNativeCallbackUriArry;

    public android.webkit.ValueCallback getValueNativeCallback() {
        return valueNativeCallback;
    }

    public void setValueNativeCallback(android.webkit.ValueCallback valueCallback) {
        this.valueNativeCallback = valueCallback;
    }

    public android.webkit.ValueCallback<Uri> getValueNativeCallbackUri() {
        return valueNativeCallbackUri;
    }

    public void setValueNativeCallbackUri(android.webkit.ValueCallback<Uri> valueCallback) {
        this.valueNativeCallbackUri = valueCallback;
    }

    public android.webkit.ValueCallback<Uri[]> getValueNativeCallbackUriArry() {
        return valueNativeCallbackUriArry;
    }

    public void setValueNativeCallbackUriArry(android.webkit.ValueCallback<Uri[]> valueCallback) {
        this.valueNativeCallbackUriArry = valueCallback;
    }

    private CrossAppWebViewNative.OnValueCallbackListenner onNativeValueCallbackListenner = new CrossAppWebViewNative.OnValueCallbackListenner() {
        @Override
        public void OnValueCallback(android.webkit.ValueCallback<Uri> valueCallback) {
            Log.d("ceshi", "回调1");
            showBottomDialog();
            setValueNativeCallback(valueCallback);
        }

        @Override
        public void OnValueCallback(android.webkit.ValueCallback valueCallback, String acceptType) {
            Log.d("ceshi", "回调2");
            showBottomDialog();
            setValueNativeCallbackUri(valueCallback);

        }

        @Override
        public void OnValueCallback(android.webkit.ValueCallback<Uri> valueCallback, String acceptType, String capture) {
            Log.d("ceshi", "回调3");
            showBottomDialog();
            setValueNativeCallbackUri(valueCallback);
        }

        @Override
        public void OnValueCallback(android.webkit.WebView webView, android.webkit.ValueCallback<Uri[]> filePathCallback, android.webkit.WebChromeClient.FileChooserParams fileChooserParams) {
            Log.d("ceshi", "回调4");
            showBottomDialog();
            setValueNativeCallbackUriArry(filePathCallback);
        }
    };
}
