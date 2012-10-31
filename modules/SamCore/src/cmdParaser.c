#include"cmdParaser.h"


char  *cmdParaValue = NULL;
int   cmdIndex = 1;

DLL_EXPORTS char* getCmdPara(int argc,char **argv)
{
    char *cmd = NULL;
    for (; cmdIndex<argc; cmdIndex++)
    {
        if ('-' == argv[cmdIndex][0])
        {
            cmd = argv[cmdIndex++];
            if((cmdIndex>=argc)||('-'==argv[cmdIndex][0]))
                cmdParaValue = NULL;
            else 
                cmdParaValue  = argv[cmdIndex];              
            break;
        }
    }
    return cmd;
}
  
