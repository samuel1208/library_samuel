#ifndef _IPL_PROJRCT_H_
#define _IPL_PROJRCT_H_

#include "define.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

/***********************************************************/
/*******   detect the vertical lines in building scene *****/
/***********************************************************/
DLL_EXPORTS int PreDetection_Building(
								      const cv::Mat&	 srcImg,		    //	the image format should be RGB or gray
								      int        srcImg_Format,     //  0 rgb, 1 gray
								      int        *vLineNum
								      );

/***********************************************************/
/*******   detect the gaps between lines in Text image *****/
/***********************************************************/
DLL_EXPORTS int PreDetection_Text(
							      const cv::Mat&	 srcImg,
							      int        srcImg_Format,     //  0 rgb, 1 gray
							      float      *probility 
							      );

#endif