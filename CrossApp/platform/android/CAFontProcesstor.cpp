
#include "platform/CAFontProcesstor.h"
#include "images/CAImage.h"
#include "platform/CAFileUtils.h"
#include "support/ccUtils.h"
#include "jni/JniHelper.h"
#include <jni.h>
#include <string.h>
#include <vector>
#include <stdio.h>

NS_CC_BEGIN

std::vector<CAData*> s_gDataVec;
DSize s_gDimensions;

extern "C"
{
    /**
     * this method is called by java code to init width, height and pixels data
     */
    JNIEXPORT void JNICALL Java_org_CrossApp_lib_CrossAppBitmap_nativeInitBitmapDC(JNIEnv*  env, jobject thiz, int width, int height, jbyteArray pixels)
    {
        s_gDimensions = DSize(width, height);
        ssize_t length = width * height * 4;
        unsigned char *bytes = (unsigned char*)malloc(sizeof(unsigned char) * length);
        env->GetByteArrayRegion(pixels, 0, length, (jbyte*)bytes);
        
        CAData* data = new CAData();
        data->fastSet(bytes, length);
        s_gDataVec.push_back(data);
    }
};

int getTextAlign(const CAFont& font, CATextAlignment textAlignment)
{
    int a = 0x31 ;
    
    CATextAlignment h = textAlignment ;
    CAVerticalTextAlignment v = CAVerticalTextAlignment::Top ;
    
    //CENTER        = 0x33, /** Horizontal center and vertical center. */
    if(h == CATextAlignment::Center && v == CAVerticalTextAlignment::Center)
    {
        a = 0x33 ;
    }
    //TOP           = 0x13, /** Horizontal center and vertical top. */
    if(h == CATextAlignment::Center && v == CAVerticalTextAlignment::Top)
    {
        a = 0x13 ;
    }
    //TOP_RIGHT     = 0x12, /** Horizontal right and vertical top. */
    else if(h == CATextAlignment::Right && v == CAVerticalTextAlignment::Top)
    {
        a = 0x12 ;
    }
    //RIGHT         = 0x32, /** Horizontal right and vertical center. */
    else if(h == CATextAlignment::Right && v == CAVerticalTextAlignment::Center)
    {
        a = 0x32 ;
    }
    //BOTTOM_RIGHT  = 0x22, /** Horizontal right and vertical bottom. */
    else if(h == CATextAlignment::Right && v == CAVerticalTextAlignment::Bottom)
    {
        a = 0x22 ;
    }
    //BOTTOM        = 0x23, /** Horizontal center and vertical bottom. */
    else if(h == CATextAlignment::Center && v == CAVerticalTextAlignment::Bottom)
    {
        a = 0x23 ;
    }
    //BOTTOM_LEFT   = 0x21, /** Horizontal left and vertical bottom. */
    else if(h == CATextAlignment::Left && v == CAVerticalTextAlignment::Bottom)
    {
        a = 0x21 ;
    }
    //LEFT          = 0x31, /** Horizontal left and vertical center. */
    else if(h == CATextAlignment::Left && v == CAVerticalTextAlignment::Center)
    {
        a = 0x31 ;
    }
    //TOP_LEFT      = 0x11, /** Horizontal left and vertical top. */
    else if(h == CATextAlignment::Left && v == CAVerticalTextAlignment::Top)
    {
        a = 0x11 ;
    }
    return  a ;
}

void dipFontToPxFont(CAFont& font)
{
    font.fontSize = s_dip_to_px(font.fontSize);
    font.lineSpacing = ceilf(s_dip_to_px(font.lineSpacing));
    font.shadow.shadowOffset.width = s_dip_to_px(font.shadow.shadowOffset.width) /2;
    font.shadow.shadowOffset.height = s_dip_to_px(font.shadow.shadowOffset.height) /2;
    font.shadow.shadowBlur = s_dip_to_px(font.shadow.shadowBlur);
    font.stroke.strokeSize = s_dip_to_px(font.stroke.strokeSize);
}

