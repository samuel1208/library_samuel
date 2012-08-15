
#ifndef _IMAGE_PROCESS_CORE_H_
#define _IMAGE_PROCESS_CORE_H_


#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORTS int  MedianFilter(
						      const unsigned char *srcImg , int srcStep,
				              unsigned char *dstImg,  int dstStep,
				              int width, int height,  int kernelSize
						 );

DLL_EXPORTS int  GaussianFilter(const unsigned char *srcImg , int srcStep,
							    unsigned char *dstImg,  int dstStep,
							    int width, int height,  int channel,
							    float *gauss_kernel,    int kernelSize, float kernelSum);


#ifdef __cplusplus
}
#endif

#endif  






