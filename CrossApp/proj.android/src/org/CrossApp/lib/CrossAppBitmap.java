
package org.CrossApp.lib;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.FontMetrics;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.text.Html;
import android.text.Layout;
import android.text.StaticLayout;
import android.text.TextPaint;
import android.text.TextUtils;
import android.util.Log;
import android.widget.TextView;

public final class CrossAppBitmap {
    // ===========================================================
    // Constants
    // ===========================================================

    private static final int HORIZONTAL_ALIGN_LEFT = 1;
    private static final int HORIZONTAL_ALIGN_RIGHT = 2;
    private static final int HORIZONTAL_ALIGN_CENTER = 3;
    private static final int VERTICAL_ALIGN_TOP = 1;
    private static final int VERTICAL_ALIGN_BOTTOM = 2;
    private static final int VERTICAL_ALIGN_CENTER = 3;
    
    // ===========================================================
    // Fields
    // ===========================================================

    private static Context sContext;

    // ===========================================================
    // Getter & Setter
    // ===========================================================

    public static void setContext(final Context context) {
        CrossAppBitmap.sContext = context;
    }

    // ===========================================================
    // Methods for/from SuperClass/Interfaces
    // ===========================================================

    // ===========================================================
    // Methods
    // ===========================================================

    private static native void nativeInitBitmapDC(final int width,
            final int height, final byte[] pixels);

    //http://egoco.de/post/19077604048/calculating-the-height-of-text-in-android
    public static int getTextHeight(String text, int maxWidth, float textSize, Typeface typeface) {
        TextPaint paint = new TextPaint(Paint.ANTI_ALIAS_FLAG | Paint.SUBPIXEL_TEXT_FLAG);
        paint.setTextSize(textSize);
        paint.setTypeface(typeface);

        int lineCount = 0;

        int index = 0;
        int length = text.length();

        while(index < length) {
            index += paint.breakText(text, index, length, true, maxWidth, null);
            lineCount++;
        }

        float actualHeight = (Math.abs(paint.ascent()) + Math.abs(paint.descent()));

        return (int)Math.floor(lineCount * actualHeight);
    }

    public static Typeface calculateShrinkTypeFace(String text, int width, int height, Layout.Alignment hAlignment, float textSize, TextPaint paint, boolean enableWrap)
    {
        if (width == 0 || height == 0) {
            return  paint.getTypeface();
        }
        float actualWidth = width + 1;
        float actualHeight = height + 1;
        float fontSize = textSize + 1;

        if (!enableWrap) {
            while (actualWidth > width || actualHeight > height) {
                fontSize = fontSize - 1;

                actualWidth = (int)Math.ceil( StaticLayout.getDesiredWidth(text, paint));
                actualHeight = getTextHeight(text, (int)actualWidth, fontSize, paint.getTypeface());

                paint.setTextSize(fontSize);
                if (fontSize <= 0) {
                    paint.setTextSize(textSize);
                    break;
                }
            }
        } else {
            while (actualHeight > height || actualWidth > width) {
                fontSize = fontSize - 1;

                Layout layout = new StaticLayout(text, paint, (int) width, hAlignment,1.0f,0.0f,false);
                actualWidth = layout.getWidth();
                actualHeight = layout.getLineTop(layout.getLineCount());

                paint.setTextSize(fontSize);

                if (fontSize <= 0) {
                    paint.setTextSize(textSize);
                    break;
                }
            }

        }
        return paint.getTypeface();
    }
    
