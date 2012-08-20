
#include <stdio.h>
#include <math.h>
#include "IPL_fft.h"
#include "MemManager.h"

/*                  DFT
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

/*                  IDFT
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

DLL_EXPORTS int FFT_1D(double *src, complex_t *dst, int N)
{
   long n,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   n = 1;
   for (i=0;i<N;i++) 
      n *= 2;

   /* Do the bit reversal */
   i2 = n >> 1;
   j = 0;

for (i=0;i<n;i++) {
  dst[i].imaginary=0;
  dst[i].real=src[i];
 }

for (i=0;i<n-1;i++) {
      if (i < j) {
	dst[i].real = src[j];
	dst[j].real = src[i];	
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

 for(i=0; i<n; i++)
   printf("%f\n", dst[i].real);
   /* Compute the FFT */

   c1 = -1.0; 
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<N;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<n;i+=l2) {
            i1 = i + l1;
            t1 = u1 * dst[i1].real - u2 * dst[i1].imaginary;
            t2 = u1 * dst[i1].imaginary + u2 * dst[i1].real;
            dst[i1].real = dst[i].real - t1; 
            dst[i1].imaginary = dst[i].imaginary - t2;
            dst[i].real += t1;
            dst[i].imaginary += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = -sqrt((1.0 - c1) / 2.0);
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   for (i=0;i<n;i++)
   {
      dst[i].real /= n;
      dst[i].imaginary /= n;
   }
   
   return  0;
}
//DLL_EXPORTS  int  IFFT_1D(complex_t *src, double *dst, int N);
