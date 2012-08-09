
#ifndef _IMAGE_PROCESS_CORE_H_
#define _IMAGE_PROCESS_CORE_H_


#include "define.h"

enum COLOR_CVT_FLAG
{
	IPL_RGB2HSL  = 0,
	IPL_RGB2HSV  = 1,
	IPL_RGB2GRAY = 2,
};

enum RESIZE_FLAG
{
	IPL_RESIZE_INTERPOLATION_BILINEAR  = 0,
};

DLL_EXPORTS int  CVTColor(
					      const unsigned char *srcImg , int srcStep,
				          unsigned char *dstImg,  int dstStep,
				          int width, int height,  int CVTFormat
					 );

DLL_EXPORTS int  Resize(const unsigned char *srcImg, int srcWidth, int srcHeight, int srcStep,
					    unsigned char *dstImg, int dstWidth, int dstHeight, int dstStep,
					    float xScale, float yScale, int channel, int InterpolationFlag );

#endif  






