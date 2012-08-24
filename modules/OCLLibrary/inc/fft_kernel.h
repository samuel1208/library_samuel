#ifndef __FFT_KERNEL_H__
#define __FFT_KERNEL_H__

#include "OpenCLCore.h"
#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORTS  int  FFT_1D_OCL(cl_mem src,  cl_mem dst, int N, int sign, cl_event *ev);

/*
sign = 1  : FFT
sign = -1 : IFFT
*/
DLL_EXPORTS  int  FFT_2D_OCL(cl_mem src,  cl_mem dst, cl_mem temp, int N_x, int N_y, int sign,  cl_event *ev);

#ifdef __cplusplus
}
#endif

#endif