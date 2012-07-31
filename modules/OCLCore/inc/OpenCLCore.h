/*----------------------------------------------------------------------------------------------
*
* This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary and 		
* confidential information. 
* 
* The information and code contained in this file is only for authorized ArcSoft employees 
* to design, create, modify, or review.
* 
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
* 
* If you are not an intended recipient of this file, you must not copy, distribute, modify, 
* or take any action in reliance on it. 
* 
* If you have received this file in error, please immediately notify ArcSoft and 
* permanently delete the original and any copy of any file and any printout thereof.
*
*-------------------------------------------------------------------------------------------------*/

#ifndef _OPENCL_CORE_H
#define _OPENCL_CORE_H

#ifdef IPL_DLL
    #if defined(_WIN32) && !defined(__GNUC__)
        #ifdef EXPORT_IPL
            #define IPL_API __declspec(dllexport)
        #else
            #define IPL_API __declspec(dllimport)
        #endif
    #else  /* unix */
        #define IPL_API __attribute__((visibility("default")))
    #endif  
#else
    #define IPL_API
#endif

#include "cl.h"
#include "cl_platform.h"
#include "SAK_core.h"


/************************************************************************************/
/*******************************  memory copy type  *********************************/
/************************************************************************************/
enum clMemcpyFlag 
{
	clMemcpyHostToDevice    = 0 ,
	clMemcpyDeviceToHost    = 1 ,	
	clMemcpyDeviceToDevice  = 2 ,
	clMemcpyImgToImg  = 3 ,
	clMemcpyImgToBuf  = 4 ,
	clMemcpyBufToImg  = 5 ,
};
typedef struct _KERNEL_API
{
	cl_kernel   g_function1_kernel;
}KERNEL_API;

typedef struct _OCL_ENGINE
{
	cl_context			  g_CLContext ;			//cl context
	cl_device_id 		  g_CLDevices ;		    //cl devices
	size_t				  g_CLDeviceListSize  ;	//cl device size list
	cl_command_queue	  g_CLCommandQueue ;	//cl command queue
	cl_program            g_CLProgram ;         //cl program
	KERNEL_API            kernel_API;
   
} OCL_ENGINE;
/************************************************************************************/

/************************************************************************************/
/*****************************  function interface  *********************************/
/************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

//create an OCL engine 
IPL_API 	MHandle CreateEngine_OCL();
IPL_API     MResult ReleaseEngine_OCL(MHandle oclHandle);

////initialize opencl and make sure the function is invoked before any OpenCL kernel begins

//
////Load cl program
//IPL_API cl_int	clLoadProgram(const char *programs);
//
////Load cl kernels
//IPL_API cl_int	clCreateKernelFunc(cl_kernel *kernel, const char *kernelName);
//
//
//
////run the kernel function
//IPL_API cl_int	clRunKernelFunc(cl_kernel *kernel, 
//										cl_uint  work_dim, 										
//										const size_t *global_work_size,
//										const size_t *local_work_size,
//										const size_t *global_work_offset = NULL,
//										cl_uint       num_events_in_wait_list = 0,
//										const cl_event *event_wait_list = NULL,
//										cl_event *ev = NULL);
//
////create buffer memory
//IPL_API cl_int	clMallocBuf(void **buf, size_t size, cl_mem_flags  flag = CL_MEM_READ_WRITE);
//
////copy buffer memory between cpu and gpu
//IPL_API cl_int	clMemcpyBuf(void **src, void **dst, 
//					                size_t size, 
//									clMemcpyFlag flag, 
//									size_t src_offset = 0, 
//									size_t dst_offset = 0,
//									cl_command_queue cq = 0, 
//									cl_bool sync = CL_TRUE, 
//									cl_event* ev = NULL);
//
////create buffer memory
//IPL_API cl_int	clMallocImg2D(void **img, 
//						              size_t width, 
//									  size_t height,
//									  cl_image_format* imageFormat = NULL, 
//									  cl_mem_flags flag = CL_MEM_READ_WRITE);
//
////copy memory from cpu to gpu
//// width , height and src_origin set a rectangular region of the img
//IPL_API cl_int	clMemcpyImg2D(void **src, void **dst, 
//						              size_t width,
//									  size_t height, 
//									  clMemcpyFlag flag ,
//									  size_t *src_origin = NULL, 
//									  size_t *dst_originint = NULL, 
//									  size_t rowPitch = 0,   //  set the row pitch of CPU buffer
//									  size_t bufOffset = 0,
//									  cl_command_queue cq = 0,
//									  cl_bool sync = CL_TRUE ,
//									  cl_event* ev = NULL);
//
////free the buffer memory
//IPL_API void	clMemFree(void **mem);
//
////get the OpenCL handle
////get OpenCL context
//IPL_API cl_context         GetContext();
//
////get OpenCL program
//IPL_API cl_program         GetProgram();
//
////get command queue of OpenCL
//IPL_API cl_command_queue   GetCommandQueue();
//
////release opencl and make sure the function is invoked after all of OpenCL kernels are finished working
//IPL_API void	clRelease();
//IPL_API void	clReleaseKernels();


#ifdef __cplusplus
}
#endif

#endif