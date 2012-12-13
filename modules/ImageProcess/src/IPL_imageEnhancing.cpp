

#include "IPL_imageEnhancing.h"
#include "MemManager.h"
#include "stdlib.h"

/*
  Histogram Equalization
 */
DLL_EXPORTS  int HistEqualization(unsigned char *srcImg, int widthStep, int width, int height)
{
    int i, j;
    double hist[256]={0};
    int max_pix, min_pix, val, pix_num;
    unsigned char *ptr=NULL;
 
    if (NULL==srcImg)
        return -1;

    //get the histgram
    for(i=0; i<height; i++)
    {
        ptr = srcImg + i*widthStep;
        for(j=0; j<width; j++)      
            hist[ptr[j]]++;       
    }

    //get the max and min pixel value
    for(i=0; i<256; ++i)
    {
        if(hist[i]>0)
        {
            min_pix = i;
            break;
        }
    }
    for(i=255; i>0; --i)
    {
        if(hist[i]>0)
        {
            max_pix = i;
            break;
        }
    }     
   
    // scan        
    pix_num = width * height;    
    hist[0] /= pix_num;
    for(i=1; i<256; i++)
        hist[i] =hist[i]/pix_num +  hist[i-1];   

    max_pix = max_pix - min_pix;
    for(i=0; i<height; ++i)
    {
        ptr = srcImg + i*widthStep;
        for(j=0; j<width; ++j)
        {
            val = (int)(hist[ptr[j]]*max_pix + min_pix + 0.5);
            ptr[j] = val;
        }
    }
    
    return 0;
}


