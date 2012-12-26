
#include "IPL_project.h"
using namespace cv;

/***********************************************************/
/*******   detect the vertical lines in building scene *****/
/***********************************************************/
static Rect building_Roi(int rows, int cols)
{
	Rect rect;
	rect.x = cols/2 - cols*4/10;
	rect.y = rows/2 - rows*4/10;
	rect.width = cols*8/10;
	rect.height = rows*8/10;
	return rect;
}

DLL_EXPORTS  int  PreDetection_Building(
						                const Mat&	 srcImg,		    //	the image format should be RGB or gray
						                int   srcImg_Format,     //  0 rgb, 1 gray
						                int   *vLineNum
						               )
{
	int     res = -1;
	double  lowThresh = 50;
	double  highThresh = 200;
	int     accmThreshold = 50;
	double  minLineLength = 50;
	double  maxLineGap = 10;
	int     apertureSize = 3;

	Mat *_srcImg = NULL, *grayImg = NULL, *tempImg1 = NULL, *tempImg2 = NULL;
	Mat *resultImg = NULL;
	vector<Vec4i> lines;

	__SAM_BEGIN__;

	if(srcImg.empty())
		EXIT;

	_srcImg  = new Mat(srcImg, building_Roi(srcImg.rows, srcImg.cols));
	grayImg  = new Mat(_srcImg->rows,_srcImg->cols, CV_8UC1);
	tempImg1 = new Mat(256, 256, CV_8UC1);
	tempImg2 = new Mat(256, 256, CV_8UC1);

	if ((NULL==_srcImg) || (NULL==grayImg) || (NULL==tempImg1) || (NULL==tempImg2))
		EXIT;

	if(0 == srcImg_Format)
	{
		cvtColor(srcImg, *grayImg, CV_RGB2GRAY);
		resize(*grayImg, *tempImg1, Size(256, 256));
	}
	else
		resize(*_srcImg, *tempImg1, Size(256, 256));

	//median filter
	medianBlur(*tempImg1, *tempImg2, 5);
	//dilate the image
	{
		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
		dilate(*tempImg2, *tempImg1, element);
		dilate(*tempImg1, *tempImg2, element);
		dilate(*tempImg2, *tempImg1, element);
	}
	//canny 
	Canny(*tempImg1, *tempImg2, lowThresh, highThresh, apertureSize);
	//hough
	HoughLinesP(*tempImg2, lines, 1, CV_PI/180, accmThreshold, minLineLength, maxLineGap);

	*vLineNum = 0;
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		int x = l[2] - l[0];
		int y = l[3] - l[1];
		if(x=0)
		{
			*vLineNum +=1;
			continue;
		}
		double angle = atan(y*1.0/x);
		angle = 180*angle/CV_PI;  
		if ( (angle>70) || (angle <-70) )
			*vLineNum +=1;
	}

#define SHOW_BUILDING_RESULT_IMAGE
#ifdef  SHOW_BUILDING_RESULT_IMAGE
	resultImg = new Mat(256, 256, CV_8UC3);
	if(NULL == resultImg)
	{
		res = -1;
		EXIT;
	}

	cvtColor(*tempImg2, *resultImg, CV_GRAY2RGB);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line( *resultImg, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
	}
	imshow("Build_Pre_Detection", *resultImg);
#endif

	res = 0;
	__SAM_END__;
	if(_srcImg)  delete _srcImg;
	if(grayImg)  delete grayImg;
	if(tempImg1) delete tempImg1;
	if(tempImg2) delete tempImg2;
	if(resultImg)delete resultImg;
	return  res;
}

						  
/***********************************************************/
/*******   detect the gaps between lines in Text image *****/
/***********************************************************/
//should add rotate processing
DLL_EXPORTS  int  PreDetection_Text(const Mat&	 srcImg,
					                int    srcImg_Format,     //  0 rgb, 1 gray
					                float  *probility  )
{
	int res = -1;
	int newWidth;
	int newHeight;
	double *sum_row   = NULL;
	int    *dist      = NULL;
	int    *hist_dist = NULL;
	Mat *resizeImg = NULL, *grayImg = NULL;
	
	double average = 0;
	unsigned char *p = NULL;
	int  distCount = 0;

	__SAM_BEGIN__;

	//resize the image 	
	if(srcImg.empty())
		EXIT;

	if(srcImg.cols > srcImg.rows)
	{
		newWidth = 320;
		newHeight = (320*srcImg.rows)/srcImg.cols;
	}
	else
	{
		newHeight = 320;
		newWidth = (320*srcImg.cols)/srcImg.rows;
	}

	resizeImg = new Mat(newHeight, newWidth,CV_8UC3);
	grayImg = new Mat(newHeight, newWidth,CV_8UC1);
	if ((NULL == resizeImg) || (NULL == grayImg))
		EXIT;

	if (0 == srcImg_Format)
	{
		resize(srcImg, *resizeImg, Size(newHeight, newWidth));
		cvtColor(*resizeImg, *grayImg, CV_RGB2GRAY);
	}
	else
		resize(srcImg, *grayImg, Size(newHeight, newWidth));

	sum_row = new double[newHeight];;
	dist = new int[newHeight];
	hist_dist = new int[newHeight];
	memset(sum_row, 0, newHeight*sizeof(double));
	memset(dist, 0, newHeight*sizeof(int));
	memset(hist_dist, 0, newHeight*sizeof(int));

	if ( (NULL == sum_row)||(NULL == dist)||(NULL == hist_dist) )
		EXIT;

	//project to Y direction
	
	for (int h=0; h<newHeight; h++)
	{
		p = (unsigned char *)grayImg->data + h * grayImg->step;
		for(int w=0; w<newWidth; w++)
		{
			sum_row[h] +=p[w]*1.0/newWidth;
		}
		average += sum_row[h]/newHeight;
	}
	for (int h=0; h<newHeight; h++)
	{
		if(sum_row[h] < average)
			sum_row[h] = 0;
		else
			sum_row[h] = 255;
	}

	// statistic the histogram
	dist[distCount] = 1;
	for (int h=0; h<newHeight-1; h++)
	{	
		if(sum_row[h] != sum_row[h+1])
			distCount++;
		dist[distCount] += 1;
	}
	distCount += 1;
	for (int h=0; h<distCount; h++)
		hist_dist[dist[h]] += 1;

	//arrange the histogram
	int temp;
	for (int i=0; i<distCount-1; i++)
	{
		for (int j=i+1; j<distCount; j++)
		{
			if(hist_dist[i]<hist_dist[j])
			{
				temp = hist_dist[i];
				hist_dist[i] = hist_dist[j];
				hist_dist[j] = temp;
			}
		}
	}

	*probility = (float)((hist_dist[0]+hist_dist[1]+hist_dist[2])*1.0/distCount);

	res = 0;
	__SAM_END__;
	if(sum_row)   delete []sum_row;
	if(dist)      delete []dist;
	if(hist_dist) delete []hist_dist;
	if(resizeImg) delete resizeImg;
	if(grayImg)   delete grayImg;
	return res;
}

/***********************************************************/
/*   detect the horizon line and analysis HSV distribution 
     to omit  the beach in  sky                            */
/***********************************************************/