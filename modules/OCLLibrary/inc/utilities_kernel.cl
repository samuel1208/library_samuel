
#pragma OPENCL EXTENSION  cl_khr_local_int32_base_atomics : enable
#pragma OPENCL EXTENSION  cl_khr_local_int32_extended_atomics : enable
#pragma OPENCL EXTENSION  cl_khr_global_int32_base_atomics : enable
#pragma OPENCL EXTENSION  cl_khr_fp64 : enable
/***********************************************************************************
                              set memory value with zero 
 ***********************************************************************************/
__kernel  void  zeroMemory_kernel1(__global char *data, int dataSize)
{
	uint tId_Gx = get_global_id(0) ;
	if(tId_Gx < dataSize)
		data[tId_Gx] = 0;
}
__kernel  void  zeroMemory_kernel2(__global char2 *data, int dataSize)
{
	uint tId_Gx = get_global_id(0) ;
	if(tId_Gx < dataSize)
		data[tId_Gx] = (char2)(0,0);
}
__kernel  void  zeroMemory_kernel4(__global char4 *data, int dataSize)
{
	uint tId_Gx = get_global_id(0) ;
	if(tId_Gx < dataSize)
		data[tId_Gx] = (char4)(0,0,0,0);
}

/***********************************************************************************
                                 histgram statistics
 ***********************************************************************************/
