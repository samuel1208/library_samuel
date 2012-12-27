#include "multiThread.h"

#ifdef WIN32
#include <process.h>
#include <windows.h>
typedef  unsigned (__stdcall *ThreadFunc)(MVoid *);

DLL_EXPORTS  MHandle  MThreadCreate(MVoid *pFunc,  MVoid *pPara)
{
   	if(NULL == pFunc)
		return NULL;	
    return (MHandle)_beginthreadex(NULL, 0, (ThreadFunc)pFunc, pPara, 0, NULL);
}

DLL_EXPORTS  int  MThreadDestory(MHandle hThread)
{
	if( 0 == CloseHandle(hThread))
        return -1;
    return 0;
}

DLL_EXPORTS  int  MWaitForSingleThread(MHandle hThread, ThreadRes *pResult, MUInt32 dwTimeOut)
{
    if (WAIT_OBJECT_0 != WaitForSingleObject(hThread, dwTimeOut))
		return -1;
	if(NULL != pResult)
	{
		if (0 == GetExitCodeThread(hThread, pResult))
			return -1;
	}
	return 0;
 
}


#elif defined UNIX
#include <pthread.h>

typedef  MVoid* (*ThreadFunc)(MVoid *);

DLL_EXPORTS  MHandle  MThreadCreate(MVoid *pFunc,  MVoid *pPara)
{
    pthread_t pthread ;
    int res;
	if(NULL == pFunc)
		return NULL;	
    
    res = pthread_create(&pthread,NULL, (ThreadFunc)pFunc, pPara);
	return 0==res ? (MVoid*)pthread : NULL ;
}

DLL_EXPORTS  int  MThreadDestory(MHandle hThread)
{
	return 0;
}

DLL_EXPORTS  int  MWaitForSingleThread(MHandle hThread, ThreadRes *pResult, MUInt32 dwTimeOut)
{
	pthread_t pthread = (pthread_t)hThread;
	if(NULL == hThread) 
        return -1;
	return pthread_join(pthread, pResult);
    // 0 is the right return value
}

#endif
