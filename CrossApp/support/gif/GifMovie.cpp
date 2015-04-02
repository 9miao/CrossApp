#include "GIFMovie.h"

NS_CC_BEGIN

static void packARGB32(CAColor4B* pixel, GifByteType alpha, GifByteType red,
	GifByteType green, GifByteType blue)
{
	pixel->a = alpha;
	pixel->r = red;
	pixel->g = green;
	pixel->b = blue;
}

//static int DecodeCallBackProc(GifFileType* gif, GifByteType* bytes, int size)
//{
//	FILE* file = (FILE*) gif->UserData;
//	return fread(bytes, 1, size, file);
//}

static const char *filepath;
static int DecodeCallBackProc(GifFileType* gif, GifByteType* bytes, int size)
{
    //读取的字节数，读取失败则为0
    static unsigned long len = 0;
    //读取的内容
    static unsigned char *data = CCFileUtils::sharedFileUtils()->getFileData(filepath, "r", &len);
    static int offset = 0;
    
    memcpy(bytes, data + offset, size);
    offset += size;
    return size;
}
static void getColorFromTable(int idx, CAColor4B* dst, const ColorMapObject* cmap)
{
	char colIdx = idx >= cmap->ColorCount ? 0 : idx;
	GifColorType* col = &cmap->Colors[colIdx];
	packARGB32(dst, 0xFF, col->Red, col->Green, col->Blue);
}

GIFMovie::GIFMovie()
{   fGIF = NULL;
    fCurrIndex = -1;
    fLastDrawIndex = -1;
}

bool GIFMovie::init(const char* fileName)
{
	FILE* file = GifUtils::openFile(fileName);
    int error = 0;
    filepath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName).c_str();
    
    fGIF = DGifOpen(file,&DecodeCallBackProc,&error);
    if (NULL == fGIF || DGifSlurp(fGIF) != GIF_OK)
    {
        GifUtils::closeFile(file);
        DGifCloseFile(fGIF);
        fGIF = NULL;
        return false;
    }
    
    GifUtils::closeFile(file);
    return true;
}

GIFMovie::~GIFMovie()
{
    if (fGIF)
        DGifCloseFile(fGIF);
}
 
static GLubyte savedimage_duration(const SavedImage* image)
{
	GLubyte duration = 0;
    for (int j = 0; j < image->ExtensionBlockCount; j++)
    {
        if (image->ExtensionBlocks[j].Function == GRAPHICS_EXT_FUNC_CODE)
        {
            int size = image->ExtensionBlocks[j].ByteCount;
            //assert(size >= 4);
            if(size < 4) break;
            const uint8_t* b = (const uint8_t*)image->ExtensionBlocks[j].Bytes;
            duration =  ((b[2] << 8) | b[1]) * 10;
			break;
        }
    }
	duration = duration <= 50 ? 50 : duration;
    return duration;
}

GifFrame GIFMovie::getGifFrameByIndex(unsigned int frameIndex)
{
	GifFrame gif;
    
	if(fGIF == NULL || frameIndex > getGifCount())
	{
		return gif;
	}
	int duration = savedimage_duration(&fGIF->SavedImages[frameIndex]);
	fCurrIndex = frameIndex;
	onGetBitmap(&m_bitmap);
    
	gif.m_frameData.m_duration = duration;
	gif.m_frameData.m_index = frameIndex;
    gif.m_bm = &m_bitmap;

	return gif;
}

bool GIFMovie::onGetInfo(Info* info)
{
    if (NULL == fGIF)
        return false;
 
    GLubyte dur = 0;
    for (int i = 0; i < fGIF->ImageCount; i++)
        dur += savedimage_duration(&fGIF->SavedImages[i]);
 
    info->fDuration = dur;
    info->fWidth = fGIF->SWidth;
    info->fHeight = fGIF->SHeight;
    info->fIsOpaque = false;    // how to compute?
    return true;
}
 
bool GIFMovie::onSetTime(GLubyte time)
{
	if (NULL == fGIF)
		return false;

	GLubyte dur = 0;
	for (int i = 0; i < fGIF->ImageCount; i++)
	{
		dur += savedimage_duration(&fGIF->SavedImages[i]);
		if (dur >= time)
		{
			fCurrIndex = i;
			return fLastDrawIndex != fCurrIndex;
		}
	}
	fCurrIndex = fGIF->ImageCount - 1;
    return true;
}

static void copyLine(unsigned char* dst_r, unsigned char* dst_g, unsigned char* dst_b, unsigned char* dst_a, const unsigned char* src, const ColorMapObject* cmap,
                    int transparent, int width)
{
	for (; width > 0; width--, src++, dst_r+=4,dst_g+=4,dst_b+=4,dst_a+=4)
    {
		if (*src != transparent)
        {
			const GifColorType& col = cmap->Colors[*src];
            *dst_r = col.Red;
            *dst_g = col.Green;
            *dst_b = col.Blue;
            *dst_a = 0xFF;
		}
	}
}
 
