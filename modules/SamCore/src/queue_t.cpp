#include "queue_t.hpp"
#include "MemManager.h"

/*
             specialization :: int
*/
Queue_t<int>::Queue_t(int size)
{
    _maxSize = size;
    _head = _tail = 0;
    _vArray = NULL;
}

Queue_t<int>::~Queue_t()
{
    if(NULL != _vArray)
        MM_MemFree(NULL, (void**)&_vArray );
}

int Queue_t<int>::init ()
{
    _vArray = (int*)MM_MemAlloc(NULL, _maxSize*sizeof(int));
    if(NULL == _vArray)
        return -1;
    return 0;
}

int Queue_t<int>:: isEmpty()
{
    return (_head == _tail)? 1 : 0;
}

int Queue_t<int>:: reset(int size)
{
    _head = _tail = 0;
    if(size > 0)
    {
        if(NULL != _vArray)
            MM_MemFree(NULL, (void**)&_vArray );
        _vArray = (int*)MM_MemAlloc(NULL, _maxSize*sizeof(int));
        if(NULL == _vArray)
            return -1;
        _maxSize = size;        
    }    
    return 0;
}

int Queue_t<int>:: push(int val)
{
    if ((_tail-_head) >= _maxSize)
        return -1;
    _vArray[(_tail++)%_maxSize] = val;
    return 0;
}

int Queue_t<int>:: pop(int& val)
{
    if(_tail==_head)
        return -1;
    val = _vArray[(_head++)%_maxSize];
    return 0;
}

