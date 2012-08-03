
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <kernelInterface.h>
#include <utilities_API.h>
#include <OpenCLCore.h>
#include <string>

int main(int argc, char **argv)
{
	int status  = -1;
	
	int *c_data = (int *)malloc(512*512*4);
	int *result = (int *)malloc(512*512*4);

	for(int i=0; i<512*512; i++)
		c_data[i] = 1;
    status = clInitialKernelAPI();

	cl_mem g_data = NULL;
	cl_mem g_temp = NULL;
	cl_mem g_result = NULL;
	
	status = clMallocBuf((void**)(&g_data),512*512*4);
	status = clMallocBuf((void**)(&g_result),512*512*4);

	status = clMemcpyBuf((void**)(&c_data),(void**)(&g_data),512*512*4, clMemcpyHostToDevice);

	//status = ScanEexclusive(g_data, g_result, 512*512, NULL);
	status = ZeroMemory_OCL(g_data, 512*512*4, NULL);

	status = clMemcpyBuf((void**)(&g_data),(void**)(&result),512*512*4, clMemcpyDeviceToHost);

	for(int i=0; i<512*512; i++)
		printf("%d \n", result[i]);
		
	clReleaseKernelAPI();

    return 0;
}
