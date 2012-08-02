
#ifndef _OPENCL_KERNEL_INTRRFACE_H
#define _OPENCL_KERNEL_INTRRFACE_H

#include "define.h"
#include "OpenCLCore.h"


typedef struct _KERNEL_API_
{
	cl_kernel  g_HistogramStatistics_kernel;
}KERNEL_API;

typedef struct _OCL_ENGINE_
{
	//the OpenCL hadle
	OCL_CORE   ocl_Core;
	//the kernel API
	KERNEL_API kernel_API;
} OCL_ENGINE;
/************************************************************************************/
/*****************************  function interface  *********************************/
/************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORTS  int  clCreateEngine(MHandle OCL_ENGINE);
DLL_EXPORTS  int  clReleaseEngine(MHandle OCL_ENGINE);

#ifdef __cplusplus
}
#endif

#endif