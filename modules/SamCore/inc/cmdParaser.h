#ifndef __CMD_PARASER_H__
#define __CMD_PARASER_H__

#include"define.h"
extern char  *cmdParaValue;
extern int   cmdIndex ;

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORTS char* getCmdPara(int argc,char **argv);


#ifdef __cplusplus
}
#endif

#endif
