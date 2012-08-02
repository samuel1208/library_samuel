
#include "kernelInterface.h"
#include "stdlib.h"

static int clCreateKetnelFunc(MHandle oclHandle, KERNEL_API *kernel_API);
static int clReleaseKetnelFunc(KERNEL_API *kernel_API);

DLL_EXPORTS  int  clCreateEngine(MHandle handle)
{
	int status = -1;
	int offset = 0;	
	char *sourceCode = NULL;
	OCL_ENGINE *clEngine = NULL;

	if ((NULL == handle))
		goto exit;

	clEngine = (OCL_ENGINE *)handle;
	sourceCode = (char *)malloc(5*1024*1024);
	if(NULL == sourceCode)
		goto exit;

	if(0 != clGetCode("F:/Samuel/library_samuel/modules/OCLLibrary/inc/utilities_kernel.cl", sourceCode, &offset))
		goto exit;

	if(0 != clIntialEnviroment(&(clEngine->ocl_Core), sourceCode))
		goto exit;

	if(0 != clCreateKetnelFunc(&(clEngine->ocl_Core), &(clEngine->kernel_API)))
		goto exit;

	status = 0;
exit:
	if(NULL != sourceCode) free(sourceCode);
	if(0 != status) clReleaseEngine(clEngine);
	return status;
}

DLL_EXPORTS  int  clReleaseEngine(MHandle handle)
{
	int status = -1;
	OCL_ENGINE *clEngine = NULL;

	if (NULL == handle)
		return status;
	clEngine = (OCL_ENGINE *)handle;

	status = clReleaseKetnelFunc(&(clEngine->kernel_API));
	status = clUnintialEnviroment(&(clEngine->ocl_Core));

	return status;
}

static int clCreateKetnelFunc(MHandle oclHandle, KERNEL_API *kernel_API)
{
	int status = -1;
	cl_program   cp = 0;

	if ((NULL == kernel_API) || (NULL == oclHandle))
		return status;	

	cp = ((OCL_CORE *)oclHandle)->g_CLProgram;

	//create the function
	kernel_API->g_HistogramStatistics_kernel = clCreateKernel(cp, "HistogramStatistics_kernel", &status);

	return status;

}

static int clReleaseKetnelFunc(KERNEL_API *kernel_API)
{
	int status = -1;

	if (NULL == kernel_API)
		return status;

	//release the function
	status = clReleaseKernel(kernel_API->g_HistogramStatistics_kernel);

	return status;
}