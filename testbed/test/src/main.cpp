#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include <IPL_fft.h>
#include <IPL_core.h>
#include <Time_t.h>

int main(int argc, char **argv)
{
	int status  = -1;
	const int N=1024;
	double  src[N] ={0};
	double  img[N] = {0};
	complex_t dst1[N] = {0} ;
	complex_t dst2[N] = {0};
	
	for (int i=0; i<N; i++)
		src[i] = i;
	// time_stamp(0, NULL);
	for(int i=0 ; i<1; i++)
	DFT_1D(src, dst1, N);
	//for(int i=0; i<9; i++)
	//	printf("%f, %f\n", dst1[i].real, dst1[i].imaginary);
	//	time_stamp(1, "DFT");

	printf("\n  \n");

	time_stamp(0, NULL);
	for(int i=0; i<1; i++)
	FFT_1D(src, dst2,10);
	//	for(int i=0; i<9; i++)
	//	printf("\n%f, %f\n", dst2[i].real, dst2[i].imaginary);
	time_stamp(1, "FFT");
	for( int i=0; i< N/2; i++)
	{
	  if ((fabs(dst1[i].real- dst2[i].real)<0.000001f) || (fabs(dst1[i].imaginary-dst2[i].imaginary)<0.000001f))
	    {
	      printf("%d: error\n ", i);
	      break;
	    }
	}
	return 0;
}
