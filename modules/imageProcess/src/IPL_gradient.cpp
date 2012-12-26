#include "IPL_gradient.hpp"
#include "IPL_core.h"
#include "MemManager.h"

template<typename T>
DLL_EXPORTS int gradient_sobel(
                               T *gradient, int gradStep, 
                               unsigned char *gray, int grayStep, 
                               int width, int height
                               )
{
    int res = -1;
    int w, h;
    unsigned char *srcPtr = NULL;
    T *dstPtr = NULL;
    int grad_x, grad_y;
    __SAM_BEGIN__;
    
    if((NULL == gradient) || (NULL == gray))
        EXIT;

    //first line
    srcPtr = gray;
    dstPtr = gradient;
    w=0;
    grad_x = srcPtr[w+1] + 2*srcPtr[w+1] +  srcPtr[w+1+grayStep]
           -(srcPtr[w] + 2*srcPtr[w] +  srcPtr[w+grayStep]);
    grad_y = srcPtr[w+grayStep] + 2*srcPtr[w+grayStep] +  srcPtr[w+1+grayStep]
           -(srcPtr[w] + 2*srcPtr[w] +  srcPtr[w+1]);
    grad_x = SAM_ABS(grad_x);
    grad_y = SAM_ABS(grad_y);
    dstPtr[w] = grad_x + grad_y;
    for(w=1; w<width-1; w++)
    {
        grad_x = srcPtr[w+1] + 2*srcPtr[w+1] +  srcPtr[w+1+grayStep]
                -(srcPtr[w-1] + 2*srcPtr[w-1] +  srcPtr[w-1+grayStep]);
        grad_y = srcPtr[w-1+grayStep] + 2*srcPtr[w+grayStep] +  srcPtr[w+1+grayStep]
                -(srcPtr[w-1] + 2*srcPtr[w] +  srcPtr[w+1]);
        grad_x = SAM_ABS(grad_x);
        grad_y = SAM_ABS(grad_y);
        dstPtr[w] = grad_x + grad_y;        
    }
    grad_x = srcPtr[w] + 2*srcPtr[w] +  srcPtr[w+grayStep]
           -(srcPtr[w-1] + 2*srcPtr[w-1] +  srcPtr[w-1+grayStep]);
    grad_y = srcPtr[w-1+grayStep] + 2*srcPtr[w+grayStep] +  srcPtr[w+grayStep]
           -(srcPtr[w-1] + 2*srcPtr[w] +  srcPtr[w]);
    grad_x = SAM_ABS(grad_x);
    grad_y = SAM_ABS(grad_y);
    dstPtr[w] = grad_x + grad_y;
    //middle line
    for(h=1; h<height-1; h++)
    {
        srcPtr = gray + h*grayStep;
        dstPtr = gradient + h*gradStep;        
        w=0;
        grad_x = srcPtr[w+1-grayStep] + 2*srcPtr[w+1] +  srcPtr[w+1+grayStep]
               -(srcPtr[w-grayStep] + 2*srcPtr[w] +  srcPtr[w+grayStep]);
        grad_y = srcPtr[w+grayStep] + 2*srcPtr[w+grayStep] +  srcPtr[w+1+grayStep]
               -(srcPtr[w-grayStep] + 2*srcPtr[w-grayStep] +  srcPtr[w+1-grayStep]);
        grad_x = SAM_ABS(grad_x);
        grad_y = SAM_ABS(grad_y);
        dstPtr[w] = grad_x + grad_y;

        for(w=1; w<width-1; w++)
        {
            grad_x = srcPtr[w+1-grayStep] + 2*srcPtr[w+1] +  srcPtr[w+1+grayStep]
                   -(srcPtr[w-1-grayStep] + 2*srcPtr[w-1] +  srcPtr[w-1+grayStep]);
            grad_y = srcPtr[w-1+grayStep] + 2*srcPtr[w+grayStep] +  srcPtr[w+1+grayStep]
                   -(srcPtr[w-1-grayStep] + 2*srcPtr[w-grayStep] +  srcPtr[w+1-grayStep]);
            grad_x = SAM_ABS(grad_x);
            grad_y = SAM_ABS(grad_y);
            dstPtr[w] = grad_x + grad_y;
        }
        grad_x = srcPtr[w-grayStep] + 2*srcPtr[w] +  srcPtr[w+grayStep]
               -(srcPtr[w-1-grayStep] + 2*srcPtr[w-1] +  srcPtr[w-1+grayStep]);
        grad_y = srcPtr[w-1+grayStep] + 2*srcPtr[w+grayStep] +  srcPtr[w+grayStep]
                   -(srcPtr[w-1-grayStep] + 2*srcPtr[w-grayStep] +  srcPtr[w-grayStep]);
        grad_x = SAM_ABS(grad_x);
        grad_y = SAM_ABS(grad_y);
        dstPtr[w] = grad_x + grad_y;
    }
    //last line
    srcPtr = gray + h*grayStep;
    dstPtr = gradient + h*gradStep;
    w=0;
    grad_x = srcPtr[w+1-grayStep] + 2*srcPtr[w+1] +  srcPtr[w+1]
           -(srcPtr[w-grayStep] + 2*srcPtr[w] +  srcPtr[w]);
    grad_y = srcPtr[w] + 2*srcPtr[w] +  srcPtr[w+1]
           -(srcPtr[w-grayStep] + 2*srcPtr[w-grayStep] +  srcPtr[w+1-grayStep]);
    grad_x = SAM_ABS(grad_x);
    grad_y = SAM_ABS(grad_y);
    dstPtr[w] = grad_x + grad_y;
    for(w=1; w<width-1; w++)
    {
        grad_x = srcPtr[w+1-grayStep] + 2*srcPtr[w+1] +  srcPtr[w+1]
                -(srcPtr[w-1-grayStep] + 2*srcPtr[w-1] +  srcPtr[w-1]);
        grad_y = srcPtr[w-1] + 2*srcPtr[w] +  srcPtr[w+1]
                -(srcPtr[w-1-grayStep] + 2*srcPtr[w-grayStep] +  srcPtr[w+1-grayStep]);
        grad_x = SAM_ABS(grad_x);
        grad_y = SAM_ABS(grad_y);
        dstPtr[w] = grad_x + grad_y;        
    }
    grad_x = srcPtr[w-grayStep] + 2*srcPtr[w] +  srcPtr[w]
           -(srcPtr[w-1-grayStep] + 2*srcPtr[w-1] +  srcPtr[w-1]);
    grad_y = srcPtr[w-1] + 2*srcPtr[w] +  srcPtr[w]
           -(srcPtr[w-1-grayStep] + 2*srcPtr[w-grayStep] +  srcPtr[w-grayStep]);
    grad_x = SAM_ABS(grad_x);
    grad_y = SAM_ABS(grad_y);
    dstPtr[w] = grad_x + grad_y;

    res = 0;
    __SAM_END__;
    return res;
}


