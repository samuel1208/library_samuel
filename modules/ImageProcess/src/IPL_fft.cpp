
#include <stdio.h>
#include <math.h>
#include "IPL_fft.h"
#include "MemManager.h"

/*                  *** DFT***
                  N-1
                  ---
              1   \          - j*k*2*pi*n /N
      X(n) = ---   >   x(k) e                     n=0..N-1
              N   /                                
                  ---
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
	dst[i].real /= 1;
	dst[i].imaginary /= 1;
    }
    
    status = 0;
    __SAM_END__;
    return status;
}

/*                    ***IDFT***
               N-1
               ---
               \          - j*k*2*pi*n /N
      X(n) =    >   x(k) e                     n=0..N-1
               /                                
               ---
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

DLL_EXPORTS int FFT_1D(double *src, complex_t *dst, int N)
{
    int num, num2, i,j,k,l, i1,l1,l2;
    double c1,c2,t1,t2,u1,u2, z;

    if ((NULL == src) || (NULL == dst))
        return -1;
    /* Calculate the number of points */
    num = 1;
    for (i=0;i<N;i++) 
        num *= 2;

    /* Do the bit reversal */
    num2 = num >> 1;
    j = 0;

    for (i=0;i<num;i++) 
    {
        dst[i].imaginary=0;
	dst[i].real=src[i];
    }

    //Rader algorithm
    for (i=0;i<num-1;i++)
    {
        if (i < j)
	{
	    dst[i].real = src[j];
	    dst[j].real = src[i];	
	}
	k = num2;
	while (k <= j) 
	{
	    j -= k;
	    k >>= 1;
	}
	j += k;
    }

    c1 = -1.0; 
    c2 = 0.0;
    l2 = 1;
    for (l=0;l<N;l++) 
    {
        l1 = l2;
	l2 <<= 1;
	u1 = 1.0; 
	u2 = 0.0;
	for (j=0;j<l1;j++) 
	{
	    for (i=j;i<num;i+=l2) 
	    {
	        i1 = i + l1;
		t1 = u1 * dst[i1].real - u2 * dst[i1].imaginary;
		t2 = u1 * dst[i1].imaginary + u2 * dst[i1].real;
		dst[i1].real = dst[i].real - t1; 
		dst[i1].imaginary = dst[i].imaginary - t2;
		dst[i].real += t1;
		dst[i].imaginary += t2;
	    }
	    z  = u1 * c1 - u2 * c2;
	    u2 = u1 * c2 + u2 * c1;
	    u1 = z;
	}
	c2 = -sqrt((1.0 - c1) / 2.0);
	c1 = sqrt((1.0 + c1) / 2.0);
    }

    /* Scaling for forward transform */
    for (i=0;i<num;i++)
    {
        dst[i].real /= num;
	dst[i].imaginary /= num;
    }
    return  0;
}

DLL_EXPORTS  int FFT_1D_1(double *src, complex_t *dst, int N)
{
    int m,i,j,k, num,num2, le, lei, ip;;
    double t1,t2, c1,c2, u1,u2, z;
    
    if ((NULL == src) || (NULL == dst))
        return -1;

    num = 1;
    for (i=0;i<N;i++) 
        num *= 2;

    /* Do the bit reversal */
    num2 = num >> 1;
    j = 0;

    for (i=0;i<num;i++) 
    {
        dst[i].imaginary=0;
	dst[i].real=src[i];
    }

    //Rader algorithm
    for (i=0;i<num-1;i++)
    {
        if (i < j)
	{
	    dst[i].real = src[j];
	    dst[j].real = src[i];	
	}
	k = num2;
	while (k <= j) 
	{
	    j -= k;
	    k >>= 1;
	}
	j += k;
    }

    

    for(m=1; m<=N; m++)
    {
        le = 2<<(m-1);
	lei = le/2;
	u1 = 1.0;
	u2 = 0.0;
	c1 = cos(PI_T/lei);
	c2 = -sin(PI_T/lei);
	
	for(j=0; j<lei; j++)
	{
	    for(i=j; i<num; i+=le)
	    {
	        ip = i+lei;		
		t1 = u1 * dst[ip].real - u2 * dst[ip].imaginary;
		t2 = u1 * dst[ip].imaginary + u2 * dst[ip].real;
		dst[ip].real = dst[i].real - t1; 
		dst[ip].imaginary = dst[i].imaginary - t2;
		dst[i].real += t1;
		dst[i].imaginary += t2;
	    }
	    z  = u1 * c1 - u2 * c2;
	    u2 = u1 * c2 + u2 * c1;
	    u1 = z;
	}

    }
    for (i=0;i<num;i++)
    {
        dst[i].real /= num;
	dst[i].imaginary /= num;
    }
    return 0;
}
//DLL_EXPORTS  int  IFFT_1D(complex_t *src, double *dst, int N);
