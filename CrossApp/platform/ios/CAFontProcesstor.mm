

#include "CAFontProcesstor.h"
#include "images/CAImage.h"
#import <UIKit/UIKit.h>

#if !defined(CC_TARGET_OS_TVOS)
#import<CoreMotion/CoreMotion.h>
#endif
#import<CoreFoundation/CoreFoundation.h>
#import <CoreText/CoreText.h>

#import <AudioToolbox/AudioToolbox.h>

static id _createSystemFont(const CrossApp::CAFont& font)
{
    float shrinkFontSize = (font.fontSize);
    
    NSString * fntName      = [NSString stringWithUTF8String:font.fontName.c_str()];
    // On iOS custom fonts must be listed beforehand in the App info.plist (in order to be usable) and referenced only the by the font family name itself when
    // calling [UIFont fontWithName]. Therefore even if the developer adds 'SomeFont.ttf' or 'fonts/SomeFont.ttf' to the App .plist, the font must
    // be referenced as 'SomeFont' when calling [UIFont fontWithName]. Hence we strip out the folder path components and the extension here in order to get just
    // the font family name itself. This stripping step is required especially for references to user fonts stored in CCB files; CCB files appear to store
    // the '.ttf' extensions when referring to custom fonts.
    fntName = [[fntName lastPathComponent] stringByDeletingPathExtension];
    
    // create the font
    UIFont* iosFont = [UIFont fontWithName:fntName size:shrinkFontSize];
    
    if (!iosFont)
    {
        iosFont = [UIFont fontWithName:@"Heiti SC" size:shrinkFontSize];
    }

    return iosFont;
}

NSTextAlignment _calculateTextAlignment(CrossApp::CATextAlignment alignment)
{
    NSTextAlignment nsAlignment;
    switch (alignment)
    {
        case CrossApp::CATextAlignment::Right:
            nsAlignment = NSTextAlignmentRight;
        case CrossApp::CATextAlignment::Center:
            nsAlignment = NSTextAlignmentCenter;
            break;
        case CrossApp::CATextAlignment::Left:
        default:
            nsAlignment = NSTextAlignmentLeft;
            break;
    }
    
    return nsAlignment;
}

CGRect _calculateStringRect(NSAttributedString *str, id font, CGSize constrainSize, bool enableWrap, float fontSize)
{
    CGSize textRect = CGSizeZero;
    textRect.width = constrainSize.width > 0 ? constrainSize.width
    : CGFLOAT_MAX;
    textRect.height = constrainSize.height > 0 ? constrainSize.height
    : CGFLOAT_MAX;

    CGRect dim;
    dim.origin = CGPointZero;
    dim.size = [str boundingRectWithSize:CGSizeMake(textRect.width, textRect.height)
                            options:(NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingTruncatesLastVisibleLine)
                            context:nil].size;
    
    dim.size.width = ceilf(dim.size.width);
    dim.size.height = MIN(ceilf(dim.size.height), constrainSize.height);
    
    return dim;
}

NS_CC_BEGIN

void dipFontToPxFont(CAFont& font)
{
    font.fontSize = s_dip_to_px(font.fontSize);
    font.lineSpacing = s_dip_to_px(font.lineSpacing);
    font.shadow.shadowOffset.width = s_dip_to_px(font.shadow.shadowOffset.width) /2;
    font.shadow.shadowOffset.height = s_dip_to_px(font.shadow.shadowOffset.height) /2;
    font.shadow.shadowBlur = s_dip_to_px(font.shadow.shadowBlur);
    font.stroke.strokeSize = s_dip_to_px(font.stroke.strokeSize) / [[UIScreen mainScreen] scale];
}

