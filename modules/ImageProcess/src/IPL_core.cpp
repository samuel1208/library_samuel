

#include "IPL_core.h"
#include "stdio.h"
#include "float.h"
#include "MemManager.h"
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
static int CVT_RGB2YCrCb(const unsigned char *srcImg,  int srcStep,
					           unsigned char *dstImg,  int dstStep,
					           int width, int height);
//resize
static  int  Resize_Bilinear(const unsigned char *srcImg, int srcWidth, int srcHeight, int srcStep,
							       unsigned char *dstImg, int dstWidth, int dstHeight, int dstStep,
							       float xScale, float yScale, int channel);

//erode and dilate

static  int  __dilate_x( unsigned char *srcImg, int srcStep, 
                         unsigned char *dstImg, int dstStep, 
                         int width_src, int height_src, int semi_win_size);

static  int  __erode_x( unsigned char *srcImg, int srcStep, 
                         unsigned char *dstImg, int dstStep, 
                         int width_src, int height_src, int semi_win_size);
								   
/************************  convert color   ************************/
/********  support RGB to HSL, HSV, GRAY, YCrCb      **************/
/******************************************************************/

/*   H=[0,180], S=[0,255], L=[0-255] :  same to OpenCV    */
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
    int i, j ;

	__SAM_BEGIN__;

	if ( (NULL == srcImg) || (NULL == dstImg))
		EXIT;

    for (i=0; i<height; i++)
	{
        ptr1 = srcImg + i*srcStep;
        ptr2 = dstImg + i*dstStep;
		for (j=0; j<width; j++)
		{
            r = ptr1[j*3]/255.0f;
			g = ptr1[j*3+1]/255.0f;
			b = ptr1[j*3+2]/255.0f;
			maxPix = SAM_MAX(SAM_MAX(r,g),b);
			minPix = SAM_MIN(SAM_MIN(r,g),b);

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

/*   H=[0,180], S=[0,255], V=[0-255] :  same to OpenCV    */
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
    int i, j;

	__SAM_BEGIN__;

	if ( (NULL == srcImg) || (NULL == dstImg))
		EXIT;

	for (i=0; i<height; i++)
	{
		ptr1 = srcImg + i*srcStep;
		ptr2 = dstImg + i*dstStep;
		for (j=0; j<width; j++)
		{
			r = ptr1[j*3]/255.0f;
			g = ptr1[j*3+1]/255.0f;
			b = ptr1[j*3+2]/255.0f;
			maxPix = SAM_MAX(SAM_MAX(r,g),b);
			minPix = SAM_MIN(SAM_MIN(r,g),b);

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
    int i, j;
	__SAM_BEGIN__;

	if ( (NULL == srcImg) || (NULL == dstImg))
		EXIT;


	for (i=0; i<height; i++)
	{
		ptr1 = srcImg + i*srcStep;
		ptr2 = dstImg + i*dstStep;
		for(j=0; j<width; j++)		
			ptr2[j]  = (int)(ptr1[j*3]*0.229f + ptr1[j*3+1]*0.587f + ptr1[j*3+2]*0.114f);
	}

	res = 0;
	__SAM_END__;	
	return  res;
}

/*    the same with OpenCV  */
static int CVT_RGB2YCrCb(const unsigned char *srcImg,  int srcStep,
					           unsigned char *dstImg,  int dstStep,
					           int width, int height)
{
	#define fix(x,n)      (int)((x)*(1 << (n)) + 0.5)	
	#define yuvYr_32f 0.299f
	#define yuvYg_32f 0.587f
	#define yuvYb_32f 0.114f
	#define yuvCr_32f 0.713f
	#define yuvCb_32f 0.564f
	
	#define yuv_shift 14
	#define yuvYr  fix(yuvYr_32f,14)
	#define yuvYg  fix(yuvYg_32f,14)
	#define yuvYb  fix(yuvYb_32f,14)
	#define yuvCr  fix(yuvCr_32f,14)
	#define yuvCb  fix(yuvCb_32f,14)
	
	#define  CV_CAST_8U(t)  (unsigned char)(!((t) & ~255) ? (t) : (t) > 0 ? 255 : 0)
	#define  CV_DESCALE(x,n)     (((x) + (1 << ((n)-1))) >> (n))
    #define yuv_descale(x)  CV_DESCALE((x), 14)

	int res = -1;
	const unsigned char *pRGB = NULL;
	unsigned char *pYCrCb = NULL;
    int y, r, g, b, i,j;

	__SAM_BEGIN__;

	if ( (NULL == srcImg) || (NULL == dstImg))
		EXIT;


	for (int i=0; i<height; i++)
	{
		pRGB = srcImg + i*srcStep;
		pYCrCb = dstImg + i*dstStep;        
        for(j=0; j<width; j++)
		{
			b = pRGB[j*3+2]; r=pRGB[j*3];
			y = yuv_descale(b*yuvYb + yuvYg*pRGB[j*3+1] + yuvYr*r);
			r = yuv_descale((r-y)*yuvCr) + 128;
			b = yuv_descale((b-y)*yuvCb) + 128;
			pYCrCb[j] = CV_CAST_8U(y);
			pYCrCb[j*3+1] = CV_CAST_8U(r);
			pYCrCb[j*3+1] = CV_CAST_8U(b);
		}
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
	case IPL_RGB2YCrCb:
		res = CVT_RGB2YCrCb(srcImg, srcStep, dstImg, dstStep, width, height);
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
		yIndex_b = SAM_MIN((int)(yIndex), srcHeight-2);
		yWeight = yIndex - yIndex_b;
		ptr1 = srcImg + (int)(yIndex)*srcStep;
		ptr2 = dstImg + h*dstStep;
		for (int w=0; w<dstWidth; w++)
		{
			xIndex = w*xScale;
			xIndex_b = SAM_MIN((int)(xIndex), srcWidth-2);
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




/************************   integral image   ************************/
/*         alloc the memory inside for integral image               */
#define   INT_IMAGE_EDGE      (2)
DLL_EXPORTS int  IntegralImage(
                               const unsigned char *srcImg,  int srcStep,  int srcWidth,   int srcHeight,
                                     unsigned int  **intImg, int &intStep, int &intWidth, int &intHeight
                               )
{
    int res =-1;   
    unsigned int *__pIntImg = NULL;
    const unsigned char* __pSrc=NULL;
    unsigned  int size = 0, rowSum=0;
    int i,j;
    __SAM_BEGIN__;
    
    intStep = intWidth = srcWidth + INT_IMAGE_EDGE*2;
    intHeight = srcHeight + INT_IMAGE_EDGE*2;
    size = intWidth*intHeight;
     
    if( size>= 4096*4096)
        EXIT;
    
    *intImg = (unsigned int*)MM_MemAlloc(NULL, size*sizeof(int));
    if ((NULL==srcImg) || (NULL==intImg)) 
        EXIT;
    
    MM_MemSet(*intImg, 0, intWidth*sizeof(int)*INT_IMAGE_EDGE);

    //for first line
    __pIntImg = *intImg + intWidth * INT_IMAGE_EDGE;
    __pSrc = srcImg + srcWidth;
    rowSum=0;
    for(i=0; i<INT_IMAGE_EDGE; i++)
        __pIntImg[i] = 0;
    for( ; i<intWidth-INT_IMAGE_EDGE; i++)
    {
        rowSum += __pSrc[i-INT_IMAGE_EDGE];
        __pIntImg[i]= rowSum;
    }
    for( ; i<intWidth; i++)
        __pIntImg[i] = rowSum;
    
    for(i=INT_IMAGE_EDGE+1; i<intHeight-INT_IMAGE_EDGE; i++)
    {        
        __pIntImg = *intImg + intWidth * i;
        __pSrc = srcImg + srcWidth*(i-INT_IMAGE_EDGE);
        rowSum = 0;
        for(j=0; j<INT_IMAGE_EDGE; j++)
            __pIntImg[j] = 0;
        for( ; j<intWidth-INT_IMAGE_EDGE; j++)
        {
            rowSum += __pSrc[j-INT_IMAGE_EDGE];
            __pIntImg[j] = rowSum + __pIntImg[j-intWidth]; 
        }
        rowSum = __pIntImg[j-1];
        for( ; j<intWidth; j++)
            __pIntImg[j] = rowSum;
    }
    
    for( ; i<intHeight; i++)
    {
        __pIntImg = *intImg + intWidth * i;
        MM_MemCpy(__pIntImg-intWidth, __pIntImg, intWidth*sizeof(int));
    }
    
    res = 0;
    __SAM_END__; 
    if((0!=res) && (NULL != *intImg))
        MM_MemFree(NULL, (void**)intImg);
    return res;
}
/****************************************************************************/

/***********************       erode and dilate       **********************/
/*                   do erode or dilate in the srcimg                      */
//max semi_win_size = 5;
#define GET_MIN_VAL(win_size, pTempVal, tempVal, i)   \
		tempVal = pTempVal[0];                        \
		for(i=1; i<win_size; i++)                     \
	        tempVal = SAM_MIN(tempVal, pTempVal[i])        

#define GET_MAX_VAL(win_size, pTempVal, tempVal, i)   \
    	tempVal = pTempVal[0];                        \
	    for(i=1; i<win_size; i++)                     \
	        tempVal = SAM_MAX(tempVal, pTempVal[i])      

int __erode_x(unsigned char *srcImg, int srcStep, unsigned char *dstImg, int dstStep, int width_src, int height_src, int semi_win_size)
{
	int x, y, i;
	int index, __index;
	unsigned char pTempVal[11]={0}, tempVal;
	unsigned char *pSrc;
	int win_size = 0;
	if((NULL == dstImg) || (NULL == srcImg))
		return -1;
	
	if(semi_win_size<1)
		semi_win_size= 1;
	else if(semi_win_size>5)
		semi_win_size = 5;

	win_size = 2*semi_win_size+1;

	//width must >= 2
	for (y=0; y<height_src; y++)
	{
		pSrc = srcImg + y*srcStep;
	
		tempVal = pSrc[0];
		for(x=0; x<=semi_win_size; x++)
			pTempVal[x] = tempVal;
		for(; x<win_size-1; x++)
			pTempVal[x] = pSrc[x-semi_win_size];

		index = win_size-1;
		for(x=0; x< width_src-semi_win_size; x++)
		{
			__index = index % win_size;
			//get the value 
			pTempVal[__index] = pSrc[x+semi_win_size];
			GET_MIN_VAL(win_size, pTempVal, tempVal, i);
			dstImg[y+x*dstStep] = tempVal;
			index++;
		}
				 
		for(; x<width_src; x++)
		{
			__index = index % win_size;
			//get the value 
			pTempVal[__index] = pSrc[width_src-1];
			GET_MIN_VAL(win_size, pTempVal, tempVal, i);
			dstImg[y+x*dstStep] = tempVal;	
			index++;
		}
	}
	return 0;
}


int __dilate_x(unsigned char *srcImg, int srcStep, unsigned char *dstImg, int dstStep, int width_src, int height_src, int semi_win_size)
{
	int x, y, i;
	int index, __index;
	unsigned char pTempVal[11]={0}, tempVal;
	unsigned char *pSrc;
	int win_size = 0;
	if((NULL == dstImg) || (NULL == srcImg))
		return -1;
	
	if(semi_win_size<1)
		semi_win_size= 1;
	else if(semi_win_size>5)
		semi_win_size = 5;
	
	win_size = 2*semi_win_size+1;
	
	//width must >= 2
	for (y=0; y<height_src; y++)
	{
		pSrc = srcImg + y*srcStep;
		
		tempVal = pSrc[0];
		for(x=0; x<=semi_win_size; x++)
			pTempVal[x] = tempVal;
		for(; x<win_size-1; x++)
			pTempVal[x] = pSrc[x-semi_win_size];
		
		index = win_size-1;
		for(x=0; x< width_src-semi_win_size; x++)
		{
			__index = index % win_size;
			//get the value 
			pTempVal[__index] = pSrc[x+semi_win_size];
			GET_MAX_VAL(win_size, pTempVal, tempVal, i);
			dstImg[y+x*dstStep] = tempVal;
			index++;
		}
		
		for(; x<width_src; x++)
		{
			__index = index % win_size;
			//get the value 
			pTempVal[__index] = pSrc[width_src-1];
			GET_MAX_VAL(win_size, pTempVal, tempVal, i);
			dstImg[y+x*dstStep] = tempVal;	
			index++;
		}
	}
	return 0;
}

DLL_EXPORTS  int erode(unsigned char *srcImg, int srcStep, unsigned char *temp, int width_src, int height_src, int semi_win_size)
{
	if ( (NULL == srcImg) || (NULL == temp))
		return -1;
	if(0 != __erode_x(srcImg, srcStep, temp, height_src, width_src, height_src, semi_win_size))
		return -1;
	if(0 != __erode_x(temp, height_src, srcImg, srcStep, height_src, width_src, semi_win_size))
		return -1;
	return 0;
}

DLL_EXPORTS  int dilate(unsigned char *srcImg, int srcStep, unsigned char *temp, int width_src, int height_src, int semi_win_size)
{
	if ( (NULL == srcImg) || (NULL == temp))
		return -1;
	if(0 != __dilate_x(srcImg, srcStep, temp, height_src, width_src, height_src, semi_win_size))
		return -1;
	if(0 != __dilate_x(temp, height_src, srcImg, srcStep, height_src, width_src, semi_win_size))
		return -1;
	return 0;
}
/****************************************************************************/