__kernel  void  histogramStatistics_kernel(__global uchar *srcData,
								           __global int   *histData,
								           int dataSize,
								           __local int* lMem)
{
	uint tId_Bx = get_local_id(0);
	uint tId_Gx = get_global_id(0) ;

	lMem[tId_Bx] = 0;
	barrier(CLK_LOCAL_MEM_FENCE);

	if(tId_Gx < dataSize)
	{
		atom_add(lMem + srcData[tId_Gx], 1);
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	atom_add(histData + tId_Bx,lMem[tId_Bx]);
}

/***********************************************************************************
                                 reduction problem
 ***********************************************************************************/
__kernel  void  reduction_Min_kernel(__global int *g_idata,
						             __global int *g_odata,
							         int dataSize,
								     __local  int *lMem)
{
	uint tId_Bx  = get_local_id(0); 
	uint bId_x   = get_group_id(0);	
	uint blockSizeX  = get_local_size(0);
	uint tId_Gx  = get_global_id(0);

	int index = tId_Bx+blockSizeX*2*bId_x;
	if(index + blockSizeX < dataSize)
		lMem[tId_Bx] = min(g_idata[index] , g_idata[index + blockSizeX]);
	else if(index > dataSize-1)
		lMem[tId_Bx] = 255;
	else 		
		lMem[tId_Bx] = g_idata[index];

	barrier(CLK_LOCAL_MEM_FENCE);

    if (blockSizeX== 256)
	{ if(tId_Bx < 128) lMem[tId_Bx] = min( lMem[tId_Bx], lMem[tId_Bx + 128]);blockSizeX>>=1; barrier(CLK_LOCAL_MEM_FENCE);}
	if (blockSizeX== 128)
	{ if(tId_Bx < 64) lMem[tId_Bx] = min( lMem[tId_Bx], lMem[tId_Bx + 64]);blockSizeX>>=1; barrier(CLK_LOCAL_MEM_FENCE);}
	if (blockSizeX== 64)
	{ if(tId_Bx < 32) lMem[tId_Bx] = min( lMem[tId_Bx], lMem[tId_Bx + 32]);blockSizeX>>=1; barrier(CLK_LOCAL_MEM_FENCE);}
	if (blockSizeX== 32)
	{ if(tId_Bx < 16) lMem[tId_Bx] = min( lMem[tId_Bx], lMem[tId_Bx + 16]);blockSizeX>>=1; barrier(CLK_LOCAL_MEM_FENCE);}
	if (blockSizeX== 16)
	{ if(tId_Bx < 8) lMem[tId_Bx] = min( lMem[tId_Bx], lMem[tId_Bx + 8]);blockSizeX>>=1; barrier(CLK_LOCAL_MEM_FENCE);}
	if (blockSizeX== 8)
	{ if(tId_Bx < 4) lMem[tId_Bx] = min( lMem[tId_Bx], lMem[tId_Bx + 4]);blockSizeX>>=1; barrier(CLK_LOCAL_MEM_FENCE);}
	if (blockSizeX== 4)
	{ if(tId_Bx < 2) lMem[tId_Bx] = min( lMem[tId_Bx], lMem[tId_Bx + 2]);blockSizeX>>=1; barrier(CLK_LOCAL_MEM_FENCE);}
	if (blockSizeX== 2)
	{ if(tId_Bx < 1) lMem[tId_Bx] = min( lMem[tId_Bx], lMem[tId_Bx + 1]);blockSizeX>>=1; barrier(CLK_LOCAL_MEM_FENCE);}

	if (0 == tId_Bx)
		g_odata[bId_x] = lMem[0];
}

/********************************************************************************************
                                           scan 
*********************************************************************************************/
#define NUM_BANKS 16
#define LOG_NUM_BANKS 4   //'>>4' replace '/16'

//#define ZERO_BANK_CONFLICTS 
#ifdef ZERO_BANK_CONFLICTS 
#define CONFLICT_FREE_OFFSET(index) ((index)>>LOG_NUM_BANKS+(index)>>(2*LOG_NUM_BANKS))
#else
#define CONFLICT_FREE_OFFSET(index) 0// ((index)>>LOG_NUM_BANKS)
#endif

void  __scanExclusiveInBlockKernel(const __global int*  g_idata,
								   __global int*        g_odata,								  
								   int                  n,
								   int                  thid,
								   int                  blockSize,
								   __local  int*        localMem)
{
	//int thid = tId_Bx.x; 
	int ai = thid;
	int bi = thid + blockSize;
	int bankOffsetA = CONFLICT_FREE_OFFSET(ai);
	int bankOffsetB = CONFLICT_FREE_OFFSET(bi);

	 
	localMem[ai+bankOffsetA] = (ai < n) ? g_idata[ai] : 0;
	localMem[bi+bankOffsetB] = (bi < n) ? g_idata[bi] : 0;
	int offset = 1;
	if(n < blockSize*2)
	{
		int i = 1;
		n = n>>1;
		while( n > 0)
		{
			n = n>>1;
			i++;
		}
		n = 1<<i;
	}
	//up-sweep
	for(int d=n>>1; d>0; d>>=1)
	{
		barrier(CLK_LOCAL_MEM_FENCE);
		if(thid<d)
		{
			int ai = offset*(2*thid+1)-1;
			int bi = offset*(2*thid+2)-1;
			ai    += CONFLICT_FREE_OFFSET(ai);
			bi    += CONFLICT_FREE_OFFSET(bi);
			localMem[bi] += localMem[ai];
		}
		offset *=2;
	}
	// clear the right value 
	if(thid == 0)
	{
		int index = n-1;
		index += CONFLICT_FREE_OFFSET(index);
		localMem[index] = 0;
	}
	//down-sweep
	for(int d=1;d<n; d*=2)
	{
		offset >>=1;
		barrier(CLK_LOCAL_MEM_FENCE);
		if(thid < d)
		{
			int ai = offset*(2*thid+1)-1;
			int bi = offset*(2*thid+2)-1;
			ai +=  CONFLICT_FREE_OFFSET(ai);
			bi +=  CONFLICT_FREE_OFFSET(bi);
			int t = localMem[ai];
			localMem[ai] = localMem[bi];
			localMem[bi] += t;  
		}
	}
	barrier(CLK_LOCAL_MEM_FENCE);
	if(ai<n)
		g_odata[ai] = localMem[ai+bankOffsetA];
	if(bi<n)
		g_odata[bi] = localMem[bi+bankOffsetB];
}
__kernel  void  scan_Exclusive_Kernel(const __global  int*  g_idata,
							          __global int*         g_odata,
							          __global int*         g_blockSum,
							          int                   arrayNum,
							          __local  int*         localMem)
							  
{
	 //__local int g_sMem[256*2];
	 int tId_Bx = get_local_id(0);
     int bId_x  = get_group_id(0);
	 int blockSize = get_local_size(0);
	 int blockNum  = get_num_groups(0);

	 int  arrayNumInBlock = 2*blockSize;
	 if ( (blockNum-1) == bId_x)
		 arrayNumInBlock = arrayNum - 512*bId_x;

	 __scanExclusiveInBlockKernel(g_idata+bId_x*2*blockSize,
		                          g_odata+bId_x*2*blockSize,
								  arrayNumInBlock,tId_Bx,
								  blockSize,localMem);

	// if(blockNum>1/*(bId_x != blockNum-1)*/&&(tId_Bx == blockSize-1))
	 if(blockNum>1)
		 if(tId_Bx == blockSize-1)
		 {
			 int blockSumIdex = (bId_x<<1)*blockSize+arrayNumInBlock-1;
			 g_blockSum[bId_x] = g_odata[blockSumIdex] + g_idata[blockSumIdex];
		                          
		 }
}

__kernel  void  uniformAdd_Kernel(__global int2*       g_data, 
                                  const __global int*  uniforms, 
                                  uint                 arrayNum )
{
	int tId_Bx = get_local_id(0);
    int bId_x  = get_group_id(0);
	int blockSize = get_local_size(0);
	int index     = tId_Bx + bId_x*blockSize;
	//int validNum  = arrayNum
    __local int uni;

    if (0 == tId_Bx)
        uni = uniforms[bId_x];
    barrier(CLK_LOCAL_MEM_FENCE);

	//int2 value = (int2)(0,0);
	if( (index>>1) < arrayNum)
		g_data[index].x += uni;
	if( ((index>>1)+1) < arrayNum)
		g_data[index].y += uni;
	//g_data[index] = value;
}

/*********************************************************************************
                                   matrix transpose 
**********************************************************************************/

/*************************************************************************
                     matrix transpose  : only support uchar 
					 the shared memory exit bank conflict
 *************************************************************************/
//have bank conflict, need +1 int each line
__kernel void transpose_kernel(__global uchar  *idata,
							   __global uchar  *odata, 
							   int width, int height,
							   __local uchar *lMem)
{
#if 1
	uint tId_Bx  = get_local_id(0);
	uint tId_By  = get_local_id(1); 
	uint bId_x   = get_group_id(0);
	uint bId_y   = get_group_id(1);	
	uint blockSizeX   = get_local_size(0);
	uint blockSizeY   = get_local_size(1);
	uint tId_Gx = get_global_id(0);
    uint tId_Gy = get_global_id(1); 

	if ((tId_Gx<width) && (tId_Gy<height))
	{
		lMem[tId_Bx + tId_By*blockSizeX] = idata[tId_Gx +tId_Gy*width];
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	uint  xIndex = bId_y * blockSizeX + tId_Bx;
	uint  yIndex = bId_x * blockSizeX + tId_By;

	if ((xIndex<height) && (yIndex<width))
	{
		odata[yIndex*height + xIndex] = lMem[tId_By + tId_Bx*blockSizeX];
	}
#else 	
	uint tId_Gx = get_global_id(0);
    uint tId_Gy = get_global_id(1); 
	if ((tId_Gx<width) && (tId_Gy<height))
		odata[tId_Gy+tId_Gx*height] = idata[tId_Gx +tId_Gy*width];
#endif
}