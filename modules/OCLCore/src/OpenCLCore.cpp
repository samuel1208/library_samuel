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
#define EXPORT_IPL


#include "OpenCLCore.h"

/************************************************************************************/
/*                          the static function for OpenCL                          */
/************************************************************************************/

static cl_int  clInitialize(cl_context ct , cl_command_queue cq , cl_device_id cd, cl_device_type device_type);
static cl_int  clLoadProgram(const char* source, cl_context ct, cl_device_id cd, cl_program cp);
static int     GetCode_OCL(char *fileName, char *sourceCode, int *offset);

MResult CreateEngine_OCL(MHandle handle)
{
	MResult status = MERROR;
	OCL_ENGINE *oclHandle = NULL;
	char *sourceCode = NULL;
	char *fileName = NULL;
	int codeOffset = 0;

    __SAK_BEGIN__;


	if(NULL == handle)
		return NULL;
	oclHandle = (OCL_ENGINE *)handle;

    status = clInitialize(oclHandle->g_CLContext, oclHandle->g_CLCommandQueue, oclHandle->g_CLDevices, CL_DEVICE_TYPE_GPU);
	if(MOK != status)
		return status;

#if ENCRYPT_OCL_KERNEL
	sourceCode = kernel;
#else
	sourceCode = (char *)malloc(10*1024*1024);
	if(NULL == sourceCode)
		return NULL;

	fileName = "F:\\Project\\StereoMatch-yuli\\StereoMatch_OCL\\BSLibs\\OpenCLCore\\inc\\kernel_OCL.cl";
	status = GetCode_OCL(fileName,sourceCode, &codeOffset);
	if(MOK != status)
		return status;

#endif

	status = clLoadProgram(sourceCode, oclHandle->g_CLContext, oclHandle->g_CLDevices, oclHandle->g_CLProgram);
	if(MOK != status)
		return status;


	__SAK_END__;
	if(NULL != sourceCode) free(sourceCode);
	if(NULL != OCL_ENGINE) free
	return oclHandle;

}

//
int  GetCode_OCL(char *fileName, char *sourceCode, int *offset)
{
	char          *buf=NULL;
	FILE          *file = NULL;
	unsigned int  size = 0;
	int           _offset = 0;

	file = fopen(fileName,"rb");
	if(NULL == file)
		return -1;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	buf = (char*)malloc((size+1)*sizeof(char));
	if(NULL == buf)
		return -1;
	fread(buf, sizeof(char), size, file);
	fclose(file);

	buf[size] = '\0';

	_offset = *offset;
	if(0 == _offset)
		memcpy(sourceCode, buf, (size+1)*sizeof(char));
	else
		strcat(sourceCode+_offset, buf);
	*offset = _offset + size;

	free(buf);
	return 0;
}
//initialize OPENCL  environment 
cl_int clInitialize(cl_context ct, cl_command_queue cq, cl_device_id cd, cl_device_type device_type)
{
	//status
	cl_int	status = -1;
	if ((NULL == ct) || (NULL == cd) || (NULL == cq))
		return status;

	cl_uint	platformNum = 0;
	status = clGetPlatformIDs(0 , NULL , &platformNum);
	if (0 == platformNum)
		return status;

	cl_platform_id*	platformId = new cl_platform_id[platformNum];
	status = clGetPlatformIDs(platformNum, platformId, 0);
	if (CL_SUCCESS != status)
		return status;

	//g_CLDevices = new cl_device_id[10];
	status = clGetDeviceIDs(platformId[0],device_type, 1,&cd,NULL);
	//if(CL_SUCCESS != status)
	//	status = clGetDeviceIDs(targetPlatform,CL_DEVICE_TYPE_CPU, 1,g_CLDevices,NULL);
	if (CL_SUCCESS != status)
		return status;

	//create the context
	cl_context_properties cps[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platformId[0], 0};
	ct = clCreateContext(cps, 1, &cd, NULL, NULL, &status);
	if (CL_SUCCESS != status)
		return status;

	//create the command
	cq = clCreateCommandQueue(ct, cd, 0, &status);
	if (CL_SUCCESS != status)
		return status;
	return status;
}

