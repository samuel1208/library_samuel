#ifndef __FFT_H__
#define __FFT_H__

#include "define.h"

DLL_EXPORTS  int  DFT_1D(double *src,  complex_t *dst, int N);

DLL_EXPORTS  int  IDFT_1D(complex_t *src, double *dst, int N);

DLL_EXPORTS  int  FFT_1D(double *src,  complex_t *dst, int N);

DLL_EXPORTS  int  IFFT_1D(complex_t *src, double *dst, int N);

#endif
