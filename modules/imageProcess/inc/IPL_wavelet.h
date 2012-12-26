#ifndef _IPL_WAVELET_H_
#define _IPL_WAVELET_H_

#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif

// need float image data
DLL_EXPORTS  void  DWT(float  *srcImg, int widthStep, int width, int height, int channel, int nLayer);

DLL_EXPORTS  void  IDWT(float  *srcImg, int widthStep, int width, int height, int channel, int nLayer);

#ifdef __cplusplus
}
#endif

#endif