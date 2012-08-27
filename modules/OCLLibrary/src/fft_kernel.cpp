
#include "fft_kernel.h"
#include "kernelType.h"

enum FFT_TYPE
{
	FFT_R2C = 0,
	FFT_C2C = 1,
	FFT_C2R = 2,
};

DLL_EXPORTS  int  FFT_1D_OCL(cl_mem src,  cl_mem dst, int N, int sign, cl_event *ev)
{
	cl_int  status = -1;
	size_t  threadsInBlock[2];
	size_t  total_threads[2];
	cl_kernel kernel = NULL;
	cl_command_queue  cq  = GetOclCommandQueue();

	int num=1, i=0;
	for (i=0;i<N;i++) 
        num <<= 1;
		
	if(1 == sign)
		kernel = kernelAPI.g_FFT_1D_R2C_kernel;
	else if ( -1 == sign)
		kernel = kernelAPI.g_FFT_1D_C2R_kernel;
	else 
		return status;


	// block size
	threadsInBlock[0] = num;
	total_threads[0] = num;

	//set kernel argument
	status = clSetKernelArg( kernel , 0 , sizeof(cl_mem)     , &src);
	status = clSetKernelArg( kernel , 1 , sizeof(cl_mem)     , &dst);
	status = clSetKernelArg( kernel , 2 , sizeof(int)        , &N);
	status = clSetKernelArg( kernel , 3 , sizeof(int)        , &num);
	status = clSetKernelArg( kernel , 4 , sizeof(double)*num*2 , NULL);

	if( CL_SUCCESS!= status )
		return status;

	status= clEnqueueNDRangeKernel( cq , kernel , 1 , NULL , total_threads , threadsInBlock , 0 , NULL , ev );

	return status;
}

/*
    X direction operate 1D FFT
*/
static  int  FFT_2D_X_kernel(cl_mem src,  cl_mem dst, int N, int num_x, int num_y, FFT_TYPE fft_type, int sign, cl_event *ev)
{
	cl_int  status = -1;
	size_t  threadsInBlock[2];
	size_t  total_threads[2];
	cl_kernel kernel = NULL;



	cl_command_queue  cq  = GetOclCommandQueue();
	switch(fft_type )
	{
	case FFT_R2C :
		kernel = kernelAPI.g_FFT_2D_R2C_kernel;
		break;
	case FFT_C2C:
		kernel = kernelAPI.g_FFT_2D_C2C_kernel;
		break;
	case FFT_C2R:
		kernel = kernelAPI.g_FFT_2D_C2R_kernel;
		break;
	default:
		status = -1;
		return status;
	}

	// block size
	threadsInBlock[0] = num_x;
	total_threads[0]  = num_y*threadsInBlock[0];

	//set kernel argument
	status = clSetKernelArg( kernel , 0 , sizeof(cl_mem)     , &src);
	status = clSetKernelArg( kernel , 1 , sizeof(cl_mem)     , &dst);
	status = clSetKernelArg( kernel , 2 , sizeof(int)        , &N);
	status = clSetKernelArg( kernel , 3 , sizeof(int)        , &num_x);
	status = clSetKernelArg( kernel , 4 , sizeof(int)        , &num_y);
	if(FFT_C2C == fft_type)
	{
		status = clSetKernelArg( kernel , 5 , sizeof(int)        , &sign);
		status = clSetKernelArg( kernel , 6 , sizeof(double)*num_x*2 , NULL);
	}
	else
		status = clSetKernelArg( kernel , 5 , sizeof(double)*num_x*2 , NULL);

	if( CL_SUCCESS!= status )
		return status;

	status= clEnqueueNDRangeKernel( cq , kernel , 1 , NULL , total_threads , threadsInBlock , 0 , NULL , ev );

	return status;
}

DLL_EXPORTS  int  FFT_2D_OCL(cl_mem src,  cl_mem dst, cl_mem temp, int N_x, int N_y, int sign, cl_event *ev)
{
	cl_int  status = -1;

	int num_x=1, num_y=1, i=0;
	for (i=0;i<N_x;i++) 
        num_x <<= 1;
	for (i=0;i<N_y;i++) 
        num_y <<= 1;

	//do transpose in the kernel, the global writing is not merge,  
	if(1 == sign)
	{
		status = FFT_2D_X_kernel(src, temp, N_x, num_x, num_y, FFT_R2C, 1 , NULL);
		status = FFT_2D_X_kernel(temp, dst, N_y, num_y, num_x, FFT_C2C, 1 , NULL);
	}
	else if(-1 == sign)
	{
		status = FFT_2D_X_kernel(src, temp, N_x, num_x, num_y, FFT_C2C, -1 , NULL);
		status = FFT_2D_X_kernel(temp, dst, N_y, num_y, num_x, FFT_C2R, -1 , NULL);		
	}
	return status;
}