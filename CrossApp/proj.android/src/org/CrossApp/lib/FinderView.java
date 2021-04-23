/**
 *  2014-7-15   涓����11:14:21
 *  Created By niexiaoqiang
 */

package org.CrossApp.lib;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.view.View;

/**
 * ��ユ�炬��
 * @author niexiaoqiang
 */
public class FinderView extends View {
	private static final long ANIMATION_DELAY = 10L;
	private Paint finderMaskPaint;
	private int measureedWidth;
	private int measureedHeight;
	private Paint paint;
	private Paint textviewPaint;
	private static final int SPEEN_DISTANCE = 15;
	private int slideTop;
	private String text;   
	private Context context;
	public FinderView(Context context) {
		super(context);
		init(context);
	}

	public FinderView(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		init(context);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		
		Rect rect = new Rect();
		    textviewPaint.getTextBounds(text, 0, text.length(), rect);
		    int width = rect.width();//文本的宽度
		    int height = rect.height();//文本的高度
		    float x =(getWidth()-width)/2;
		    canvas.drawText(text, x , measureedHeight/2+  (measureedWidth / 2 + 200)/2+height+ CrossAppHelper.dip2px(context, 15),textviewPaint);
		
		canvas.drawRect(leftRect, finderMaskPaint);
		canvas.drawRect(topRect, finderMaskPaint);
		canvas.drawRect(rightRect, finderMaskPaint);
		canvas.drawRect(bottomRect, finderMaskPaint);
		//��绘��
		paint.setColor(Color.GREEN);
		canvas.drawRect(new Rect(leftRect.width(), topRect.height(), leftRect.width()+5,
				bottomRect.top), paint);
		canvas.drawRect(new Rect(middleRect.right,middleRect.top,middleRect.right+5,middleRect.bottom), paint);
		canvas.drawRect(new Rect(middleRect.left,middleRect.bottom,middleRect.right+5,middleRect.bottom+5), paint);
		canvas.drawRect(new Rect(middleRect.left,middleRect.top,middleRect.right+5,middleRect.top+5), paint);

//		slideTop = lineRect.top;
//		slideTop += SPEEN_DISTANCE;
//		if(slideTop >= middleRect.bottom){
//			slideTop = middleRect.top;
//		}
//        lineRect.top = slideTop;  
//        lineRect.bottom = slideTop + 5; 
//		canvas.drawRect(lineRect, paint);

		postInvalidateDelayed(ANIMATION_DELAY, middleRect.left, middleRect.top, middleRect.right, middleRect.bottom);
	}

	private Rect topRect = new Rect();
	private Rect bottomRect = new Rect();
	private Rect rightRect = new Rect();
	private Rect leftRect = new Rect();
	private Rect middleRect = new Rect();

	private Rect lineRect = new Rect();
	private int lineHeight;

	private void init(Context context) {
		this.context=context;
		text=context.getResources().getString(R.string.finderview_tag);
		int finder_mask = 0x00000000;//颜色
		finderMaskPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
		finderMaskPaint.setColor(finder_mask);
		lineHeight = 5;
		paint = new Paint();
		
		textviewPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
		textviewPaint.setColor(Color.parseColor("#ffffff"));
		textviewPaint.setTextSize(CrossAppHelper.dip2px(context, 16));
		
		
	}

	//////////////��板��璇ユ�规��//////////////////////
	/**
	 * ��规����剧��size姹���虹�╁舰妗���ㄥ�剧�������ㄤ��缃�锛�tip锛���告�烘��杞�90搴�浠ュ��锛������������剧�����妯�������锛�������浼���������版�讹�����浜�浜ゆ��
	 * @param w
	 * @param h
	 * @return
	 */
	public Rect getScanImageRect(int w, int h) {
		//���姹���哄�������╁舰
		Rect rect = new Rect();
		rect.left = middleRect.left;
		rect.right = middleRect.right;
		float temp = h / (float) measureedHeight;
		rect.top = (int) (middleRect.top * temp);
		rect.bottom = (int) (middleRect.bottom * temp);
		return rect;
	}

	////////////////////////////////////
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		measureedWidth = MeasureSpec.getSize(widthMeasureSpec);
		measureedHeight = MeasureSpec.getSize(heightMeasureSpec);
		int borderWidth = measureedWidth / 2 + 200;
		middleRect.set((measureedWidth - borderWidth) / 2, (measureedHeight - borderWidth) / 2, (measureedWidth - borderWidth) / 2 + borderWidth, (measureedHeight - borderWidth) / 2 + borderWidth);
		lineRect.set(middleRect);
		lineRect.bottom = lineRect.top + lineHeight;
		leftRect.set(0, middleRect.top, middleRect.left, middleRect.bottom);
		topRect.set(0, 0, measureedWidth, middleRect.top);
		rightRect.set(middleRect.right, middleRect.top, measureedWidth, middleRect.bottom);
		bottomRect.set(0, middleRect.bottom, measureedWidth, measureedHeight);
	}
}