    /**
     * 
     * @param string   
     * @param fontName 
     * @param fontSize    
     * @param fontTintR   		
     * @param fontTintG  
     * @param fontTintB   
     * @param fontTintA  
     * @param alignment 
     * @param width       
     * @param height      
     * @param shadow    
     * @param shadowDX  
     * @param shadowDY  
     * @param shadowBlur  
     * @param shadowOpacity  
     * @param stroke       
     * @param strokeR    
     * @param strokeG   
     * @param strokeB    
     * @param strokeA    
     * @param strokeSize  
     * @param bold
     * @param underLine
     * @param deleteLine
     * @param italics
     * @param italics_v
     * @param wordWrap
     * @param lineSpacing
     * 
     * 
     * @return
     */
    public static boolean createTextBitmapShadowStroke(
    		byte[] strs,
    		final String fontName, 
    		int fontSize,
            int fontTintR, int fontTintG, int fontTintB, int fontTintA,
            int alignment, 
            int width, 
            int height, 
            int shadow, 
            float shadowDX, 
            float shadowDY, 
            float shadowBlur, 
            
            int shadowr, 
            int shadowg, 
            int  shadowb, 
            int shadowa, 
            
            int stroke, 
            int strokeR, 
            int strokeG, 
            int strokeB, 
            int strokeA, 
            float strokeSize,
            int bold ,                                        
            int undeerLine, 
            int deleteLine , 
            int italics , 
            float italics_v , 
            int wordWrap,
            float lineSpacing
    		) 
    {
    	
    	String string = new String(strs) ; 
        Layout.Alignment hAlignment = Layout.Alignment.ALIGN_NORMAL;
        int horizontalAlignment = alignment & 0x0F;
        switch (horizontalAlignment) {
            case HORIZONTAL_ALIGN_CENTER:
                hAlignment = Layout.Alignment.ALIGN_CENTER;
                break;
            case HORIZONTAL_ALIGN_RIGHT:
                hAlignment = Layout.Alignment.ALIGN_OPPOSITE;
                break;
            case HORIZONTAL_ALIGN_LEFT:
                break;
            default:
                break;
        }
        
        
        TextPaint paint = CrossAppBitmap.newPaint(fontName, fontSize);
        
        italics_v = -italics_v ; 
        paint.setFakeBoldText(bold > 0 ?true : false);
        paint.setTextSkewX(italics > 0 ? italics_v : 0);     
        paint.setUnderlineText(undeerLine  >0 ? true : false); 
        paint.setStrikeThruText(deleteLine > 0 ? true: false); 
        
        if (shadowBlur>0) {
        	int color = Color.argb(shadowa, shadowr, shadowg, shadowb) ; 
        	paint.setShadowLayer(2 , 2 , 2,color);
		}
        
        
        int _increase =  italics > 0 ?  (int) ( fontSize * Math.abs(italics_v) * 0.5f ) : 0 ; 
        int _italics_trans = italics > 0 ? (   italics_v > 0 ?  _increase : -_increase  ) : 0 ; 
        
        if (stroke>0) {
            paint.setStyle(TextPaint.Style.STROKE);
            paint.setStrokeWidth(strokeSize/2);
        }
        
        //resize size
        width = width == 0xffffffff ? 8192 : width ; 
        
        int maxWidth = width;
        
        if (maxWidth <= 0) {
            maxWidth = (int)Math.ceil( StaticLayout.getDesiredWidth(string, paint));
        }
        
        Layout layout = null;
        int layoutWidth = 0;
        int layoutHeight = 0;
        
        //if number of lines = 1 , lineSpacing 
        StaticLayout pramsLayout = new StaticLayout(string, paint, maxWidth , hAlignment,1.0f,lineSpacing,false) ; 
        lineSpacing = pramsLayout.getLineCount() == 1 ? 0.0f : lineSpacing ;
        pramsLayout = null ; 
        
        CharSequence c = string; 
        if (string.contains("</font>")) { 
			c = HtmlParser.buildSpannedText(string,new CrossAppTagHandler()) ; 
		}else if (wordWrap==0) {
			c = autoSplitText(paint, string, maxWidth) ; 
		}
        CharSequence newc=c;
        {
        	 StaticLayout layoutpre = new StaticLayout(c, paint, maxWidth , hAlignment,1.0f,lineSpacing,false);
        	 int line_height = layoutpre.getLineTop(1) ; 
             int number_of_linespre = height / line_height == 0 ? 1 : height / line_height ; 
             float number_hei = (float)height / (float)line_height == 0.0f ? 1.0f : (float)height / (float)line_height ; 
             int heightpre = (int)(line_height * number_hei ) ; 
             
             number_of_linespre = (int)Math.round(number_hei) ;
             int pos=myEllipsize(layoutpre,paint,c,number_of_linespre,maxWidth);
             if(pos<c.length()&&pos!=-1)
             {
            	 //Log.d("java","ellipsize pos:"+pos+"length:"+c.length());
            	 newc=c.subSequence(0,pos)+"...";
            	 //Log.d("java","ellipsize text:"+newc);
             }
             
        }
        
        layout = new StaticLayout(newc, paint, maxWidth , hAlignment,1.0f,lineSpacing,false);
        //layout = new StaticLayout(c,0,c.length(), paint, maxWidth , hAlignment,1.0f,lineSpacing,false,TextUtils.TruncateAt.END,maxWidth);
       
        
        //get number of lines
        int line_height = layout.getLineTop(1) ; 
        int number_of_lines = height / line_height == 0 ? 1 : height / line_height ; 
        float number_hei = (float)height / (float)line_height == 0.0f ? 1.0f : (float)height / (float)line_height ; 
        height = (int)(line_height * number_hei ) ; 
        
        number_of_lines = (int)Math.round(number_hei) ; 

        layoutWidth = layout.getWidth();
        layoutHeight = layout.getLineTop(number_of_lines> layout.getLineCount()  ? layout.getLineCount() : number_of_lines);
        
        int bitmapWidth = Math.max(layoutWidth, width);
        int bitmapHeight = layoutHeight;
        
        if (bitmapWidth == 0 || bitmapHeight == 0) {
            return false;
        }
        
        int offsetX = 0;
        if (horizontalAlignment == HORIZONTAL_ALIGN_CENTER) {
            offsetX = (bitmapWidth - layoutWidth) / 2;
        }
        else if (horizontalAlignment == HORIZONTAL_ALIGN_RIGHT) {
            offsetX = bitmapWidth - layoutWidth;
        }
        offsetX -= _italics_trans ; 
        
        int offsetY = 0;
        int verticalAlignment   = (alignment >> 4) & 0x0F;
        switch (verticalAlignment)
        {
            case VERTICAL_ALIGN_CENTER:
                offsetY = (bitmapHeight - layoutHeight) / 2;
                break;
            case VERTICAL_ALIGN_BOTTOM:
                offsetY = bitmapHeight - layoutHeight;
                break;
        }
        
        int wid = bitmapWidth + _increase ; 
        Bitmap bitmap = Bitmap.createBitmap(wid, bitmapHeight, Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        canvas.translate(offsetX, offsetY);
        if ( stroke>0 )
        {
            paint.setARGB(strokeA, strokeR, strokeG, strokeB);
            layout.draw(canvas);
        }
        paint.setStyle(TextPaint.Style.FILL);
        paint.setARGB(fontTintA, fontTintR, fontTintG, fontTintB);
        layout.draw(canvas);
        
        CrossAppBitmap.initNativeObject(bitmap);
        return true;
    }
    
    
    
    /**
     * 
     * @param tvPaint
     * @param rawText
     * @param tvWidth
     * @return
     */
	public static String autoSplitText( Paint tvPaint , String rawText ,  float tvWidth) {
		
		// ��ԭʼ�ı����в��
		String[] rawTextLines = rawText.replaceAll("\r", "").split("\n");
		StringBuilder sbNewText = new StringBuilder();
		for (String rawTextLine : rawTextLines) {
			if (tvPaint.measureText(rawTextLine) <= tvWidth) {
				// ������п���ڿؼ����ÿ��֮�ڣ��Ͳ�������
				sbNewText.append(rawTextLine);
			} else {
				// ������п�ȳ����ؼ����ÿ�ȣ����ַ��������ڳ������ÿ�ȵ�ǰһ���ַ����ֶ�����
				float lineWidth = 0;
				for (int cnt = 0; cnt != rawTextLine.length(); ++cnt) {
					char ch = rawTextLine.charAt(cnt);
					lineWidth += tvPaint.measureText(String.valueOf(ch));
					if (lineWidth <= tvWidth) {
						sbNewText.append(ch);
					} else {
						sbNewText.append("\n");
						lineWidth = 0;
						--cnt;
					}
				}
			}
			sbNewText.append("\n");
		}

		// �ѽ�β�����\nȥ��
		if (!rawText.endsWith("\n")) {
			sbNewText.deleteCharAt(sbNewText.length() - 1);
		}

		return sbNewText.toString();
	}
    

  public static float heightForFont(int fontSize)
  {
  
	  Paint p = new Paint();  
      p.setTextSize(fontSize);  
      FontMetrics fm = p.getFontMetrics();  
      
      int textHeight = (int) (Math.ceil(fm.descent - fm.ascent) + 2);  
	  
      return textHeight;
  }
  
  public static float heightForTextAtWidth(byte[] stringbyte , int width , int fontsize ,float lineSpacing)
  {	  
      String string=new String(stringbyte);
	  TextPaint paint = new TextPaint();
      paint.setTextSize(fontsize);
	  Layout layout = new StaticLayout(string, paint, width , Layout.Alignment.ALIGN_CENTER,1.0f,lineSpacing,false);
	  
      return layout.getHeight();
  }
  
  public static float widthForTextAtOneLine(byte[] stringbyte ,  int fontSize)
  {
      String string=new String(stringbyte);
      TextPaint textPaint = new TextPaint();
      textPaint.setTextSize(fontSize);//设置字体大小
      return Layout.getDesiredWidth(string,textPaint)+1 ;//防止Android上显示不全 
  }

    private static TextPaint newPaint(final String fontName, final int fontSize) {
        final TextPaint paint = new TextPaint();
        paint.setTextSize(fontSize);
        paint.setAntiAlias(true);

        // Set type face for paint, now it support .ttf file.
        if (fontName != null && fontName.endsWith(".ttf")) {
            try {
                final Typeface typeFace = CrossAppTypefaces.get(
                        CrossAppBitmap.sContext, fontName);
                paint.setTypeface(typeFace);
            } catch (final Exception e) {
                Log.e("Cocos2dxBitmap", "error to create ttf type face: "
                        + fontName);

                // The file may not find, use system font.
                paint.setTypeface(Typeface.create(fontName, Typeface.NORMAL));
            }
        } else {
            paint.setTypeface(Typeface.create(fontName, Typeface.NORMAL));
        }
        return paint;
    }
    
    private static void initNativeObject(final Bitmap bitmap) {
        final byte[] pixels = CrossAppBitmap.getPixels(bitmap);
        if (pixels == null) {
            return;
        }
        
        CrossAppBitmap.nativeInitBitmapDC(bitmap.getWidth(),
                bitmap.getHeight(), pixels);
    }

    private static byte[] getPixels(final Bitmap bitmap) {
        if (bitmap != null) {
            final byte[] pixels = new byte[bitmap.getWidth()
                    * bitmap.getHeight() * 4];
            final ByteBuffer buf = ByteBuffer.wrap(pixels);
            buf.order(ByteOrder.nativeOrder());
            bitmap.copyPixelsToBuffer(buf);
            return pixels;
        }

        return null;
    }

    public static int getFontSizeAccordingHeight(int height) {
        TextPaint paint = new TextPaint();
        Rect bounds = new Rect();

        paint.setTypeface(Typeface.DEFAULT);
        int text_size = 1;
        boolean found_desired_size = false;

        while (!found_desired_size) {
            paint.setTextSize(text_size);
            String text = "SghMNy";
            paint.getTextBounds(text, 0, text.length(), bounds);

            text_size++;

            if (height - bounds.height() <= 2) {
                found_desired_size = true;
            }
        }
        return text_size;
    }

    private static String getStringWithEllipsis(String string, float width, float fontSize) {
        if (TextUtils.isEmpty(string)) {
            return "";
        }

        TextPaint paint = new TextPaint();
        paint.setTypeface(Typeface.DEFAULT);
        paint.setTextSize(fontSize);

        return TextUtils.ellipsize(string, paint, width,
                TextUtils.TruncateAt.END).toString();
    }
    
    private static int myEllipsize(StaticLayout layout,TextPaint tp, CharSequence cs, int line, int lineWidth) {
    	  int count = layout.getLineCount();
    	  int pos = -1;

    	  if (count > line) {
    	    int start = layout.getLineStart(line - 1);
    	    final int range[] = {0};
    	    TextUtils.ellipsize(cs.subSequence(start, cs.length()), tp, lineWidth,
    	        TextUtils.TruncateAt.END, false, new TextUtils.EllipsizeCallback() {
    	          @Override
    	          public void ellipsized(int start, int end) {
    	            range[0] = start;  // �����ı�������ʼ
    	          }
    	        });
    	    pos = start + range[0];
    	  }
    	  return pos;
    	}
}