static void blitNormal(Bitmap* bm, const SavedImage* frame, const ColorMapObject* cmap,
                    int transparent)
{
	GifWord width = bm->m_width;
	GifWord height = bm->m_hight;
    const unsigned char* src = (unsigned char*)frame->RasterBits;
    
    unsigned char* r = bm->getAddr(frame->ImageDesc.Left, frame->ImageDesc.Top, 0);
    unsigned char* g = bm->getAddr(frame->ImageDesc.Left, frame->ImageDesc.Top, 1);
    unsigned char* b = bm->getAddr(frame->ImageDesc.Left, frame->ImageDesc.Top, 2);
    unsigned char* a = bm->getAddr(frame->ImageDesc.Left, frame->ImageDesc.Top, 3);

    GifWord copyWidth = frame->ImageDesc.Width;
    if (frame->ImageDesc.Left + copyWidth > width) {
        copyWidth = width - frame->ImageDesc.Left;
    }
 
    GifWord copyHeight = frame->ImageDesc.Height;
    if (frame->ImageDesc.Top + copyHeight > height) {
        copyHeight = height - frame->ImageDesc.Top;
    }

    for (; copyHeight > 0; copyHeight--)
    {
        copyLine(r,g,b,a, src, cmap, transparent, copyWidth);
        src += frame->ImageDesc.Width;
        r += width;
        g += width;
        b += width;
        a += width;
    }
}
 
static void fillRect(Bitmap* bm, GifWord left, GifWord top, GifWord width, GifWord height,
                    const CAColor4B& col)
{
	int bmWidth = bm->m_width;
	int bmHeight = bm->m_hight;
    
    unsigned char* r = bm->getAddr(left, top, 0);
    unsigned char* g = bm->getAddr(left, top, 1);
    unsigned char* b = bm->getAddr(left, top, 2);
    unsigned char* a = bm->getAddr(left, top, 3);

	GifWord copyWidth = width;
    if (left + copyWidth > bmWidth)
    {
        copyWidth = bmWidth - left;
    }
 
    GifWord copyHeight = height;
    if (top + copyHeight > bmHeight)
    {
        copyHeight = bmHeight - top;
    }
 
	for (; copyHeight > 0; copyHeight--)
	{
		for(int wIndex = 0; wIndex < bmWidth; wIndex++, r+=4,g+=4,b+=4,a+=4)
		{
			*r = col.r;
            *g = col.r;
            *b = col.r;
            *a = col.r;
		}
	}
}
 
static void drawFrame(Bitmap* bm, const SavedImage* frame, const ColorMapObject* cmap)
{
    int transparent = -1;
 
    for (int i = 0; i < frame->ExtensionBlockCount; ++i) {
        ExtensionBlock* eb = frame->ExtensionBlocks + i;
        if (eb->Function == GRAPHICS_EXT_FUNC_CODE &&
            eb->ByteCount == 4) {
            bool has_transparency = ((eb->Bytes[0] & 1) == 1);
            if (has_transparency) {
                transparent = (unsigned char)eb->Bytes[3];
            }
        }
    }
 
    if (frame->ImageDesc.ColorMap != NULL) {
        // use local color table
        cmap = frame->ImageDesc.ColorMap;
    }
 
    if (cmap == NULL || cmap->ColorCount != (1 << cmap->BitsPerPixel)) {
        
        return;
    }
 
    blitNormal(bm, frame, cmap, transparent);   
}
 
static bool checkIfWillBeCleared(const SavedImage* frame)
{
    for (int i = 0; i < frame->ExtensionBlockCount; ++i) {
        ExtensionBlock* eb = frame->ExtensionBlocks + i;
        if (eb->Function == GRAPHICS_EXT_FUNC_CODE &&
            eb->ByteCount == 4) {
            // check disposal method
            int disposal = ((eb->Bytes[0] >> 2) & 7);
            if (disposal == 2 || disposal == 3) {
                return true;
            }
        }
    }
    return false;
}
 
static void getTransparencyAndDisposalMethod(const SavedImage* frame, bool* trans, int* disposal)
{
    *trans = false;
    *disposal = 0;
    for (int i = 0; i < frame->ExtensionBlockCount; ++i) {
        ExtensionBlock* eb = frame->ExtensionBlocks + i;
        if (eb->Function == GRAPHICS_EXT_FUNC_CODE &&
            eb->ByteCount == 4) {
            *trans = ((eb->Bytes[0] & 1) == 1);
            *disposal = ((eb->Bytes[0] >> 2) & 7);
        }
    }
}
 