template<typename T>
DLL_EXPORTS  int  gradient_roberts(
                                   T *gradient, int gradStep,
                                   unsigned char *gray, int grayStep, 
                                   int width, int height
                                   )
{
    int res = -1;
    int w, h;
    unsigned char *srcPtr = NULL;
    T *dstPtr = NULL;
    int grad_x, grad_y;
    __SAM_BEGIN__;
    
    if((NULL == gradient) || (NULL == gray))
        EXIT;

    for(h=0; h<height-1; h++)
    {
        srcPtr = gray + h*grayStep;
        dstPtr = gradient + h*gradStep;        
        for(w=0; w<width-1; w++)
        {
            grad_x = srcPtr[w+1+grayStep] - srcPtr[w];
            grad_y = srcPtr[w+grayStep] - srcPtr[w+1];
            grad_x = SAM_ABS(grad_x);
            grad_y = SAM_ABS(grad_y);
            dstPtr[w] = grad_x + grad_y;
        }
        grad_y = grad_x = srcPtr[w+grayStep] - srcPtr[w];
        grad_y = SAM_ABS(grad_y);
        dstPtr[w] = 2*grad_y;
    }
    //last line
    srcPtr = gray + h*grayStep;
    dstPtr = gradient + h*gradStep;   
    for(w=0; w<width-1; w++)
    {
        grad_x = srcPtr[w+1] - srcPtr[w];
        grad_y = srcPtr[w] - srcPtr[w+1]; 
        grad_x = SAM_ABS(grad_x);
        grad_y = SAM_ABS(grad_y);
        dstPtr[w] = grad_x + grad_y;        
    }
    dstPtr[w] = 0;

    res = 0;
    __SAM_END__;
    return res;
}

