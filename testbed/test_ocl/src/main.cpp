
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

	//FILE *fp = fopen("f:/cos.txt", "w");
	//for(int i=0; i<1024; i++)
	//{
	//	if ((i!=0)&&(i%16 ==0))
	//		fprintf(fp, "\n");
	//		fprintf(fp,"%.6f, %.6f, ", cos(3.1415927*i/1024), -sin(3.1415927*i/1024));
	//}
	//return 1;
	int status  = -1;
	
	const int N = 1024;
	double *c_data = (double *)malloc(N*sizeof(double)*N);
	double *result = (double *)malloc(N*2*sizeof(double)*N);
	double *Iresult = (double *)malloc(N*sizeof(double)*N);
	complex_t *dst1= (complex_t *)malloc(N*sizeof(complex_t)*N);;

	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		     c_data[i*N + j] = j;		
	}
    status = clInitialKernelAPI();

	cl_mem g_data = NULL;
	cl_mem g_result = NULL;
	cl_mem g_temp = NULL;
	
	time_stamp(0, NULL);
	status = clMallocBuf((void**)(&g_data),N*N*sizeof(double));
	status = clMallocBuf((void**)(&g_result),N*N*2*sizeof(double));
	status = clMallocBuf((void**)(&g_temp),N*N*2*sizeof(double));

	status = clMemcpyBuf((void**)(&c_data),(void**)(&g_data),N*N*sizeof(double), clMemcpyHostToDevice);

	for(int i=0; i<1000; i++)
	status = FFT_1D_OCL(g_data, g_result, log2(N),1,NULL);
	clFinish(GetOclCommandQueue());
	status = clMemcpyBuf((void**)(&g_result),(void**)(&result),N*2*sizeof(double), clMemcpyDeviceToHost);
	time_stamp(1,"GPU time: ");
	printf(" \n GPU result: \n ");
	for(int i=0; i<8; i++)
	{
		printf("%f  %f \n ", result[i*2], result[i*2+1]);		
	}


	time_stamp(0, NULL);
	for(int i=0; i<20; i++)
	status = FFT_2D_OCL(g_data, g_result,g_temp, log2(N),log2(N),1 ,NULL);
	clFinish(GetOclCommandQueue());
	time_stamp(1,"\nGPU 2D time: ");
	status = clMemcpyBuf((void**)(&g_result),(void**)(&result),N*N*2*sizeof(double), clMemcpyDeviceToHost);
	
	printf("  GPU 2D result: \n ");
	for(int i=0; i<16; i++)
	{
		printf("%f, %f, %f, %f, %f, %f\n ", result[i*2], result[i*2+1], result[i*2+N*2], result[i*2+1+N*2], result[i*2+8*N*2], result[i*2+1+8*N*2]);		
	}
	FILE * file=fopen("F:/log.txt", "w");
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
			fprintf(file,"%f %f,", result[i*2*N + j*2] ,result[i*2*N + j*2+1]);		
		fprintf(file, "\n");
	}

	time_stamp(0, NULL);
	for(int i=0; i<20; i++)
	status = FFT_2D_OCL(g_result,g_data, g_temp, log2(N),log2(N),-1 ,NULL);
	clFinish(GetOclCommandQueue());
	time_stamp(1,"\nGPU 2D IFFT time: ");
	status = clMemcpyBuf((void**)(&g_data),(void**)(&Iresult),N*N*sizeof(double), clMemcpyDeviceToHost);
	
	printf("  GPU 2D IFFT result: \n ");
	for(int i=0; i<16; i++)
	{
		printf("%f, %f, %f, %f, %f, %f\n ", Iresult[i], Iresult[i+N], Iresult[i+8*N]);		
	}
	FILE * file1=fopen("F:/Ilog.txt", "w");
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
			fprintf(file1,"%f ,",Iresult[i*N + j] );		
		fprintf(file1, "\n");
	}
	clReleaseKernelAPI();

	
	time_stamp(0, NULL);
	for(int i=0; i<1000; i++)
	FFT_R2C_1D(c_data, dst1,log2(N));
	time_stamp(1,"\nCPU time: ");
	printf(" \n CPU result: \n ");
    for(int i=0; i<8; i++)
      	printf("%f, %f\n", dst1[i].real, dst1[i].imaginary);

	time_stamp(0, NULL);	
	FFT_2D(c_data, dst1,log2(N), log2(N));
	time_stamp(1,"\nCPU 2D time: ");

	printf(" \n CPU 2D result: \n ");
    for(int i=0; i<16; i++)
      	printf("%f, %f, %f, %f, %f, %f\n", dst1[i].real, dst1[i].imaginary, dst1[i+N].real, dst1[i+N].imaginary, dst1[i+8*N].real, dst1[i+8*N].imaginary);


    return 0;
}
#endif
