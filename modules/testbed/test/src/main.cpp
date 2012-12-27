#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include <IPL_fft.h>
#include <IPL_core.h>
#include <IPL_imageEnhancing.h>
#include <Time_t.h>
#include "MemManager.h"
#include "cv.h"
#include "highgui.h"
#include "queue_t.hpp"
#include "IPL_gradient.hpp"
unsigned int log2( unsigned int x )
{
    unsigned int ans = 0 ;
    while( x>>=1 ) ans++;
    return ans ;
}
/*     test FFT*/
#if 0
int main(int argc, char **argv)
{
    int status  = -1;
    const int N=16;
    double  src[N] ={0};
    double  img[N] = {0};
    complex_t dst1[N] = {0} ;
    complex_t dst2[N] = {0};
    complex_t dst3[N] = {0};
	complex_t dst4[N] = {0};
	
    for (int i=0; i<N; i++)
        src[i] = i;
    time_stamp(0, NULL);
    // for(int i=0 ; i<1; i++)
	DFT_1D(src, dst1, N);
    for(int i=0; i<8; i++)
        printf("%f, %f\n", dst1[i].real, dst1[i].imaginary);
    time_stamp(1, "DFT");

    printf("\n FFT_1D : \n");

    time_stamp(0, NULL);
    // for(int i=0; i<1; i++)
	FFT_R2C_1D(src, dst2,log2((unsigned int)N));
    for(int i=0; i<8; i++)
      	printf("%f, %f\n", dst2[i].real, dst2[i].imaginary);
    time_stamp(1, "FFT");
 
    printf("\n FFT_1D_1: \n");

    time_stamp(0, NULL);
    // for(int i=0; i<1; i++)
	FFT_R2C_1D(src, dst3,log2((unsigned int)N));
    for(int i=0; i<N; i++)
      	printf("%f, %f\n", dst3[i].real, dst3[i].imaginary);
	printf("\n FFT_1D_2: \n");

    return 0;
}
#elif 0

int main(int argc, char** argv)
{
    unsigned char *src =(unsigned char*)MM_MemAlloc(NULL, 4*4);
    unsigned int *intImg = NULL;
    int i,j,w,h,s;
   
    for(i=0; i<4; i++)
    {
        for(j=0; j<4; j++)
        {
            src[j+i*4]=j;
            printf("%2d ", src[j+i*4]);
        }
        printf("\n");
    }
   
    IntegralImage(src, 4,4,4, &intImg, s,w,h);
    printf("%d, %d, %d \n", w, h, s);
    for(i=0; i<h; i++)
    {
        for(j=0; j<w; j++)
            printf("%4d  ", intImg[j+i*w]);
        printf("\n");
    }
    MM_MemFree(NULL, (void**)&src);
    MM_MemFree(NULL, (void**)&intImg);
    printf("Finished \n");
    return 0;
}

#elif 0
int main(int argc, char** argv)
{
    IplImage *src = cvLoadImage("/home/fshen/samuel/res_20_0.jpg");
    IplImage *_src = cvCreateImage(cvSize(src->width/2, src->height/2), 8, 3);    
    IplImage *gray = cvCreateImage(cvSize(_src->width, _src->height), 8, 1);
    IplImage *dstGray = cvCreateImage(cvSize(_src->width, _src->height), 8, 1);

    Resize((unsigned char*)src->imageData,src->width, src->height, src->widthStep,
           (unsigned char*)_src->imageData,_src->width, _src->height, _src->widthStep,
           src->width*1.0/_src->width,src->height*1.0/_src->height, 3,0 );

    CVTColor((unsigned char*)_src->imageData, _src->widthStep,(unsigned char*) gray->imageData, gray->widthStep,gray->width, gray->height, IPL_RGB2GRAY);

    cvNamedWindow("src_gray");
    cvShowImage("src_gray", gray);
    cvWaitKey(0);
    //HistEqualization((unsigned char*)gray->imageData, gray->widthStep, gray->width, gray->height);
    LaplaceEnhancing((unsigned char*)gray->imageData, gray->widthStep, (unsigned char*)dstGray->imageData, dstGray->widthStep, gray->width, gray->height, 4);
    cvNamedWindow("dst_gray");
    cvShowImage("dst_gray", dstGray);
    cvWaitKey(0);
    cvReleaseImage(&src);
    cvReleaseImage(&_src);
    cvReleaseImage(&gray);
    return 0;
}

#else //test the multithread
#include "multiThread.h"

ThreadRes  kernel(void* para)
{
    printf("thread %d\n", *((int*)(para)));
    return (ThreadRes) -10;

}
int main(int argc, char** argv)
{
 
    int thread[10]={1,2,3,4,5,6,7,8,9,10};
    MHandle thread_handle[10];
    ThreadRes result[10];
    for(int i=0; i<10; i++)
        thread_handle[i] = MThreadCreate((void*)kernel, &thread[i]);

    for(int i=0; i<10; i++)
        MWaitForSingleThread(thread_handle[i],&result[i], MWAIT_INFINITE);

    for(int i=0; i<10; i++)
    {
        printf("%d \n", (int)result[i]);
    }

    //destroy the threads
	for(int i=0; i<10; i++)
	{
		MThreadDestory(thread_handle[i]);
	}

   
    return 0;
   
}


#endif
