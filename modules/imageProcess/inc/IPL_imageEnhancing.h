#ifndef _IMAGE_ENHANCING_H_
#define _IMAGE_ENHANCING_H_


#include "define.h"
#ifdef __cplusplus
extern "C" {
#endif

    DLL_EXPORTS  int  HistEqualization(unsigned char *srcImg, int widthStep, int width, int height);

/*
  method =4 or 8; use 4 or 8 near pixel to compute the laplace factor
*/
    DLL_EXPORTS  int  LaplaceEnhancing(
                                       const unsigned char *srcImg, int srcStep, 
                                             unsigned char *dstImg, int dstStep, 
                                             int width, int height, int Connectivity
                                       );

#ifdef __cplusplus
}
#endif
#endif  
