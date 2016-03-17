package org.CrossApp.lib;

import android.os.Handler;
import android.os.Looper;
import android.util.SparseArray;
import android.view.View;
import android.widget.FrameLayout;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.FutureTask;


public class CrossAppWebViewHelper {
    private static final String TAG = CrossAppWebViewHelper.class.getSimpleName();
    private static Handler handler;
    private static CrossAppActivity CrossAppActivity;
    private static FrameLayout layout;

    private static SparseArray<CrossAppWebView> webViews;
    private static int viewTag = 0;

    public CrossAppWebViewHelper(FrameLayout layout) {
        CrossAppWebViewHelper.layout = layout;
        CrossAppWebViewHelper.handler = new Handler(Looper.myLooper());

        CrossAppWebViewHelper.CrossAppActivity = (CrossAppActivity) CrossAppActivity.getContext();
        CrossAppWebViewHelper.webViews = new SparseArray<CrossAppWebView>();
    }

    private static native boolean shouldStartLoading(int index, String message);

    public static boolean _shouldStartLoading(int index, String message) {
        return !shouldStartLoading(index, message);
    }

    private static native void didFinishLoading(int index, String message);

    public static void _didFinishLoading(int index, String message) {
        didFinishLoading(index, message);
    }

    private static native void didFailLoading(int index, String message);

    public static void _didFailLoading(int index, String message) {
        didFailLoading(index, message);
    }

    private static native void onJsCallback(int index, String message);

    public static void _onJsCallback(int index, String message) {
        onJsCallback(index, message);
    }
    
    private static native void onSetByteArrayBuffer(byte[] buf, int len);
    public static native void didLoadHtmlSource(String htmlSrc);
    public static Boolean s_bWaitGetHemlSource = false;
    public static native void pause();
    public static native void resume();

    @SuppressWarnings("unused")
    public static void createWebView(final int index) {
        pause();
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = new CrossAppWebView(CrossAppActivity, index);
                FrameLayout.LayoutParams lParams = new FrameLayout.LayoutParams(
                        FrameLayout.LayoutParams.WRAP_CONTENT,
                        FrameLayout.LayoutParams.WRAP_CONTENT);
                layout.addView(webView, lParams);
                webView.setDrawingCacheEnabled(true);

                webViews.put(index, webView);
            }
        });
        resume();
    }
    
    @SuppressWarnings("unused")
    public static int createWebView() {
        final int index = viewTag;
        createWebView(index);
        return viewTag++;
    }

    @SuppressWarnings("unused")
    public static void removeWebView(final int index) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webViews.remove(index);
                    layout.removeView(webView);
                    webView.destroy();
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setVisible(final int index, final boolean visible) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setVisibility(visible ? View.VISIBLE : View.GONE);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void setWebViewRect(final int index, final int left, final int top, final int maxWidth, final int maxHeight) {
    	Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setWebViewRect(left, top, maxWidth, maxHeight);
                }
                return true;
            }
        };
        try {
            callInMainThread(callable);
        } catch (Exception e) {
        }
    }

    @SuppressWarnings("unused")
    public static void setJavascriptInterfaceScheme(final int index, final String scheme) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setJavascriptInterfaceScheme(scheme);
                }
            }
        });
    }


    @SuppressWarnings("unused")
    public static void loadHTMLString(final int index, final String htmlString, final String data, final String baseUrl) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadDataWithBaseURL(baseUrl, data, null,null,null);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void loadUrl(final int index, final String url) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadUrl(url);
                }
            }
        });
    }
    
    @SuppressWarnings("unused")
    public static void loadUrlWithRect(final int index, final String url, final String size) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
					String[] s = size.split("-");
					webView.setWebViewRect(Integer.parseInt(s[0]), Integer.parseInt(s[1]), Integer.parseInt(s[2]), Integer.parseInt(s[3]));
					webView.loadUrl(url);
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void loadFile(final int index, final String filePath) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.loadUrl(filePath);
                }
            }
        });
    }
    
    @SuppressWarnings("unused")
    public static void getWebViewImage(final int index) {
    	Callable<byte[]> callable = new Callable<byte[]>() {
            @Override
            public byte[] call() throws Exception {
                CrossAppWebView webView = webViews.get(index);
                if (webView!=null)
                {
                	return webView.getWebViewImage();
                }
				return null;
            }
        };
        try {
        	byte[] imageData = callInMainThread(callable);
        	onSetByteArrayBuffer(imageData, imageData.length);
        } catch (Exception e) {
        }
	}

    public static void stopLoading(final int index) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.stopLoading();
                }
            }
        });

    }

    public static void reload(final int index) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.reload();
                }
            }
        });
    }

    public static <T> T callInMainThread(Callable<T> call) throws ExecutionException, InterruptedException {
        FutureTask<T> task = new FutureTask<T>(call);
        handler.post(task);
        return task.get();
    }

    @SuppressWarnings("unused")
    public static boolean canGoBack(final int index) {
        Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
                CrossAppWebView webView = webViews.get(index);
                return webView != null && webView.canGoBack();
            }
        };
        try {
            return callInMainThread(callable);
        } catch (ExecutionException e) {
            return false;
        } catch (InterruptedException e) {
            return false;
        }
    }

    @SuppressWarnings("unused")
    public static boolean canGoForward(final int index) {
        Callable<Boolean> callable = new Callable<Boolean>() {
            @Override
            public Boolean call() throws Exception {
                CrossAppWebView webView = webViews.get(index);
                return webView != null && webView.canGoForward();
            }
        };
        try {
            return callInMainThread(callable);
        } catch (ExecutionException e) {
            return false;
        } catch (InterruptedException e) {
            return false;
        }
    }

    @SuppressWarnings("unused")
    public static void goBack(final int index) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.goBack();
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void goForward(final int index) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.goForward();
                }
            }
        });
    }

    @SuppressWarnings("unused")
    public static void evaluateJS(final int index, final String js) {
    	s_bWaitGetHemlSource = false;
    	CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                	s_bWaitGetHemlSource = true;
                	webView.loadUrl("javascript:"+js);
                }
            }
        });
    	
        try {
        	while (s_bWaitGetHemlSource) {
            	Thread.sleep(100);
            }
        } catch (Exception e) {
        }
	}

    @SuppressWarnings("unused")
    public static void setScalesPageToFit(final int index, final boolean scalesPageToFit) {
        CrossAppActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                CrossAppWebView webView = webViews.get(index);
                if (webView != null) {
                    webView.setScalesPageToFit(scalesPageToFit);
                }
            }
        });
    }
    
    public static String[] getAllWebviews() {

    	String [] strs = new String[1 + webViews.size() * 3];
    	
    	int index = 0;
    	strs[index++] = String.valueOf(viewTag);
    	
    	for (int i=0; i<webViews.size(); i++) {
    		CrossAppWebView webView = webViews.get(webViews.keyAt(i));
            if (webView != null) {
            	strs[index++] = String.valueOf(webView.getViewTag());
				strs[index++] = webView.getUrl();
            	strs[index++] = webView.getWebViewRectString();
            }
    	}
    	return strs;
    }
    
    public static void setAllWebviews(String[] strs) {
    	int cnt = (strs.length-1) / 3;
    	
    	viewTag = Integer.parseInt(strs[0]);
    	
    	for (int i=0; i<cnt; i++) {
    		int index = Integer.parseInt(strs[3*i+1]);
    		createWebView(index);
    		loadUrlWithRect(index, strs[3*i+2], strs[3*i+3]);
    	}
    }
}
