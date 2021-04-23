

package org.CrossApp.lib;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

import android.app.Dialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnDismissListener;
import android.graphics.Color;
import android.text.TextPaint;
import android.util.Log;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.ScrollView;
import android.widget.TextView;


public final class CrossAppAlertView
{
	private static CrossAppActivity context = null;
	
	private static HashMap<Integer, AlertView> dict = null;
	
	public final static int HANDLER_SHOW_DIALOG = 1;
	public final static int HANDLER_SHOW_EDITBOX_DIALOG = 2;
	
	
	public static class AlertView
	{
		public String titile;
		public String message;
		public ArrayList<String> bottonTitles = null;
		
		public AlertView(String title, String message)
		{
			this.titile = title;
			this.message = message;
			this.bottonTitles = new ArrayList<String>();
		}
		
		public void addButton(String title)
		{
			this.bottonTitles.add(title);
		}
	}
	
	public static void checkAliveAlertView(){
		if (dict != null && dict.size() >0) {
			Set<Integer> keys = dict.keySet() ; 
			
			Iterator<Integer> iterator = keys.iterator();
			while (iterator.hasNext()) {
				Integer key = iterator.next() ; 
				show(key);
			}
		}
		
	}
	
	
	
	public static void createAlert(final String pTitle, final String pMessage, final int key)
	{
		if (dict == null)
    	{
    		dict = new HashMap<Integer, AlertView>();
    		context = CrossAppActivity.getContext();
    	}
		
		AlertView alertView = new AlertView(pTitle, pMessage);
		
		dict.put(key, alertView);
	}
	
	public static void addButton(final String pTitle, final int key)
	{
		final AlertView alertView = dict.get(key);
		if (alertView != null)
		{
			alertView.addButton(pTitle);
		}
	}
	
	private static native void onClick(int index, int key);
	
	public static void show(final int key)
	{
		final AlertView alertView =  dict.get(key);
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				
				final Dialog dialog = new Dialog(context) ; 
				dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
				dialog.getWindow().getDecorView().setBackgroundColor(Color.TRANSPARENT);
				dialog.setCanceledOnTouchOutside(false);
				
				
				int dimi_dialog_min_width = CrossAppHelper.dip2px(context, 250) ; 
				int dimi_msg_horizental_margin = CrossAppHelper.dip2px(context, 16) ; 
				int dimi_title_vertical_margin = CrossAppHelper.dip2px(context, 8) ;
				int dimi_msg_vertical_margin = CrossAppHelper.dip2px(context, 4) ;
				int dimi_btn_hei = CrossAppHelper.dip2px(context, 38);
				int txt_size_title = CrossAppHelper.dip2px(context, 16) ; 
				int txt_size_msg = CrossAppHelper.dip2px(context, 14) ; 
				String btn_txt_color_hex = "#417bf9";
				
				//initial Dialog contentview 
				LinearLayout container = new LinearLayout(context) ; 
				container.setOrientation(LinearLayout.VERTICAL);
				
				container.setBackgroundResource(R.drawable.img_cross_alert_def_05);
				
				//add titleview to contentview
				TextView titleView = new TextView(context) ; 
				titleView.setGravity(Gravity.CENTER);
				titleView.setTextSize(TypedValue.COMPLEX_UNIT_PX, txt_size_title);
				titleView.setTextColor(Color.parseColor("#000000"));
				titleView.getPaint().setFakeBoldText(true);
				titleView.setSingleLine(true);
				titleView.setText(alertView.titile);
				
				LinearLayout.LayoutParams title_param = new LinearLayout.LayoutParams(dimi_dialog_min_width, LayoutParams.WRAP_CONTENT) ; 
				title_param.topMargin = (dimi_title_vertical_margin * 2);
				
				title_param.gravity = Gravity.CENTER_HORIZONTAL ; 
				container.addView(titleView,title_param);	
				
				
				//add msgview to contentview
				TextView msgView = new TextView(context) ; 
				msgView.setGravity(Gravity.CENTER);
				msgView.setTextSize(TypedValue.COMPLEX_UNIT_PX,txt_size_msg);
				msgView.setTextColor(Color.parseColor("#000000")); 
				msgView.setText(alertView.message);
				
				
				LinearLayout.LayoutParams msg_param = new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT) ; 
				msg_param.gravity = Gravity.CENTER_HORIZONTAL ; 
				msg_param.topMargin = dimi_msg_vertical_margin;
				msg_param.leftMargin= dimi_msg_horizental_margin ; 
				msg_param.rightMargin = dimi_msg_horizental_margin ; 
				container.addView(msgView,msg_param);	
				