NSAttributedString* NSAttributedStringForText(const std::string& text, const CAFont& font, const DSize& dim, CATextAlignment textAlignment)
{
    NSString * str = [NSString stringWithUTF8String:text.c_str()];
    
    if (font.italics)
    {
        int increase = ceilf(font.italicsValue / 0.5f);
        
        if (increase > 0)
        {
            for (int i=0; i<increase; i++)
            {
                str = [str stringByAppendingString:@" "];
            }
        }
        
        if (increase < 0)
        {
            for (int i=0; i<std::abs(increase); i++)
            {
                str = [NSString stringWithFormat:@" %@", str];
            }
        }
    }
    
    float shrinkFontSize = (font.fontSize);
    id iosfont = _createSystemFont(font);
    
    // color
    UIColor* foregroundColor = [UIColor colorWithRed:font.color.r / 255.f
                                               green:font.color.g / 255.f
                                                blue:font.color.b / 255.f
                                               alpha:font.color.a / 255.f];
    
    NSTextAlignment textAlign = _calculateTextAlignment(textAlignment);
    
    NSMutableParagraphStyle *paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
    
    [paragraphStyle setLineBreakMode:font.wordWrap ? NSLineBreakByWordWrapping : NSLineBreakByCharWrapping];

    [paragraphStyle setLineSpacing:font.lineSpacing];
    [paragraphStyle setAlignment:textAlign];
    
    // adjust text rect according to overflow
    NSMutableDictionary* tokenAttributesDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                foregroundColor,    NSForegroundColorAttributeName,
                                                iosfont,            NSFontAttributeName,
                                                paragraphStyle,     NSParagraphStyleAttributeName,
                                                nil];
    
    
    if (font.bold)
    {
        [tokenAttributesDict setObject:@(-shrinkFontSize / 20.f) forKey:NSStrokeWidthAttributeName];
        [tokenAttributesDict setObject:foregroundColor forKey:NSStrokeColorAttributeName];
    }
    
    if (font.italics)
    {
        [tokenAttributesDict setObject:@(font.italicsValue) forKey:NSObliquenessAttributeName];
    }
    
    if (font.underLine)
    {
        [tokenAttributesDict setObject:@(NSUnderlineStyleSingle) forKey:NSUnderlineStyleAttributeName];
        /*
         if (font.underLineColor != CAColor4B::CLEAR)
         {
         UIColor* underLineColor = [UIColor colorWithRed:font.underLineColor.r/255.0
         green:font.underLineColor.g/255.0
         blue:font.underLineColor.b/255.0
         alpha:font.underLineColor.a/255.0];
         [tokenAttributesDict setObject:underLineColor forKey:NSUnderlineColorAttributeName];
         }
         */
    }
    
    if (font.deleteLine)
    {
        [tokenAttributesDict setObject:@(NSUnderlineStyleSingle) forKey:NSStrikethroughStyleAttributeName];
        /*
         if (font.deleteLineColor != CAColor4B::CLEAR)
         {
         UIColor* deleteLineColor = [UIColor colorWithRed:font.deleteLineColor.r/255.0
         green:font.deleteLineColor.g/255.0
         blue:font.deleteLineColor.b/255.0
         alpha:font.deleteLineColor.a/255.0];
         [tokenAttributesDict setObject:deleteLineColor forKey:NSStrikethroughColorAttributeName];
         }
         */
    }
    
    if (font.stroke.strokeEnabled)
    {
        if (font.color == CAColor4B::CLEAR)
        {
            foregroundColor = [UIColor colorWithRed:font.stroke.strokeColor.r / 255.f
                                              green:font.stroke.strokeColor.g / 255.f
                                               blue:font.stroke.strokeColor.b / 255.f
                                              alpha:font.stroke.strokeColor.a / 255.f];
            
            
            [tokenAttributesDict setObject:@(fabsf(font.stroke.strokeSize)) forKey:NSStrokeWidthAttributeName];
            [tokenAttributesDict setObject:foregroundColor forKey:NSStrokeColorAttributeName];
        }
        else
        {
            UIColor *strokeColor = [UIColor colorWithRed:font.stroke.strokeColor.r / 255.f
                                                   green:font.stroke.strokeColor.g / 255.f
                                                    blue:font.stroke.strokeColor.b / 255.f
                                                   alpha:font.stroke.strokeColor.a / 255.f];
            
            
            [tokenAttributesDict setObject:@(-fabsf(font.stroke.strokeSize)) forKey:NSStrokeWidthAttributeName];
            [tokenAttributesDict setObject:strokeColor forKey:NSStrokeColorAttributeName];
        }
    }
    
    if (font.shadow.shadowEnabled)
    {
        NSShadow* shadow = [[[NSShadow alloc] init] autorelease];
        
        [shadow setShadowOffset:CGSizeMake(font.shadow.shadowOffset.width, font.shadow.shadowOffset.height)];
        [shadow setShadowBlurRadius:font.shadow.shadowBlur];
        
        UIColor* shadowColor = [UIColor colorWithRed:font.shadow.shadowColor.r / 255.f
                                               green:font.shadow.shadowColor.g / 255.f
                                                blue:font.shadow.shadowColor.b / 255.f
                                               alpha:font.shadow.shadowColor.a / 255.f];
        
        [shadow setShadowColor:shadowColor];
        
        [tokenAttributesDict setObject:shadow forKey:NSShadowAttributeName];
    }
    
    return [[[NSAttributedString alloc] initWithString:str attributes:tokenAttributesDict] autorelease];
}

