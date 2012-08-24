
#include "kernelInterface.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "OpenCLCore.h"
#include "kernelType.h"

KERNEL_API kernelAPI;

static  int  clCreateKetnelFunc(KERNEL_API *kernel_API);
static  int  clReleaseKetnelFunc(KERNEL_API *kernel_API);
static  int  clGetCode(char *fileName, char *sourceCode, int *offset);


DLL_EXPORTS  int  clInitialKernelAPI()
{
	int status = -1;
	int offset = 0;	
	char *sourceCode = NULL;

	sourceCode = (char *)malloc(5*1024*1024);
	if(NULL == sourceCode)
		goto exit;

	if(0 != clGetCode("F:/Samuel/library_samuel/modules/OCLLibrary/inc/utilities_kernel.cl", sourceCode, &offset))
		goto exit;

	if(0 != clGetCode("F:/Samuel/library_samuel/modules/OCLLibrary/inc/fft_kernel.cl", sourceCode, &offset))
		goto exit;

	if(0 != clIntialEnviroment(sourceCode, NULL, NULL ,NULL))
		goto exit;

	if(0 != clCreateKetnelFunc(&kernelAPI))
		goto exit;

	status = 0;
exit:
	if(NULL != sourceCode) free(sourceCode);
	if(0 != status) clReleaseKernelAPI();
	return status;
}

DLL_EXPORTS  int  clReleaseKernelAPI()
{
	int status = -1;

	status = clReleaseKetnelFunc(&kernelAPI);
	status = clUnintialEnviroment();

	return status;
}

static int clCreateKetnelFunc(KERNEL_API *kernel_API)
{
	int status = -1;
	cl_program   cp = 0;

	if (NULL == kernel_API)
		return status;	

	cp = GetOclProgram();
	if(NULL == cp)
		return status;

	//create the function
	kernel_API->g_HistogramStatistics_kernel = clCreateKernel(cp, "histogramStatistics_kernel", &status);
	kernel_API->g_Reduction_Min_kernel       = clCreateKernel(cp, "reduction_Min_kernel",       &status);
	kernel_API->g_Scan_Exclusive_Kernel      = clCreateKernel(cp, "scan_Exclusive_Kernel",      &status);
	kernel_API->g_UniformAdd_Kernel          = clCreateKernel(cp, "uniformAdd_Kernel",          &status);
	//zero memory
	kernel_API->g_ZeroMemory_kernel1         = clCreateKernel(cp, "zeroMemory_kernel1",         &status);
	kernel_API->g_ZeroMemory_kernel2         = clCreateKernel(cp, "zeroMemory_kernel2",         &status);
	kernel_API->g_ZeroMemory_kernel4         = clCreateKernel(cp, "zeroMemory_kernel4",         &status);
	//transpose
	kernel_API->g_Transpose_kernel           = clCreateKernel(cp, "transpose_kernel"  ,         &status);

	//FFT
	kernel_API->g_FFT_1D_R2C_kernel          = clCreateKernel(cp, "fft_1D_R2C_kernel"  ,        &status);
	kernel_API->g_FFT_2D_R2C_kernel          = clCreateKernel(cp, "fft_2D_R2C_kernel"  ,        &status);	
	kernel_API->g_FFT_2D_C2C_kernel          = clCreateKernel(cp, "fft_2D_C2C_kernel"  ,        &status);
	kernel_API->g_FFT_2D_C2R_kernel          = clCreateKernel(cp, "fft_2D_C2R_kernel"  ,        &status);
	return status;
}

static int clReleaseKetnelFunc(KERNEL_API *kernel_API)
{
	int status = -1;

	if (NULL == kernel_API)
		return status;

	//release the function
	status = clReleaseKernel(kernel_API->g_HistogramStatistics_kernel);
	status = clReleaseKernel(kernel_API->g_Reduction_Min_kernel);
	status = clReleaseKernel(kernel_API->g_Scan_Exclusive_Kernel);
	status = clReleaseKernel(kernel_API->g_UniformAdd_Kernel);
	status = clReleaseKernel(kernel_API->g_ZeroMemory_kernel1);
	status = clReleaseKernel(kernel_API->g_ZeroMemory_kernel2);
	status = clReleaseKernel(kernel_API->g_ZeroMemory_kernel4);
	status = clReleaseKernel(kernel_API->g_Transpose_kernel);
	status = clReleaseKernel(kernel_API->g_FFT_1D_R2C_kernel);
	status = clReleaseKernel(kernel_API->g_FFT_2D_R2C_kernel);
	status = clReleaseKernel(kernel_API->g_FFT_2D_C2C_kernel);
	status = clReleaseKernel(kernel_API->g_FFT_2D_C2R_kernel);
	return status;
}

/*******************************************************************
fileName   : file's path ,including the file's name.
sourceCode : The pointer to store the source codes.
offset     : The offset of the sourceCode pointer.
             First must be equal to zero , can be set to zero.
return :  0 means correct.
********************************************************************/
static  int  clGetCode(char *fileName, char *sourceCode, int *offset)
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