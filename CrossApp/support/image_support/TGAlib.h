

#ifndef __SUPPORT_DATA_SUPPORT_TGALIB_H__
#define __SUPPORT_DATA_SUPPORT_TGALIB_H__

namespace CrossApp {

enum {
    TGA_OK,
    TGA_ERROR_FILE_OPEN,
    TGA_ERROR_READING_FILE,
    TGA_ERROR_INDEXED_COLOR,
    TGA_ERROR_MEMORY,
    TGA_ERROR_COMPRESSED_FILE,
};

/** TGA format */
typedef struct sImageTGA {
    int status;
    unsigned char type, pixelDepth;
    
    /** map width */
    signed short width;
    
    /** map height */
    signed short height;
    
    /** raw data */
    unsigned char *imageData;
    int flipped;
} tImageTGA;

/// load the image header fields. We only keep those that matter!
bool tgaLoadHeader(unsigned char *Buffer, unsigned long bufSize, tImageTGA *psInfo);

/// loads the image pixels. You shouldn't call this function directly
bool tgaLoadImageData(unsigned char *Buffer, unsigned long bufSize, tImageTGA *psInfo);

/// this is the function to call when we want to load an image
tImageTGA * tgaLoad(const char *pszFilename);

// /converts RGB to grayscale
void tgaRGBtogreyscale(tImageTGA *psInfo);

/// releases the memory used for the image
void tgaDestroy(tImageTGA *psInfo);

}//namespace CrossApp 

#endif // __SUPPORT_DATA_SUPPORT_TGALIB_H__
