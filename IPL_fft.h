#ifndef __FFT_H__
#define __FFT_H__


#include "define.h"


DLL_EXPORTS   template<typename T>  int  DFT_1D(T *src, complex_t *dst,  int N);
DLL_EXPORTS   template<typename T>  int  IDFT_1D(complex_t *src, T *dst, int N);  


#endif
