
#include "OpenCLCore.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/************************************************************************************/
/*                          the static function for OpenCL                          */
/************************************************************************************/

static  cl_int  clInitialize(cl_context *ct, cl_command_queue* cq, cl_device_id *cd, cl_device_type device_type);
static  cl_int  clLoadProgram(const char* source, cl_context *ct, cl_device_id *cd, cl_program *cp);

DLL_EXPORTS int clIntialEnviroment(MHandle OCLEngine, char *sourceCode)
{
	int  status = -1;
	OCL_CORE *oclHandle = NULL;
	
	if ((NULL == sourceCode) || (NULL == OCLEngine))
		return status;	

	oclHandle = (OCL_CORE *)OCLEngine;

    status = clInitialize(&(oclHandle->g_CLContext), &(oclHandle->g_CLCommandQueue), &(oclHandle->g_CLDevices), CL_DEVICE_TYPE_GPU);
	if(0 != status)
		return status;

	status = clLoadProgram(sourceCode, &(oclHandle->g_CLContext), &(oclHandle->g_CLDevices), &(oclHandle->g_CLProgram));

	return status;
}

DLL_EXPORTS   int  clUnintialEnviroment(MHandle OCLEngine)
{
	int  status = -1;
	OCL_CORE *oclHandle = NULL;

	if( NULL == OCLEngine)
		return status;
	oclHandle = (OCL_CORE *)OCLEngine;

	status = clReleaseProgram(oclHandle->g_CLProgram);
	status = clReleaseContext(oclHandle->g_CLContext);

	return status;
}

DLL_EXPORTS  int  clGetCode(char *fileName, char *sourceCode, int *offset)
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

	if(NULL != offset)
		_offset = *offset;
	else _offset=0;

	if(0 == _offset)
		memcpy(sourceCode, buf, (size+1)*sizeof(char));
	else
		strcat(sourceCode+_offset, buf);

	if(NULL != offset)
		*offset = _offset + size;

	free(buf);
	return 0;
}
//initialize OPENCL  environment 
cl_int clInitialize(cl_context *ct, cl_command_queue* cq, cl_device_id *cd, cl_device_type device_type)
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

	status = clGetDeviceIDs(platformId[0],device_type, 1,cd,NULL);
	if(CL_SUCCESS != status)
		return status;

	//create the context
	cl_context_properties cps[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platformId[0], 0};
	*ct = clCreateContext(cps, 1, cd, NULL, NULL, &status);
	if (CL_SUCCESS != status)
		return status;

	//create the command
	*cq = clCreateCommandQueue(*ct, *cd, 0, &status);
	if (CL_SUCCESS != status)
		return status;

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



// malloc the OpenCL buffer memory
DLL_EXPORTS  int  clMallocBuf(cl_context cc, void **buf, size_t size, cl_mem_flags flag)
{
	cl_int status = -1; 
	//create the buffer
	*buf = clCreateBuffer(cc, CL_MEM_READ_WRITE, size, 0, &status);
	return status;
}

//copy buffer memory between cpu to gpu
DLL_EXPORTS  int  clMemcpyBuf(cl_command_queue cq,
	                          void **src, void **dst, 
							  size_t size , 
							  clMemcpyFlag flag, 
							  size_t src_offset,
							  size_t dst_offset, 
							  cl_bool sync, 
							  cl_event *ev)
{
	cl_int status = -1;
  
	if (cq == 0)
		return status;

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
DLL_EXPORTS  int  clMallocImg2D(cl_context cc,
	                            void **img, 
								size_t width, 
								size_t height,
								cl_image_format* imageFormat, 
								cl_mem_flags flag)
{
	int status = -1;
	cl_image_format format;

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
DLL_EXPORTS  int  clMemcpyImg2D(cl_command_queue cq,
	                            void **src, void **dst, 
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

