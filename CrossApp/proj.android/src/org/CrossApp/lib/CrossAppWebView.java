package org.CrossApp.lib;

import java.io.ByteArrayOutputStream;
import java.lang.reflect.Method;
import java.net.URI;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.net.Uri;
import android.net.http.SslError;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
//import android.webkit.SslErrorHandler;
//import android.webkit.WebChromeClient;
//import android.webkit.WebView;
//import android.webkit.WebViewClient;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.tencent.smtt.export.external.interfaces.IX5WebChromeClient;
import com.tencent.smtt.export.external.interfaces.SslErrorHandler;
import com.tencent.smtt.sdk.ValueCallback;
import com.tencent.smtt.sdk.WebChromeClient;
import com.tencent.smtt.sdk.WebSettings.LayoutAlgorithm;
import com.tencent.smtt.sdk.WebView;
import com.tencent.smtt.sdk.WebViewClient;

public class CrossAppWebView extends WebView {
    private static final String TAG = CrossAppWebViewHelper.class.getSimpleName();

    private int viewTag;
    private View videoView;
    private IX5WebChromeClient.CustomViewCallback videoCallback;
    private String jsScheme;
    private String szWebViewRect;

    public CrossAppWebView(Context context) {
        this(context, -1);


    }

    @SuppressLint("SetJavaScriptEnabled")
    public CrossAppWebView(Context context, int viewTag) {
        super(context);
        this.viewTag = viewTag;
        this.jsScheme = "";
        this.szWebViewRect = "0-0-0-0";

        this.setFocusable(true);
        this.setFocusableInTouchMode(true);

        this.getSettings().setSupportZoom(true);
        this.getSettings().setBuiltInZoomControls(true);
        this.getSettings().setJavaScriptEnabled(true);
        this.addJavascriptInterface(new InJavaScriptLocalObj(), "local_obj");
        this.getSettings().setUseWideViewPort(true);
        this.getSettings().setDomStorageEnabled(true);
        this.getSettings().setLayoutAlgorithm(LayoutAlgorithm.SINGLE_COLUMN);
        this.getSettings().setLoadWithOverviewMode(true);

        /**
         * x5视频 有问题 暂不能使用
         * 请使用原生  createWebView(index, 0);//0==默认非原生webview（为x5 ）；1==原生webview
         */
//        Bundle data = new Bundle();
//        data.putBoolean("standardFullScreen", true);
////true表示标准全屏，false表示X5全屏；不设置默认false，
////        data.putBoolean("supportLiteWnd", false);
//////false：关闭小窗；true：开启小窗；不设置默认true，
////        data.putInt("DefaultVideoScreen", 2);
//////1：以页面内开始播放，2：以全屏开始播放；不设置默认：1
//        getX5WebViewExtension().invokeMiscMethod("setVideoParams", data);


        // `searchBoxJavaBridge_` has big security risk. http://jvn.jp/en/jp/JVN53768697
        try {
            Method method = this.getClass().getMethod("removeJavascriptInterface", new Class[]{String.class});
            method.invoke(this, "searchBoxJavaBridge_");
        } catch (Exception e) {
            Log.d(TAG, "This API level do not support `removeJavascriptInterface`");
        }

        this.setWebViewClient(new CrossAppWebViewClient());
        this.setWebChromeClient(new MyWebChromeClient());
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            return CrossAppGLSurfaceView.getInstance().onKeyDown(keyCode, event);
        }
        return super.onKeyDown(keyCode, event);
    }

    public void setJavascriptInterfaceScheme(String scheme) {
        this.jsScheme = scheme != null ? scheme : "";
    }

    public void setScalesPageToFit(boolean scalesPageToFit) {
        this.getSettings().setSupportZoom(scalesPageToFit);
    }

    private Bitmap bmp = null;
    private ByteBuffer imageData = null;

    private static native void onSetByteArrayBuffer(int index, byte[] buf, int wdith, int height);

    public void getWebViewImage() {
        bmp = this.getDrawingCache();
        if (bmp != null && imageData == null) {
            imageData = ByteBuffer.allocate(bmp.getRowBytes() * bmp.getHeight());
            bmp.copyPixelsToBuffer(imageData);

            CrossAppActivity.getContext().runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    onSetByteArrayBuffer(viewTag, imageData.array(), bmp.getWidth(), bmp.getHeight());
                    imageData = null;
                }
            });
        }
    }

    class CrossAppWebViewClient extends WebViewClient {
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, final String urlString) {
            if (urlString.startsWith("weixin://wap/pay?")) {
                try {
                    Intent intent = new Intent();
                    intent.setAction(Intent.ACTION_VIEW);
                    intent.setData(Uri.parse(urlString));
                    CrossAppActivity.getContext().startActivity(intent);
                } catch (Exception exception) {
                    Toast.makeText(CrossAppActivity.getContext(), "支付失败,请重试", Toast.LENGTH_SHORT).show();
                }
                CrossAppActivity.getContext().runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        CrossAppWebViewHelper._onJsCallback(viewTag, urlString);
                    }
                });
                return true;
            } else if (urlString.contains("platformapi/startapp")) {
                try {
                    Intent intent = new Intent();
                    intent.setAction(Intent.ACTION_VIEW);
                    intent.setData(Uri.parse(urlString));
                    intent.addCategory("android.intent.category.BROWSABLE");
                    intent.setComponent(null);
                    CrossAppActivity.getContext().startActivity(intent);
                } catch (Exception exception) {
                    Toast.makeText(CrossAppActivity.getContext(), "支付失败,请重试", Toast.LENGTH_SHORT).show();
                }
                CrossAppActivity.getContext().runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        CrossAppWebViewHelper._onJsCallback(viewTag, urlString);
                    }
                });
                return true;
            }

            URI uri = URI.create(urlString);
            if (uri != null && uri.getScheme().equals(jsScheme)) {
                CrossAppActivity.getContext().runOnGLThread(new Runnable() {
                    @Override
                    public void run() {
                        CrossAppWebViewHelper._onJsCallback(viewTag, urlString);
                    }
                });
                return true;
            }
            return CrossAppWebViewHelper._shouldStartLoading(viewTag, urlString);
        }

        @Override
        public void onPageFinished(WebView view, final String url) {
            super.onPageFinished(view, url);
            CrossAppActivity.getContext().runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    CrossAppWebViewHelper._didFinishLoading(viewTag, url);
                }
            });
        }

        @Override
        public void onReceivedError(WebView view, int errorCode, String description, final String failingUrl) {
            super.onReceivedError(view, errorCode, description, failingUrl);
            CrossAppActivity.getContext().runOnGLThread(new Runnable() {
                @Override
                public void run() {
                    CrossAppWebViewHelper._didFailLoading(viewTag, failingUrl);
                }
            });

        }

        //        @Override
        public void onReceivedSslError(WebView view, SslErrorHandler handler, SslError error) {
            handler.proceed();
        }
    }

    final class InJavaScriptLocalObj {
        public void showSource(String html) {
            CrossAppWebViewHelper.didLoadHtmlSource(html);
            CrossAppWebViewHelper.s_bWaitGetHemlSource = false;
        }
    }

    public void setWebViewRect(int left, int top, int maxWidth, int maxHeight) {
        fixSize(left, top, maxWidth, maxHeight);
        this.szWebViewRect = String.format("%d-%d-%d-%d", left, top, maxWidth, maxHeight);
    }

    public String getWebViewRectString() {
        return szWebViewRect;
    }

    public int getViewTag() {
        return viewTag;
    }

    private void fixSize(int left, int top, int width, int height) {
        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT);
        layoutParams.gravity = Gravity.LEFT | Gravity.TOP;
        layoutParams.leftMargin = left;
        layoutParams.topMargin = top;
        layoutParams.width = width;
        layoutParams.height = height;
        this.setLayoutParams(layoutParams);
    }


    public class MyWebChromeClient extends WebChromeClient {

        // Android 3.0 以下
        public void openFileChooser(ValueCallback<Uri> valueCallback) {
            CrossAppActivity.getContext().getOnValueCallbackListenner().OnValueCallback(valueCallback);
        }

        // Android 3~4.1
        public void openFileChooser(ValueCallback valueCallback, String acceptType) {
            CrossAppActivity.getContext().getOnValueCallbackListenner().OnValueCallback(valueCallback, acceptType);
        }

        // Android  4.1以上
        public void openFileChooser(ValueCallback<Uri> valueCallback, String acceptType, String capture) {
            CrossAppActivity.getContext().getOnValueCallbackListenner().OnValueCallback(valueCallback, acceptType, capture);
        }

        // Android 5.0以上
        @Override
        public boolean onShowFileChooser(WebView webView, ValueCallback<Uri[]> filePathCallback, WebChromeClient.FileChooserParams fileChooserParams) {
            CrossAppActivity.getContext().getOnValueCallbackListenner().OnValueCallback(webView, filePathCallback, fileChooserParams);
            return true;
        }

        @Override
        public void onShowCustomView(View view, int i, IX5WebChromeClient.CustomViewCallback customViewCallback) {
            super.onShowCustomView(view, i, customViewCallback);
            Log.d("cross----", "展示2");
        }

        @Override
        public void onShowCustomView(final View view, final IX5WebChromeClient.CustomViewCallback customViewCallback) {
            super.onShowCustomView(view, customViewCallback);

            CrossAppActivity.getContext().runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    videoCallback = customViewCallback;
                    if (videoView != null) {
                        Log.d("cross----", "videoView != null");
                        videoCallback.onCustomViewHidden();
                        return;
                    }
                    view.setTag("videoView");
                    videoView = view;
                    setVisibility(View.GONE);
                    videoView.setVisibility(VISIBLE);
                    CrossAppActivity.getFrameLayout().addView(view);

                    videoView.bringToFront();
                    // 横屏显示
                    CrossAppActivity.getContext().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
                    //设置全屏
                    CrossAppActivity.getContext().getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                            WindowManager.LayoutParams.FLAG_FULLSCREEN);
                    Log.d("cross----", "展示1");

                }
            });
        }

        @Override
        public void onHideCustomView() {
            super.onHideCustomView();
            CrossAppActivity.getContext().runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (videoView == null) {
                        Log.d("cross----", "videoView == null");
                        return;
                    }

                    setVisibility(VISIBLE);
                    videoView.setVisibility(GONE);
                    CrossAppActivity.getFrameLayout().removeView(videoView);
                    videoView = null;

                    try {
                        videoCallback.onCustomViewHidden();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    //换成竖屏
                    CrossAppActivity.getContext().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
                    Log.d("cross----", "隐藏2");
                }
            });
        }
    }

    public interface OnValueCallbackListenner {
        void OnValueCallback(ValueCallback<Uri> valueCallback);

        void OnValueCallback(ValueCallback valueCallback, String acceptType);

        void OnValueCallback(ValueCallback<Uri> valueCallback, String acceptType, String capture);

        void OnValueCallback(WebView webView, ValueCallback<Uri[]> filePathCallback, WebChromeClient.FileChooserParams fileChooserParams);

    }


}