CAImage* CAFontProcesstor::imageForText(const std::string& text, CAFont font, DSize& dimensions, CATextAlignment textAlignment)
{
    CAImage* ret = nullptr;
    
    do
    {
        CC_BREAK_IF(text.empty());

        dipFontToPxFont(font);
        dimensions = DSize(s_dip_to_px(dimensions.width), s_dip_to_px(dimensions.height));
        
        JniMethodInfo methodInfo;
        if (! JniHelper::getStaticMethodInfo(methodInfo, "org/CrossApp/lib/CrossAppBitmap", "createTextBitmapShadowStroke", "([BLjava/lang/String;IIIIIIIIIFFFIIIIIIIIIFIIIIFIF)Z"))
        {
            break ;
        }
        // Do a full lookup for the font path using FileUtils in case the given font name is a relative path to a font file asset,
        // or the path has been mapped to a different location in the app package:
        std::string fullPathOrFontName = font.fontName;
        if(FileUtils::getInstance()->isFileExist(fullPathOrFontName)) {
            fullPathOrFontName = FileUtils::getInstance()->fullPathForFilename(font.fontName);
            // If the path name returned includes the 'assets' dir then that needs to be removed, because the android.content.Context
            // requires this portion of the path to be omitted for assets inside the app package.
            if (fullPathOrFontName.find("assets/") == 0)
            {
                fullPathOrFontName = fullPathOrFontName.substr(strlen("assets/"));   // Chop out the 'assets/' portion of the path.
            }
        }
        
        
        jstring jstrFont = methodInfo.env->NewStringUTF(fullPathOrFontName.c_str());
        
        int count = text.length();
        jbyteArray bytes = methodInfo.env->NewByteArray(count);
        methodInfo.env->SetByteArrayRegion(bytes, 0, count, reinterpret_cast<const jbyte*>(text.c_str()));
        
        if(!methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID,
                                                    bytes,
                                                    jstrFont,
                                                    (int)font.fontSize,
                                                    font.color.r,
                                                    font.color.g,
                                                    font.color.b,
                                                    font.color.a,
                                                    getTextAlign(font, textAlignment),
                                                    (int)dimensions.width,
                                                    (int)dimensions.height,
                                                    (int)font.shadow.shadowEnabled,
                                                    font.shadow.shadowOffset.width,
                                                    font.shadow.shadowOffset.height,
                                                    font.shadow.shadowBlur,
                                                    font.shadow.shadowColor.r,
                                                    font.shadow.shadowColor.g,
                                                    font.shadow.shadowColor.b,
                                                    font.shadow.shadowColor.a,
                                                    (int)font.stroke.strokeEnabled,
                                                    font.stroke.strokeColor.r,
                                                    font.stroke.strokeColor.g,
                                                    font.stroke.strokeColor.b,
                                                    font.stroke.strokeColor.a,
                                                    font.stroke.strokeSize,
                                                    (int)font.bold,
                                                    (int)font.underLine,
                                                    (int)font.deleteLine ,
                                                    (int)font.italics,
                                                    font.italicsValue ,
                                                    (int)font.wordWrap,
                                                    font.lineSpacing
                                                    ))
        {
            break ;
        }
        
        methodInfo.env->DeleteLocalRef(jstrFont);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        methodInfo.env->DeleteLocalRef(bytes);
        
        dimensions = DSize(s_px_to_dip(s_gDimensions.width), s_px_to_dip(s_gDimensions.height));
        
        CAData* data = s_gDataVec.front();
        s_gDataVec.clear();
        ret = CAImage::createWithRawDataNoCache(data, CAImage::PixelFormat::RGBA8888, s_gDimensions.width, s_gDimensions.height, true);
        data->release();
    } while (0);
    
    
    return ret;
}


