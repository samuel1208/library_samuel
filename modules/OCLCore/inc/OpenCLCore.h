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

#include "cl.h"
#include "define.h"
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

/************************************************************************************/

/************************************************************************************/
/*****************************  function interface  *********************************/
/************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

//create an OCL engine 
// if ct,cq,cd != NULL , intialize the envrioment form outside
DLL_EXPORTS  int  clIntialEnviroment(char *sourceCode, cl_context ct, cl_command_queue cq, cl_device_id cd);
DLL_EXPORTS  int  clUnintialEnviroment();


//create buffer memory
DLL_EXPORTS  int  clMallocBuf(void **buf, size_t size, cl_mem_flags  flag = CL_MEM_READ_WRITE);

//copy buffer memory between cpu and gpu
DLL_EXPORTS  int  clMemcpyBuf(void **src, void **dst, 
					          size_t size, 
							  clMemcpyFlag flag, 
							  size_t src_offset = 0, 
							  size_t dst_offset = 0,
							  cl_bool sync = CL_TRUE, 
							  cl_command_queue cq = NULL,
							  int      ev_num = 0,
							  cl_event *ev_wait = NULL,
							  cl_event *ev = NULL);

//create buffer memory
DLL_EXPORTS  int  clMallocImg2D(void **img, 
								size_t width, 
								size_t height,
								cl_image_format* imageFormat = NULL, 
								cl_mem_flags flag = CL_MEM_READ_WRITE);

//copy memory from cpu to gpu
// width , height and src_origin set a rectangular region of the img
DLL_EXPORTS  int  clMemcpyImg2D(void **src, void **dst, 
								size_t width,
								size_t height, 
								clMemcpyFlag flag ,
								size_t *src_origin = NULL, 
								size_t *dst_origin = NULL, 
								size_t rowPitch = 0,   //  set the row pitch of CPU buffer
								size_t bufOffset = 0,
								cl_bool sync = CL_TRUE ,
								cl_command_queue cq = NULL,
							    int      ev_num = 0,
							    cl_event *ev_wait = NULL,
							    cl_event *ev = NULL);

//free the buffer memory
DLL_EXPORTS  int  clMemFree(void **mem);

//get ocl_core para
DLL_EXPORTS  cl_context			  GetOclContext() ;			//cl context
DLL_EXPORTS	 cl_command_queue	  GetOclCommandQueue0() ;	//cl command queue
DLL_EXPORTS	 cl_command_queue	  GetOclCommandQueue1() ;	//cl command queue
DLL_EXPORTS  cl_program           GetOclProgram() ;         //cl program
//



#ifdef __cplusplus
}
#endif

#endif