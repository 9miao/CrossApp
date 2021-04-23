

#include "CAFontProcesstor.h"
#include "images/CAImage.h"

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#import <AVFoundation/AVFoundation.h>
#import <AVKit/AVKit.h>

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

NSRect _calculateStringSize(NSAttributedString *str, id font, CGSize constrainSize, bool enableWrap)
{
    NSSize textRect = NSZeroSize;
    textRect.width = constrainSize.width > 0 ? constrainSize.width
    : CGFLOAT_MAX;
    textRect.height = constrainSize.height > 0 ? constrainSize.height
    : CGFLOAT_MAX;
    
    NSRect dim = NSZeroRect;
    dim.size = [str boundingRectWithSize:textRect options:(NSStringDrawingOptions)(NSStringDrawingUsesLineFragmentOrigin) context:nil].size;
    
    dim.size.width = ceilf(dim.size.width);
    dim.size.height = MIN(ceilf(dim.size.height), constrainSize.height) ;
    
    return dim;
}

static NSFont* _createSystemFont(const std::string& fontName, float size)
{
    NSString * fntName = [NSString stringWithUTF8String:fontName.c_str()];
    fntName = [[fntName lastPathComponent] stringByDeletingPathExtension];
    
    CGFloat fontSize = size;
    
    // font
    NSFont *font = [[NSFontManager sharedFontManager]
                    fontWithFamily:fntName
                    traits:NSUnboldFontMask | NSUnitalicFontMask
                    weight:0
                    size:fontSize];
    
    if (font == nil) {
        font = [[NSFontManager sharedFontManager]
                fontWithFamily:@"Helvetica"
                traits:NSUnboldFontMask | NSUnitalicFontMask
                weight:0
                size:fontSize];
    }
    return font;
}

NS_CC_BEGIN

