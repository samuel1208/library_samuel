#include "bow.hpp"
#include "cmdParaser.h"
#include <iostream>
#include <fstream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <Time_t.h>

using namespace std;
using namespace cv;
using namespace sam_bow;

// static int calcMaxDesCount(BowParam &params, int descSize)
// {
//     const int elemSize = 4;
//     const int descByteSize = descSize * elemSize;
//     const int bytesInMB = 1048576;
//     return (params.memoryAvailable * bytesInMB) / descByteSize; // Total number of descs to use for training.
// }

Ptr<Mat> build_vocabularies(
    BowParam &params,
    string &fileList,
    string &outputPath)
{

    unsigned int detectorType = params.detectorType;
    unsigned int detectorAdapterType = params.dtcAdapterType;
    int wordCount = params.maxVisualWordNum;

    Ptr<Mat> vocabulary = new Mat();

    if (detectorType >= params.countOfDetectors())
        detectorType %= params.countOfDetectors();
    if (detectorAdapterType >= params.countOfDtcAdapters())
        detectorAdapterType %= params.countOfDtcAdapters();

    __SAM_BEGIN__;

    Ptr<FeatureDetector> detector;
    if (1) {
        string detectorName(params.dtcAdapterNames()[detectorAdapterType]);
        detectorName += params.detectorNames()[detectorType];

        detector = FeatureDetector::create(detectorName);
    }
    else {
        detector = createDetector(params.detectorNames()[detectorType],
                                  params.dtcAdapterNames()[detectorAdapterType]);
    }

    RNG& rng = theRNG();
    TermCriteria terminate_criterion;
    terminate_criterion.epsilon = FLT_EPSILON;
    int attempts = 3;
    int flag = KMEANS_PP_CENTERS;
    BOWKMeansTrainer bowTrainer(wordCount, terminate_criterion, attempts, flag);

    int feature_num = 0;
    int maxDescriptionCount = 500000;
    int count = 0;
    Mat descriptions;
    Ptr<Mat> allDescriptions;
    ifstream ifile(fileList.c_str(), ifstream::in);
    if(!ifile.is_open())
        return NULL;

    // get the feature dimention
    {
        string file , className;
        ifile>>file;
        if(0 != readDescriptions(file.c_str(), descriptions, className))
            return NULL;
        feature_num = descriptions.cols;
        allDescriptions = new Mat(maxDescriptionCount,feature_num, CV_32FC1);  
       
        for (int j = 0; j < descriptions.rows; j++)
        {        
            if(count >= maxDescriptionCount )
                break;
            float *p = descriptions.ptr<float>(j);
            float *q = allDescriptions->ptr<float>(count);
            for (int k = 0; k < feature_num; k++)
                *q++ = *p++;
            ++count;
        }
    }
    
    
    while(!ifile.eof())
    {
        string file , className;
        ifile>>file;
        if (count >= maxDescriptionCount )
                break;
        if(0 != readDescriptions(file.c_str(), descriptions, className))
            continue;
        for (int j = 0; j < descriptions.rows; j++)
        {        

            if (count >= maxDescriptionCount )
                break;
            float *p = descriptions.ptr<float>(j);
            float *q = allDescriptions->ptr<float>(count);

             for (int k = 0; k < feature_num; k++)
                *q++ = *p++;
            ++count;
        }
    }
    cout<<"cluster "<<count<<" features : start"<<endl;
    
    bowTrainer.add(*allDescriptions);
         
    *vocabulary = bowTrainer.cluster();

    writeVocabulary(outputPath, *vocabulary);
 
    __SAM_END__;

    return vocabulary;
}


////////////////////////////////////////////////////////////////////////////////
/////////////////////////         Test            //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <string>

void usage()
{
    cout << "buildVocabulary - \n"
         << " buildVocabulary [-t <detector>] [-T <detector adapter]  [-n <visual-word-number>] [-i inputFilePath]  [-o <result-dir>]"
         << " DESCRIPTION\n"
         << "  -t:  detector type\n"
         << "  -T:  detector adapter type\n"
         << "  -n:  visual word number, default 1000.\n"
         << "  -o:  The base(result) directory\n"
         << "  -h:  help\n"
         << "\n"
         << "   Detectors:\n"
         << "     0:SIFT, 1:SURF, 2:ORB, 3:FAST, 4:STAR, 5:MSER, 6:GFTT, 7:HARRIS, 8:Dense, 9:SimpleBlob\n"
         << "   Detector Adapters:\n" 
         << "     0: , 1:Grid, 2:Pyramid\n"
         << "   Sample file: Please refer to src/samples_example.yml.\n"
         << endl;
}

int main (int argc, char **argv)
{
    BowParam params;
    char *samplesFileName = NULL;
    char *vocabularyFileName = NULL;
    string outputPath;
    string fileList;
///
/// Parse Parameters
///

    char *cmdPara = NULL;
    while(NULL != (cmdPara=getCmdPara(argc, argv)))
    {
        if ((0==strcmp(cmdPara, "-t")) || (0==strcmp(cmdPara, "--detectorType"))) 
            params.detectorType = atoi(getCmdParaValue()); 
        else if ((0==strcmp(cmdPara, "-T")) || (0==strcmp(cmdPara, "--dtcAdapterType"))) 
            params.dtcAdapterType = atoi(getCmdParaValue());
        else if (0==strcmp(cmdPara, "-n")) 
          params.maxVisualWordNum =  atoi(getCmdParaValue());         
        else if ((0==strcmp(cmdPara, "-i")) || (0==strcmp(cmdPara, "--inputPath")))
            fileList  = getCmdParaValue();
        else if ((0==strcmp(cmdPara, "-o")) || (0==strcmp(cmdPara, "--outputPath")))
            outputPath = getCmdParaValue();
        else if ((0==strcmp(cmdPara, "-h")) || (0==strcmp(cmdPara, "--help")))
        {
            usage();
            return 1;
        }
        else
        { 
            cerr<<"Please input the right parameters"<<endl;
            return 1;
        }
    }

    if ((NULL==outputPath.c_str()) || (NULL == fileList.c_str()))
    {
        cerr << "Error: <samples-filename> must be specified." << endl;
        return 1;
    }

///
/// MAIN
///
    time_stamp(0, NULL);

    initModule_nonfree(); // It should be called otherwise SURF and SIFT will
    
    Ptr<Mat> words = build_vocabularies(params,fileList, outputPath);
    if (words.empty()) {
        cerr << "Warning: There is none word." << endl;
        return 1;
    }

    time_stamp(1, "build vocabulary");
    cout<< "Finished \n";   
    return 0;
}
