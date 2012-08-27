
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
    N_2 = N >>1;

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

// use sqrt replace the sin cos operating
DLL_EXPORTS int FFT_1D_t(double *src, complex_t *dst, int N)
{
	int status = -1;
    int num, num2, i,j,k,l, i1,l1,l2;
    double c1,c2,t1,t2,u1,u2, z;

	__SAM_BEGIN__;

    if ((NULL == src) || (NULL == dst))
        EXIT;
    /* Calculate the number of points */
    num = 1;
    for (i=0;i<N;i++) 
        num <<= 1;

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

    status = 0;
    __SAM_END__;
    return status;
}

DLL_EXPORTS  int FFT_R2C_1D(double *src, complex_t *dst, int N)
{
	int status = -1;
    int m,i,j,k, num,num2, le, lei, ip;;
    double t1,t2, c1,c2, u1,u2, z;
    
	__SAM_BEGIN__;
    if ((NULL == src) || (NULL == dst))
        EXIT;

    num = 1;
    for (i=0;i<N;i++) 
        num <<= 1;

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
    status = 0;
    __SAM_END__;
    return status;
}
DLL_EXPORTS  int  FFT_C2R_1D(complex_t *src, double *dst, int N)
{
    int status = -1;
    int m,i,j,k, num,num2, le, lei, ip;;
    double t1,t2, c1,c2, u1,u2, z;
	complex_t *_dst = NULL;
    
	__SAM_BEGIN__;
    if ((NULL == src) || (NULL == dst))
        EXIT;

    num = 1;
    for (i=0;i<N;i++) 
        num <<= 1;

    /* Do the bit reversal */
    num2 = num >> 1;
    j = 0;

	_dst = (complex_t*) MM_MemAlloc(NULL, num*sizeof(complex_t));

	if(NULL == _dst)
		EXIT;

	MM_MemCpy(src, _dst, num*sizeof(complex_t));


    //Rader algorithm
    for (i=0;i<num-1;i++)
    {
        if (i < j)
	{
	    _dst[i] = src[j];
	    _dst[j] = src[i];	
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
	c2 = sin(PI_T/lei);
	
	for(j=0; j<lei; j++)
	{
	    for(i=j; i<num; i+=le)
	    {
	        ip = i+lei;		
		t1 = u1 * _dst[ip].real - u2 * _dst[ip].imaginary;
		t2 = u1 * _dst[ip].imaginary + u2 * _dst[ip].real;
		_dst[ip].real = _dst[i].real - t1; 
		_dst[ip].imaginary = _dst[i].imaginary - t2;
		_dst[i].real += t1;
		_dst[i].imaginary += t2;
	    }
	    z  = u1 * c1 - u2 * c2;
	    u2 = u1 * c2 + u2 * c1;
	    u1 = z;
	}

    }
    for (i=0;i<num;i++)
    {
        dst[i] = _dst[i].real;
    }
    status = 0;
    __SAM_END__;
	if(_dst) MM_MemFree(NULL, (void**)&_dst);
    return status;
}

DLL_EXPORTS  int FFT_C2C_1D(complex_t *src, complex_t *dst, int N, int dir)
{
	int status = -1;
    int m,i,j,k, num,num2, le, lei, ip;;
    double t1,t2, c1,c2, u1,u2, z;

	__SAM_BEGIN__;
    if ((NULL == src) || (NULL == dst) || (1 != dir*dir))
        EXIT;

    num = 1;
    for (i=0;i<N;i++) 
        num <<= 1;

    /* Do the bit reversal */
    num2 = num >> 1;
    j = 0;

    for (i=0;i<num;i++) 
	    dst[i]=src[i];

    //Rader algorithm
    for (i=0;i<num-1;i++)
    {
        if (i < j)
	{
	    dst[i] = src[j];
	    dst[j] = src[i];	
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
	c2 = -sin(PI_T/lei)*dir;
	
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
	if(1==dir)
	{
		for (i=0;i<num;i++)
		{
			dst[i].real /= num;
			dst[i].imaginary /= num;
		}
	}
    status = 0;
    __SAM_END__;
    return status;
}

DLL_EXPORTS  int  FFT_2D(double *src, complex_t *dst, int N_x, int N_y)
{
	int status = -1;
	int i, j, num_x=1, num_y=1;
	complex_t *temp = NULL;

	__SAM_BEGIN__;
	if ((NULL == src) || (NULL == dst))
		EXIT;

	for (i=0;i<N_x;i++) 
        num_x <<= 1;
	for (i=0;i<N_y;i++) 
        num_y <<= 1;

	temp = (complex_t *)MM_MemAlloc(NULL, num_x*num_y*sizeof(complex_t));
	if(NULL == temp)
		EXIT;

	for(i=0; i<num_y; i++)
		FFT_R2C_1D(src+i*num_x, temp+i*num_x, N_x);

	//transpose
	for(i=0; i<num_y; i++)
	{
		for(j=0; j<num_x; j++)
		{
			dst[j*num_y + i] = temp[i*num_x + j];
		}
	}

	for(i=0; i<num_y; i++)
		FFT_C2C_1D(dst+i*num_y, temp+i*num_y, N_y, 1);

	//transpose
	for(i=0; i<num_x; i++)
	{
		for(j=0; j<num_y; j++)
		{
			dst[j*num_x + i]= temp[i*num_y + j] ;
		}
	}
	
	status = 0;
    __SAM_END__;
	if(NULL != temp)MM_MemFree(NULL, (void**)(&temp));
    return status;

}
DLL_EXPORTS  int  IFFT_2D(complex_t *src, double *dst, int N_x, int N_y)
{
	int status = -1;
	int i, j, num_x=1, num_y=1;
	complex_t *temp0 = NULL,  *temp1=NULL;
	double *temp2 = NULL;

	__SAM_BEGIN__;
	if ((NULL == src) || (NULL == dst))
		EXIT;

	for (i=0;i<N_x;i++) 
        num_x <<= 1;
	for (i=0;i<N_y;i++) 
        num_y <<= 1;

	temp0 = (complex_t *)MM_MemAlloc(NULL, num_x*num_y*sizeof(complex_t));
	temp1 = (complex_t *)MM_MemAlloc(NULL, num_x*num_y*sizeof(complex_t));
	temp2 = (double *)MM_MemAlloc(NULL, num_x*num_y*sizeof(double));
	if ((NULL == temp0) || (NULL == temp1) || (NULL == temp2))
		EXIT;

	for(i=0; i<num_y; i++)
		FFT_C2C_1D(src+i*num_x, temp0+i*num_x, N_x, -1);
	//transpose
	for(i=0; i<num_y; i++)
	{
		for(j=0; j<num_x; j++)
		{
			temp1[j*num_y + i] = temp0[i*num_x + j];
		}
	}

	for(i=0; i<num_y; i++)
		FFT_C2R_1D(temp1+i*num_y, temp2+i*num_y, N_y);

	//transpose
	for(i=0; i<num_x; i++)
	{
		for(j=0; j<num_y; j++)
		{
			dst[j*num_x + i]= temp2[i*num_y + j] ;
		}
	}
	
	status = 0;
    __SAM_END__;
	if(NULL != temp0)MM_MemFree(NULL, (void**)(&temp0));
	if(NULL != temp1)MM_MemFree(NULL, (void**)(&temp1));
	if(NULL != temp2)MM_MemFree(NULL, (void**)(&temp2));
    return status;

}
