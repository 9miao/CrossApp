package org.CrossApp.lib;

import java.io.ByteArrayOutputStream;
import java.lang.reflect.Method;
import java.net.URI;
import java.nio.ByteBuffer;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.net.http.SslError;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.webkit.SslErrorHandler;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;

public class CrossAppWebView extends WebView {
    private static final String TAG = CrossAppWebViewHelper.class.getSimpleName();

    private int viewTag;
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

        this.getSettings().setSupportZoom(false);
        this.getSettings().setBuiltInZoomControls(true);
        this.getSettings().setJavaScriptEnabled(true);
        this.addJavascriptInterface(new InJavaScriptLocalObj(), "local_obj");
        this.getSettings().setUseWideViewPort(true);
        
        // `searchBoxJavaBridge_` has big security risk. http://jvn.jp/en/jp/JVN53768697
        try {
            Method method = this.getClass().getMethod("removeJavascriptInterface", new Class[]{String.class});
            method.invoke(this, "searchBoxJavaBridge_");
        } catch (Exception e) {
            Log.d(TAG, "This API level do not support `removeJavascriptInterface`");
        }

        this.setWebViewClient(new CrossAppWebViewClient());
        this.setWebChromeClient(new WebChromeClient());
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
    
    public byte[] getWebViewImage() {

    	Bitmap bmp = getDrawingCache();
    	if (bmp != null)
    	{
    		ByteBuffer buf = ByteBuffer.allocate(bmp.getRowBytes() * bmp.getHeight());
    		bmp.copyPixelsToBuffer(buf);
    		return buf.array();
    	}
   		return null;
    }

    class CrossAppWebViewClient extends WebViewClient {
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, String urlString) {
            URI uri = URI.create(urlString);
            if (uri != null && uri.getScheme().equals(jsScheme)) {
                CrossAppWebViewHelper._onJsCallback(viewTag, urlString);
                return true;
            }
            return CrossAppWebViewHelper._shouldStartLoading(viewTag, urlString);
        }

        @Override
        public void onPageFinished(WebView view, String url) {
            super.onPageFinished(view, url);
            CrossAppWebViewHelper._didFinishLoading(viewTag, url);
        }

        @Override
        public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
            super.onReceivedError(view, errorCode, description, failingUrl);
            CrossAppWebViewHelper._didFailLoading(viewTag, failingUrl);
        }

	@Override
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
    public String getWebViewRectString(){
    	return szWebViewRect;
    }
    
    public int getViewTag() {
    	return viewTag;
    }

    private void fixSize(int left, int top, int width, int height) {
        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        layoutParams.gravity = Gravity.LEFT|Gravity.TOP;
        layoutParams.leftMargin = left;
        layoutParams.topMargin = top;
        layoutParams.width = width;
        layoutParams.height = height;
        this.setLayoutParams(layoutParams);
    }
}
