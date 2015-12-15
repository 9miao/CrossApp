package org.CrossApp.lib;

import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;

/**
 * 用于代码生成Drawable图形
 * @author liuguoyan
 *
 */
public final class DrawableShapUtil {
	
	/**
	 * 生成正方形圆角的shape
	 * @param context
	 * @param colorresid
	 * @param ridous
	 * @return
	 */
	public static Drawable getCornerRectAngle(Context context ,int colorresid,int ridous,Rect rect){
		return getCorlorRectAngleContaionColor(context, context.getResources().getColor(colorresid), ridous, rect) ; 
	}
	
	public static Drawable getCorlorRectAngle(Context context ,String colorHex,int ridous,Rect rect){
		return getCorlorRectAngleContaionColor(context, Color.parseColor(colorHex), ridous, rect) ; 
	}
	
	/**
	 * 
	 * @param context
	 * @param color 
	 * @param ridous
	 * @param rect
	 * @return
	 */
	public static Drawable getCorlorRectAngleContaionColor(Context context ,int color,int ridous,Rect rect){
		
		GradientDrawable mGradientDrawable = new GradientDrawable() ; 
		mGradientDrawable.setAlpha(100);
		mGradientDrawable.setCornerRadius(ridous);
		mGradientDrawable.setBounds(rect) ; 
		mGradientDrawable.setColor(color);
		
		return mGradientDrawable ; 
	}
	
	/**
	 * 得到某种颜色的矩形
	 * @param context
	 * @param colorresid
	 * @return
	 */
	public static Drawable getRectAngle(Context context ,int colorresid,Rect rect){
		
		GradientDrawable mGradientDrawable = new GradientDrawable() ; 
		mGradientDrawable.setAlpha(100);
		mGradientDrawable.setBounds(rect) ; 
		mGradientDrawable.setColor(context.getResources().getColor(colorresid));
		
		return mGradientDrawable ; 
	}
	
	
}
