
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "define.h"
#include <IPL_fft.h>
#include <IPL_core.h>

int main(int argc, char **argv)
{
	int status  = -1;
	const int N=8;
	double  src[N] ={0};
	complex_t dst[N] ={0};
	
	for (int i=0; i<N; i++)
		src[i] = i;
   
	DFT_1D(src, dst, N);
	for(int i=0; i<N; i++)
		printf("%f, %f\n", dst[i].real, dst[i].imaginary);

	IDFT_1D(dst, src, N);
	printf("\nIDFT:\n");
	for(int i=0; i<N; i++)
		printf("%f \n", src[i]);
	return 0;
}
