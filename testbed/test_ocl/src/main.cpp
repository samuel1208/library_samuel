
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <kernelInterface.h>
#include <utilities_API.h>

int main(int argc, char **argv)
{
	int status  = -1;
	OCL_ENGINE *cl_Engine= NULL;
	cl_Engine = (OCL_ENGINE *)malloc(sizeof(OCL_ENGINE));
	unsigned char *c_data = (unsigned char *)malloc(1024); 
	int *c_hist = (int *)malloc(256*sizeof(int));
	memset(c_hist, 0 , 1024);

	for(int i=0; i<1024; i++)
		c_data[i] = i%256;
    status = clCreateEngine(cl_Engine);

	cl_mem g_data = NULL;
	cl_mem g_hist = NULL;
	
	status = clMallocBuf(cl_Engine->ocl_Core.g_CLContext, (void**)(&g_data),1024);
	status = clMallocBuf(cl_Engine->ocl_Core.g_CLContext, (void**)(&g_hist),256*sizeof(int));

	status = clMemcpyBuf(cl_Engine->ocl_Core.g_CLCommandQueue,  (void**)(&c_data),(void**)(&g_data),1024, clMemcpyHostToDevice);
	status = clMemcpyBuf(cl_Engine->ocl_Core.g_CLCommandQueue,  (void**)(&c_hist),(void**)(&g_hist), 256*4, clMemcpyHostToDevice);


	status = HistogramStatistics_OCL(g_data, g_hist, 1024, cl_Engine->ocl_Core.g_CLCommandQueue,
		                    cl_Engine->kernel_API.g_HistogramStatistics_kernel, NULL);

	status = clMemcpyBuf(cl_Engine->ocl_Core.g_CLCommandQueue,(void**)(&g_hist), (void**)(&c_hist), 256*4, clMemcpyDeviceToHost);

	for(int i=0; i<256; i++)
		printf("%d \n", c_hist[i]);
		
	clReleaseEngine(cl_Engine);

	//test for opencv2.4.2
	/*
    const char *keymap =  
      "{ f | filename | default_value | file name }"
      "{ o | option   | 100           | option }" ;
    cv::CommandLineParser parser(argc, argv, keymap) ;

    std:: string file = parser.get<std::string>("filename",0);
    int o = parser.get<int>("option");
    
    printf("%s, %d", file.c_str() , o);
    printf("ok");
	*/
    return 0;
}
