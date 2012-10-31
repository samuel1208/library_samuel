
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <kernelInterface.h>
#include <utilities_API.h>
#include <OpenCLCore.h>
#include <string>
#include <IPL_fft.h>
#include <IPL_core.h>
#include <Time_t.h>
#include <fft_kernel.h>

unsigned int log2( unsigned int x )
{
  unsigned int ans = 0 ;
  while( x>>=1 ) ans++;
  return ans ;
}

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
	status = ZeroMemory_OCL(g_data, 512*512*4, NULL, 0, NULL, NULL);

	status = clMemcpyBuf((void**)(&g_data),(void**)(&result),512*512*4, clMemcpyDeviceToHost);

	for(int i=0; i<512*512; i++)
		printf("%d \n", result[i]);
		
	clReleaseKernelAPI();

    return 0;
}
#elif 0//test FFT
int main(int argc, char **argv)
{
	int status  = -1;
	
	const int N = 1024;
	double *c_data = (double *)malloc(N*sizeof(double)*N);
	double *result = (double *)malloc(N*2*sizeof(double)*N);
	double *Iresult = (double *)malloc(N*sizeof(double)*N);
	complex_t *dst1= (complex_t *)malloc(N*sizeof(complex_t)*N);
	cl_mem g_data = NULL;
	cl_mem g_Iresult = NULL;
	cl_mem g_result = NULL;
	cl_mem g_temp = NULL;

	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		     c_data[i*N + j] = i*N + j+1024;		
	}
    status = clInitialKernelAPI();

	status = clMallocBuf((void**)(&g_data),N*N*sizeof(double));
	status = clMallocBuf((void**)(&g_result),N*N*2*sizeof(double));
	status = clMallocBuf((void**)(&g_Iresult),N*N*sizeof(double));
	status = clMallocBuf((void**)(&g_temp),N*N*2*sizeof(double));
	status = clMemcpyBuf((void**)(&c_data),(void**)(&g_data),N*N*sizeof(double), clMemcpyHostToDevice);
	//GPU 1D FFT 
	time_stamp(0, NULL);
	for(int i=0; i<1000; i++)
	status = FFT_1D_OCL(g_data, g_result, log2(N),1, NULL, 0, NULL, NULL);
	clFinish(GetOclCommandQueue0());
	status = clMemcpyBuf((void**)(&g_result),(void**)(&result),N*2*sizeof(double), clMemcpyDeviceToHost);
	time_stamp(1,"GPU time: ");
	printf(" \n GPU 1D result: \n ");
	for(int i=0; i<MIN(8, N); i++)
	{
		printf("%f  %f \n ", result[i*2], result[i*2+1]);		
	}
	//GPU 1D IFFT
	time_stamp(0, NULL);
	for(int i=0; i<1000; i++)
	status = FFT_1D_OCL(g_result, g_Iresult, log2(N),-1, NULL, 0, NULL, NULL);
	clFinish(GetOclCommandQueue0());
	status = clMemcpyBuf((void**)(&g_Iresult),(void**)(&Iresult),N*sizeof(double), clMemcpyDeviceToHost);
	time_stamp(1,"GPU time: ");
	printf(" \n GPU IFFT 1D Iresult: \n ");
	for(int i=0; i<MIN(8, N); i++)
	{
		printf("%f  %f \n ", Iresult[i]);		
	}

	//GPU 2D FFT
	time_stamp(0, NULL);
	for(int i=0; i<20; i++)
	status = FFT_2D_OCL(g_data, g_result,g_temp, log2(N),log2(N),1 ,NULL, 0, NULL, NULL);
	clFinish(GetOclCommandQueue0());
	time_stamp(1,"\nGPU 2D time: ");
	status = clMemcpyBuf((void**)(&g_result),(void**)(&result),N*N*2*sizeof(double), clMemcpyDeviceToHost);
	printf("  GPU 2D result: \n ");
	for(int i=0; i<MIN(8, N); i++)
	{
		printf("%f, %f, %f, %f\n ", result[i*2], result[i*2+1], result[i*2+N*2], result[i*2+1+N*2], result[i*2+8*N*2], result[i*2+1+8*N*2]);		
	}

	//GPU 2D IFFT
	time_stamp(0, NULL);
	for(int i=0; i<20; i++)
	status = FFT_2D_OCL(g_result,g_Iresult, g_temp, log2(N),log2(N),-1 ,NULL, 0, NULL, NULL);
	clFinish(GetOclCommandQueue0());
	time_stamp(1,"\nGPU 2D IFFT time: ");
	status = clMemcpyBuf((void**)(&g_Iresult),(void**)(&Iresult),N*N*sizeof(double), clMemcpyDeviceToHost);
	printf("  GPU 2D IFFT result: \n ");
	for(int i=0; i<MIN(8, N); i++)
	{
		printf("%f, %f\n ", Iresult[i], Iresult[i+N]);		
	}
	clReleaseKernelAPI();

	// CPU 1D FFT
	time_stamp(0, NULL);
	for(int i=0; i<1000; i++)
	status = FFT_R2C_1D(c_data, dst1,log2(N));
	time_stamp(1,"\nCPU time: ");
	printf(" \n CPU result: \n ");
    for(int i=0; i<MIN(8, N); i++)
      	printf("%f, %f\n", dst1[i].real, dst1[i].imaginary);

	// CPU 1D IFFT
	time_stamp(0, NULL);
	for(int i=0; i<1000; i++)
	status = FFT_C2R_1D(dst1,result, log2(N));
	time_stamp(1,"\nCPU time: ");
	printf(" \n CPU IFFT 1D result: \n ");
    for(int i=0; i<MIN(8, N); i++)
      	printf("%f, %f\n", result[i]);

	// CPU 2D
	time_stamp(0, NULL);	
	status = FFT_2D(c_data, dst1,log2(N), log2(N));
	time_stamp(1,"\nCPU 2D time: ");

	printf(" \n CPU 2D result: \n ");
    for(int i=0; i<MIN(8, N); i++)
      	printf("%f, %f, %f, %f\n", dst1[i].real, dst1[i].imaginary, dst1[i+N].real, dst1[i+N].imaginary);

	//cpu 2d IFFT
	time_stamp(0, NULL);	
	status = IFFT_2D(dst1,Iresult, log2(N), log2(N));
	time_stamp(1,"\nCPU 2D time: ");
	printf(" \n CPU 2D IFFT result: \n ");
    for(int i=0; i<MIN(8, N); i++)
      	printf("%f, %f\n ", Iresult[i], Iresult[i+N]);	


    return 0;
}
#else 
//test map memory
int main()
{
	int status  = -1;
	const int C= 10;	
	const int N = 1024*1024;
	int *c_data[2];
	int *result[2];
	cl_command_queue cq[2];
	cl_event ev[C]={0};
	cl_event ev2[C]={0};
	cl_event ev3[C]={0};
	int index=0;
	int verify = 0; 
	cl_event ev_temp[2]={0};

	for(int i=0; i<2; i++)
	{
		c_data[i] = (int *)malloc(N*sizeof(int));
		result[i] = (int *)malloc(N*sizeof(int));
	}
	cl_mem g_data[2] ;
	cl_mem g_result[2] ;
	cl_mem g_temp[2];

    status = clInitialKernelAPI();
	cq[0] = GetOclCommandQueue0();
	cq[1] = GetOclCommandQueue1();
	for(int i=0; i<2; i++)
	{
		status = clMallocBuf((void**)(&g_data[i]),N*sizeof(int));
		status = clMallocBuf((void**)(&g_result[i]),N*sizeof(int));
		status = clMallocBuf((void**)(&g_temp[i]),N*sizeof(int));	
	}
	for(int i=0; i<N; i++)
	{
		c_data[0][i] = 1;	
	}
	status = clMemcpyBuf((void**)(&c_data[0]),(void**)(&g_data[0]),N*sizeof(int), clMemcpyHostToDevice);
	time_stamp(0, NULL);
	for(int k=0; k<C; k++)
	{
		//initial
		for(int i=0; i<N; i++)
		{
			c_data[0][i] = k+1;	
		}
		status = clMemcpyBuf((void**)(&c_data[0]),(void**)(&g_data[0]),N*sizeof(int), clMemcpyHostToDevice);
		status = Test_OCL(g_data[0], g_result[0], N, NULL, 0, NULL ,NULL);
		status = clMemcpyBuf((void**)(&g_result[0]),(void**)(&result[0]),N*sizeof(int), clMemcpyDeviceToHost);
		if(verify)
		{
			for(int i=0; i<N; i++)
			{
				if(c_data[0][i] != result[0][i])
				{
					printf("result : error  in one command \n");
					break;
				}
			}
		}
		printf("%d \n",result[0][0]);
	}
	clFinish(cq[0]);
	time_stamp(1,"\nGPU use one command time: ");

	time_stamp(0, NULL);
	for(int i=0; i<N; i++)
	{
		c_data[index][i] = 1+C;	
	}
	status = clMemcpyBuf((void**)(&c_data[index]),(void**)(&g_data[index]),N*sizeof(int), clMemcpyHostToDevice, 0, 0, 0, cq[1], 0, 0, &(ev[0]));
	//status = clEnqueueWriteBuffer(cq[1], g_data[index], 1, 0, N*sizeof(int),c_data[index] , 0, 0,&(ev[0]));
	status = Test_OCL(g_data[index], g_result[index], N, NULL, 1, &(ev[0]), &(ev2[0]));
		
	for(int k=1; k<C; k++)
	{
		
		index = (index+1)%2;		
		for(int i=0; i<N; i++)
		{
			c_data[index][i] = k+1+C;	
		}
		status = clMemcpyBuf((void**)(&c_data[index]),(void**)(&g_data[index]),N*sizeof(int), clMemcpyHostToDevice, 0, 0, CL_FALSE, cq[1], 0, 0, &(ev[k]));
		//clEnqueueWriteBuffer(cq[1], g_data[index], 1, 0, N*sizeof(int),c_data[index] , 0, 0, &(ev[k]));
		int t=(index+1)%2;
		status = clMemcpyBuf((void**)(&g_result[t]),(void**)(&result[t]),N*sizeof(int), clMemcpyDeviceToHost, 0, 0, CL_FALSE, cq[1], 1, &(ev2[k-1]), &(ev3[k-1]));
		//status = clEnqueueReadBuffer(cq[1], g_result[t], 1, 0, N*sizeof(int), result[t],1, &(ev2[k-1]), &(ev3[k-1]));
		if(verify)
		{
			for(int i=0; i<N; i++)
			{
				if(c_data[t][i] != result[t][i])
				{
					printf("result : error  in two command \n");
					break;
				}
			}
		}
		printf("%d \n",result[t][N-1]);	
		ev_temp[0] =  ev[k]; ev_temp[1] = ev3[k-1];
		status = Test_OCL(g_data[index], g_result[index], N, NULL, 2, ev_temp, &(ev2[k]));		

	}
	status = clMemcpyBuf((void**)(&g_result[index]),(void**)(&result[index]),N*sizeof(int), clMemcpyDeviceToHost, 0, 0, CL_TRUE, cq[1]);
	//status = clEnqueueReadBuffer(cq[1], g_result[index], 1, 0, N*sizeof(int), result[index],0, NULL, NULL/*1, &(ev2[k-1]), &(ev3[k-1])*/);

	clFinish(cq[0]);
	clFinish(cq[1]);
	time_stamp(1,"\nGPU use two command time: ");
	
	clReleaseKernelAPI();

	return 0;
}
#endif
