#ifndef __IPL_GRADIENT_HPP__
#define __IPL_GRADIENT_HPP__
#include "define.h"

template<typename T>
DLL_EXPORTS  int  gradient_sobel(
                                 T *gradient, int gradStep,
                                 unsigned char *gray, int grayStep,
                                 int width, int height
                                 );

template<typename T>
DLL_EXPORTS  int  gradient_roberts(
                                   T *gradient, int gradStep, 
                                   unsigned char *gray, int grayStep, 
                                   int width, int height
                                   );
/*  
                               Morphology gradient
*/
template<typename T>
DLL_EXPORTS  int  gradient_mutilScale(
                                      T *gradient, int gradStep, 
                                      unsigned char *gray, int grayStep, 
                                      int width, int height, int level, int average
                                      );



#endif