// return true if area of 'target' is completely covers area of 'covered'
static bool checkIfCover(const SavedImage* target, const SavedImage* covered)
{
    if (target->ImageDesc.Left <= covered->ImageDesc.Left
        && covered->ImageDesc.Left + covered->ImageDesc.Width <=
            target->ImageDesc.Left + target->ImageDesc.Width
        && target->ImageDesc.Top <= covered->ImageDesc.Top
        && covered->ImageDesc.Top + covered->ImageDesc.Height <=
            target->ImageDesc.Top + target->ImageDesc.Height) {
        return true;
    }
    return false;
}
 
static void disposeFrameIfNeeded(Bitmap* bm, const SavedImage* cur, const SavedImage* next,
                                Bitmap* backup, const CAColor4B& color)
{
    // We can skip disposal process if next frame is not transparent
    // and completely covers current area
    bool curTrans;
    int curDisposal;
    getTransparencyAndDisposalMethod(cur, &curTrans, &curDisposal);
    bool nextTrans;
    int nextDisposal;
    getTransparencyAndDisposalMethod(next, &nextTrans, &nextDisposal);


	//#define DISPOSAL_UNSPECIFIED      0       /* No disposal specified. */
	//#define DISPOSE_DO_NOT            1       /* Leave image in place */
	//#define DISPOSE_BACKGROUND        2       /* Set area too background color */
	//#define DISPOSE_PREVIOUS          3       /* Restore to previous content */
	
    if ((curDisposal == 2 || curDisposal == 3)
        && (nextTrans || !checkIfCover(next, cur))) {
        switch (curDisposal) {
        // restore to background color
        // -> 'background' means background under this image.
        case 2:
            fillRect(bm, cur->ImageDesc.Left, cur->ImageDesc.Top,
                    cur->ImageDesc.Width, cur->ImageDesc.Height,
                    color);
            break;
 
        // restore to previous
        case 3:
			{
				bm->swap(backup);
			}
			
            break;
        }
    }
 
    // Save current image if next frame's disposal method == 3
	if (nextDisposal == 3)
		memcpy(backup->getAddr(0,0,0), bm->getAddr(0,0,0), bm->getPixelLenth() * sizeof(unsigned int));
}

bool GIFMovie::onGetBitmap(Bitmap* bm)
{
    const GifFileType* gif = fGIF;
    if (NULL == gif)
        return false;
 
    if (gif->ImageCount < 1) {
        return false;
    }
 
	const int width = bm->m_width = gif->SWidth;
	const int height = bm->m_hight = gif->SHeight;
	if (width <= 0 || height <= 0) {
		return false;
	}

	if (fLastDrawIndex >= 0 && fLastDrawIndex == fCurrIndex) {
		return true;
	}

	int startIndex = fLastDrawIndex + 1;
	if (fLastDrawIndex < 0 || !bm->isValid()) {
		// first time
		fBackup.m_width = width;
		fBackup.m_hight = height;
		bm->allocateBitmap();
		fBackup.allocateBitmap();
		startIndex = 0;
	} else if (startIndex > fCurrIndex) {
		// rewind to 1st frame for repeat
		startIndex = 0;
	}
 
    int lastIndex = fCurrIndex;
    if (lastIndex < 0) {
        // first time
        lastIndex = 0;
    } else if (lastIndex > fGIF->ImageCount - 1) {
        // this block must not be reached.
        lastIndex = fGIF->ImageCount - 1;
    }
 
	CAColor4B bgColor;
    if (gif->SColorMap != NULL) {
        const GifColorType& col = gif->SColorMap->Colors[fGIF->SBackGroundColor];
		bgColor = ccc4(col.Red, col.Green, col.Blue, 0xFF);
    }
 
	static CAColor4B paintingColor;
    // draw each frames - not intelligent way
	for (int i = startIndex; i <= lastIndex; i++) {
		const SavedImage* cur = &fGIF->SavedImages[i];
		if (i == 0) {
			bool trans;
			int disposal;
			getTransparencyAndDisposalMethod(cur, &trans, &disposal);
			if (!trans && gif->SColorMap != NULL) {
				paintingColor = bgColor;
			}

			bm->eraseColor(paintingColor);
			fBackup.eraseColor(paintingColor);
		} else {
			// Dispose previous frame before move to next frame.
			const SavedImage* prev = &fGIF->SavedImages[i-1];
			disposeFrameIfNeeded(bm, prev, cur, &fBackup, paintingColor);
		}

		// Draw frame
		// We can skip this process if this index is not last and disposal
		// method == 2 or method == 3
      //  CCLog("lastIndex=%d, %d",lastIndex, fCurrIndex);
		if (i == lastIndex || !checkIfWillBeCleared(cur)) {
			drawFrame(bm, cur, gif->SColorMap);
		}
	}
 
    // save index
    fLastDrawIndex = lastIndex;

    return true;
}

NS_CC_END