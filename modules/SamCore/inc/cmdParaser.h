#ifndef __CMD_PARASER_H__
#define __CMD_PARASER_H__

#include"define.h"

#ifdef __cplusplus
extern "C" {
#endif

DLL_EXPORTS char* getCmdPara(int argc,char **argv);
DLL_EXPORTS char* getCmdParaValue();

/*cmdIndex point to  the next cmd para*/
DLL_EXPORTS int   getCmdIndex();



#ifdef __cplusplus
}
#endif

#endif