NSAttributedString* NSAttributedStringForText(const std::string& text, const CAFont& font, const DSize& dim, CATextAlignment textAlignment)
{
    NSString * str  = [NSString stringWithUTF8String:text.c_str()];

    if (font.italics)
    {
        int increase = ceilf(font.fontSize * font.italicsValue / 0.5f);
        
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
    id nsfont = _createSystemFont(font.fontName, shrinkFontSize);

    // color
    NSColor* foregroundColor = [NSColor colorWithDeviceRed:font.color.r/255.0
                                                     green:font.color.g/255.0
                                                      blue:font.color.b/255.0
                                                     alpha:font.color.a/255.0];
    
    // alignment
    NSTextAlignment textAlign = _calculateTextAlignment(textAlignment);
    
    NSMutableParagraphStyle *paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
    [paragraphStyle setLineBreakMode:font.wordWrap ? NSLineBreakByWordWrapping : NSLineBreakByCharWrapping];
    [paragraphStyle setLineSpacing:font.lineSpacing * 2];
    [paragraphStyle setAlignment:textAlign];
    
    // attribute
    NSMutableDictionary* tokenAttributesDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                foregroundColor,   NSForegroundColorAttributeName,
                                                nsfont,            NSFontAttributeName,
                                                paragraphStyle,    NSParagraphStyleAttributeName,
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
         NSColor* underLineColor = [NSColor colorWithDeviceRed:font.underLineColor.r/255.0
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
         NSColor* deleteLineColor = [NSColor colorWithDeviceRed:font.deleteLineColor.r/255.0
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
            foregroundColor = [NSColor colorWithDeviceRed:font.stroke.strokeColor.r / 255.f
                                                    green:font.stroke.strokeColor.g / 255.f
                                                     blue:font.stroke.strokeColor.b / 255.f
                                                    alpha:font.stroke.strokeColor.a / 255.f];
            
            
            [tokenAttributesDict setObject:@(fabsf(font.stroke.strokeSize)) forKey:NSStrokeWidthAttributeName];
            [tokenAttributesDict setObject:foregroundColor forKey:NSStrokeColorAttributeName];
        }
        else
        {
            NSColor *strokeColor = [NSColor colorWithDeviceRed:font.stroke.strokeColor.r / 255.f
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
        
        NSColor* shadowColor = [NSColor colorWithDeviceRed:font.shadow.shadowColor.r / 255.f
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
                
        NSMutableAttributedString *stringWithAttributes = [[NSMutableAttributedString alloc] init];
        
        for (auto& var : elements)
        {
            CAFont font = var.font;
            NSAttributedString* attributedString = NSAttributedStringForText(var.text, font, dim, textAlignment);
            [stringWithAttributes appendAttributedString:attributedString];
        }
        
        CAFont firstFont = elements.front().font;

        float shrinkFontSize = (firstFont.fontSize);
        id nsfont = _createSystemFont(firstFont.fontName, shrinkFontSize);
        
        NSRect textRect = _calculateStringSize(stringWithAttributes, nsfont, CGSizeMake(dim.width, dim.height), firstFont.wordWrap);
        
        // Mac crashes if the width or height is 0
        CC_BREAK_IF(textRect.size.width <= 0 || textRect.size.height <= 0);
        
        NSInteger POTWide = textRect.size.width;
        NSInteger POTHigh = textRect.size.height;
        
        [[NSGraphicsContext currentContext] setShouldAntialias:NO];
        
        NSImage *image = [[NSImage alloc] initWithSize:NSMakeSize(POTWide, POTHigh)];
        [image lockFocus];
        // patch for mac retina display and lableTTF
        [[NSAffineTransform transform] set];
        
        
//        [stringWithAttributes drawInRect:textRect];
        //modify by zmr 用于解决多行文本，显示不下时最后一行以省略号显示
        NSStringDrawingContext *drawcontext = [[NSStringDrawingContext alloc] init];
        [stringWithAttributes drawWithRect:textRect options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingTruncatesLastVisibleLine context:drawcontext];
        [drawcontext release];
        
        NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc] initWithFocusedViewRect:NSMakeRect (0.0f, 0.0f, POTWide, POTHigh)];
        [image unlockFocus];
        
        dim = DSize((POTWide), (POTHigh));

        CIImage * ciImage = [[CIImage alloc] initWithBitmapImageRep:bitmap];
        [bitmap release];
        
        CIContext *ciContext = [CIContext contextWithOptions:nil];
        CGImageRef videoImage = [ciContext createCGImage:ciImage fromRect:ciImage.extent];
        [ciImage release];
        
        size_t bitsPerComponent = CGImageGetBitsPerComponent(videoImage);
        size_t bitsPerPixel = CGImageGetBitsPerPixel(videoImage);
        size_t bytesPerRow = CGImageGetBytesPerRow(videoImage);
        size_t width = CGImageGetWidth(videoImage);
        size_t height = CGImageGetHeight(videoImage);

        CGDataProviderRef provider = CGImageGetDataProvider(videoImage);
        CFDataRef ref = CGDataProviderCopyData(provider);
        CGImageRelease(videoImage);
        
        unsigned char* data = (unsigned char*)CFDataGetBytePtr(ref);
        ssize_t length = (ssize_t)CFDataGetLength(ref);
        CGFloat pixelsWide = bytesPerRow / (bitsPerPixel / bitsPerComponent);
        CGFloat pixelsHigh = length / (bitsPerPixel / bitsPerComponent) / pixelsWide ;
        
        CAData* cross_data = CAData::create();
        cross_data->copy(data, length);
        CFRelease(ref);
        
        ret = CrossApp::CAImage::createWithRawDataNoCache(cross_data, CrossApp::CAImage::PixelFormat::RGBA8888, pixelsWide, pixelsHigh, true);
        
    } while (0);
    
    return ret;
}

CAImage* CAFontProcesstor::imageForText(const std::string& text, CAFont font, DSize& dim, CATextAlignment textAlignment)
{
    CAImage* ret = nullptr;
    do {
        CC_BREAK_IF(text.empty());

        NSAttributedString *stringWithAttributes = NSAttributedStringForText(text, font, dim, textAlignment);
        
        float shrinkFontSize = (font.fontSize);
        id nsfont = _createSystemFont(font.fontName, shrinkFontSize);
        
        NSRect textRect = _calculateStringSize(stringWithAttributes, nsfont, CGSizeMake(dim.width, dim.height), font.wordWrap);
        
        // Mac crashes if the width or height is 0
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
        
        [[NSGraphicsContext currentContext] setShouldAntialias:NO];
        
        NSImage *image = [[NSImage alloc] initWithSize:NSMakeSize(POTWide, POTHigh)];
        [image lockFocus];
        // patch for mac retina display and lableTTF
        [[NSAffineTransform transform] set];
        
//        [stringWithAttributes drawInRect:textRect];
        //modify by zmr 用于解决多行文本，显示不下时最后一行以省略号显示
        NSStringDrawingContext *drawcontext = [[NSStringDrawingContext alloc] init];
        [stringWithAttributes drawWithRect:textRect options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingTruncatesLastVisibleLine context:drawcontext];
        [drawcontext release];
        
        NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc] initWithFocusedViewRect:NSMakeRect (0.0f, 0.0f, POTWide, POTHigh)];
        [image unlockFocus];

        dim = DSize((POTWide), (POTHigh));

        CIImage * ciImage = [[CIImage alloc] initWithBitmapImageRep:bitmap];
        [bitmap release];
        
        CIContext *ciContext = [CIContext contextWithOptions:nil];
        CGImageRef videoImage = [ciContext createCGImage:ciImage fromRect:ciImage.extent];
        [ciImage release];
        
        size_t bitsPerComponent = CGImageGetBitsPerComponent(videoImage);
        size_t bitsPerPixel = CGImageGetBitsPerPixel(videoImage);
        size_t bytesPerRow = CGImageGetBytesPerRow(videoImage);
        size_t width = CGImageGetWidth(videoImage);
        size_t height = CGImageGetHeight(videoImage);
 
        CGDataProviderRef provider = CGImageGetDataProvider(videoImage);
        CFDataRef ref = CGDataProviderCopyData(provider);
        CGImageRelease(videoImage);
        
        unsigned char* data = (unsigned char*)CFDataGetBytePtr(ref);
        ssize_t length = (ssize_t)CFDataGetLength(ref);
        CGFloat pixelsWide = bytesPerRow / (bitsPerPixel / bitsPerComponent);
        CGFloat pixelsHigh = length / (bitsPerPixel / bitsPerComponent) / pixelsWide ;
        
        CAData* cross_data = CAData::create();
        cross_data->copy(data, length);
        CFRelease(ref);
        
        ret = CrossApp::CAImage::createWithRawDataNoCache(cross_data, CrossApp::CAImage::PixelFormat::RGBA8888, pixelsWide, pixelsHigh, true);
        
    } while (0);
    
    
    return ret;
}

