

#include "IPL_core.h"
#include "stdio.h"
#include "float.h"

//color convert
static int CVT_RGB2HSL(const unsigned char *srcImg,  int srcStep,
				             unsigned char *dstImg,  int dstStep,
				             int width, int height);
static int CVT_RGB2HSV(const unsigned char *srcImg,  int srcStep,
					         unsigned char *dstImg,  int dstStep,
					         int width, int height);
static int CVT_RGB2GRAY(const unsigned char *srcImg,  int srcStep,
					          unsigned char *dstImg,  int dstStep,
					          int width, int height);
//resize
static  int  Resize_Bilinear(const unsigned char *srcImg, int srcWidth, int srcHeight, int srcStep,
							       unsigned char *dstImg, int dstWidth, int dstHeight, int dstStep,
							       float xScale, float yScale, int channel);
								   
/************************  convert color   ************************/
/****************  support RGB to HSL, HSV, GRAY ******************/
/******************************************************************/
static int CVT_RGB2HSL(const unsigned char *srcImg,  int srcStep,
				             unsigned char *dstImg,  int dstStep,
				             int width, int height)
{
	int res = -1;
	const unsigned char *ptr1 = NULL;
	unsigned char *ptr2 = NULL;
	float maxPix, minPix;
	float r, g, b;
	float h, s, l;
	float diff;

	__SAM_BEGIN__;

	if ( (NULL == srcImg) || (NULL == dstImg))
		EXIT;

	for (int i=0; i<height; i++)
	{
		ptr1 = srcImg + i*srcStep;
		ptr2 = dstImg + i*dstStep;
		for (int j=0; j<width; j++)
		{
			r = ptr1[j*3]/255.0f;
			g = ptr1[j*3+1]/255.0f;
			b = ptr1[j*3+2]/255.0f;
			maxPix = MAX(MAX(r,g),b);
			minPix = MIN(MIN(r,g),b);

			l = (maxPix+minPix)*0.5f;
			diff = maxPix-minPix;

			h = s = 0.0f;

			if (diff > FLT_EPSILON)
			{
				//compute s
				if (l<0.5)			
					s = diff/(maxPix+minPix);
				else if (l>=0.5)
					s = diff/(2-maxPix-minPix);
				//compute h
				if (maxPix == r)
					h = (g-b)/diff;
				else if (maxPix == g)
					h = 2 + (b-r)/diff;
				else
					h = 4 + (r-g)/diff;
				if (h<0) h += 6 ;
			}
			ptr2[j*3]   = (int)(h*30);
			ptr2[j*3+1] = (int)(s*255);
			ptr2[j*3+2] = (int)(l*255);
	     }
	}

	res = 0;
	__SAM_END__;	
	return  res;
}

static int CVT_RGB2HSV(const unsigned char *srcImg,  int srcStep,
					         unsigned char *dstImg,  int dstStep,
					         int width, int height)
{
	int res = -1;
	const unsigned char *ptr1 = NULL;
	unsigned char *ptr2 = NULL;
	float maxPix, minPix;
	float r, g, b;
	float h, s, v;
	float diff;

	__SAM_BEGIN__;

	if ( (NULL == srcImg) || (NULL == dstImg))
		EXIT;

	for (int i=0; i<height; i++)
	{
		ptr1 = srcImg + i*srcStep;
		ptr2 = dstImg + i*dstStep;
		for (int j=0; j<width; j++)
		{
			r = ptr1[j*3]/255.0f;
			g = ptr1[j*3+1]/255.0f;
			b = ptr1[j*3+2]/255.0f;
			maxPix = MAX(MAX(r,g),b);
			minPix = MIN(MIN(r,g),b);

			v = maxPix;
			diff = maxPix-minPix;
			h = s = 0;			

			if(diff>FLT_EPSILON)
			{
				s = diff/(v + FLT_EPSILON);
				if (maxPix==r)
					h = (g - b)*diff;
				else if (maxPix==g)
					h = (b-r)*diff + 2;
				else
					h = (r-g)*diff + 4;
				if (h<0) h += 6;
			}	

			ptr2[j*3]   = (int)(h*30);
			ptr2[j*3+1] = (int)(s*255);
			ptr2[j*3+2] = (int)(v*255);
		}
	}

	res = 0;
	__SAM_END__;	
	return  res;
}

