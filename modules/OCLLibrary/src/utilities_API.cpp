
#include "utilities_API.h"

DLL_EXPORTS  int  HistogramStatistics_OCL(cl_mem srcData, cl_mem  histData, int dataSize,
	                                      cl_command_queue  cq ,cl_kernel kernel, cl_event *ev)
{
	cl_int  status = CL_SUCCESS;
	size_t  threadsInBlock[2];
	size_t  total_threads[2];

	//set kernel argument
	status = clSetKernelArg( kernel , 0 , sizeof(cl_mem)     , &srcData);
	status = clSetKernelArg( kernel , 1 , sizeof(cl_mem)     , &histData);
	status = clSetKernelArg( kernel , 2 , sizeof(int)        , &dataSize);
	status = clSetKernelArg( kernel , 3 , sizeof(int)*256    , NULL);

	if( CL_SUCCESS!= status )
		return -1;

	threadsInBlock[0] = 256;
	total_threads[0] = (dataSize+threadsInBlock[0]-1)/threadsInBlock[0]*threadsInBlock[0];

	if (CL_SUCCESS != clEnqueueNDRangeKernel( cq , kernel , 1 , NULL , total_threads , threadsInBlock , 0 , NULL , ev ))
		return -1;
	return 0;
}