float CAFontProcesstor::heightForFont(const CAFont& font)
{
    float ret = 0;
    do
    {
        float shrinkFontSize = (font.fontSize);
        id nsfont = _createSystemFont(font.fontName, shrinkFontSize);
        CC_BREAK_IF(!nsfont);
        
        // alignment
        
        NSMutableParagraphStyle *paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
        [paragraphStyle setLineBreakMode:NSLineBreakByTruncatingTail];
        [paragraphStyle setAlignment:NSTextAlignmentLeft];
        
        NSMutableDictionary* tokenAttributesDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                    [NSColor whiteColor],   NSForegroundColorAttributeName,
                                                    nsfont,                NSFontAttributeName,
                                                    paragraphStyle,         NSParagraphStyleAttributeName,
                                                    nil];
        
        
        if (font.color == CAColor4B::CLEAR && font.stroke.strokeEnabled)
        {
            [tokenAttributesDict setObject:@(font.stroke.strokeSize) forKey:NSStrokeWidthAttributeName];
            [tokenAttributesDict setObject:[NSColor whiteColor] forKey:NSStrokeColorAttributeName];
        }
        
        if (font.italics) [tokenAttributesDict setObject:@(font.italicsValue) forKey:NSObliquenessAttributeName];
        if (font.underLine) [tokenAttributesDict setObject:@(NSUnderlineStyleSingle) forKey:NSUnderlineStyleAttributeName];

        if (font.shadow.shadowEnabled)
        {
            NSShadow* shadow = [[[NSShadow alloc] init] autorelease];
            
            [shadow setShadowOffset:CGSizeMake(font.shadow.shadowOffset.width, font.shadow.shadowOffset.height)];
            [shadow setShadowBlurRadius:font.shadow.shadowBlur];
            
            NSColor* shadowColor = [NSColor whiteColor];
            
            [shadow setShadowColor:shadowColor];
            
            [tokenAttributesDict setObject:shadow forKey:NSShadowAttributeName];
        }
        NSAttributedString* str = [[[NSAttributedString alloc] initWithString:@"A" attributes:tokenAttributesDict] autorelease];
        NSSize textSize = NSMakeSize(CGFLOAT_MAX, 0);
        NSSize dim = [str boundingRectWithSize:textSize options:(NSStringDrawingOptions)(NSStringDrawingUsesLineFragmentOrigin) context:nil].size;

        ret = ceilf((dim.height));
        
    } while (0);
    
    
    return ret;
}