DLL_EXPORTS  int  LaplaceEnhancing(
                                   const unsigned char *srcImg, int srcStep, 
                                         unsigned char *dstImg, int dstStep,
                                         int width, int height, int Connectivity
                                  )
{

    int w, h, val;
    const unsigned char *srcPtr;
    unsigned char *dstPtr;
 
    if ((NULL==srcImg) || (NULL==dstImg))
        return -1;

    if (4 == Connectivity)
    {
        //first line
        dstPtr = dstImg;
        srcPtr = srcImg;
        w=0;
        val = 5*srcPtr[w] - (srcPtr[w]+srcPtr[w+1]+srcPtr[w]+srcPtr[w+srcStep]);
        dstPtr[w] = SAM_MIN(SAM_MAX(val, 0),255);;
        for(w=1; w<width-1; ++w)
        {
            val = 5*srcPtr[w] - (srcPtr[w-1] + srcPtr[w+1] + srcPtr[w] + srcPtr[w+srcStep]); 
            dstPtr[w] = SAM_MIN(SAM_MAX(val, 0),255);;
        }
        val = 5*srcPtr[w] - (srcPtr[w-1] + srcPtr[w] + srcPtr[w] + srcPtr[w+srcStep]);
        dstPtr[w] = SAM_MIN(SAM_MAX(val, 0),255);

        //middle lines
        for(h=1; h<height-1; ++h)
        {
            srcPtr = srcImg + h*srcStep;
            dstPtr = dstImg + h*dstStep;
            w=0;
            val = 5*srcPtr[w] - (srcPtr[w] + srcPtr[w+1] + srcPtr[w-srcStep] + srcPtr[w+srcStep]);
            dstPtr[w] = SAM_MIN(SAM_MAX(val, 0),255);
            for(w=1; w<width-1; ++w)
            {
                val = 5*srcPtr[w] -(srcPtr[w-1] + srcPtr[w+1] + srcPtr[w-srcStep] + srcPtr[w+srcStep]);
                dstPtr[w] = SAM_MIN(SAM_MAX(val, 0),255);
            }
            val = 5*srcPtr[w] -(srcPtr[w-1] + srcPtr[w] + srcPtr[w-srcStep] + srcPtr[w+srcStep]);
            dstPtr[w] = SAM_MAX(SAM_MIN(val, 0),255);
        }

        // last lines
        srcPtr = srcImg + h*srcStep;
        dstPtr = dstImg + h*dstStep;
        w=0;
        val = 5*srcPtr[w] - (srcPtr[w] + srcPtr[w+1] + srcPtr[w-srcStep] + srcPtr[w]);
        dstPtr[w] = SAM_MAX(SAM_MIN(val, 0),255);
        for(w=1; w<width-1; ++w)
        {
            val =5*srcPtr[w] -(srcPtr[w-1] + srcPtr[w+1] + srcPtr[w-srcStep] + srcPtr[w]);
            dstPtr[w] = SAM_MIN(SAM_MAX(val, 0),255);
        }
        val = 5*srcPtr[w] -(srcPtr[w-1] + srcPtr[w] + srcPtr[w-srcStep] + srcPtr[w]);
        dstPtr[w] = SAM_MIN(SAM_MAX(val, 0),255);
    }
    else // default is 8
    {
        //first line
        dstPtr = dstImg;
        srcPtr = srcImg;
        w=0;
        val = 9*srcPtr[w] - (srcPtr[w]+srcPtr[w]+srcPtr[w+1]
                             +srcPtr[w]+srcPtr[w+1]
                             +srcPtr[w+srcStep]+srcPtr[w+srcStep]+srcPtr[w+1+srcStep]);
        dstPtr[w]= SAM_MIN(SAM_MAX(val, 0),255);
        for(w=1; w<width-1; ++w)
        {
            val = 9*srcPtr[w] - (srcPtr[w-1] + srcPtr[w] + srcPtr[w+1]
                                 +srcPtr[w-1] + srcPtr[w+1]
                                 +srcPtr[w-1+srcStep]+srcPtr[w+srcStep]+srcPtr[w+1+srcStep]);
            dstPtr[w]= SAM_MIN(SAM_MAX(val, 0),255);
        }
        val = 9*srcPtr[w] - (srcPtr[w-1] + srcPtr[w] + srcPtr[w]
                            +srcPtr[w-1] + srcPtr[w]
                            +srcPtr[w-1+srcStep] + srcPtr[w+srcStep] + srcPtr[w+srcStep]);
        dstPtr[w]= SAM_MIN(SAM_MAX(val, 0),255);
        //middle line
        for(h=1; h<height-1; ++h)
        {
            srcPtr = srcImg + h*srcStep;
            dstPtr = dstImg + h*dstStep;
            w=0;
            val = 9*srcPtr[w] - (srcPtr[w-srcStep] + srcPtr[w-srcStep] + srcPtr[w+1-srcStep]
                                +srcPtr[w-1] + srcPtr[w+1]
                                +srcPtr[w+srcStep] + srcPtr[w+srcStep] + srcPtr[w+1+srcStep]);
            dstPtr[w] = SAM_MIN(SAM_MAX(val, 0),255);
            for(w=1; w<width-1; ++w)
            {
                val = 9*srcPtr[w] -(srcPtr[w-1-srcStep] + srcPtr[w-srcStep] + srcPtr[w+1-srcStep]
                                   +srcPtr[w-1] + srcPtr[w+1]
                                   +srcPtr[w-1+srcStep] + srcPtr[w+srcStep] + srcPtr[w+1+srcStep]);
                dstPtr[w] = SAM_MIN(SAM_MAX(val, 0),255);
            }
            val = 9*srcPtr[w] -(srcPtr[w-1-srcStep] + srcPtr[w-srcStep] + srcPtr[w-srcStep]
                               +srcPtr[w-1] + srcPtr[w]
                               +srcPtr[w-1+srcStep] + srcPtr[w+srcStep] + srcPtr[w+srcStep]);
            dstPtr[w] = SAM_MAX(SAM_MIN(val, 0),255);
        }
        // last lines
        srcPtr = srcImg + h*srcStep;
        dstPtr = dstImg + h*dstStep;
        w=0;
        val = 9*srcPtr[w] - (srcPtr[w-srcStep] + srcPtr[w-srcStep] + srcPtr[w+1-srcStep]
                            +srcPtr[w] + srcPtr[w+1]
                            +srcPtr[w] + srcPtr[w] + srcPtr[w+1]);
        dstPtr[w]= SAM_MIN(SAM_MAX(val, 0),255);
        for(w=1; w<width-1; ++w)
        {
           val = 9*srcPtr[w] -(srcPtr[w-1-srcStep] + srcPtr[w-srcStep] + srcPtr[w+1-srcStep]
                              +srcPtr[w-1]+srcPtr[w+1]
                              +srcPtr[w-1] + srcPtr[w] + srcPtr[w+1]);
           dstPtr[w]= SAM_MIN(SAM_MAX(val, 0),255);
        }
        val = 9*srcPtr[w] - (srcPtr[w-1-srcStep] + srcPtr[w-srcStep] + srcPtr[w-srcStep]
                            +srcPtr[w-1] + srcPtr[w]
                            +srcPtr[w-1] + srcPtr[w] + srcPtr[w]);
        dstPtr[w]= SAM_MIN(SAM_MAX(val, 0),255);
    }
    return 0;
}
