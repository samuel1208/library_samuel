#ifndef __FFT_H__
#define __FFT_H__

#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORTS  int  DFT_1D(double *src,  complex_t *dst, int N);

DLL_EXPORTS  int  IDFT_1D(complex_t *src, double *dst, int N);

DLL_EXPORTS  int  FFT_R2C_1D(double *src,  complex_t *dst, int N);
DLL_EXPORTS  int  FFT_C2C_1D(complex_t *src, complex_t *dst, int N, int dir);
DLL_EXPORTS  int  FFT_C2R_1D(complex_t *src, double *dst,    int N);

DLL_EXPORTS  int  FFT_2D(double *src, complex_t *dst, int N_x, int N_y);
DLL_EXPORTS  int  IFFT_2D(complex_t *src, double *dst, int N_x, int N_y);

#ifdef __cplusplus
}
#endif

#endif