				CAFrameLayout rConerLayout = new CAFrameLayout(context) ; 
				container.addView(rConerLayout);
				//initial button view container
				ScrollView scrollView = new ScrollView(context) ; 
				scrollView.setVerticalScrollBarEnabled(false);
				FrameLayout.LayoutParams scroll_param = new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT) ; 
				scroll_param.topMargin = dimi_title_vertical_margin * 2 ; 
				scroll_param.gravity = Gravity.CENTER_HORIZONTAL ; 
				rConerLayout.addView(scrollView,scroll_param);
				
				//add buttons 
				LinearLayout buttons_container = new LinearLayout(context) ; 
				buttons_container.setOrientation(alertView.bottonTitles.size() < 3 ? LinearLayout.HORIZONTAL : LinearLayout.VERTICAL);
				for (int i = 0; i < alertView.bottonTitles.size(); i++) 
				{
					final int position = i ; 
					TextView textView = new TextView(context) ; 
					textView.setTextSize(TypedValue.COMPLEX_UNIT_PX,txt_size_msg);
					if (alertView.bottonTitles.size()<=2 || i==alertView.bottonTitles.size()-1)
					{
						textView.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.BOTTOM);
					}
					else
					{
						textView.setGravity(Gravity.CENTER);
					}
					textView.setText(alertView.bottonTitles.get(i));
					textView.setClickable(true);
					textView.setTextColor(Color.parseColor(btn_txt_color_hex));
					LinearLayout.LayoutParams txt_params = new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dimi_btn_hei) ; 
					
					if (alertView.bottonTitles.size()==1) 
					{
						textView.setBackgroundResource(R.drawable.selector_crossapp_alert_02);
					}
					else if (alertView.bottonTitles.size()==2) 
					{
						txt_params.weight = 1 ;
						textView.setBackgroundResource(i == 0 ? R.drawable.selector_crossapp_alert_03: R.drawable.selector_crossapp_alert_04);
					}
					else
					{
						textView.setBackgroundResource((i==alertView.bottonTitles.size()-1) ? R.drawable.selector_crossapp_alert_02 : R.drawable.selector_crossapp_alert_01);
					}
					buttons_container.addView(textView, txt_params);
					
					textView.setOnClickListener(new View.OnClickListener() {
						@Override
						public void onClick(View v)
						{
							dialog.dismiss();
							context.runOnGLThread(new Runnable() 
                        	{
                                @Override
                                public void run()
                                {
                                	dict.remove(key);
        							CrossAppAlertView.onClick(position, key);
                                }
                            });
						}
					});
					
				}
				scrollView.addView(buttons_container); 
				
				dialog.setContentView(container);
				dialog.show(); 
				
			}
		});
		
	}
	

	public static void showEditTextDialog(final String pTitle, final String pContent, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength) 
	{

	}
	
	public static class EditBoxMessage 
	{
		public String title;
		public String content;
		public int inputMode;
		public int inputFlag;
		public int returnType;
		public int maxLength;
		
		public EditBoxMessage(String title, String content, int inputMode, int inputFlag, int returnType, int maxLength)
		{
			this.content = content;
			this.title = title;
			this.inputMode = inputMode;
			this.inputFlag = inputFlag;
			this.returnType = returnType;
			this.maxLength = maxLength;
		}
	}
}
