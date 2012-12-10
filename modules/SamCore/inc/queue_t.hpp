#ifndef  __QUEUE_T_HPP__
#define  __QUEUE_T_HPP__

#include "define.h"

template<typename T> 
class DLL_EXPORTS Queue_t  
{
 private:
	int _maxSize;
	T *_vArray;
    int _head, _tail;
 public:
    int push(T val);
    int pop(T& val);
    int isEmpty();
    int init();
    int reset(int size);
 public:
    Queue_t(int size);
    ~Queue_t();    
};

/*
             specialization :: int
*/
template<> 
class DLL_EXPORTS Queue_t<int>  
{
 private:
	int _maxSize;
	int *_vArray;
    int _head, _tail;
 public:
    int push(int val);
    int pop(int& val);
    int isEmpty();
    int init();
    int reset(int size);
 public:
    Queue_t(int size);
    ~Queue_t();    
};
#endif
