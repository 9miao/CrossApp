package com.umeng;

import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.content.Context;

import com.umeng.analytics.AnalyticsConfig;
import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.social.UMPlatformData;
import com.umeng.analytics.social.UMPlatformData.GENDER;
import com.umeng.analytics.social.UMPlatformData.UMedia;

public class AnalyticsHome
{	
	static private Context mContext;
	
	public static void init(Activity activity)
	{
		AnalyticsHome.mContext = activity;
		
//		MobclickAgent.setDebugMode(true);
//      SDK在统计Fragment时，需要关闭Activity自带的页面统计，
//		然后在每个页面中重新集成页面统计的代码(包括调用了 onResume 和 onPause 的Activity)。
		MobclickAgent.openActivityDurationTrack(false);
//		MobclickAgent.setAutoLocation(true);
//		MobclickAgent.setSessionContinueMillis(1000);
		
		MobclickAgent.updateOnlineConfig(AnalyticsHome.mContext);
	}
	
	public static void startWithAppkey(String appkey, String channel)
	{
		AnalyticsConfig.setAppkey(appkey);
		AnalyticsConfig.setChannel(channel);
	}
	
	public static void onPageStart(String viewID) 
	{
		MobclickAgent.onPageStart( viewID );
		onResume();
	}
	
	public static void onPageEnd(String viewID)
	{
		MobclickAgent.onPageEnd(viewID);
		onPause();
	}
	
	public static void onResume() 
	{
		MobclickAgent.onResume(AnalyticsHome.mContext);
	}
	
	public static void onPause()
	{
		MobclickAgent.onPause(AnalyticsHome.mContext);
	}
	
	//0
	public static void onEventBegin(String eventId)
	{
		MobclickAgent.onEventBegin(AnalyticsHome.mContext, eventId);
	}
	
	public static void onEventEnd(String eventId)
	{
		MobclickAgent.onEventEnd(AnalyticsHome.mContext, eventId);
	}
	
	public static void onEvent(String eventId)
	{
		MobclickAgent.onEvent(AnalyticsHome.mContext, eventId);
	}
	
	//1
	public static void onEventBegin(String eventId, String value)
	{
		MobclickAgent.onEventBegin(AnalyticsHome.mContext, eventId, value);
	}
	
	public static void onEventEnd(String eventId, String value)
	{
		MobclickAgent.onEventEnd(AnalyticsHome.mContext, eventId, value);
	}
	
	public static void onEvent(String eventId, String value)
	{
		System.out.println("----x" + eventId + value);
		MobclickAgent.onEvent(AnalyticsHome.mContext, eventId, value);
	}
	
	//2
	public static void onEvent(String eventId, HashMap<String,String> map)
	{
		MobclickAgent.onEvent(AnalyticsHome.mContext, eventId, map);
	}
	
	public static void onEventValue(String eventId, Map<String,String> map, int value)
	{
		MobclickAgent.onEventValue(AnalyticsHome.mContext, eventId, map, value);
	}
}