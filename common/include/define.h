#ifndef DEFINE_H
#define DEFINE_H
//
typedef  long                MLong ;  //4 for 32 bit devices , 8 for 64 bit devices, 
typedef  unsigned long       MULong ;                                       //it's useful to store the address value 
typedef  char                MInt8 ;
typedef  unsigned char       MUInt8;
typedef  short int           MInt16 ;
typedef  unsigned short int  MUInt16 ;
typedef  int                 MInt32;
typedef  unsigned int        MUInt32 ;
typedef  long long           MInt64 ;
typedef  unsigned long long  MUInt64;

typedef  int                 MBool ; 
typedef  void                MVoid ;
typedef  void*               MHandle ;

//#define MNull		0
#define MFalse		0
#define MTrue		1

//the dll macro
#ifndef  DLL_LIBRARY
    #define DLL_EXPORTS 
#else
    #ifdef WIN32
        #ifdef  DLL_API
            #define DLL_EXPORTS __declspec(dllexport)
        #else
            #define DLL_EXPORTS // not use the import optimization//#define DLL_EXPORTS __declspec(dllimport)
	    #endif
    #elif  defined UNIX	    	
        #ifdef  DLL_API
            #define DLL_EXPORTS  __attribute__((visibility("default")))
        #else
	        #define DLL_EXPORTS 
	    #endif
   #endif
#endif 



#endif
