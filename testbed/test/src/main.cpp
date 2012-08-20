
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "define.h"
#include <IPL_fft.h>
#include <IPL_core.h>
#include <Time_t.h>
int main(int argc, char **argv)
{
	int status  = -1;
	const int N=16;
	double  src[N] ={0};
	double  img[N] = {0};
	complex_t dst[N] ={0};
	
	for (int i=0; i<N; i++)
		src[i] = i;
        time_stamp(0, NULL);
	DFT_1D(src, dst, N);
	for(int i=0; i<N; i++)
		printf("%f, %f\n", dst[i].real, dst[i].imaginary);
	time_stamp(1, "DFT");

	printf("\n  \n");

	time_stamp(0, NULL);
	FFT_1D(src, dst,4);
	for(int i=0; i<N; i++)
		printf("\n%f, %f\n", dst[i].real, dst[i].imaginary);
	time_stamp(1, "FFT");
	return 0;
}
