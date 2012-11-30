

#include "IPL_core.h"
#include "stdio.h"
#include "float.h"

//gaussian filter
static int convolution_X(const unsigned char *srcImg , int srcStep,
						       unsigned char *dstImg,  int dstStep,
                               int width, int height,  int channel, 
							   float *kernel,int kernelSize, float kernelSum);
							
							
/************************  median filter  *************************
1. Only support 1 channel
2. Can't separate it into x and y directions 
3. 3 channeles is not same  as 1 channel , In 3 channels get the 
min sum distance of itself with other pixels 
/******************************************************************/
DLL_EXPORTS int  MedianFilter(const unsigned char *srcImg , int srcStep,
				                    unsigned char *dstImg,  int dstStep,
				                    int width, int height,  int kernelSize)
{
	int  res = -1;	
	const unsigned char  *ptr1 = NULL;	
	unsigned char        *ptr2 = NULL;
	const unsigned char  *ptr3 = NULL;
	unsigned char        *val  = NULL;
	unsigned char        temp;
	int semiKernelSize = kernelSize/2;

	__SAM_BEGIN__;

	val = new unsigned char[kernelSize*kernelSize];
	if ((NULL == srcImg) || (NULL == dstImg) || (NULL == val))
		EXIT;
	
	for (int h=0; h<height; h++ )
	{
		ptr1 = srcImg + h*srcStep;
		ptr2 = dstImg + h*dstStep;
		for (int w=0; w<width; w++)
		{
			if ( (w<semiKernelSize) || (w>=width-semiKernelSize) ||
				 (h<semiKernelSize) || (h>=height-semiKernelSize) )
			{
				ptr2[w] = ptr1[w];
				continue;
			}
			//get the data
			for (int i=0; i<kernelSize; i++)
			{
				ptr3 = ptr1 + (i-semiKernelSize) * srcStep;
				for (int j=0; j<kernelSize; j++)
                    val[i*kernelSize +j] = ptr3[w+j-semiKernelSize];
			}
			//only need to sort semi size
			for (int i=0; i<kernelSize*kernelSize/2 +1; i++)
			{
				for (int j=i+1; j<kernelSize*kernelSize; j++)
				{
					if (val[i]>val[j])
					{
						temp = val[i];
						val[i] = val[j];
						val[j] = temp;
					}
				}
			}
			ptr2[w] = val[(kernelSize*kernelSize)>>1];	
		}
	}

	res = 0;
	__SAM_END__;	
	if (val) delete []val;
	return  res;
}

/***********************  gaussian filter  ************************ 
                can  separate it into x and y directions   
******************************************************************/
static int convolution_X(const unsigned char *srcImg , int srcStep,
						       unsigned char *dstImg,  int dstStep,
							   int width, int height,  int channel, 
						 float *kernel,int kernelSize, float kernelSum)
{
	int res = -1;
	int semiKernelSize = kernelSize/2;
	int w, h, c, k;
	const unsigned char *ptr1 = NULL;
	unsigned char *ptr2 = NULL;
	double  val[3];

	__SAM_BEGIN__;

	if ((NULL == srcImg) || (NULL == dstImg) || (NULL == kernel))
		EXIT;


	for (h=0; h<height; h++)
	{
		ptr1 = srcImg + h*srcStep;
		ptr2 = dstImg + h*channel;
		for (w=0; w<semiKernelSize; w++)
		{
            int index =0;
			val[0] = val[1] = val[2] = 0;
            for(k=0; k<kernelSize; k++)
			{
                index = w-semiKernelSize+k;
                if(index < 0)
                    index = 0;
                for(c=0; c<channel; c++)
                {
                    val[c] += ptr1[(w-semiKernelSize+k)*channel + c] * kernel[k];
                }
            }
			for(c=0; c<channel; c++)
			{
				ptr2[w*dstStep+c] = val[c]/kernelSum;
            }
		}
		for(w=semiKernelSize; w<width-semiKernelSize; w++)
		{
			val[0] = val[1] = val[2] = 0;
			for(k=0; k<kernelSize; k++)
			{
				for(c=0; c<channel; c++)
				{
					val[c] += ptr1[(w-semiKernelSize+k)*channel + c] * kernel[k];
				}
			}
			for(c=0; c<channel; c++)
			{
				ptr2[w*dstStep+c] = val[c]/kernelSum;
			}
		}
		for(w=width-semiKernelSize; w<width; w++)
		{
            int index =0;
			val[0] = val[1] = val[2] = 0;
            for(k=0; k<kernelSize; k++)
			{
                index = w-semiKernelSize+k;
                if(index > width-1)
                    index = width-1;
                for(c=0; c<channel; c++)
                {
                    val[c] += ptr1[(w-semiKernelSize+k)*channel + c] * kernel[k];
                }
            }
			for(c=0; c<channel; c++)
			{
				ptr2[w*dstStep+c] = val[c]/kernelSum;
            }		   
		}
	}

	res = 0;
	__SAM_END__;	
	return res;
}


DLL_EXPORTS int  GaussianFilter(const unsigned char *srcImg , int srcStep,
				                unsigned char *dstImg,  int dstStep,
                                int width, int height,  int channel,
                                float *gauss_kernel,    int kernelSize, float kernelSum)
{
	int  res = -1;	
	int semiKernelSize = kernelSize/2;
	unsigned char *tempData = NULL; 

	__SAM_BEGIN__;
	
	tempData = new unsigned char[height*width*channel];
	if ((NULL == srcImg) || (NULL == dstImg) || (NULL == tempData))
		EXIT;

	if(NULL == gauss_kernel)
	{
		gauss_kernel = new float[kernelSize];
		if(NULL == gauss_kernel)
			EXIT;
		else
		{
			//get gaussian kernel value 	
			int val = 1;
			kernelSum = gauss_kernel[0] = val;
			for (int i=1; i<kernelSize; i++)
			{
				val *= (kernelSize-i);
				val /= i;
				gauss_kernel[i] = val;
				kernelSum += val;
			}
		}
	}
	res = convolution_X(srcImg, srcStep, tempData, height*channel, width, height, channel, gauss_kernel, kernelSize, kernelSum);
	res = convolution_X(tempData, height*channel, dstImg, dstStep, height, width, channel, gauss_kernel, kernelSize, kernelSum);

	__SAM_END__;	
	if (gauss_kernel) delete []gauss_kernel;
	if (tempData) delete []tempData;
	return  res;
}
