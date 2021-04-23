package org.CrossApp.lib;

import java.util.ArrayDeque;
import java.util.Stack;

import org.xml.sax.Attributes;
import org.xml.sax.ContentHandler;
import org.xml.sax.Locator;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;

import android.graphics.Color;
import android.graphics.Typeface;
import android.os.Parcel;
import android.text.Editable;
import android.text.Html;
import android.text.ParcelableSpan;
import android.text.Spanned;
import android.text.TextPaint;
import android.text.style.AbsoluteSizeSpan;
import android.text.style.CharacterStyle;
import android.text.style.StrikethroughSpan;
import android.text.style.UpdateAppearance;

/**
 * Created by liuguoyan on 2017/6/13.
 * 
 * <br/>
 * <hr/>
 * Samples:<br/>
 * <shadow color='#00ff00'><font color='#ff6c00' size='80'> <border  color='#cccccc' width='2'>1500/天</border></font></shadow> 
 * <del><font  color='#808080' size='50'>原价:20000元 </font></del>
 * <font>我是谁？</font>
 * 
 * <br/>
 * <hr/>
 * usage:<br/>
 * txtview.setText(HtmlParser.buildSpannedText(htmlStr,new CrossAppTagHandler()));  
 */

public class CrossAppTagHandler implements HtmlParser.TagHandler {
    /**
     * html 标签的开始下标
     */
    private Stack<Integer> startIndex;

    private Stack<String> propertyValue;

    @Override
    public boolean handleTag(boolean opening, String tag, Editable output, Attributes attributes) {
        if (opening) {
            handlerStartTAG(tag, output, attributes);
        } else {
            handlerEndTAG(tag, output, attributes);
        }
        return handlerBYDefault(tag);
    }
    
    
    /****  handlerStartTAG   & handlerEndTAG  ****/
    
    private void handlerStartTAG(String tag, Editable output, Attributes attributes) {
        if (tag.equalsIgnoreCase("font")) {
            handlerStartFONT(output, attributes);
        } else if (tag.equalsIgnoreCase("del")) {
            handlerStartDEL(output);
        }else if (tag.equalsIgnoreCase("shadow")) {
			handlerStartShadow(output, attributes);
		}else if (tag.equalsIgnoreCase("border")) {
			handlerStartBorder(output, attributes);
		}else if (tag.equalsIgnoreCase("i")) {
			handlerStartItalic(output, attributes);
		}
    }
    
    
    private void handlerEndTAG(String tag, Editable output, Attributes attributes) {
        if (tag.equalsIgnoreCase("font")) {
            handlerEndFONT(output);
        } else if (tag.equalsIgnoreCase("del")) {
            handlerEndDEL(output);
        }else if (tag.equalsIgnoreCase("shadow")) {
			handlerEndShadow(output);
		}else if (tag.equalsIgnoreCase("border")) {
			handlerEndBorder(output);
		}else if (tag.equalsIgnoreCase("i")) {
			handlerEndItalic(output); 
		}
    }
    
    /**>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    
    
    /****  handlerStartFONT  & handlerEndFONT  ****/
    private void handlerStartFONT(Editable output, Attributes attributes) {
        if (startIndex == null) {
            startIndex = new Stack();
        }
        startIndex.push(output.length());
        
        if (propertyValue == null) {
            propertyValue = new Stack();
        }
        
        propertyValue.push(HtmlParser.getValue(attributes, "size"));
    }  
    
