#include "bow.hpp"

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include "cmdParaser.h"
#include "imageDataSet.hpp"

using namespace std;
using namespace sam_bow;

static void usage()
{
    cout << "generate-csv-data  --  Convert feature description files into a CSV file,\n"
         << "where the first column is the class label (now only support two-class problem,\n"
         << "1 for positive and -1 for negtive. And the oters columns are features value.\n"
         << "\n"
         << "SYNOPSIS\n"
         << "  generate-csv-data [-i|--inputPath <dir>] [-o|--outputPath <dir>] -c|--class <class-name>\n"
         << "  -c|--class    The __class-name__ specify the positive class name.\n"
         << endl;
}



int main(int argc, char* argv[])
{
    char *outputPath = NULL;
    char *inputFileList = NULL ;
    char *className = NULL ;
    
    char *cmdPara=NULL;
    while(NULL != (cmdPara=getCmdPara(argc, argv)))
    {   
        if ((0==strcmp(cmdPara, "-c")) || (0==strcmp(cmdPara, "--className")))
            className = getCmdParaValue();        
        else if ((0==strcmp(cmdPara, "-i")) || (0==strcmp(cmdPara, "--inputFileList")))
            inputFileList  = getCmdParaValue();
        else if ((0==strcmp(cmdPara, "-o")) || (0==strcmp(cmdPara, "--outputPath")))
            outputPath = getCmdParaValue();
    
        else if ((0==strcmp(cmdPara, "-h")) || (0==strcmp(cmdPara, "--help")))
        {
            usage();
            return 1;
        }
        else
        { 
            cerr<<"Please input the write parameters"<<endl;
            return 1;
        }
    }
    if ((NULL==outputPath) || (NULL==inputFileList))
    {
        cerr<<"please specfy the input and output path"<<endl;
        return 1;
    }

    ifstream fileList(inputFileList, ifstream::in);
    ofstream output(outputPath, ofstream::out);
    if(!fileList.is_open() ||  !output.is_open())
    {
        cerr<<"Can't open the fileList file or output file\n";
        return 1;
    }
    while(!fileList.eof())
    {
        string fileName;
        fileList>>fileName;
        if(0 != convertToCSV(fileName, className, output))
        {
            cout<<"WARNING: can't open file "<<fileName.c_str()<<endl;
            continue;
        }
    }
    fileList.close();
    output.close();

	return 0;
}
