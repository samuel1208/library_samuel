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
    const int N=16;
    double  src[N] ={0};
    double  img[N] = {0};
    complex_t dst1[N] = {0} ;
    complex_t dst2[N] = {0};
    complex_t dst3[N] = {0};
	
    for (int i=0; i<N; i++)
        src[i] = i;
    time_stamp(0, NULL);
    // for(int i=0 ; i<1; i++)
	DFT_1D(src, dst1, N);
    for(int i=0; i<8; i++)
        printf("%f, %f\n", dst1[i].real, dst1[i].imaginary);
    time_stamp(1, "DFT");

    printf("\n  \n");

    time_stamp(0, NULL);
    // for(int i=0; i<1; i++)
	FFT_1D(src, dst2,4);
    for(int i=0; i<8; i++)
      	printf("%f, %f\n", dst2[i].real, dst2[i].imaginary);
    time_stamp(1, "FFT");
 
    printf("\n  \n");

    time_stamp(0, NULL);
    // for(int i=0; i<1; i++)
	FFT_1D_1(src, dst3,4);
    for(int i=0; i<8; i++)
      	printf("%f, %f\n", dst3[i].real, dst3[i].imaginary);
    return 0;
}
