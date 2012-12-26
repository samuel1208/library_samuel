
#ifndef _IMAGE_PROCESS_CORE_H_
#define _IMAGE_PROCESS_CORE_H_


#include "define.h"

enum COLOR_CVT_FLAG
{
	IPL_RGB2HSL   = 0,
	IPL_RGB2HSV   = 1,
	IPL_RGB2GRAY  = 2,
    IPL_RGB2YCrCb = 3,
};

enum RESIZE_FLAG
{
	IPL_RESIZE_INTERPOLATION_BILINEAR  = 0,
};
#ifdef __cplusplus
extern "C" {
#endif

    DLL_EXPORTS  int  CVTColor(
                               const unsigned char *srcImg , int srcStep,
                                     unsigned char *dstImg,  int dstStep,
                                     int width, int height,  int CVTFormat
                              );

    DLL_EXPORTS  int  Resize(
                             const unsigned char *srcImg, int srcWidth, int srcHeight, int srcStep,
                                   unsigned char *dstImg, int dstWidth, int dstHeight, int dstStep,
                                   float xScale, float yScale, int channel, int InterpolationFlag 
                            );

    DLL_EXPORTS  int  IntegralImage(
                                    const unsigned char *srcImg,  int srcStep,  int srcWidth,  int srcHeight,
                                          unsigned int **intImg,  int &intStep, int &intWidth, int &intHeight
                                   );

    DLL_EXPORTS  int  dilate(
                             unsigned char *srcImg, int srcStep, 
                             unsigned char *temp,   int width_src, int height_src, int semi_win_size
                            );
    
    DLL_EXPORTS  int  erode(
                            unsigned char *srcImg, int srcStep, 
                            unsigned char *temp,   int width_src, int height_src, int semi_win_size
                           );

#ifdef __cplusplus
}
#endif
#endif  