float CAFontProcesstor::heightForTextAtWidth(const std::string& text, const CAFont& font, float width)
{
    float ret = 0;
    do
    {
        CC_BREAK_IF(text.empty());
        
        NSString* string = [NSString stringWithUTF8String:text.c_str()];
        CC_BREAK_IF(!string);
        
        float shrinkFontSize = (font.fontSize);
        id nsfont = _createSystemFont(font.fontName, shrinkFontSize);
        CC_BREAK_IF(!nsfont);
        
        NSMutableParagraphStyle *paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
        [paragraphStyle setLineBreakMode:font.wordWrap ? NSLineBreakByWordWrapping : NSLineBreakByCharWrapping];
        [paragraphStyle setLineSpacing:font.lineSpacing * 2];
        [paragraphStyle setAlignment:NSTextAlignmentLeft];
        
        // attribute
        NSMutableDictionary* tokenAttributesDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                    [NSColor whiteColor],   NSForegroundColorAttributeName,
                                                    nsfont,                NSFontAttributeName,
                                                    paragraphStyle,         NSParagraphStyleAttributeName,
                                                    nil];
        
        
        if (font.color == CAColor4B::CLEAR && font.stroke.strokeEnabled)
        {
            [tokenAttributesDict setObject:@(font.stroke.strokeSize) forKey:NSStrokeWidthAttributeName];
            [tokenAttributesDict setObject:[NSColor whiteColor] forKey:NSStrokeColorAttributeName];
        }
        
        if (font.italics) [tokenAttributesDict setObject:@(font.italicsValue) forKey:NSObliquenessAttributeName];
        if (font.underLine) [tokenAttributesDict setObject:@(NSUnderlineStyleSingle) forKey:NSUnderlineStyleAttributeName];
        
        if (font.shadow.shadowEnabled)
        {
            NSShadow* shadow = [[[NSShadow alloc] init] autorelease];
            
            [shadow setShadowOffset:CGSizeMake(font.shadow.shadowOffset.width, font.shadow.shadowOffset.height)];
            [shadow setShadowBlurRadius:font.shadow.shadowBlur];
            
            NSColor* shadowColor = [NSColor whiteColor];
            
            [shadow setShadowColor:shadowColor];
            
            [tokenAttributesDict setObject:shadow forKey:NSShadowAttributeName];
        }
        
        NSAttributedString* str = [[[NSAttributedString alloc] initWithString:string attributes:tokenAttributesDict] autorelease];
        NSSize textSize = NSMakeSize(width, 0);
        NSSize dim = [str boundingRectWithSize:textSize options:(NSStringDrawingOptions)(NSStringDrawingUsesLineFragmentOrigin) context:nil].size;

        ret = ceilf((dim.height));
        
    } while (0);
    
    return ret;
}

float CAFontProcesstor::widthForTextAtOneLine(const std::string& text, const CAFont& font)
{
    float ret = 0;
    do
    {
        CC_BREAK_IF(text.empty());
        NSString* string = [NSString stringWithUTF8String:text.c_str()];
        CC_BREAK_IF(!string);
        
        float shrinkFontSize = (font.fontSize);
        id nsfont = _createSystemFont(font.fontName, shrinkFontSize);
        CC_BREAK_IF(!nsfont);

        NSMutableParagraphStyle *paragraphStyle = [[[NSMutableParagraphStyle alloc] init] autorelease];
        [paragraphStyle setLineBreakMode:NSLineBreakByTruncatingTail];
        [paragraphStyle setAlignment:NSTextAlignmentLeft];
        
        // attribute
        NSMutableDictionary* tokenAttributesDict = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                                    [NSColor whiteColor],   NSForegroundColorAttributeName,
                                                    nsfont,                NSFontAttributeName,
                                                    paragraphStyle,         NSParagraphStyleAttributeName,
                                                    nil];
        
        
        if (font.color == CAColor4B::CLEAR && font.stroke.strokeEnabled)
        {
            [tokenAttributesDict setObject:@(font.stroke.strokeSize) forKey:NSStrokeWidthAttributeName];
            [tokenAttributesDict setObject:[NSColor whiteColor] forKey:NSStrokeColorAttributeName];
        }
        
        if (font.italics) [tokenAttributesDict setObject:@(font.italicsValue) forKey:NSObliquenessAttributeName];
        
        if (font.shadow.shadowEnabled)
        {
            NSShadow* shadow = [[[NSShadow alloc] init] autorelease];
            
            [shadow setShadowOffset:CGSizeMake(font.shadow.shadowOffset.width, font.shadow.shadowOffset.height)];
            [shadow setShadowBlurRadius:font.shadow.shadowBlur];
            
            NSColor* shadowColor = [NSColor whiteColor];
            
            [shadow setShadowColor:shadowColor];
            
            [tokenAttributesDict setObject:shadow forKey:NSShadowAttributeName];
        }
        
        NSAttributedString* str = [[[NSAttributedString alloc] initWithString:string attributes:tokenAttributesDict] autorelease];
        NSSize textSize = NSMakeSize(CGFLOAT_MAX, 0);
        NSSize dim = [str boundingRectWithSize:textSize options:(NSStringDrawingOptions)(NSStringDrawingUsesLineFragmentOrigin) context:nil].size;

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
        
        ret = ceilf((dim.width));
        
    } while (0);
    return ret;
}

NS_CC_END
