
#ifndef _OPENCL_KERNEL_INTRRFACE_H
#define _OPENCL_KERNEL_INTRRFACE_H

#include "define.h"


/************************************************************************************/
/*****************************  function interface  *********************************/
/************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORTS  int  clInitialKernelAPI();
DLL_EXPORTS  int  clReleaseKernelAPI();

#ifdef __cplusplus
}
#endif

#endif