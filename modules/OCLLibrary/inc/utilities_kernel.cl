
#pragma OPENCL EXTENSION  cl_khr_local_int32_base_atomics : enable
#pragma OPENCL EXTENSION  cl_khr_local_int32_extended_atomics : enable
#pragma OPENCL EXTENSION  cl_khr_global_int32_base_atomics : enable


__kernel  void HistogramStatistics_kernel(__global uchar *srcData,
								          __global int  *histData,
								          int dataSize,
								          __local int* lMem)
{
	uint threadIdXInBlock  = get_local_id(0);
	uint threadIdXInGlobal = get_global_id(0) ;

	lMem[threadIdXInBlock] = 0;
	barrier(CLK_LOCAL_MEM_FENCE);

	if(threadIdXInGlobal < dataSize)
	{
		atom_add(lMem + srcData[threadIdXInGlobal], 1);
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	atom_add(histData + threadIdXInBlock,lMem[threadIdXInBlock]);
}