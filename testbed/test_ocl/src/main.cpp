
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <kernelInterface.h>
#include <utilities_API.h>
#include <OpenCLCore.h>
#include <string>
#if 0
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
#else
int main(int argc, char **argv)
{
	int status  = -1;
	
	unsigned char *c_data = (unsigned char *)malloc(10*10);
	unsigned char *result = (unsigned char *)malloc(10*10);

	for(int i=0; i<10*10; i++)
	{
		c_data[i] = i;
		if(i%10==0)
			printf("\n");
		printf("%3d  ", c_data[i]);
		
	}
    status = clInitialKernelAPI();

	cl_mem g_data = NULL;
	cl_mem g_result = NULL;
	
	status = clMallocBuf((void**)(&g_data),100);
	status = clMallocBuf((void**)(&g_result),100);

	status = clMemcpyBuf((void**)(&c_data),(void**)(&g_data),100, clMemcpyHostToDevice);

	status = ZeroMemory_OCL(g_result, 10*10,NULL);
	status = Transpose_OCL(g_data, g_result, 10,10 ,NULL);

	status = clMemcpyBuf((void**)(&g_result),(void**)(&result),100, clMemcpyDeviceToHost);
	//status = clMemcpyBuf((void**)(&g_data),(void**)(&result),100, clMemcpyDeviceToHost);

	printf(" \n result \n ");
	for(int i=0; i<100; i++)
	{
		if(i%10==0)
			printf("\n");
		printf("%3d  ", result[i]);		
	}
		
	clReleaseKernelAPI();

    return 0;
}
#endif
