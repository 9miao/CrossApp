package org.CrossApp.lib;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetFileDescriptor;
import android.graphics.Bitmap;
import android.graphics.SurfaceTexture;
import android.media.MediaMetadataRetriever;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnSeekCompleteListener;
import android.net.Uri;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Surface;
import android.view.TextureView;
import android.widget.FrameLayout;
import android.widget.FrameLayout.LayoutParams;

@TargetApi(14)
public class CrossAppVideoPlayer extends TextureView implements TextureView.SurfaceTextureListener{
	
	//缓冲为空，正在缓冲
	public static final int PlaybackBufferEmpty = 0 ;
	//缓冲达到可播放
	public static final int PlaybackLikelyToKeepUp = 1 ;
	
	//暂停
	public static final int PlayStatePause = 0 ;
	//播放中
	public static final int PlayStatePlaying = 1 ;
	//回放中
	public static final int PlayStatePlayback = 2 ;
	
	public static Map<Integer, CrossAppVideoPlayer> players = new HashMap<Integer, CrossAppVideoPlayer>() ; 
	
	public static CrossAppVideoPlayer create(int key){
		
		final CrossAppVideoPlayer player = new CrossAppVideoPlayer(CrossAppActivity.getContext()) ;
		
		CrossAppActivity.getContext().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				player.setAlpha(0);
				LayoutParams p = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT) ; 
				CrossAppActivity.getFrameLayout().addView(player, p);
			}
		});
		return player ; 
	}
	
	public static void setUrl(String path , int key){
		CrossAppVideoPlayer  p = getPlayerByKey(key);
		p.setIsLocalVideo(false);
		p.setUrl(path);
	}
	
	public static void setFilePath(String path , int key){
		
		final CrossAppVideoPlayer  p = getPlayerByKey(key);
		if (!path.startsWith("/")) {
			p.setIsAssert(true);
		}
		
		p.setIsLocalVideo(true);
		p.setUrl(path);
		
		p.sendFirsttFrame();  
	}
	
	/**
	 * 播放
	 */
	public static void play4native(final  int key){
		
		if(players.size()>0){
			Set<Integer> keys = players.keySet() ; 
			Iterator<Integer> it = keys.iterator() ; 
			while (it.hasNext()) {
				Integer integer = (Integer) it.next();				
				if (integer != key && players.get(integer).getMediaPlayer().isPlaying()) {
					players.get(integer).getMediaPlayer().pause();
					players.get(integer).setVideoState(VideoState.pause);
				}
			}
		}
		
		final CrossAppVideoPlayer player = getPlayerByKey(key) ; 
		
		player.onSetStart(); 
	}
	
	/**
	 * 停止
	 */
	public static void pause4native(int key){
		getPlayerByKey(key).pause();  
	}
	
	/**
	 * 得到时长
	 * @return
	 */
	public static int getDuration(int key){
		return getPlayerByKey(key).getMediaPlayer().getDuration() ; 
	}
	
	/**
	 * 获取当前播放的点
	 * @return
	 */
	public static int  getCurrentTime(int key){
		return getPlayerByKey(key).getMediaPlayer().getCurrentPosition() ; 
	}
	
	public static void stop4native(int key){
		CrossAppVideoPlayer player = getPlayerByKey(key) ;
		player.stop();
	}
	
	public synchronized static void setCurrentTime4native(final float current,final int key){
		
		final CrossAppVideoPlayer player = getPlayerByKey(key) ;
		player.block_progress_update(true);
		
		
		CrossAppActivity.getContext().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				player.getMediaPlayer().seekTo((int)current);
			}
		});
	}
	
	public static int[] getPresentationSize4native(int key){
		CrossAppVideoPlayer player = getPlayerByKey(key) ;
		int[] size = {player.getMediaPlayer().getVideoWidth() , player.getMediaPlayer().getVideoHeight()};
		return size ; 
	}
	
	public synchronized static CrossAppVideoPlayer getPlayerByKey(int key){
		CrossAppVideoPlayer player = players.get(key) ; 
		return player ;
	}
	
	/** 删除Map中的Player */
	public static void removeById(final int key){
		
		CrossAppActivity.getContext().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				CrossAppVideoPlayer player = getPlayerByKey(key) ;
				CrossAppActivity.getFrameLayout().removeView(player);
				players.remove(key) ; 
				player.destoryMediaPlayer();
				player = null ; 
			}
		});
	}
	
	/**
	 * 显示一个界面
	 * @param url
	 */
	public static void showAVPlayerController(String url, int local){
		String loadString = url ; 
		if (local>0) {
			loadString = "file:///android_asset/" + url;
		}
		Uri uri = Uri.parse(loadString);     
		//调用系统自带的播放器
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setDataAndType(uri, "video/*");  
		CrossAppActivity.getContext().startActivity(intent);
	}
	
	
	public static void addVideoByKey(int key){
		CrossAppVideoPlayer p = CrossAppVideoPlayer.create(key) ; 
		p.setKey(key);
		players.put(key, p) ; 
	}
	
	
	public static  native void onFrameAttached(int key , byte[] bytes, int width , int height) ; 
	
	public static  native void onFrameImageAttached(int key , byte[] bytes, int length) ; 
	
	public static native void onPeriodicTime(int key , float current, float duratuon) ; // 监听播放进度
	
	public static native void onLoadedTime(int key , float current, float duratuon) ; // 监听缓冲进度
	
	public static native void onDidPlayToEndTime(int key);// 监听播放完毕
	
	public static native void onTimeJumped(int key);// 监听快进或者慢进或者跳过某段播放
	
	public static native void onPlayBufferLoadingState(int key, int state) ; // 监听缓冲状态
	
	public static native void onPlayState(int key , int state) ; // 监听播放状态
	
	/**  
     * 把Bitmap转Byte  
     */    
    public static byte[] bitmap2Bytes(Bitmap bm){    
    	
    	if(bm== null) return new byte[0];
    	
    	ByteBuffer imageData = ByteBuffer.allocate(bm.getRowBytes() * bm.getHeight());
    	bm.copyPixelsToBuffer(imageData);
    	
    	return imageData.array() ; 
    }
    
	/*********************************************************************************************************************************/
	/**																			子类实现																								  ****/
	/*********************************************************************************************************************************/
	
    private String url;
    
    public VideoState mState;
    
    private MediaPlayer mMediaPlayer;
    
    /** 毫秒帧率  1000 / 每秒帧数 */
    private final int FRAME_RATE = 50 ; 
    
    /** 刷新进度的速率 */
    private static final int PROGRESS_RATE =50 ; 
    
    /** 刷新进度消息 */
    private static final int WHAT_PROGRESS = 0 ; 
    /** 刷新界面消息 */
    private static final int WHAT_REFRESH_FRAME = 1 ; 
    
    private int mVideoWidth;//视频宽度
    private int mVideoHeight;//视频高度
    
    private int key = -1 ; 
    
    /**  本地视频 */
    private boolean localvideo = true ; 
    
    /** 是否 assert 资源 */
    private boolean assert_res = false ; 
    
    /** 阻塞更新进度条 */
    private boolean block_progress_update = false ; 
    
    
    public void setKey(int key){
    	this.key = key  ; 
    }
    
    public int getKey(){
    	return key ; 
    }
    
    public void onSetStart(){
    	CrossAppActivity.getContext().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				CrossAppVideoPlayer.this.play(true);  
				CrossAppVideoPlayer.this.setOnVideoPlayingListener(new CrossAppVideoPlayer.OnVideoPlayingListener() {
		            @Override
		            public void onVideoSizeChanged(int vWidth, int vHeight) {
		            }
		            @Override
		            public void onStart() {
		            	CrossAppActivity.getContext().runOnGLThread(new Runnable() {
							@Override
							public void run() {
								onPlayBufferLoadingState(key, PlaybackBufferEmpty);
							}
						});
		            }
		            @Override
		            public void onPlaying(final int duration,final  int percent) {
		            	//调用native 的进度方法
		            	CrossAppVideoPlayer p = getPlayerByKey(key) ;
		            	if (p != null && p.getMediaPlayer()!= null && p.getMediaPlayer().isPlaying()) {
		            		CrossAppActivity.getContext().runOnGLThread(new Runnable() {
								@Override
								public void run() {
									onPeriodicTime(key, (float)percent, (float)duration);
								}
							});
		            		
						}
		            }
		            @Override
		            public void onPause() {
		            }
		            @Override
		            public void onRestart() {
		            }
		            @Override
		            public void onPlayingFinish() {
		            	//调用native 的方法
		            	CrossAppActivity.getContext().runOnGLThread(new Runnable() {
							@Override
							public void run() {
								onDidPlayToEndTime(key);
							}
						});
		            	
		            }
		            @Override
		            public void onTextureDestory() {
		                pause();
		            }
		            @Override
		            public void onFrameGet( Bitmap bitmap) {
		            	if (bitmap != null) {
		            		final int wid = bitmap.getWidth() ;
			            	final int hei = bitmap.getHeight() ;
			            	final byte[] byts = bitmap2Bytes(bitmap) ; 
			            	bitmap.recycle();
			            	bitmap = null ; 
			            	//调用Native刷新方法
			            	CrossAppActivity.getContext().runOnGLThread(new Runnable() 
			            	{
								@Override
								public void run() 
								{
									onFrameAttached(key , byts , wid , hei); 
								}
							});
						}
		            }	
		            @Override
		            public void onBufferCached(final int current) {
		            	
		            	CrossAppVideoPlayer p = getPlayerByKey(key) ;
		            	if (p.getMediaPlayer().isPlaying()) {
		            		CrossAppActivity.getContext().runOnGLThread(new Runnable() {
								@Override
								public void run() {
									onLoadedTime(key, current, 100);
									if(current == 100) onPlayBufferLoadingState(key, PlaybackLikelyToKeepUp);
								}
							});
		            	}
		            }
		            @Override
		            public void onSeekChanged(MediaPlayer arg0) {
		            	CrossAppActivity.getContext().runOnGLThread(new Runnable() {
							@Override
							public void run() {
								onTimeJumped(key);
								block_progress_update(false);
							}
						});
		            }
		        });
			}
		});
    }
    
    /** 阻塞进度更新 */
    public synchronized void block_progress_update(boolean bool){
    	this.block_progress_update  = bool ; 
    	if(!block_progress_update){
    		getPlayingProgress(); 
    	}
    }
    
    public void setIsAssert(boolean bool){
    	this.assert_res = bool ; 
    }
    
    public interface OnVideoPlayingListener {
        void onVideoSizeChanged(int vWidth,int vHeight);
        void onStart();
        void onPlaying(int duration, int percent);
        void onPause();
        void onRestart();
        void onPlayingFinish();
        void onTextureDestory();
        void onFrameGet(Bitmap bitmap) ;
        void onBufferCached(int percent) ; 
        void onSeekChanged(MediaPlayer mp) ; 
    }
    
    public MediaPlayer getMediaPlayer(){
    	return mMediaPlayer ; 
    }
    
    public void destoryMediaPlayer(){
    	if(mMediaPlayer!= null){
    		mMediaPlayer.stop();  
        	mMediaPlayer.reset(); 
        	mMediaPlayer.release();
        	mMediaPlayer = null ; 
    	}
    }
    
    //播放状态
    public enum VideoState{
        init,palying,pause
    }
    
    public void setIsLocalVideo(boolean local){
    	this.localvideo = local ; 
    }
    
    public void setVideoState(VideoState state){
    	mState = state ; 
    	CrossAppActivity.getContext().runOnGLThread(new Runnable() {
			@Override
			public void run() {
				if (mState == VideoState.palying) {
		    		onPlayState(getKey(), PlayStatePlaying);
				}else if (mState == VideoState.pause) {
					onPlayState(getKey(), PlayStatePause);
				}
			}
		});
    	
    }
    
    private OnVideoPlayingListener listener;
    public void setOnVideoPlayingListener(OnVideoPlayingListener listener){
        this.listener = listener;
    }
    
    public CrossAppVideoPlayer(Context context) {
        super(context);
        init();
    }
    
    private void init(){
        
        CrossAppActivity.getContext().runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mProgressHandler = new Handler(){
			        @Override
			        public void handleMessage(Message msg) {
			            super.handleMessage(msg);
			            if (msg.what == WHAT_PROGRESS){
			                if (listener!=null  && mMediaPlayer!= null &&  mMediaPlayer.isPlaying() && !block_progress_update){
			                	listener.onPlaying(mMediaPlayer.getDuration(),mMediaPlayer.getCurrentPosition());
			                    sendEmptyMessageDelayed(WHAT_PROGRESS,PROGRESS_RATE);
			                }
			            }else if (msg.what == WHAT_REFRESH_FRAME ) {
							//刷新帧
			            	if (mState == VideoState.palying && listener!= null && !block_progress_update) {
								listener.onFrameGet(CrossAppVideoPlayer.this.getBitmap() );
			            		sendEmptyMessageDelayed(WHAT_REFRESH_FRAME, FRAME_RATE) ; 
							}
						}
			        }
			    };
			}
		});
        setSurfaceTextureListener(this);
    }
    
    public void setUrl(String url){
        this.url = url;
        
        CrossAppActivity.msHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				play(false);
				userPerformStarCall = new OnPreparedAndUserPerformStarCall() {
					@Override
					public void onCall() {
						getPlayerByKey(getKey()).play(true);
					}
				};
				
			}
		}, 200) ; 
        
    }
    
    
    interface OnPreparedAndUserPerformStarCall{
    	public void onCall();
    }
    
    private OnPreparedAndUserPerformStarCall userPerformStarCall ; 
    
    public void play(boolean send_status){
    	
        if (mMediaPlayer==null || mMediaPlayer.isPlaying()) return;
        
        if(userPerformStarCall == null && !_available_prepaired){
        	userPerformStarCall = new OnPreparedAndUserPerformStarCall() {
				@Override
				public void onCall() {
					getPlayerByKey(getKey()).play(true);
				}
			};
			return ; 
        } ; 
        
        try {
        	
        	if (_available_prepaired && currentUrl .equals(url)) {
        		mMediaPlayer.start();
        		
			} else {
				
				mMediaPlayer.reset();
				
				if(assert_res){
					AssetFileDescriptor descriptor = null;  
			        try {  
			            descriptor = CrossAppActivity.getContext(). getAssets().openFd(url);  
			        } catch (IOException e) {  
			            e.printStackTrace();  
			        }  
			        mMediaPlayer.setDataSource(descriptor.getFileDescriptor(),  
		                    descriptor.getStartOffset(), descriptor.getLength());  
		            descriptor.close();  
				} else {
					mMediaPlayer.setDataSource(CrossAppVideoPlayer.this.url);
				}
				
//	            mMediaPlayer.prepare();
//	            mMediaPlayer.start();
	            
	           mMediaPlayer.prepareAsync(); 
	            
	            currentUrl = url ; 
			}
        	if(send_status)setVideoState(VideoState.palying);
            
            if (listener!=null) listener.onStart();
            mMediaPlayer.setOnSeekCompleteListener(new OnSeekCompleteListener() {
				@Override
				public void onSeekComplete(MediaPlayer mp) {
					listener.onSeekChanged(mp);
				}
			});
            getPlayingProgress();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    private String currentUrl = "" ; 
    
    public void pause(){
        if (mMediaPlayer==null) return;
        
        if (mMediaPlayer.isPlaying()){
            mMediaPlayer.pause();
            setVideoState(VideoState.pause);
            if (listener!=null) listener.onPause();
        }else{
            mMediaPlayer.start();
            setVideoState(VideoState.palying);
            if (listener!=null) listener.onRestart();
            getPlayingProgress();
        }
    }
    
    public void stop(){
        if (mState == VideoState.palying  || mState == VideoState.pause){
        	CrossAppActivity.getContext().runOnUiThread(new Runnable() {
				@Override
				public void run() {
					mMediaPlayer.pause();
		        	mMediaPlayer.seekTo(0);
		        	sendFirsttFrame();
		        	CrossAppActivity.getContext().runOnGLThread(new Runnable() {
						@Override
						public void run() {
							onPeriodicTime(getKey(), 0, mMediaPlayer.getDuration());
				        	setVideoState(VideoState.pause);
						}
					});
				}
			});
        }
    }
    
    public void sendFirsttFrame(){
    	
    	if(localvideo){
    		MediaMetadataRetriever media = new MediaMetadataRetriever();
    		
    		Bitmap bitmap = null ; 
    		
    		if (assert_res) {
    			try {
    				AssetFileDescriptor descriptor = null;  
    		        try {  
    		            descriptor = CrossAppActivity.getContext(). getAssets().openFd(url);  
    		        } catch (IOException e) {  
    		            e.printStackTrace();  
    		        }  
    		        media.setDataSource(descriptor.getFileDescriptor(),  
		            descriptor.getStartOffset(), descriptor.getLength());  
    		        bitmap = media.getFrameAtTime();
    	            descriptor.close();  
				} catch (Exception e) {
					e.printStackTrace();
				}
			} else {
				media.setDataSource(url);
				bitmap = media.getFrameAtTime();
			}
    		
    		final byte[] bit = Bitmap2Bytes(bitmap) ; 
        	
			CrossAppActivity.getContext().runOnGLThread(new Runnable() {
				@Override
				public void run() {
					onFrameImageAttached(getKey(), bit, bit.length);
				}
			});
		} else {
			CrossAppActivity.getContext().runOnGLThread(new Runnable() {
				@Override
				public void run() {
					onFrameImageAttached(getKey(), new byte[0], 0);
				}
			});
		}
    }
    
    /**  
     * 把Bitmap转Byte  
     */    
    public static byte[] Bitmap2Bytes(Bitmap bm){    
        ByteArrayOutputStream baos = new ByteArrayOutputStream();    
        bm.compress(Bitmap.CompressFormat.JPEG, 100, baos);    
        return baos.toByteArray();    
    }
    
    //播放进度获取
    private void getPlayingProgress(){
        mProgressHandler.sendEmptyMessage(WHAT_PROGRESS);
        mProgressHandler.sendEmptyMessage(WHAT_REFRESH_FRAME) ; 
    }
    
    private boolean _available_prepaired = false ; 
    
    public Handler mProgressHandler = null ; 
    
    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
    	
        if (mMediaPlayer==null){
            mMediaPlayer = new MediaPlayer();
            mMediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
                @Override
                public void onPrepared(MediaPlayer mp) {
                    //当MediaPlayer对象处于Prepared状态的时候，可以调整音频/视频的属性，如音量，播放时是否一直亮屏，循环播放等。
                    mMediaPlayer.setVolume(1f,1f);
                    _available_prepaired = true ; 
                    if (userPerformStarCall != null) {
                    	userPerformStarCall.onCall();
					}
                    userPerformStarCall = null ; 
                    CrossAppActivity.getContext().runOnGLThread(new Runnable() {
						@Override
						public void run() {
							onPlayBufferLoadingState(getKey(), PlaybackLikelyToKeepUp);
						}
					});
                }
            });
            mMediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
                @Override
                public boolean onError(MediaPlayer mp, int what, int extra) {
                    return false;
                }
            });

            mMediaPlayer.setOnBufferingUpdateListener(new MediaPlayer.OnBufferingUpdateListener() {
                @Override
                public void onBufferingUpdate(MediaPlayer mp, int percent) {
                    //此方法获取的是缓冲的状态
                    if(listener != null){
                    	listener.onBufferCached(percent);
                    }
                }
            });
            
            //播放完成的监听
            mMediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                @Override
                public void onCompletion(MediaPlayer mp) {
                    setVideoState(VideoState.init);
                    if (listener!=null) listener.onPlayingFinish();
                }
            });
            
            mMediaPlayer.setOnVideoSizeChangedListener(new MediaPlayer.OnVideoSizeChangedListener() {
                @Override
                public void onVideoSizeChanged(MediaPlayer mp, int width, int height) {
                    mVideoHeight = mMediaPlayer.getVideoHeight();
                    mVideoWidth = mMediaPlayer.getVideoWidth();
                    updatePlayerSize();
                    if (listener!=null){
                        listener.onVideoSizeChanged(mVideoWidth,mVideoHeight);
                    }
                }
            });
        }
        
        //拿到要展示的图形界面
        Surface mediaSurface = new Surface(surface);
        //把surface
        mMediaPlayer.setSurface(mediaSurface);
        
        if(onDestoried_position > 0){
        	play(true);
        }else {
        	setVideoState(VideoState.init);
		}
        
    }
    
    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
    }
    
    public void updatePlayerSize(){
  	  FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) this.getLayoutParams();
        params.width = mVideoWidth ; 
        params.height = mVideoHeight ; 
        this.requestLayout();
    }
    
    private int onDestoried_position = 0 ; 
    
    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
    	
    	if (mMediaPlayer.isPlaying()) {
    		onDestoried_position = mMediaPlayer.getCurrentPosition() ; 
		}
    	
        if (listener!=null)listener.onTextureDestory();
        return false;
    }
    
    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
    	
    }
    
    /**
     * 获取本地视频第一帧的数据
     * @param filePath
     * @return
     */
    public static void getFirstFrameImageWithFilePath(String filePath){
    	Bitmap bitmap = null ; 
    	MediaMetadataRetriever media = new MediaMetadataRetriever();
    	if (!filePath.startsWith("/")) {
			try {
				AssetFileDescriptor descriptor = null;  
		        try {  
		            descriptor = CrossAppActivity.getContext(). getAssets().openFd(filePath);  
		        } catch (IOException e) {  
		            e.printStackTrace();  
		        }  
		        media.setDataSource(descriptor.getFileDescriptor(),  
	                    descriptor.getStartOffset(), descriptor.getLength());  
		        bitmap = media.getFrameAtTime();
	            descriptor.close();  
			} catch (Exception e) {
				e.printStackTrace();
			}
		}else {
			media.setDataSource(filePath);
			bitmap = media.getFrameAtTime();
		}
    	if(bitmap != null){
    		byte[] bit = Bitmap2Bytes(bitmap) ; 
        	onBitmapFrameAttached(bit, bit.length);
    	}else {
    		onBitmapFrameAttached(new byte[0], 0);
		}
    }
    
    /**
     * 获取到了第一帧数据
     * @param byt
     * @param length
     */
    public static native void onBitmapFrameAttached(byte[] byt , int length) ;
    
    
    
}
