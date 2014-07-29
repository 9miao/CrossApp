

#include "platform/CCImage.h"
#include "images/CAImage.h"
#if defined(__native_client__) || defined(EMSCRIPTEN)
// TODO(sbc): I'm pretty sure all platforms should be including
// webph headers in this way.
#include "webp/decode.h"
#else
#include "decode.h"
#endif
#include "ccMacros.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

NS_CC_BEGIN

bool CCImage::_initWithWebpData(void *pData, int nDataLen)
{
	bool bRet = false;
	do
	{
        WebPDecoderConfig config;
        if (WebPInitDecoderConfig(&config) == 0) break;
        if (WebPGetFeatures((uint8_t*)pData, nDataLen, &config.input) != VP8_STATUS_OK) break;
        if (config.input.width == 0 || config.input.height == 0) break;
        
        config.output.colorspace = MODE_RGBA;
        m_nBitsPerComponent = 8;
        m_nWidth    = config.input.width;
        m_nHeight   = config.input.height;
        m_bHasAlpha = true;
        
        int bufferSize = m_nWidth * m_nHeight * 4;
        m_pData = new unsigned char[bufferSize];
        
        config.output.u.RGBA.rgba = (uint8_t*)m_pData;
        config.output.u.RGBA.stride = m_nWidth * 4;
        config.output.u.RGBA.size = bufferSize;
        config.output.is_external_memory = 1;

        if (WebPDecode((uint8_t*)pData, nDataLen, &config) != VP8_STATUS_OK)
        {
            delete []m_pData;
            m_pData = NULL;
            break;
        }
               
        bRet = true;
	} while (0);
	return bRet;
}

NS_CC_END
