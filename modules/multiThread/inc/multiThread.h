#ifndef _MULTI_THREAD_H_
#define _MULTI_THREAD_H_
#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MWAIT_INFINITE		(~0)

#ifdef WIN32
    typedef unsigned ThreadRes;   //return type of the thread function
#elif defined UNIX
    typedef MVoid*   ThreadRes;   //return type of the thread function
#endif 

DLL_EXPORTS  MHandle  MThreadCreate(MVoid *pFunc,  MVoid *pPara);
DLL_EXPORTS  int      MThreadDestory(MHandle hThread);
DLL_EXPORTS  int	  MWaitForSingleThread(MHandle hThread, ThreadRes *pResult, MUInt32 dwTimeOut);


#ifdef __cplusplus
}
#endif



#endif