CAImage* CAFontProcesstor::imageForRichText(const std::vector<CARichLabel::Element>& elements, DSize& dim, CATextAlignment textAlignment)
{
    CAImage* ret = nullptr;
    do
    {
        CC_BREAK_IF(elements.empty());
        
        dim = DSize(s_dip_to_px(dim.width), s_dip_to_px(dim.height));
        
        NSMutableAttributedString *stringWithAttributes = [[NSMutableAttributedString alloc] init];
        
        for (auto& var : elements)
        {
            CAFont font = var.font;
            dipFontToPxFont(font);
            NSAttributedString* attributedString = NSAttributedStringForText(var.text, font, dim, textAlignment);
            [stringWithAttributes appendAttributedString:attributedString];
        }
        
        CAFont firstFont = elements.front().font;
        dipFontToPxFont(firstFont);

        float shrinkFontSize = (firstFont.fontSize);
        id iosfont = _createSystemFont(firstFont);
        
        CGRect textRect = _calculateStringRect(stringWithAttributes, iosfont, CGSizeMake(dim.width, dim.height), firstFont.wordWrap, shrinkFontSize);
        
        CC_BREAK_IF(textRect.size.width <= 0 || textRect.size.height <= 0);

        NSInteger POTWide = textRect.size.width;
        NSInteger POTHigh = textRect.size.height;
        
        ssize_t length = POTWide * POTHigh * 4;
        unsigned char *bytes = (unsigned char*)malloc(sizeof(unsigned char) * length);
        memset(bytes, 0, length);
        
        // draw text
        CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
        CGContextRef context        = CGBitmapContextCreate(bytes,
                                                            POTWide,
                                                            POTHigh,
                                                            8,
                                                            POTWide * 4,
                                                            colorSpace,
                                                            kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        if (!context)
        {
            CGColorSpaceRelease(colorSpace);
            CC_SAFE_FREE(bytes);
            break;
        }
        
        // text color
        CGContextSetRGBFillColor(context,
                                 firstFont.color.r / 255.f,
                                 firstFont.color.g / 255.f,
                                 firstFont.color.b / 255.f,
                                 firstFont.color.r / 255.f);
        
        // move Y rendering to the top of the image
        CGContextTranslateCTM(context, 0.0f, POTHigh);
        
        //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
        CGContextScaleCTM(context, 1.0f, -1.0f);
        // store the current context
        UIGraphicsPushContext(context);
        
        CGColorSpaceRelease(colorSpace);
        
        CGContextSetShouldSubpixelQuantizeFonts(context, false);
        
        CGContextBeginTransparencyLayerWithRect(context, textRect, NULL);
        
        /*
         if (true)
         {
         CGContextSetTextDrawingMode(context, kCGTextStroke);
         
         NSMutableDictionary* tokenAttributesDict2 = [NSMutableDictionary dictionaryWithObjectsAndKeys:
         foregroundColor,   NSForegroundColorAttributeName,
         iosfont,           NSFontAttributeName,
         paragraphStyle,    NSParagraphStyleAttributeName, nil];
         
         
         NSAttributedString *strokeString =[[[NSAttributedString alloc] initWithString:str attributes:tokenAttributesDict2] autorelease];
         
         [strokeString drawInRect:textRect];
         }
         */
        
        CGContextSetTextDrawingMode(context, kCGTextFill);
        
        // actually draw the text in the context
        //[stringWithAttributes drawInRect:textRect];
        //modify by zmr 用于解决多行文本，显示不下时最后一行以省略号显示
        NSStringDrawingContext *drawcontext = [[NSStringDrawingContext alloc] init];
        [stringWithAttributes drawWithRect:textRect options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingTruncatesLastVisibleLine context:drawcontext];
        [drawcontext release];
        
        CGContextEndTransparencyLayer(context);
        
        // pop the context
        UIGraphicsPopContext();
        
        // release the context
        CGContextRelease(context);
        
        unsigned int pixelsWide = static_cast<unsigned int>(POTWide);
        unsigned int pixelsHigh = static_cast<unsigned int>(POTHigh);
        
        dim = DSize(s_px_to_dip(pixelsWide), s_px_to_dip(pixelsHigh));
        
        CAData* data = new CAData();
        data->fastSet(bytes, length);
        ret = CAImage::createWithRawDataNoCache(data, CAImage::PixelFormat::RGBA8888, pixelsWide, pixelsHigh, true);
        data->release();
        
    } while (0);
    
    
    return ret;
}

CAImage* CAFontProcesstor::imageForText(const std::string& text, CAFont font, DSize& dim, CATextAlignment textAlignment)
{
    CAImage* ret = nullptr;
    do
    {
        CC_BREAK_IF(text.empty());
        
        dim = DSize(s_dip_to_px(dim.width), s_dip_to_px(dim.height));
        dipFontToPxFont(font);
        
        NSAttributedString *stringWithAttributes = NSAttributedStringForText(text, font, dim, textAlignment);
        
        float shrinkFontSize = (font.fontSize);
        id iosfont = _createSystemFont(font);
        
        CGRect textRect = _calculateStringRect(stringWithAttributes, iosfont, CGSizeMake(dim.width, dim.height), font.wordWrap, shrinkFontSize);
        
        CC_BREAK_IF(textRect.size.width <= 0 || textRect.size.height <= 0);
        
        
        if (font.italics)
        {
            float increase = shrinkFontSize * font.italicsValue * 0.5f;
            if (font.italicsValue > 0)
            {
                textRect.size.width += increase;
            }
            else if (font.italicsValue < 0)
            {
                textRect.size.width -= increase;
                textRect.origin.x -= increase;
            }
        }

        NSInteger POTWide = textRect.size.width;
        NSInteger POTHigh = textRect.size.height;
        
        ssize_t length = POTWide * POTHigh * 4;
        unsigned char *bytes = (unsigned char*)malloc(sizeof(unsigned char) * length);
        memset(bytes, 0, length);
        
        // draw text
        CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
        CGContextRef context        = CGBitmapContextCreate(bytes,
                                                            POTWide,
                                                            POTHigh,
                                                            8,
                                                            POTWide * 4,
                                                            colorSpace,
                                                            kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        if (!context)
        {
            CGColorSpaceRelease(colorSpace);
            CC_SAFE_FREE(bytes);
            break;
        }
        
        // text color
        CGContextSetRGBFillColor(context,
                                 font.color.r / 255.f,
                                 font.color.g / 255.f,
                                 font.color.b / 255.f,
                                 font.color.r / 255.f);
        
        // move Y rendering to the top of the image
        CGContextTranslateCTM(context, 0.0f, POTHigh);
        
        //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
        CGContextScaleCTM(context, 1.0f, -1.0f);
        // store the current context
        UIGraphicsPushContext(context);
        
        CGColorSpaceRelease(colorSpace);
        
        CGContextSetShouldSubpixelQuantizeFonts(context, false);
        
        CGContextBeginTransparencyLayerWithRect(context, textRect, NULL);

        CGContextSetTextDrawingMode(context, kCGTextFill);
        
        // actually draw the text in the context
        //[stringWithAttributes drawInRect:textRect];
        //modify by zmr 用于解决多行文本，显示不下时最后一行以省略号显示
        NSStringDrawingContext *drawcontext = [[NSStringDrawingContext alloc] init];
        [stringWithAttributes drawWithRect:textRect options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingTruncatesLastVisibleLine context:drawcontext];
        [drawcontext release];
        /*
         if (true)
         {
         CGContextSetTextDrawingMode(context, kCGTextStroke);
         
         NSMutableDictionary* tokenAttributesDict2 = [NSMutableDictionary dictionaryWithObjectsAndKeys:
         foregroundColor,   NSForegroundColorAttributeName,
         iosfont,           NSFontAttributeName,
         paragraphStyle,    NSParagraphStyleAttributeName, nil];
         
         
         NSAttributedString *strokeString =[[[NSAttributedString alloc] initWithString:str attributes:tokenAttributesDict2] autorelease];
         
         [strokeString drawInRect:textRect];
         }
         */
        
        CGContextEndTransparencyLayer(context);
        
        // pop the context
        UIGraphicsPopContext();
        
        // release the context
        CGContextRelease(context);
        
        unsigned int pixelsWide = static_cast<unsigned int>(POTWide);
        unsigned int pixelsHigh = static_cast<unsigned int>(POTHigh);
        
        dim = DSize(s_px_to_dip(pixelsWide), s_px_to_dip(pixelsHigh));
        
        CAData* data = new CAData();
        data->fastSet(bytes, length);
        ret = CAImage::createWithRawDataNoCache(data, CAImage::PixelFormat::RGBA8888, pixelsWide, pixelsHigh, true);
        data->release();
        
    } while (0);
    
    
    return ret;
}

float CAFontProcesstor::heightForFont(const CAFont& f)
{
    float ret = 0;
    do
    {
        CAFont font = f;
        dipFontToPxFont(font);
        
        id iosfont = _createSystemFont(font);
        CC_BREAK_IF(!iosfont);
        
        // alignment
        
        NSMutableParagraphStyle *paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
        [paragraphStyle setLineBreakMode:NSLineBreakByTruncatingTail];
        [paragraphStyle setAlignment:NSTextAlignmentLeft];
        
        NSMutableDictionary* tokenAttributesDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                    [UIColor whiteColor],   NSForegroundColorAttributeName,
                                                    iosfont,                NSFontAttributeName,
                                                    paragraphStyle,         NSParagraphStyleAttributeName,
                                                    nil];
        
        
        if (font.color == CAColor4B::CLEAR && font.stroke.strokeEnabled)
        {
            [tokenAttributesDict setObject:@(font.stroke.strokeSize) forKey:NSStrokeWidthAttributeName];
            [tokenAttributesDict setObject:[UIColor whiteColor] forKey:NSStrokeColorAttributeName];
        }
        
        if (font.italics) [tokenAttributesDict setObject:@(font.italicsValue) forKey:NSObliquenessAttributeName];
        if (font.underLine) [tokenAttributesDict setObject:@(NSUnderlineStyleSingle) forKey:NSUnderlineStyleAttributeName];

        if (font.shadow.shadowEnabled)
        {
            NSShadow* shadow = [[[NSShadow alloc] init] autorelease];
            
            [shadow setShadowOffset:CGSizeMake(font.shadow.shadowOffset.width, font.shadow.shadowOffset.height)];
            [shadow setShadowBlurRadius:font.shadow.shadowBlur];
            
            UIColor* shadowColor = [UIColor whiteColor];
            
            [shadow setShadowColor:shadowColor];
            
            [tokenAttributesDict setObject:shadow forKey:NSShadowAttributeName];
        }
        
        NSAttributedString* str = [[[NSAttributedString alloc] initWithString:@"A" attributes:tokenAttributesDict] autorelease];
        CGSize textSize = CGSizeMake(CGFLOAT_MAX, 0);
        CGSize dim;
        dim = [str boundingRectWithSize:CGSizeMake(textSize.width, textSize.height)
                                options:(NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingTruncatesLastVisibleLine)
                                context:nil].size;
        
        dim.width = ceilf(s_px_to_dip(dim.width));
        dim.height = ceilf(s_px_to_dip(dim.height));
        
        ret = dim.height;
        
    } while (0);
    
    return ret;
}

float CAFontProcesstor::heightForTextAtWidth(const std::string& text, const CAFont& f, float width)
{
    float ret = 0;
    do
    {
        CC_BREAK_IF(text.empty());
        
        NSString* string = [NSString stringWithUTF8String:text.c_str()];
        CC_BREAK_IF(!string);
        
        CAFont font = f;
        dipFontToPxFont(font);
        width = s_dip_to_px(width);
        
        id iosfont = _createSystemFont(font);
        CC_BREAK_IF(!iosfont);
        
        NSMutableParagraphStyle *paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
        [paragraphStyle setLineBreakMode:font.wordWrap ? NSLineBreakByWordWrapping : NSLineBreakByCharWrapping];
        [paragraphStyle setLineSpacing:font.lineSpacing];
        [paragraphStyle setAlignment:NSTextAlignmentLeft];
        
        NSMutableDictionary* tokenAttributesDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                    [UIColor whiteColor],   NSForegroundColorAttributeName,
                                                    iosfont,                NSFontAttributeName,
                                                    paragraphStyle,         NSParagraphStyleAttributeName,
                                                    nil];
        
        
        if (font.color == CAColor4B::CLEAR && font.stroke.strokeEnabled)
        {
            [tokenAttributesDict setObject:@(font.stroke.strokeSize) forKey:NSStrokeWidthAttributeName];
            [tokenAttributesDict setObject:[UIColor whiteColor] forKey:NSStrokeColorAttributeName];
        }
        
        if (font.italics) [tokenAttributesDict setObject:@(font.italicsValue) forKey:NSObliquenessAttributeName];
        if (font.underLine) [tokenAttributesDict setObject:@(NSUnderlineStyleSingle) forKey:NSUnderlineStyleAttributeName];
        
        if (font.shadow.shadowEnabled)
        {
            NSShadow* shadow = [[[NSShadow alloc] init] autorelease];
            
            [shadow setShadowOffset:CGSizeMake(font.shadow.shadowOffset.width, font.shadow.shadowOffset.height)];
            [shadow setShadowBlurRadius:font.shadow.shadowBlur];
            
            UIColor* shadowColor = [UIColor whiteColor];
            
            [shadow setShadowColor:shadowColor];
            
            [tokenAttributesDict setObject:shadow forKey:NSShadowAttributeName];
        }
        
        NSAttributedString* str = [[[NSAttributedString alloc] initWithString:string attributes:tokenAttributesDict] autorelease];
        CGSize textSize = CGSizeMake(width, 0);
        CGSize dim;
        dim = [str boundingRectWithSize:CGSizeMake(textSize.width, textSize.height)
                                options:(NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingTruncatesLastVisibleLine)
                                context:nil].size;
        
        dim.width = ceilf(s_px_to_dip(dim.width));
        dim.height = ceilf(s_px_to_dip(dim.height));
        
        ret = dim.height;
        
    } while (0);
    
    return ret;
}

