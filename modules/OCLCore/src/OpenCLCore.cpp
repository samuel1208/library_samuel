
#include "OpenCLCore.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct _OCL_CORE_
{
	cl_context			  g_CLContext ;			//cl context
	cl_device_id 		  g_CLDevices ;		    //cl devices
	size_t				  g_CLDeviceListSize  ;	//cl device size list
	cl_command_queue	  g_CLCommandQueue ;	//cl command queue
	cl_program            g_CLProgram ;         //cl program
} OCL_CORE;
/************************************************************************************/
/*                          the static function for OpenCL                          */
/************************************************************************************/
static  OCL_CORE ocl_core[1]= {0};

static  cl_int  clInitialize(cl_context ct, cl_command_queue cq, cl_device_id cd);
static  cl_int  clLoadProgram(const char* source, cl_context *ct, cl_device_id *cd, cl_program *cp);

// reserve the interface to get GPU info from out side
DLL_EXPORTS  int  clIntialEnviroment(char *sourceCode, cl_context ct, cl_command_queue cq, cl_device_id cd)
{
	int  status = -1;
	
	if (NULL == sourceCode)
		return status;	

	memset(ocl_core, 0, sizeof(OCL_CORE));

    status = clInitialize(ct, cq, cd);
	if(0 != status)
		return status;

	status = clLoadProgram(sourceCode, &(ocl_core[0].g_CLContext), &(ocl_core[0].g_CLDevices), &(ocl_core[0].g_CLProgram));

	return status;
}

DLL_EXPORTS   int  clUnintialEnviroment()
{
	int  status = -1;

	status = clReleaseProgram(ocl_core[0].g_CLProgram);
	status = clReleaseContext(ocl_core[0].g_CLContext);
	
	memset(ocl_core, 0, sizeof(OCL_CORE));
	return status;
}

DLL_EXPORTS  cl_context	 GetOclContext() 
{
	return ocl_core[0].g_CLContext;
}		
DLL_EXPORTS	 cl_command_queue  GetOclCommandQueue()
{
	return ocl_core[0].g_CLCommandQueue;
}
DLL_EXPORTS  cl_program   GetOclProgram() 
{
	return ocl_core[0].g_CLProgram;
}
// malloc the OpenCL buffer memory
DLL_EXPORTS  int  clMallocBuf(void **buf, size_t size, cl_mem_flags flag)
{
	cl_int status = -1; 
	cl_context cc = ocl_core[0].g_CLContext;
	//create the buffer
	*buf = clCreateBuffer(cc, CL_MEM_READ_WRITE, size, 0, &status);
	return status;
}

//copy buffer memory between cpu to gpu
DLL_EXPORTS  int  clMemcpyBuf(void **src, void **dst, 
							  size_t size , 
							  clMemcpyFlag flag, 
							  size_t src_offset,
							  size_t dst_offset, 
							  cl_bool sync, 
							  cl_event *ev)
{
	cl_int status = -1;

	cl_command_queue cq = ocl_core[0].g_CLCommandQueue;

	if (clMemcpyHostToDevice == flag)
		status = clEnqueueWriteBuffer(cq, (cl_mem)(*dst), sync, dst_offset, size, 
		                              (void*)((char*)(*src)+src_offset), 0, 0, ev);
	else if (clMemcpyDeviceToHost == flag)
		status = clEnqueueReadBuffer( cq, (cl_mem)(*src), sync, src_offset, size, 
	                                  (void*)((char*)(*dst)+dst_offset), 0, 0, ev);	
	else if (clMemcpyDeviceToDevice == flag)
		status = clEnqueueCopyBuffer( cq, (cl_mem)(*src), (cl_mem)(*dst), 
		                              src_offset, dst_offset, size, 0, 0, ev);

	return status;
}
//
//Create image 2d 
DLL_EXPORTS  int  clMallocImg2D(void **img, 
								size_t width, 
								size_t height,
								cl_image_format* imageFormat, 
								cl_mem_flags flag)
{
	int status = -1;
	cl_image_format format;
	cl_context cc = ocl_core[0].g_CLContext;

	if (NULL == imageFormat)
	{
		format.image_channel_data_type  = CL_SIGNED_INT32;
		format.image_channel_order      = CL_R;
	}
	else
	{
		format.image_channel_data_type  = imageFormat->image_channel_data_type;
		format.image_channel_order      = imageFormat->image_channel_order;
	}

	//create the buffer	
	*img = clCreateImage2D(cc, flag, &format, width, height, 0, 0, &status);

	return status;
}
//
DLL_EXPORTS  int  clMemcpyImg2D(void **src, void **dst, 
								size_t width,
								size_t height, 
								clMemcpyFlag flag ,
								size_t *src_origin , 
								size_t *dst_origin , 
								size_t rowPitch,   //  set the row pitch of CPU buffer
								size_t bufOffset ,
								cl_bool sync  ,
								cl_event* ev )
{
    int status = -1;

	size_t *sOrigin;
	size_t *dOrigin;
	size_t region[3] = {width, height, 1};
	cl_command_queue cq = ocl_core[0].g_CLCommandQueue;

	if (NULL == src_origin)
	{
		sOrigin = new size_t[3];
		sOrigin[0] = 0;
		sOrigin[1] = 0;
		sOrigin[2] = 0;
	}
	else 
		sOrigin = src_origin;

	if (NULL == dst_origin)
	{
		dOrigin = new size_t[3];
		dOrigin[0] = 0;
		dOrigin[1] = 0;
		dOrigin[2] = 0;
	}
	else 
		dOrigin = dst_origin;

	if (clMemcpyHostToDevice == flag)
		status = clEnqueueWriteImage(cq, (cl_mem)(*dst), sync, dOrigin, region, rowPitch, 0, (void*)(*src), 0, 0, ev);
	else if (clMemcpyDeviceToHost == flag)
		status = clEnqueueReadImage( cq, (cl_mem)(*src), sync, sOrigin, region, rowPitch, 0, (void*)(*dst), 0, 0, ev);	
	else if (clMemcpyImgToImg == flag)
		status = clEnqueueCopyImage( cq, (cl_mem)(*src), (cl_mem)(*dst), sOrigin, dOrigin, region, 0, 0, ev);
	else if (clMemcpyImgToBuf == flag)
		status = clEnqueueCopyImageToBuffer(cq, (cl_mem)(*src), (cl_mem)(*dst), sOrigin, region, bufOffset, 0, 0, ev);
	else if (clMemcpyBufToImg == flag)
		status = clEnqueueCopyBufferToImage(cq, (cl_mem)(*src), (cl_mem)(*dst), bufOffset, sOrigin, region, 0, 0, ev);


	if (NULL == dst_origin)
		delete []dOrigin;
	if (NULL == src_origin)
		delete []sOrigin;
	return status;
}


