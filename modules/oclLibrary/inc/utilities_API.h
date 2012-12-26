#ifndef _UTILITIES_API_H
#define _UTILITIES_API_H

#include "OpenCLCore.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORTS  int  ZeroMemory_OCL(cl_mem data, int dataSize, 
	                             cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev);

DLL_EXPORTS  int  HistogramStatistics_OCL(cl_mem srcData, cl_mem  histData, int dataSize, 
	                                      cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev);

DLL_EXPORTS  int  Reduction_Min_OCL(cl_mem g_idata, cl_mem g_temp, cl_mem g_odata, int dataSize,
                                    cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev);

DLL_EXPORTS  int  ScanEexclusive_OCL(cl_mem g_idata, cl_mem  g_odata, int arrayNum,
	                                 cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev);

DLL_EXPORTS  int  Transpose_OCL(cl_mem g_idata, cl_mem  g_odata, int width, int height,
	                            cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev);

DLL_EXPORTS  int  Test_OCL(cl_mem src, cl_mem dst, int length,
	                       cl_command_queue  cq, int ev_num, cl_event *ev_wait, cl_event *ev);

#ifdef __cplusplus
}
#endif



#endif