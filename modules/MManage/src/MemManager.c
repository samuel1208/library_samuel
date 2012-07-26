
#include"MemManager.h"
//================ Memory Manager Interfaces ================//

#define ALIGN_SIZE		32UL//4byte

#ifdef x64
//should align to ALIGN_SIZE
typedef struct tag_MM_MemPoolCellHeader { 
	MLong	lSize; 
	MBool	bIsUsed; 
    MUInt8  reserved[20];//long and  int is 8 bit on x64
}
#else 
typedef struct tag_MM_MemPoolCellHeader { 
	MLong	lSize; 
	MBool	bIsUsed; 
    MUInt8  reserved[24];
} 
#endif
MM_MEMPOOLCELLHEADER, *LPMM_MEMPOOLCELLHEADER; // not visiable externel



#define  T	MM_Pool_P
struct MM_Pool {
	MHandle	 exHandle;				
	void*	 buf;
	MHandle  inHandle;
};



#define MMPOOL_ISENDNODE(pNode)			((pNode)->lSize == -1)
#define MMPOOL_NODESIZE(pNode)			(MULong)((pNode)->lSize + sizeof(MM_MEMPOOLCELLHEADER)) 
#define MMPOOL_GETMEMPOINT(pNode)		(MUInt8*)((MUInt8*)(pNode) + sizeof(MM_MEMPOOLCELLHEADER))
#define MMPOOL_GETNEXTNODE(pNode)		(LPMM_MEMPOOLCELLHEADER)((MUInt8*)(pNode) + MMPOOL_NODESIZE(pNode))
#define MMPOOL_MERGE(pNode1, pNode2)	((pNode1)->lSize += MMPOOL_NODESIZE(pNode2))

//================================================================//
static MVoid*   MM_MemAllocStatic(MVoid* hContext, MULong uSize);
static MVoid    MM_MemFreeStatic(MVoid* hContext, MVoid* pMem);


DLL_EXPORTS MHandle MM_MemMgrCreate(MVoid *pMem, MLong nMemSize)
{
	MM_MEMPOOLCELLHEADER *pCellHeader = NULL , *pEndNode = NULL; 
	
	if ((NULL == pMem) || (0 == nMemSize))
		return NULL; 
	
	// create the head cell, and align to ALIGN_SIZE
	pCellHeader = (LPMM_MEMPOOLCELLHEADER)
		(((MULong)pMem + ALIGN_SIZE - 1) & (~(ALIGN_SIZE - 1))); 
	nMemSize = nMemSize - ((MULong)pCellHeader - (MULong)pMem); 
	nMemSize &= (~(ALIGN_SIZE - 1));
	nMemSize -= 2 * sizeof(MM_MEMPOOLCELLHEADER);

	if (nMemSize <= 0) 
		return NULL;		
	
	pCellHeader->lSize = nMemSize; 
	pCellHeader->bIsUsed = MFalse; 
	
	// create the end cell 
	pEndNode = (LPMM_MEMPOOLCELLHEADER)
		((MUInt8*)pCellHeader + MMPOOL_NODESIZE(pCellHeader));
	pEndNode->lSize = -1; 
	pEndNode->bIsUsed = MFalse; 
	
	return (MHandle)pCellHeader;
}
DLL_EXPORTS MVoid MM_MemMgrDestroy(MHandle* hContext)
{
	*hContext = NULL;
}
static MVoid*   MM_MemAllocStatic(MHandle hContext, MULong uSize)
{
	LPMM_MEMPOOLCELLHEADER pCellHeader = (LPMM_MEMPOOLCELLHEADER)hContext; 
	if (NULL == pCellHeader || uSize == 0) 
		return NULL; 
	
	//align to ALIGN_SIZE, so next it will also align to ALIGN_SIZE
	uSize = (uSize + ALIGN_SIZE - 1) & (~(ALIGN_SIZE - 1));
	
	while (!MMPOOL_ISENDNODE(pCellHeader))	// find one cell
	{
		if (!pCellHeader->bIsUsed && pCellHeader->lSize >= (MLong)uSize) {	// find the free space to alloc
			MLong lUnusedSize = pCellHeader->lSize - uSize - sizeof(MM_MEMPOOLCELLHEADER);
			
			if (lUnusedSize >= 32) {	// add new cell
				LPMM_MEMPOOLCELLHEADER pNewCell = (LPMM_MEMPOOLCELLHEADER)
					(MMPOOL_GETMEMPOINT(pCellHeader) + uSize);	
				pNewCell->lSize = lUnusedSize; 
				pNewCell->bIsUsed = MFalse; 
				
				pCellHeader->lSize = uSize;
			}
			pCellHeader->bIsUsed = MTrue;
			
			return MMPOOL_GETMEMPOINT(pCellHeader);
		} 
		pCellHeader = MMPOOL_GETNEXTNODE(pCellHeader);	// find next
	}
	return NULL; 
}

//the hContext must valid, and the pMem must exist in hContext
static MVoid    MM_MemFreeStatic(MHandle hContext, MVoid* pMem)
{
	LPMM_MEMPOOLCELLHEADER pCellHeader = (LPMM_MEMPOOLCELLHEADER)hContext; 
	LPMM_MEMPOOLCELLHEADER pPreHeader = NULL;
	LPMM_MEMPOOLCELLHEADER pNextHeader = NULL; 
	
	if(NULL == pCellHeader || NULL == pMem) 
		return; 
	
	// find the memory to free
	while(MMPOOL_GETMEMPOINT(pCellHeader) != pMem) {
		pPreHeader = pCellHeader; 
		pCellHeader = MMPOOL_GETNEXTNODE(pCellHeader);
		
		//if the pMem is not belong to the  MM context, throw it to 
		//windows system free  mechanism, maybe invoke crash
		if( MMPOOL_ISENDNODE(pCellHeader) )
		{
			free(pMem);
			return;
		}
	}
	
	pCellHeader->bIsUsed = MFalse;
	
	// try to merge with next cell
	pNextHeader = MMPOOL_GETNEXTNODE(pCellHeader); 
	if(!MMPOOL_ISENDNODE(pNextHeader) && !pNextHeader->bIsUsed)
		MMPOOL_MERGE(pCellHeader, pNextHeader); 

	// try to merge with pre cell 
	if(NULL != pPreHeader && !pPreHeader->bIsUsed)
		MMPOOL_MERGE(pPreHeader, pCellHeader);
}

DLL_EXPORTS  MVoid*  MM_MemAlloc(MHandle hContext, MULong uSize)
{
	if (NULL != hContext)
		return MM_MemAllocStatic(hContext, uSize);
	else
		return malloc(uSize);
}

DLL_EXPORTS  MVoid  MM_MemFree(MHandle hContext, MVoid** pMem)
{
	if (NULL != hContext)
		MM_MemFreeStatic(hContext, *pMem);
	else
		free(*pMem);
	*pMem = NULL;
}

DLL_EXPORTS  MVoid*  MM_MemSet(MVoid* pMem, MInt32 value, MULong uSize)
{
	return  memset(pMem, value, uSize);
}

DLL_EXPORTS  MVoid*  MM_MemCpy(const MVoid* pSrc, MVoid* pDst, MULong uSize)
{
	return  memcpy(pDst, pSrc, uSize);
}

DLL_EXPORTS  MInt32	MM_MemCmp(MVoid* pBuf1, MVoid* pBuf2, MULong uSize)
{
	return  memcmp(pBuf1, pBuf2, uSize);
}