//free the OpenCL  memory object
DLL_EXPORTS  int  clMemFree(void **mem)
{
	int status = -1;
	if( 0 == *mem )
		return status;
   status = clReleaseMemObject((cl_mem)(*mem));
	//set the memory none
	*mem = 0;
	return status;
}

//initialize OPENCL  environment 
static cl_int clInitialize(cl_context ct, cl_command_queue cq, cl_device_id cd)
{
	cl_int	status = -1;
	unsigned int i=0;

	//initialize the OCL envrioment  outside
	if (NULL != cd)
	{
		ocl_core[0].g_CLDevices = cd;
		if(NULL != ct)
			ocl_core[0].g_CLContext = ct;
		else
		{
			status = clGetContextInfo(ocl_core[0].g_CLContext, CL_CONTEXT_DEVICES, 1, &cd, 0);
			if (CL_SUCCESS != status)
				return status;
		}
		if (NULL != cq)
			ocl_core[0].g_CLCommandQueue = cq;
		else
		{	
			ocl_core[0].g_CLCommandQueue = clCreateCommandQueue(ocl_core[0].g_CLContext, cd, 0, &status);
			if (CL_SUCCESS != status)
				return status;
		}
	}
	else  //initialize the OCL envrioment inside
	{
		cl_uint	platformNum = 0;
		status = clGetPlatformIDs(0 , NULL , &platformNum);
		if (0 == platformNum)
			return CL_INVALID_PLATFORM;

		cl_platform_id*	platformId = new cl_platform_id[platformNum];
	    status = clGetPlatformIDs(platformNum, platformId, 0);
		if (CL_SUCCESS != status)
			return status;

		//get GPU device 
		for(i=0; i<platformNum; i++)
		{
			status = clGetDeviceIDs(platformId[i], CL_DEVICE_TYPE_GPU, 1, &(ocl_core[0].g_CLDevices),NULL);
			if (CL_SUCCESS == status)
				break;
		}
		if(CL_SUCCESS != status)
			return status;

		//create the context
		ocl_core[0].g_CLContext = clCreateContext(0, 1, &(ocl_core[0].g_CLDevices), NULL, NULL, &status);
		if (CL_SUCCESS != status)
			return status;

		//create the command
		ocl_core[0].g_CLCommandQueue = clCreateCommandQueue(ocl_core[0].g_CLContext, ocl_core[0].g_CLDevices, 0, &status);
		if (CL_SUCCESS != status)
			return status;
	}	
	status = 0;
	return status;
}

//Load OpenCL program
cl_int  clLoadProgram(const char* source, cl_context *ct, cl_device_id *cd, cl_program *cp)
{
	cl_int status = -1;

	if ((NULL == source) || (NULL == cp) || (NULL == ct) || (NULL == cd))
		return status;

	//Load the kernel
	size_t kernelSize = strlen(source);
	*cp = clCreateProgramWithSource(*ct, 1, &source, &kernelSize, &status);
	if (CL_SUCCESS != status)
		return status;

	//build the program
	status = clBuildProgram(*cp, 1, cd ,0, 0, 0);
	if (CL_SUCCESS != status)
	{
		size_t size = 0;
		clGetProgramBuildInfo(*cp, *cd, CL_PROGRAM_BUILD_LOG, 0, 0, &size);

		// get build log 
		char *errorLog = (char*)malloc(size);
		clGetProgramBuildInfo(*cp, *cd, CL_PROGRAM_BUILD_LOG, size, errorLog, NULL);
		printf("%s", errorLog);
		free(errorLog);
		return status;
	}
	status = 0;
	return status;
}