#ifndef __FFT_KERNEL_H__
#define __FFT_KERNEL_H__

#include "OpenCLCore.h"
#ifdef __cplusplus
extern "C" {
#endif

/*                 *** DFT***
                  N-1
                  ---
              1   \          - j*k*2*pi*n /N
      X(n) = ---   >   x(k) e                     n=0..N-1
              N   /                                
                  ---
                  k=0

                     ***IDFT***
               N-1
               ---
               \          j*k*2*pi*n /N
      X(n) =    >   x(k) e                     n=0..N-1
               /                                
               ---
               k=0
      e(j@) = cos(@) + jsin(@)

	 ###cos and sin function use search the table, lost some accurate.
*/

/*
    sign = 1  : FFT     real to complex  
    sign = -1 : IFFT    complex to real
*/

DLL_EXPORTS  int  FFT_1D_OCL(cl_mem src,  cl_mem dst, int N, int sign, cl_event *ev);


DLL_EXPORTS  int  FFT_2D_OCL(cl_mem src,  cl_mem dst, cl_mem temp, int N_x, int N_y, int sign,  cl_event *ev);

#ifdef __cplusplus
}
#endif

#endif