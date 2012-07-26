#ifndef MEM_MANAGEMENT_H
#define MEM_MANAGEMENT_H

#include<malloc.h>
#include<string.h>//for memcpy
#include"define.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORTS MVoid*	MM_MemAlloc(MHandle hContext, MULong uSize);
DLL_EXPORTS MVoid	MM_MemFree( MHandle hContext, MVoid** pMem);

DLL_EXPORTS MVoid*	MM_MemSet(MVoid* pMem, MInt32 value, MULong uSize);
DLL_EXPORTS MVoid*  	MM_MemCpy(const MVoid* pSrc, MVoid* pDst, MULong uSize);
DLL_EXPORTS MInt32	MM_MemCmp(MVoid* pBuf1, MVoid* pBuf2, MULong uSize);

DLL_EXPORTS MHandle  MM_MemMgrCreate(MVoid* pMem, MLong lMemSize);
DLL_EXPORTS MVoid    MM_MemMgrDestroy(MHandle* hContext);

#ifdef __cplusplus
}
#endif

#endif
