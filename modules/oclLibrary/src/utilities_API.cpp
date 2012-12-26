
#include "utilities_API.h"
#include "kernelType.h"

DLL_EXPORTS  int  ZeroMemory_OCL(cl_mem data, int dataSize, 
	                             cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev)
{
	cl_int  status = -1;
	size_t  threadsInBlock[2];
	size_t  total_threads[2];
	cl_kernel kernel = NULL;

	if(NULL == cq)
		cl_command_queue  cq  = GetOclCommandQueue0();
	if (0 == (dataSize%4))
	{
		dataSize /= 4;
		kernel = kernelAPI.g_ZeroMemory_kernel4;
	}
	else if (0 == (dataSize%2))
	{
		dataSize /= 2;
		kernel = kernelAPI.g_ZeroMemory_kernel2;
	}
	else 
		kernel = kernelAPI.g_ZeroMemory_kernel1;

	//set kernel argument
	status = clSetKernelArg( kernel , 0 , sizeof(cl_mem)     , &data);
	status = clSetKernelArg( kernel , 1 , sizeof(int)        , &dataSize);

	if( CL_SUCCESS!= status )
		return status;

	threadsInBlock[0] = 256;
	total_threads[0] = (dataSize+threadsInBlock[0]-1)/threadsInBlock[0]*threadsInBlock[0];

	status= clEnqueueNDRangeKernel(cq, kernel, 1, NULL, total_threads, threadsInBlock, ev_num, ev_wait, ev);
	return status;
}

/********************************************************************
                        Statistic the histogram
1.only uchar type data  2.histData must be set to 0(should change)
*********************************************************************/
DLL_EXPORTS  int  HistogramStatistics_OCL(cl_mem srcData, cl_mem  histData, int dataSize,
	                                      cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev)
{
	cl_int  status = -1;
	size_t  threadsInBlock[2];
	size_t  total_threads[2];

	if(NULL == cq)
		cq  = GetOclCommandQueue0();
	cl_kernel kernel = kernelAPI.g_HistogramStatistics_kernel;

	// block size
	threadsInBlock[0] = 256;
	total_threads[0] = (dataSize+threadsInBlock[0]-1)/threadsInBlock[0]*threadsInBlock[0];

	//set kernel argument
	status = clSetKernelArg( kernel , 0 , sizeof(cl_mem)     , &srcData);
	status = clSetKernelArg( kernel , 1 , sizeof(cl_mem)     , &histData);
	status = clSetKernelArg( kernel , 2 , sizeof(int)        , &dataSize);
	status = clSetKernelArg( kernel , 3 , sizeof(int)*256    , NULL);// 256 relate to the char

	if( CL_SUCCESS!= status )
		return status;

	status= clEnqueueNDRangeKernel(cq, kernel, 1, NULL, total_threads, threadsInBlock, ev_num, ev_wait, ev);
	return status;
}

/********************************************************************
                             reduction min 
blockSize=256. each block process 512 elements , data's type is int
*********************************************************************/
//
static  int  Reduction_Min_kernel(cl_mem g_idata, cl_mem g_odata, int dataSize,
	                              cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev)
{
	cl_int  status = -1;
	size_t  threadsInBlock[2];
	size_t  total_threads[2];

	if(NULL == cq)
	    cq  = GetOclCommandQueue0();
	cl_kernel kernel = kernelAPI.g_Reduction_Min_kernel;

	//block size
	threadsInBlock[0] = 256;
	total_threads[0] = (dataSize+511)/512*threadsInBlock[0];

	//set kernel argument
	status = clSetKernelArg( kernel , 0 , sizeof(cl_mem)     , &g_idata);
	status = clSetKernelArg( kernel , 1 , sizeof(cl_mem)     , &g_odata);
	status = clSetKernelArg( kernel , 2 , sizeof(int)        , &dataSize);
	status = clSetKernelArg( kernel , 3 , sizeof(int)*threadsInBlock[0]    , NULL);

	if( CL_SUCCESS!= status )
		return status;

	status= clEnqueueNDRangeKernel(cq, kernel, 1, NULL, total_threads, threadsInBlock, ev_num, ev_wait, ev);

	return status;
}

//the max data size is 512*512
DLL_EXPORTS  int  Reduction_Min_OCL(cl_mem g_idata, cl_mem g_temp, cl_mem g_odata, int dataSize,
	                                cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev)
{
	int status = -1;

	if (dataSize > 512*512)
		status = -1;
	else if (dataSize > 512)
	{
		status = Reduction_Min_kernel(g_idata, g_temp, dataSize, cq, ev_num, ev_wait, ev);
		status = Reduction_Min_kernel(g_temp, g_odata, (dataSize+511)/512, cq, ev_num, ev_wait, ev);
	}
	else 
		status = Reduction_Min_kernel(g_idata, g_odata, dataSize, cq, ev_num, ev_wait, ev);

	return status;
}

/********************************************************************
                               scan
*********************************************************************/
static int scanExclusive_Kernel(cl_mem g_idata,  cl_mem g_odata, cl_mem g_blockSum, int arrayNum,
	                            cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev)
{
	int     status = -1;
	size_t  threadsInBlock[2];
	size_t  total_threads[2];

	if(NULL == cq)
	    cq  = GetOclCommandQueue0();
	cl_kernel kernel = kernelAPI.g_Scan_Exclusive_Kernel;

	//block size	
	threadsInBlock[0] = 256;
	total_threads[0]  = ((arrayNum+threadsInBlock[0]*2-1)/(threadsInBlock[0]*2))*threadsInBlock[0];	

	unsigned int localMemSize = threadsInBlock[0]*2*sizeof(int);
	status = clSetKernelArg( kernel , 0 , sizeof(cl_mem) ,&g_idata);
	status = clSetKernelArg( kernel , 1 , sizeof(cl_mem) ,&g_odata);
	status = clSetKernelArg( kernel , 2 , sizeof(cl_mem) ,&g_blockSum);
	status = clSetKernelArg( kernel , 3 , sizeof(unsigned int) ,&arrayNum);
	status = clSetKernelArg( kernel , 4 , localMemSize ,NULL);
	if( CL_SUCCESS!= status )
		return status;

	status = clEnqueueNDRangeKernel(cq, kernel, 1, 0, total_threads, threadsInBlock, ev_num, ev_wait, ev);
	return status;
}