CAImage* CAFontProcesstor::imageForRichText(const std::vector<CARichLabel::Element>& elements, DSize& dimensions, CATextAlignment textAlignment)
{
    CAImage* ret = nullptr;
    
    do
    {
        CC_BREAK_IF(elements.empty());
        
        const char* model_font = "<font color='#%s' size='%.0f'>%s</font>" ;
        const char* model_border = "<border color='#%s' width='%.0f'>%s</border>" ;
        const char* model_shadow = "<shadow color='#%s'>%s</shadow>";
        const char* model_u = "<u>%s</u>" ;
        const char* model_del = "<del>%s</del>";
        const char* model_i = "<i italic='%f'>%s</i>";
        const char* model_b = "<b>%s</b>" ;
        
        std::string html_format ;
        
        for (auto& var : elements)
        {
            std::string text = var.text;
            CAFont font = var.font;
            dipFontToPxFont(font);
            
            std::string frontSpace;
            int frontSpaceCount = 0;
            for (auto& space : text)
            {
                CC_BREAK_IF(space != ' ');
                ++frontSpaceCount;
                frontSpace += "&nbsp;";
            }
            text = frontSpace + text.substr(frontSpaceCount, text.length() - frontSpaceCount);
            
            if (font.italics)
            {
                int increase = ceilf(font.italicsValue / 0.5f);
                
                if (increase > 0)
                {
                    for (int i=0; i<increase; i++)
                    {
                        text = text + " ";
                    }
                }
                
                if (increase < 0)
                {
                    for (int i=0; i<std::abs(increase); i++)
                    {
                        text = " " + text;
                    }
                }
            }
            
            std::string html_str ;
            //add font tag
            html_str = crossapp_format_string(model_font, int2hex(font.color.getUInt32()%0x1000000).c_str(),font.fontSize,text.c_str());
            
            
            //add border tag
            if (font.stroke.strokeEnabled) {
                html_str = crossapp_format_string(model_border, int2hex(font.stroke.strokeColor.getUInt32()%0x1000000).c_str(),font.stroke.strokeSize , model_font) ;
            }
            
            //add shadow tag
            if (font.shadow.shadowEnabled) {
                html_str = crossapp_format_string(model_shadow , int2hex(font.shadow.shadowColor.getUInt32()%0x1000000).c_str(),html_str.c_str()) ;
            }
            
            //add u tag
            if (font.underLine) {
                html_str = crossapp_format_string(model_u , html_str.c_str()) ;
            }
            
            //add del tag
            if (font.deleteLine) {
                html_str = crossapp_format_string(model_del , html_str.c_str()) ;
            }
            
            //add i tag
            if (font.italics) {
                html_str = crossapp_format_string(model_i ,font.italicsValue, html_str.c_str()) ;
            }
            
            //add b tag
            if (font.bold) {
                html_str = crossapp_format_string(model_b ,html_str.c_str()) ;
            }
            
            html_format = html_format + html_str ;
        }
        
        ret = imageForText(html_format, elements.front().font, dimensions, textAlignment) ;

    }
    while (0);
    
    return ret;
}


float CAFontProcesstor::heightForFont(const CAFont& font)
{
    //public static float heightForFont(int fontSize)
    float ret = 0 ;
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppBitmap", "heightForFont", "(I)F"))
    {
        int fontSize = (int)s_dip_to_px(font.fontSize);
        ret = (float)(jfloat)jni.env->CallStaticFloatMethod(jni.classID, jni.methodID, fontSize);
        ret = ceilf(s_px_to_dip(ret));
        jni.env->DeleteLocalRef(jni.classID);
    }
    return ret;
}

float CAFontProcesstor::heightForTextAtWidth(const std::string& text, const CAFont& font, float width)
{
    float ret = 0;
    CCLog("heightForTextAtWidth:  width %f, fontSize %f, lineSpacing %f", width, font.fontSize, font.lineSpacing);
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppBitmap", "heightForTextAtWidth", "([BIIF)F"))
    {
        //modify by zmr 改为byte传递 解决text中包含emoji时，引起的jni闪退问题
        //jstring string = jni.env->NewStringUTF(text.c_str());
        
        int count = text.length();
        jbyteArray bytes = jni.env->NewByteArray(count);
        jni.env->SetByteArrayRegion(bytes, 0, count, reinterpret_cast<const jbyte*>(text.c_str()));
        
        int w = (int)s_dip_to_px(width);
        int fontSize = (int)s_dip_to_px(font.fontSize);
        float lineSpacing = ceilf(s_dip_to_px(font.lineSpacing));
        ret = (float)(jfloat)jni.env->CallStaticFloatMethod(jni.classID, jni.methodID, bytes, w, fontSize, lineSpacing);
        ret = ceilf(s_px_to_dip(ret));
        jni.env->DeleteLocalRef(jni.classID);
        jni.env->DeleteLocalRef(bytes);
    }
    return ret;
}

float CAFontProcesstor::widthForTextAtOneLine(const std::string& text, const CAFont& font)
{
    //public static float widthForTextAtOneLine(String string ,  int fontSize)
    float ret = 0 ;
    JniMethodInfo jni;
    if (JniHelper::getStaticMethodInfo(jni, "org/CrossApp/lib/CrossAppBitmap", "widthForTextAtOneLine", "([BI)F"))
    {
        //modify by zmr 改为byte传递 解决text中包含emoji时，引起的jni闪退问题
        int count = text.length();
        jbyteArray bytes = jni.env->NewByteArray(count);
        jni.env->SetByteArrayRegion(bytes, 0, count, reinterpret_cast<const jbyte*>(text.c_str()));
        
        //jstring string = jni.env->NewStringUTF(text.c_str());
        int fontSize = (int)font.fontSize;
        ret = (float)(jfloat)jni.env->CallStaticFloatMethod(jni.classID, jni.methodID, bytes, fontSize);
//        ret = ceilf(s_px_to_dip(ret));
        jni.env->DeleteLocalRef(jni.classID);
        jni.env->DeleteLocalRef(bytes);
    }
    
    return ret;
}



NS_CC_END


