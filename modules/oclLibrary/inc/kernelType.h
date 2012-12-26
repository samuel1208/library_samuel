#ifndef _KERNEL_TYPE_H_
#define _KERNEL_TYPE_H_

typedef struct _KERNEL_API_
{
	//for test
	cl_kernel  g_Test_kernel;
	//
	cl_kernel  g_HistogramStatistics_kernel;
	//reduction
	cl_kernel  g_Reduction_Min_kernel;
	//for scan
	cl_kernel  g_Scan_Exclusive_Kernel;
	cl_kernel  g_UniformAdd_Kernel;
	//zero memory
	cl_kernel  g_ZeroMemory_kernel1;
	cl_kernel  g_ZeroMemory_kernel2;
	cl_kernel  g_ZeroMemory_kernel4;
	//transpose
	cl_kernel  g_Transpose_kernel;

	//fft
	cl_kernel  g_FFT_1D_R2C_kernel;
	cl_kernel  g_FFT_1D_C2R_kernel;
	cl_kernel  g_FFT_2D_R2C_kernel;
	cl_kernel  g_FFT_2D_C2C_kernel;
	cl_kernel  g_FFT_2D_C2R_kernel;
}KERNEL_API;

extern KERNEL_API kernelAPI;
#endif