static  int  uniformAdd_Kernel(cl_mem  g_data, cl_mem  uniforms, int arrayNum,
	                           cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev)
{
	int     status = -1;
	size_t  threadsInBlock[2];
	size_t  total_threads[2];

	if(NULL == cq)
	    cq  = GetOclCommandQueue0();
	cl_kernel kernel = kernelAPI.g_UniformAdd_Kernel;

	//block size
	threadsInBlock[0] = 256;
	total_threads[0]  = ((arrayNum+511)/512)*threadsInBlock[0];	

	status = clSetKernelArg( kernel , 0 , sizeof(cl_mem) ,&g_data);
	status = clSetKernelArg( kernel , 1 , sizeof(cl_mem) ,&uniforms);
	status = clSetKernelArg( kernel , 2 , sizeof(unsigned int) ,&arrayNum);
    if( CL_SUCCESS!= status )
		return status;
	
	status = clEnqueueNDRangeKernel(cq, kernel, 1, 0, total_threads, threadsInBlock, ev_num, ev_wait, ev);
	return status;
}

DLL_EXPORTS  int  ScanEexclusive_OCL(cl_mem g_idata, cl_mem  g_odata, int arrayNum,
	                                 cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev)
{
	//the size must samller than the size of memory;
	int status = -1;
	unsigned int sumNum = (arrayNum+511)/512;

	cl_mem g_blockSum1 = NULL;
	cl_mem g_blockSum2 = NULL;

	status = clMallocBuf((void**)(&g_blockSum1),sumNum*sizeof(int));	
	status = clMallocBuf((void**)(&g_blockSum2),sumNum*sizeof(int));
	if(0 != status)
		return status;

	status = scanExclusive_Kernel(g_idata, g_odata, g_blockSum1, arrayNum, cq, ev_num, ev_wait, ev);
	if(0 != status)
		return status;
	if (sumNum > 1)
	{
		status = ScanEexclusive_OCL( g_blockSum1, g_blockSum2, sumNum, cq, ev_num, ev_wait, ev);
		if(0 != status)
			return status;

		status = uniformAdd_Kernel( g_odata, g_blockSum2, arrayNum, cq, ev_num, ev_wait, ev);
		if(0 != status)
			return status;	
	}
	clMemFree((void**)(&g_blockSum1));
	clMemFree((void**)(&g_blockSum2));

	status = 0;
	return status;
}
/********************************************************************
                               transpose
*********************************************************************/
DLL_EXPORTS  int  Transpose_OCL(cl_mem g_idata, cl_mem  g_odata, int width, int height,
	                            cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev)
{
	int status = -1;
	size_t  threadsInBlock[2];
	size_t  total_threads[2];

	if(NULL == cq)
	    cq  = GetOclCommandQueue0();
	cl_kernel kernel = kernelAPI.g_Transpose_kernel;

	//block size
	threadsInBlock[0] = 16;
	threadsInBlock[1] = 16;
	total_threads[0] = (width + threadsInBlock[0] - 1) / threadsInBlock[0] * threadsInBlock[0];
	total_threads[1] = (height + threadsInBlock[1] - 1) / threadsInBlock[1] * threadsInBlock[1];

	//set kernel argument
	status = clSetKernelArg( kernel , 0 , sizeof(cl_mem)     , &g_idata);
	status = clSetKernelArg( kernel , 1 , sizeof(cl_mem)     , &g_odata);
	status = clSetKernelArg( kernel , 2 , sizeof(int)        , &width);
	status = clSetKernelArg( kernel , 3 , sizeof(int)        , &height);
	status = clSetKernelArg( kernel , 4 , sizeof(char)*threadsInBlock[0]*threadsInBlock[1]   , NULL);

	if( CL_SUCCESS!= status )
		return status;	

	status= clEnqueueNDRangeKernel(cq, kernel, 2, 0, total_threads, threadsInBlock, ev_num, ev_wait, ev );

	return status;
}

DLL_EXPORTS  int  Test_OCL(cl_mem src, cl_mem dst, int length, 
	                       cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev)
{
	int status = -1;
	size_t  threadsInBlock[2];
	size_t  total_threads[2];

	if(NULL == cq)
	    cq  = GetOclCommandQueue0();
	cl_kernel kernel = kernelAPI.g_Test_kernel;

	//block size
	threadsInBlock[0] = 256;
	total_threads[0] = (length + threadsInBlock[0] - 1) / threadsInBlock[0] * threadsInBlock[0];

	//set kernel argument
	status = clSetKernelArg( kernel , 0 , sizeof(cl_mem)     , &src);
	status = clSetKernelArg( kernel , 1 , sizeof(cl_mem)     , &dst);
	status = clSetKernelArg( kernel , 2 , sizeof(int)        , &length);

	if( CL_SUCCESS!= status )
		return status;	

	status= clEnqueueNDRangeKernel(cq, kernel, 1, 0, total_threads, threadsInBlock, ev_num, ev_wait, ev );
	return status;

}