    private void handlerEndFONT(Editable output) {
        if (!propertyValue.isEmpty()) {
            try {
                int value = Integer.parseInt(propertyValue.pop());
                output.setSpan(new AbsoluteSizeSpan(value), startIndex.pop(), output.length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    /**>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    
    
    
    /****  handlerStartDEL  & handlerEndDEL  ****/
    private void handlerStartDEL(Editable output) {
        if (startIndex == null) {
            startIndex = new Stack();
        }
        startIndex.push(output.length());
    }
    
    private void handlerEndDEL(Editable output) {
        output.setSpan(new StrikethroughSpan(), startIndex.pop(), output.length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
    }
    
    /**>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    
    
    /****  handlerStartShadow  & handlerEndShadow  ****/
    //<shadow color=“#0f0” />阴影
    private void handlerStartShadow(Editable output, Attributes attributes) {
        if (startIndex == null) {
            startIndex = new Stack();
        }
        startIndex.push(output.length());
        
        if (propertyValue == null) {
            propertyValue = new Stack();
        }
        
        String color = HtmlParser.getValue(attributes, "color") ; 
        String col = color == null ? "-1" : String.valueOf(Color.parseColor(color));
        
        propertyValue.push(color == null ? "-1" : col);
    }
    
    private void handlerEndShadow(Editable output) {
        if (!propertyValue.isEmpty()) {
            try {
                int value = Integer.parseInt(propertyValue.pop());
                output.setSpan(new ShadowSpan(value), startIndex.pop(), output.length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    /**>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    
    
    /****  handlerStartBorder  & handlerEndBorder  ****/
    //<border color=“#0f0”width="2" />空心字
    private void handlerStartBorder(Editable output, Attributes attributes) {
        if (startIndex == null) {
            startIndex = new Stack();
        }
        startIndex.push(output.length());
        
        if (propertyValue == null) {
            propertyValue = new Stack();
        }
        
        String color = HtmlParser.getValue(attributes, "color") ; 
        String col = color == null ? "-1" : String.valueOf(Color.parseColor(color));
        propertyValue.push(color == null ? "-1" : col);
        
        String width = HtmlParser.getValue(attributes, "width") ; 
        propertyValue.push(width == null ? "-1" : width) ; 
        
    }
    
    private void handlerEndBorder(Editable output) {
        if (!propertyValue.isEmpty()) {
            try {
            	int width = Integer.parseInt(propertyValue.pop());
                int value = Integer.parseInt(propertyValue.pop());
                output.setSpan(new BorderSpan(value, width), startIndex.pop(), output.length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    /**>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    
    
    
    
    /****  handlerStartItalic  & handlerEndItalic  ****/
    private void handlerStartItalic(Editable output, Attributes attributes) {
        if (startIndex == null) {
            startIndex = new Stack();
        }
        startIndex.push(output.length());
        
        if (propertyValue == null) {
            propertyValue = new Stack();
        }
        
        propertyValue.push(HtmlParser.getValue(attributes, "italic"));
    }
    
    private void handlerEndItalic(Editable output) {
        if (!propertyValue.isEmpty()) {
            try {
                float value = Float.parseFloat(propertyValue.pop());
                output.setSpan(new ItalicSpan(value), startIndex.pop(), output.length(), Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    /**>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    
    
    
    
    
    
    /**
     * 返回true表示不交给系统后续处理
     * false表示交给系统后续处理
     *
     * @param tag
     * @return
     */
    private boolean handlerBYDefault(String tag) {
        if (tag.equalsIgnoreCase("del")) {
            return true;
        }
        return false;
    }
}

/*******************************************************************************************************/
/**********************************自定义的CharacterStyle*****************************************/
/*******************************************************************************************************/

/**
 * 
 * border
 * @author liuguoyan
 *
 */
class BorderSpan extends CharacterStyle implements UpdateAppearance ,ParcelableSpan{
	
	private int mColor;
	
	private int borderSize ; 
	
	public BorderSpan(int color ,int borderSize) {
		this.mColor = mColor == -1 ? Color.BLACK : mColor ;
		this.borderSize = borderSize == -1 ? 2 : borderSize ; 
	}
	
	@Override
	public void updateDrawState(TextPaint tp) {
		tp.setStyle(TextPaint.Style.STROKE);
		tp.setStrokeWidth(borderSize);
	}
	
	@Override
	public int describeContents() {
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
	}

	@Override
	public int getSpanTypeId() {
		return Spanned.SPAN_EXCLUSIVE_EXCLUSIVE;
	}
}


/**
 * shadow
 * @author liuguoyan
 */
class ShadowSpan extends CharacterStyle implements UpdateAppearance ,ParcelableSpan{
	
	private int mColor;
	
	public ShadowSpan(int color) {
		this.mColor = color ;
	}
	
	@Override
	public void updateDrawState(TextPaint tp) {
		tp.setShadowLayer(2 , 3 , 3, mColor == -1 ? Color.GRAY : mColor);
	}
	
	@Override
	public int describeContents() {
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
	}

	@Override
	public int getSpanTypeId() {
		return Spanned.SPAN_EXCLUSIVE_EXCLUSIVE;
	}
	
}


/**
 * i
 * @author liuguoyan
 */
class ItalicSpan extends CharacterStyle implements UpdateAppearance ,ParcelableSpan{
	
	private float italic;
	
	public ItalicSpan(float italic) {
		this.italic = italic ;
	}
	
	@Override
	public void updateDrawState(TextPaint tp) {
		tp.setTextSkewX(-this.italic);
	}
	
	@Override
	public int describeContents() {
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
	}

	@Override
	public int getSpanTypeId() {
		return Spanned.SPAN_EXCLUSIVE_EXCLUSIVE;
	}
	
}



/*******************************************************************************************************/
/****************************************自定义的HtmlParser****************************************/
/*******************************************************************************************************/

/**
 * 
 * Created by liuguoyan on 2017/6/13.
 *
 */
class HtmlParser implements Html.TagHandler, ContentHandler
{
    //This approach has the advantage that it allows to disable processing of some tags while using default processing for others,
    // e.g. you can make sure that ImageSpan objects are not created:
    public interface TagHandler
    {
        // return true here to indicate that this tag was handled and
        // should not be processed further
        boolean handleTag(boolean opening, String tag, Editable output, Attributes attributes);
    }

    public static Spanned buildSpannedText(String html, TagHandler handler)
    {
        // add a tag at the start that is not handled by default,
        // allowing custom tag handler to replace xmlReader contentHandler
        return Html.fromHtml("<inject/>" + html, null, new HtmlParser(handler));
    }

    public static String getValue(Attributes attributes, String name)
    {
        for (int i = 0, n = attributes.getLength(); i < n; i++)
        {
            if (name.equals(attributes.getLocalName(i)))
                return attributes.getValue(i);
        }
        return null;
    }

    private final TagHandler handler;
    private ContentHandler wrapped;
    private Editable text;
    private ArrayDeque<Boolean> tagStatus = new ArrayDeque();

    private HtmlParser(TagHandler handler)
    {
        this.handler = handler;
    }

    @Override
    public void handleTag(boolean opening, String tag, Editable output, XMLReader xmlReader)
    {
        if (wrapped == null)
        {
            // record result object
            text = output;

            // record current content handler
            wrapped = xmlReader.getContentHandler();

            // replace content handler with our own that forwards to calls to original when needed
            xmlReader.setContentHandler(this);

            // handle endElement() callback for <inject/> tag
            tagStatus.addLast(Boolean.FALSE);
        }
    }

    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes)
            throws SAXException
    {
        boolean isHandled = handler.handleTag(true, localName, text, attributes);
        tagStatus.addLast(isHandled);
        if (!isHandled)
            wrapped.startElement(uri, localName, qName, attributes);
    }

    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException
    {
        if (!tagStatus.removeLast())
            wrapped.endElement(uri, localName, qName);
        handler.handleTag(false, localName, text, null);
    }

    @Override
    public void setDocumentLocator(Locator locator)
    {
        wrapped.setDocumentLocator(locator);
    }

    @Override
    public void startDocument() throws SAXException
    {
        wrapped.startDocument();
    }

    @Override
    public void endDocument() throws SAXException
    {
        wrapped.endDocument();
    }

    @Override
    public void startPrefixMapping(String prefix, String uri) throws SAXException
    {
        wrapped.startPrefixMapping(prefix, uri);
    }

    @Override
    public void endPrefixMapping(String prefix) throws SAXException
    {
        wrapped.endPrefixMapping(prefix);
    }

    @Override
    public void characters(char[] ch, int start, int length) throws SAXException
    {
        wrapped.characters(ch, start, length);
    }

    @Override
    public void ignorableWhitespace(char[] ch, int start, int length) throws SAXException
    {
        wrapped.ignorableWhitespace(ch, start, length);
    }

    @Override
    public void processingInstruction(String target, String data) throws SAXException
    {
        wrapped.processingInstruction(target, data);
    }

    @Override
    public void skippedEntity(String name) throws SAXException
    {
        wrapped.skippedEntity(name);
    }
}

