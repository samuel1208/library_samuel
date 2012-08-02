#ifndef _UTILITIES_API_H
#define _UTILITIES_API_H

#include "kernelInterface.h"
#ifdef __cplusplus
extern "C" {
#endif

//Statistic the histogram, 1.only uchar type data  2.histData must be set to 0
DLL_EXPORTS  int  HistogramStatistics_OCL(cl_mem srcData, cl_mem  histData, int dataSize,
	                                      cl_command_queue  cq ,cl_kernel kernel, cl_event *ev);



#ifdef __cplusplus
}
#endif



#endif