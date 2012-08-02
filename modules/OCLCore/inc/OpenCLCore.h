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

typedef struct _OCL_CORE_
{
	cl_context			  g_CLContext ;			//cl context
	cl_device_id 		  g_CLDevices ;		    //cl devices
	size_t				  g_CLDeviceListSize  ;	//cl device size list
	cl_command_queue	  g_CLCommandQueue ;	//cl command queue
	cl_program            g_CLProgram ;         //cl program
} OCL_CORE;
/************************************************************************************/

/************************************************************************************/
/*****************************  function interface  *********************************/
/************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

//create an OCL engine 
DLL_EXPORTS 	int    clIntialEnviroment(MHandle OCLEngine, char *sourceCode);
DLL_EXPORTS     int  clUnintialEnviroment(MHandle oclHandle);

/*******************************************************************
fileName   : file's path ,including the file's name.
sourceCode : The pointer to store the source codes.
offset     : The offset of the sourceCode pointer.
             First must be equal to zero , can be set to zero.
return :  0 means correct.
********************************************************************/
DLL_EXPORTS  int  clGetCode(char *fileName, char *sourceCode, int *offset);


////create buffer memory
DLL_EXPORTS  int  clMallocBuf(cl_context cc, void **buf, size_t size, cl_mem_flags  flag = CL_MEM_READ_WRITE);
//
////copy buffer memory between cpu and gpu
DLL_EXPORTS  int  clMemcpyBuf(cl_command_queue cq,
	                          void **src, void **dst, 
					          size_t size, 
							  clMemcpyFlag flag, 
							  size_t src_offset = 0, 
							  size_t dst_offset = 0,
							  cl_bool sync = CL_TRUE, 
							  cl_event* ev = NULL);

//create buffer memory
DLL_EXPORTS  int  clMallocImg2D(cl_context cc,
	                            void **img, 
								size_t width, 
								size_t height,
								cl_image_format* imageFormat = NULL, 
								cl_mem_flags flag = CL_MEM_READ_WRITE);

//copy memory from cpu to gpu
// width , height and src_origin set a rectangular region of the img
DLL_EXPORTS  int  clMemcpyImg2D(cl_command_queue cq,
	                            void **src, void **dst, 
								size_t width,
								size_t height, 
								clMemcpyFlag flag ,
								size_t *src_origin = NULL, 
								size_t *dst_origin = NULL, 
								size_t rowPitch = 0,   //  set the row pitch of CPU buffer
								size_t bufOffset = 0,
								cl_bool sync = CL_TRUE ,
								cl_event* ev = NULL);

//free the buffer memory
DLL_EXPORTS  int  clMemFree(void **mem);
//



#ifdef __cplusplus
}
#endif

#endif