/*  
                               Morphology gradient
*/
template<typename T>
DLL_EXPORTS  int  gradient_mutilScale(
                                      T *gradient, int gradStep,
                                      unsigned char *gray, int grayStep, 
                                      int width, int height, int level, int average
                                      )
{
    int res = -1;
    int w, h, l, _l;
    unsigned char *min = NULL, *max = NULL, *diff = NULL, *temp = NULL;
    unsigned char *cPtr1, *cPtr2, *cPtr3; 
    T *dstPtr = NULL;

    __SAM_BEGIN__;
    
    if((NULL == gradient) || (NULL == gray) || (level<2))
        EXIT;
    MM_MemSet(gradient, 0, gradStep*height*sizeof(T));
    max = (unsigned char*) MM_MemAlloc(NULL, grayStep*height);
    min = (unsigned char*) MM_MemAlloc(NULL, grayStep*height);
    diff = (unsigned char*) MM_MemAlloc(NULL, grayStep*height);
    temp = (unsigned char*) MM_MemAlloc(NULL, grayStep*height);
        
    if ((NULL == max) || (NULL == min) || (NULL==diff) || (NULL==temp))
        EXIT;

    MM_MemCpy(gray, max, grayStep*height);
    MM_MemCpy(gray, min, grayStep*height);
    for(l=0; l<level; l++)
    {
        dilate(max, grayStep, temp, width, height, 1);
        erode(min, grayStep, temp, width, height, 1);
        for(h=0; h<height; h++)
        {
            cPtr1 = max + h * grayStep;
            cPtr2 = min + h * grayStep;
            cPtr3 = diff + h * grayStep;
            for(w=0; w<width; w++)
            {
                cPtr3[w] = cPtr1[w] - cPtr2[w];
            }
        }
        for(_l=0; _l<l; _l++)
            erode(diff, grayStep, temp, width, height, 1);
        for(h=0; h<height; h++)
        {
            cPtr3 = diff + h * grayStep;
            dstPtr = gradient + h * gradStep;
            for(w=0; w<width; w++)
            {
                dstPtr[w] += cPtr3[w];
            }
        }        
    }

    if((average == 1) && (level>1))
    {
        for(h=0; h<height; h++)
        {
            dstPtr = gradient + h * gradStep;
            for(w=0; w<width; w++)
            {
                dstPtr[w] /= level;
            }
        } 
    }
    
    res = 0;
    __SAM_END__;
    if(max) MM_MemFree(NULL, (void**)&max);
    if(min) MM_MemFree(NULL, (void**)&min);
    if(diff) MM_MemFree(NULL, (void**)&diff);
    if(temp) MM_MemFree(NULL, (void**)&temp);
    return res;
}

/*Explicit Instantiation*/
template
DLL_EXPORTS  int  gradient_sobel<int>(
                                      int *gradient, int gradStep,
                                      unsigned char *gray, int grayStep,
                                      int width, int height
                                      );

template
DLL_EXPORTS  int  gradient_sobel<float>(
                                        float *gradient, int gradStep, 
                                        unsigned char *gray, int grayStep, 
                                        int width, int height
                                        );

template
DLL_EXPORTS  int  gradient_roberts<int>(
                                        int *gradient, int gradStep, 
                                        unsigned char *gray, int grayStep, 
                                        int width, int height
                                        );

template
DLL_EXPORTS  int  gradient_roberts<float>(
                                          float *gradient, int gradStep, 
                                          unsigned char *gray, int grayStep,
                                          int width, int height
                                          );


template
DLL_EXPORTS  int  gradient_mutilScale<int>(
                                           int *gradient, int gradStep,
                                           unsigned char *gray, int grayStep, 
                                           int width, int height, int level, int average
                                           );
template
DLL_EXPORTS  int  gradient_mutilScale<float>(
                                             float *gradient, int gradStep,
                                             unsigned char *gray, int grayStep, 
                                             int width, int height, int level, int average
                                             );
