
#include <stdio.h>
#include <math.h>
#include "IPL_fft.h"
#include "MemManager.h"

/*
                N-1  
    x(n) = 1/N * Σx(k)*e(-j*k*2*Pi*n/N)  for n=0,..N-1
                k=0
    e(j@) = cos(@) + jsin(@)
*/

 DLL_EXPORTS  int  DFT_1D(double *src,  complex_t *dst, int N)
{
    int  status = -1;
    int  i, j; 
    double temp=0, angle=0;
    int  N_2 = 0;

    __SAM_BEGIN__;
    
    if ((NULL == src) || (NULL == dst))
        EXIT;

    N_2 = N / 2;
    MM_MemSet(dst,0, sizeof(*dst)*N);
    for (i=0; i<=N_2; i++)
    { 
        temp = 2*PI_T*i/N;
	angle = 0;
        for (j=0; j<N; j++)
	{
	    dst[i].real += src[j] * cos(angle);
	    dst[i].imaginary -= src[j] * sin(angle);
	    angle += temp;
        }
	dst[i].real /= N;
	dst[i].imaginary /= N;
    }
    
    status = 0;
    __SAM_END__;
    return status;
}

/*
            N-1  
    x(n)  =  Σx(k)*e(-j*k*2*Pi*n/N)  for n=0,..N-1
            k=0
    e(j@) = cos(@) + jsin(@)
*/
DLL_EXPORTS   int  IDFT_1D(complex_t *src, double *dst, int N)
{
    int  status = -1;
    int  i, j; 
    double  temp=0, angle=0, value=0; 
    int N_2 = 0, index=0; 

    __SAM_BEGIN__;
    
    if ((NULL == src) || (NULL == dst))
        EXIT;
    N_2 = N / 2;

    MM_MemSet(dst,0, sizeof(*dst)*N);
    for (i=0; i<N; i++)
    { 
        temp = 2*PI_T*i/N;
	value = angle = 0;
        for (j=0; j<N; j++)
        {
	    if(j > N_2)  
	    {
	        index = N - j;
		value  +=  src[index].real * cos(angle) +  src[index].imaginary * sin(angle);
	    }
	    else
	        value  +=  src[j].real * cos(angle) - src[j].imaginary * sin(angle);
       	   
	    angle += temp;
        }
	dst[i] = value;
    }

    status = 0;
    __SAM_END__;
    return status;
}



DLL_EXPORTS  int  FFT_1D(double *src,  complex_t *dst, int N)
{
    int  status = -1;
    int  i, j; 
 
    __SAM_BEGIN__;
    
    if ((NULL == src) || (NULL == dst))
        EXIT;

    status = 0;
    __SAM_END__;
    return status;
}

//DLL_EXPORTS  int  IFFT_1D(complex_t *src, double *dst, int N);
