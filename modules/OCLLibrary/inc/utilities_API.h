#ifndef _UTILITIES_API_H
#define _UTILITIES_API_H

#include "OpenCLCore.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORTS  int  ZeroMemory_OCL(cl_mem data, int dataSize,cl_event *ev);

DLL_EXPORTS  int  HistogramStatistics_OCL(cl_mem srcData, cl_mem  histData, int dataSize, cl_event *ev);

DLL_EXPORTS  int  Reduction_Min_OCL(cl_mem g_idata, cl_mem g_temp, cl_mem g_odata, int dataSize, cl_event *ev);

DLL_EXPORTS  int  ScanEexclusive(cl_mem g_idata, cl_mem  g_odata, int arrayNum, cl_event *ev);



#ifdef __cplusplus
}
#endif



#endif