float CAFontProcesstor::widthForTextAtOneLine(const std::string& text, const CAFont& f)
{
    float ret = 0;
    do
    {
        CC_BREAK_IF(text.empty());
        
        NSString* string = [NSString stringWithUTF8String:text.c_str()];
        CC_BREAK_IF(!string);
        
        CAFont font = f;
        dipFontToPxFont(font);
        
        float shrinkFontSize = (font.fontSize);
        id iosfont = _createSystemFont(font);
        CC_BREAK_IF(!iosfont);

        NSMutableParagraphStyle *paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
        [paragraphStyle setLineBreakMode:NSLineBreakByTruncatingTail];
        [paragraphStyle setAlignment:NSTextAlignmentLeft];
        
        NSMutableDictionary* tokenAttributesDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                    [UIColor whiteColor],   NSForegroundColorAttributeName,
                                                    iosfont,                NSFontAttributeName,
                                                    paragraphStyle,         NSParagraphStyleAttributeName,
                                                    nil];
        
        if (font.color == CAColor4B::CLEAR && font.stroke.strokeEnabled)
        {
            [tokenAttributesDict setObject:@(font.stroke.strokeSize) forKey:NSStrokeWidthAttributeName];
            [tokenAttributesDict setObject:[UIColor whiteColor] forKey:NSStrokeColorAttributeName];
        }
        
        if (font.italics) [tokenAttributesDict setObject:@(font.italicsValue) forKey:NSObliquenessAttributeName];

        if (font.shadow.shadowEnabled)
        {
            NSShadow* shadow = [[[NSShadow alloc] init] autorelease];
            
            [shadow setShadowOffset:CGSizeMake(font.shadow.shadowOffset.width, font.shadow.shadowOffset.height)];
            [shadow setShadowBlurRadius:font.shadow.shadowBlur];
            
            UIColor* shadowColor = [UIColor whiteColor];
            
            [shadow setShadowColor:shadowColor];
            
            [tokenAttributesDict setObject:shadow forKey:NSShadowAttributeName];
        }
        
        NSAttributedString* str = [[[NSAttributedString alloc] initWithString:string attributes:tokenAttributesDict] autorelease];
        CGSize textSize = CGSizeMake(CGFLOAT_MAX, shrinkFontSize * 1.5f);
        CGSize dim;
        dim = [str boundingRectWithSize:CGSizeMake(textSize.width, textSize.height)
                                options:(NSStringDrawingUsesLineFragmentOrigin|NSStringDrawingTruncatesLastVisibleLine)
                                context:nil].size;
        
        if (font.italics)
        {
            float increase = shrinkFontSize * font.italicsValue * 0.5f;
            if (font.italicsValue > 0)
            {
                dim.width += increase;
            }
            else if (font.italicsValue < 0)
            {
                dim.width -= increase;
            }
        }
        
        ret = ceilf(s_px_to_dip(dim.width));
        
    } while (0);
    
    return ret;
}
NS_CC_END