//Load OpenCL program
cl_int  clLoadProgram(const char* source, cl_context ct, cl_device_id cd, cl_program cp)
{
	cl_int status = -1;

	if ((NULL == source) || (NULL == cp))
		return status;

	//Load the kernel
	size_t kernelSize = strlen(source);
	cp = clCreateProgramWithSource(ct, 1, &source, &kernelSize, &status);
	if (CL_SUCCESS != status)
		return status;

	//build the program
	status = clBuildProgram(cp, 1, &cd ,0, 0, 0);
	if (CL_SUCCESS != status)
	{
		size_t size = 0;
		clGetProgramBuildInfo(cp, cd, CL_PROGRAM_BUILD_LOG, 0, 0, &size);

		// get build log 
		char *errorLog = (char*)malloc(size);
		clGetProgramBuildInfo(cp, cd, CL_PROGRAM_BUILD_LOG, size, errorLog, NULL);
		printf("%s", errorLog);
		free(errorLog);
		return status;
	}
	return status;
}
//
////Load cl kernels
//cl_int    clCreateKernelFunc(cl_kernel *kernel, const char* kernelName)
//{
//	cl_int status = CL_SUCCESS;
//
//	//Load the kernel
//	*kernel = clCreateKernel(g_CLProgram, kernelName, &status);
//
//	//push kernel to the list , for convinent free
//	if (CL_SUCCESS == status)
//		g_CLKernels.push_back(*kernel);
//
//	return status;
//}
//
////set kernel argument
//cl_int    clSetArg(cl_kernel *kernel, size_t argIndex, size_t argSize, const void * argValue)
//{
//    return clSetKernelArg(*kernel, argIndex, argSize, argValue);
//}
//
////run the kernel function
//IPL_API cl_int	clRunKernelFunc(cl_kernel *kernel, 
//										cl_uint  work_dim, 										
//										const size_t *global_work_size,
//										const size_t *local_work_size,
//										const size_t *global_work_offset,
//										cl_uint       num_events_in_wait_list,
//										const cl_event *event_wait_list,
//										cl_event *ev)
//{
//	return clEnqueueNDRangeKernel(g_CLCommandQueue, 
//		                          *kernel, 
//								  work_dim,
//								  global_work_offset,
//								  global_work_size,
//								  local_work_size,
//								  num_events_in_wait_list,
//								  event_wait_list,
//								  ev);
//}
//
//// malloc the OpenCL buffer memory
//cl_int    clMallocBuf(void **buf, size_t size, cl_mem_flags flag)
//{
//	cl_int status = CL_SUCCESS; 
//
//	//create the buffer
//	*buf = clCreateBuffer(g_CLContext, CL_MEM_READ_WRITE, size, 0, &status);
//
//	return status;
//}
//
////copy buffer memory between cpu to gpu
//cl_int    clMemcpyBuf(void **src, void **dst, 
//					  size_t size , 
//					  clMemcpyFlag flag, 
//					  size_t src_offset,
//					  size_t dst_offset, 
//					  cl_command_queue cq, 
//					  cl_bool sync, 
//					  cl_event *ev)
//{
//	cl_int status = CL_SUCCESS;
//  
//	if (cq == 0)
//		cq = g_CLCommandQueue;
//
//	if (clMemcpyHostToDevice == flag)
//		status = clEnqueueWriteBuffer(cq, (cl_mem)(*dst), sync, dst_offset, size, 
//		                              (void*)((char*)(*src)+src_offset), 0, 0, ev);
//	else if (clMemcpyDeviceToHost == flag)
//		status = clEnqueueReadBuffer( cq, (cl_mem)(*src), sync, src_offset, size, 
//	                                  (void*)((char*)(*dst)+dst_offset), 0, 0, ev);	
//	else if (clMemcpyDeviceToDevice == flag)
//		status = clEnqueueCopyBuffer( cq, (cl_mem)(*src), (cl_mem)(*dst), 
//		                              src_offset, dst_offset, size, 0, 0, ev);
//
//	return status;
//}
//
////Create image 2d 
//cl_int    clMallocImg2D(void **img, size_t width, size_t height, 
//						cl_image_format* imageFormat, cl_mem_flags flag)
//{
//	cl_int status = CL_SUCCESS;
//	cl_image_format format;
//
//	if (NULL == imageFormat)
//	{
//		format.image_channel_data_type  = CL_SIGNED_INT32;
//		format.image_channel_order      = CL_R;
//	}
//	else
//	{
//		format.image_channel_data_type  = imageFormat->image_channel_data_type;
//		format.image_channel_order      = imageFormat->image_channel_order;
//	}
//
//	//create the buffer	
//	*img = clCreateImage2D(g_CLContext, flag, &format, width, height, 0, 0, &status);
//
//	return status;
//}
//
//cl_int	clMemcpyImg2D(void **src, void **dst, 
//						size_t width, 
//						size_t height, 
//						clMemcpyFlag flag, 
//						size_t *src_origin, 
//						size_t *dst_origin, 
//						size_t rowPitch,
//						size_t bufOffset,
//						cl_command_queue cq , 
//						cl_bool sync , 
//						cl_event* ev)
//{
//    cl_int status = CL_SUCCESS;
//
//	if (cq == 0)
//		cq = g_CLCommandQueue;
//
//	size_t *sOrigin;
//	size_t *dOrigin;
//	size_t region[3] = {width, height, 1};
//
//	if (NULL == src_origin)
//	{
//		sOrigin = new size_t[3];
//		sOrigin[0] = 0;
//		sOrigin[1] = 0;
//		sOrigin[2] = 0;
//	}
//	else 
//		sOrigin = src_origin;
//
//	if (NULL == dst_origin)
//	{
//		dOrigin = new size_t[3];
//		dOrigin[0] = 0;
//		dOrigin[1] = 0;
//		dOrigin[2] = 0;
//	}
//	else 
//		dOrigin = dst_origin;
//
//	if (clMemcpyHostToDevice == flag)
//		status = clEnqueueWriteImage(cq, (cl_mem)(*dst), sync, dOrigin, region, rowPitch, 0, (void*)(*src), 0, 0, ev);
//	else if (clMemcpyDeviceToHost == flag)
//		status = clEnqueueReadImage( cq, (cl_mem)(*src), sync, sOrigin, region, rowPitch, 0, (void*)(*dst), 0, 0, ev);	
//	else if (clMemcpyImgToImg == flag)
//		status = clEnqueueCopyImage( cq, (cl_mem)(*src), (cl_mem)(*dst), sOrigin, dOrigin, region, 0, 0, ev);
//	else if (clMemcpyImgToBuf == flag)
//		status = clEnqueueCopyImageToBuffer(cq, (cl_mem)(*src), (cl_mem)(*dst), sOrigin, region, bufOffset, 0, 0, ev);
//	else if (clMemcpyBufToImg == flag)
//		status = clEnqueueCopyBufferToImage(cq, (cl_mem)(*src), (cl_mem)(*dst), bufOffset, sOrigin, region, 0, 0, ev);
//
//
//	if (NULL == dst_origin)
//		delete []dOrigin;
//	if (NULL == src_origin)
//		delete []sOrigin;
//	return status;
//}
//
////free the OpenCL  memory object
//void	clMemFree(void **mem)
//{
//	if( 0 == *mem )
//		return;
//   clReleaseMemObject((cl_mem)(*mem));
//
//	//set the memory none
//	*mem = 0;
//}
//
//cl_context   GetContext()
//{
//	return g_CLContext;
//}
//
////get OpenCL program
//cl_program   GetProgram()
//{
//	return g_CLProgram;
//}
//
////get command queue of OpenCL
//cl_command_queue	GetCommandQueue()
//{
//	return g_CLCommandQueue;
//}
//
////release the opencl enviroment
//void	clRelease()
//{
//	clReleaseKernels();
//	clReleaseProgram(g_CLProgram);
//	clReleaseContext(g_CLContext);
//}
//
////release kernel
//void	clReleaseKernels()
//{
//	std::list<cl_kernel>::iterator it = g_CLKernels.begin();
//	while( it != g_CLKernels.end() )
//	{
//		clReleaseKernel( *it );
//		it++;
//	}
//	g_CLKernels.clear();
//}
//