static int CVT_RGB2GRAY(const unsigned char *srcImg,  int srcStep,
					          unsigned char *dstImg,  int dstStep,
					          int width, int height)
{
	int res = -1;
	const unsigned char *ptr1 = NULL;
	unsigned char *ptr2 = NULL;

	__SAM_BEGIN__;

	if ( (NULL == srcImg) || (NULL == dstImg))
		EXIT;


	for (int i=0; i<height; i++)
	{
		ptr1 = srcImg + i*srcStep;
		ptr2 = dstImg + i*dstStep;
		for(int j=0; j<width; j++)		
			ptr2[j]  = (int)(ptr1[j*3]*0.229f + ptr1[j*3+1]*0.587f + ptr1[j*3+2]*0.114f);
	}

	res = 0;
	__SAM_END__;	
	return  res;
}


DLL_EXPORTS int  CVTColor(const unsigned char *srcImg,  int srcStep,
				    unsigned char *dstImg,  int dstStep,
				    int width, int height,  int CVTFormat)
 {
 	int  res = -1;

	__SAM_BEGIN__;

	if ( (NULL == srcImg) || (NULL == dstImg))
		EXIT;
	
	switch(CVTFormat)
	{
	case IPL_RGB2HSL:
		res = CVT_RGB2HSL(srcImg, srcStep, dstImg, dstStep, width, height);
		break;
	case IPL_RGB2HSV:
		res = CVT_RGB2HSV(srcImg, srcStep, dstImg, dstStep, width, height);
		break;
	case IPL_RGB2GRAY:
		res = CVT_RGB2GRAY(srcImg, srcStep, dstImg, dstStep, width, height);
		break;
	default:
		res = -1;
	}

	__SAM_END__;	
	return  res;
 }
/******************************************************************/

/************************   resize image   ************************/
/********************     support Bilinear    *********************/
/******************************************************************/
static  int  Resize_Bilinear(const unsigned char *srcImg, int srcWidth, int srcHeight, int srcStep,
							       unsigned char *dstImg, int dstWidth, int dstHeight, int dstStep,
							       float xScale, float yScale, int channel)
{
	int  res = -1;

	const unsigned char *ptr1 = NULL;
	unsigned char *ptr2 = NULL;
	float xIndex,   yIndex;
	float xWeight,  yWeight;
	int   xIndex_b, yIndex_b;
	float x1, x2 ;

	__SAM_BEGIN__;

	if ( (NULL == srcImg) || (NULL == dstImg))
		EXIT;

	if(0 == xScale) xScale = srcWidth*1.0f/dstWidth;
	if(0 == yScale) yScale = srcHeight*1.0f/dstHeight;

	if ((int(dstWidth*xScale) > srcWidth) || ((int)(dstHeight*yScale) > srcHeight))
		EXIT;

	for (int h=0; h<dstHeight; h++)
	{
		yIndex = h*yScale;
		yIndex_b = MIN((int)(yIndex), srcHeight-2);
		yWeight = yIndex - yIndex_b;
		ptr1 = srcImg + (int)(yIndex)*srcStep;
		ptr2 = dstImg + h*dstStep;
		for (int w=0; w<dstWidth; w++)
		{
			xIndex = w*xScale;
			xIndex_b = MIN((int)(xIndex), srcWidth-2);
			xWeight = xIndex - xIndex_b;
			for (int i=0; i<channel; i++)
			{
				x1 = ptr1[xIndex_b*channel + i]*(1-xWeight) + ptr1[(xIndex_b+1)*channel + i]*xWeight;
				x2 = ptr1[xIndex_b*channel + i + srcStep]*(1-xWeight) + ptr1[(xIndex_b+1)*channel + i + srcStep]*xWeight;
				ptr2[w*channel + i] = (1 - yWeight)*x1 + yWeight * x2;
			}
		}
	}

	res = 0;
	__SAM_END__;	
	return  res;
}
DLL_EXPORTS int  Resize(const unsigned char *srcImg, int srcWidth, int srcHeight, int srcStep,
					      unsigned char *dstImg, int dstWidth, int dstHeight, int dstStep,
					      float xScale, float yScale, int channel, int InterpolationFlag)
{
	int  res = -1;

	__SAM_BEGIN__;

	if ( (NULL == srcImg) || (NULL == dstImg))
		EXIT;

	switch(InterpolationFlag)
	{
	case IPL_RESIZE_INTERPOLATION_BILINEAR:
		res = Resize_Bilinear(srcImg, srcWidth, srcHeight, srcStep,
			                  dstImg, dstWidth, dstHeight, dstStep,
			                  xScale, yScale,   channel);
		break;
	default:
		res = -1;
	}

	__SAM_END__;	
	return  res;
}
/******************************************************************/