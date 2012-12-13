#include "queue_t.hpp"
#include "MemManager.h"

template<typename T>
Queue_t<T>::Queue_t(int size)
{
    _maxSize = size;
    _head = _tail = 0;
    _vArray = NULL;
}

template<typename T>
Queue_t<T>::~Queue_t()
{
    if(NULL != _vArray)
        MM_MemFree(NULL, (void**)&_vArray );
}

template<typename T>
int Queue_t<T>::init ()
{
    _vArray = (T*)MM_MemAlloc(NULL, _maxSize*sizeof(T));
    if(NULL == _vArray)
        return -1;
    return 0;
}

template<typename T>
int Queue_t<T>:: isEmpty()
{
    return (_head == _tail)? 1 : 0;
}

template<typename T>
int Queue_t<T>:: reset(int size)
{
    _head = _tail = 0;
    if(size > 0)
    {
        if(NULL != _vArray)
            MM_MemFree(NULL, (void**)&_vArray );
        _vArray = (T*)MM_MemAlloc(NULL, _maxSize*sizeof(T));
        if(NULL == _vArray)
            return -1;
        _maxSize = size;        
    }    
    return 0;
}

template<typename T>
int Queue_t<T>:: push(T val)
{
    if ((_tail-_head) >= _maxSize)
        return -1;
    _vArray[(_tail++)%_maxSize] = val;
    return 0;
}

template<typename T>
int Queue_t<T>:: pop(T& val)
{
    if(_tail==_head)
        return -1;
    val = _vArray[(_head++)%_maxSize];
    return 0;
}

/*explict Instantiation*/
template class DLL_EXPORTS Queue_t<int>;

template class DLL_EXPORTS Queue_t<float>;

template class DLL_EXPORTS Queue_t<char>;

template class DLL_EXPORTS Queue_t<unsigned char>;
