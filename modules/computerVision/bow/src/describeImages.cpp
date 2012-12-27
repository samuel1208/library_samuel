#include "bow.hpp"
#include "cmdParaser.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#ifndef ENABLE_SHOW_IMAGE
#  include <opencv2/highgui/highgui.hpp>
#endif

#include "imageDataSet.hpp"
#include "vocDataSet.hpp"
#include "Time_t.h"
using namespace cv;
using namespace std;
using namespace sam_bow;


/*
#ifdef ENABLE_SHOW_IMAGE

static int s_delay = 0;

#  define INIT_SHOW_IMAGE() {                   \
        namedWindow("Keypoints");               \
    }

#  define SHOW_IMAGE(im, keypoints) {                                   \
        Mat toShow_;                                                    \
        drawKeypoints((im), (keypoints), toShow_);                      \
        imshow("Keypoints", toShow_);                                   \
        int c = waitKey(s_delay);                                       \
        if ((char)c == 'd') {                                           \
            if (s_delay == 0) {                                         \
                s_delay = 10;                                           \
                cout << "Delay 10 millisecond." << endl;                \
            } else {                                                    \
                s_delay = 0;                                            \
                cout << "Delay infinitly." << endl;                     \
            }                                                           \
        }                                                               \
    }

#else

#  define INIT_SHOW_IMAGE() 
#  define SHOW_IMAGE(im, keypoints)

#endif
*/

Ptr<Mat> 
describe_roi_local(
    const Mat& roi,
    const Ptr<FeatureDetector>& detector,
    const Ptr<DescriptorExtractor>& descriptor)
{
    //INIT_SHOW_IMAGE();

    Ptr<Mat> descriptions (new Mat);
    
    vector<KeyPoint> keypoints;
    detector->detect(roi, keypoints);
    
    if (!keypoints.empty()) {
        descriptor->compute(roi, keypoints, *descriptions);
        assert(!descriptions->empty());

        //SHOW_IMAGE(roi, keypoints);
    }

    return descriptions;
}


Ptr<Mat> 
describe_images_local(
    imageDataSet &imBase,
    BowParam &params,
    string &outputPath
                      )
{
    unsigned int detectorType = params.detectorType;
    unsigned int detectorAdapterType = params.dtcAdapterType;
    unsigned int descriptorType = params.descriptorType;
    unsigned int descriptorAdapterType = params.dscAdapterType;

    Ptr<Mat> ret;

    if (detectorType >= params.countOfDetectors())
        detectorType %= params.countOfDetectors();
    if (detectorAdapterType >= params.countOfDtcAdapters())
        detectorAdapterType %= params.countOfDtcAdapters();
    if (descriptorType >= params.countOfDescriptors())
        descriptorType %= params.countOfDescriptors();
    if (descriptorAdapterType >= params.countOfDscAdapters())
        descriptorAdapterType %= params.countOfDscAdapters();

    __SAM_BEGIN__;

///
/// Create and init detector and descriptor
/// 

    Ptr<FeatureDetector> detector = createDetector(params.detectorNames()[detectorType],
                                                   params.dtcAdapterNames()[detectorAdapterType]);
    Ptr<DescriptorExtractor> descriptor = createDescriptor(params.descriptorNames()[descriptorType],
                                                           params.dscAdapterNames()[descriptorAdapterType]);

    Ptr<Mat> allDescriptions(new Mat(0, descriptor->descriptorSize(), descriptor->descriptorType()));

    Ptr<Mat> im = imBase.getNextImage();
    std::string descriptionFileName;
    char str[10]={0};
    int count = 0;
    std::string fileListPath = outputPath + '/' + "fileList_"+imBase.getLabel()+".txt";
    std::fstream fileList(fileListPath.c_str(), std::fstream::out);
    if(!fileList.is_open())
    {
        std::cerr<<"ERROR:: can't open the file:" <<fileListPath.c_str()<<std::endl;
        EXIT;
    }
    for (; !im.empty(); im = imBase.getNextImage())
    {
        Ptr<Mat> descriptions=NULL;

        Ptr<Mat> sub_im = imBase.getNextObject(*im);
        for(; !sub_im.empty(); sub_im = imBase.getNextObject(*im))
        {
            descriptions = describe_roi_local(*sub_im, detector, descriptor);
            if(!descriptions->empty())
            {               
                // allDescriptions->push_back(*descriptions);  
                memset(str,0,10);
                sprintf(str,"%d", count++);
                descriptionFileName = outputPath + '/' + imBase.getCurrentImageName()+'_'+imBase.getCurrentObjectName()+str+".yml";
                fileList<< descriptionFileName.c_str()<<std::endl;
                writeDescriptions(descriptionFileName, *descriptions, imBase.getCurrentObjectName());
            }
        }
        
    }
    fileList.close();
    ret = allDescriptions;

    __SAM_END__;

    return ret;
}


Ptr<FeatureDetector> createDetector(const string &detectorName, const string &adapterName)
{
    return FeatureDetector::create(adapterName + detectorName);
}

Ptr<DescriptorExtractor> createDescriptor(const string &descriptorName, const string &adapterName)
{
    return DescriptorExtractor::create(adapterName + descriptorName);
} 


Ptr<BOWImgDescriptorExtractor> createBOWDescriptor(const string& descriptorName, const string& adapterName, const string& matcherName)
{
    Ptr<DescriptorExtractor> descriptor = createDescriptor(descriptorName, adapterName);
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(matcherName);
    return new BOWImgDescriptorExtractor(descriptor, matcher);
}

Ptr<Mat> 
describe_roi_bow (
    Mat &roi,
    Ptr<FeatureDetector> detector,
    Ptr<BOWImgDescriptorExtractor> descriptor)
{
    Ptr<Mat> descriptions (new Mat);
    
    vector<KeyPoint> keypoints;
    detector->detect(roi, keypoints);
    
    if (!keypoints.empty()) {
        descriptor->compute(roi, keypoints, *descriptions);
        assert(!descriptions->empty());
    }

    return descriptions;
}

Ptr<Mat> 
describe_images_bow (
    imageDataSet &imBase,
    BowParam& params,
    string &vocabularyPath,
    string &outputPath)
{

    Ptr<Mat> ret;

    __SAM_BEGIN__;

///
/// Create and init detector and descriptor
/// 

    Ptr<FeatureDetector> detector = createDetector(params.detectorName(),
                                                   params.dtcAdapterName());
    Ptr<DescriptorExtractor> descriptor = createDescriptor(params.descriptorName(),
                                                           params.dscAdapterName());
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(params.matcherName());
    Ptr<BOWImgDescriptorExtractor> bowDescriptor(new BOWImgDescriptorExtractor(descriptor, matcher));

    Ptr<Mat> allDescriptions;
    Mat vocabularies;
    
    Ptr<Mat> im = imBase.getNextImage();
    char str[10]={0};
    int count = 0;    
    std::string fileListPath = outputPath + '/' + "fileList_bow.txt";
    std::fstream fileList(fileListPath.c_str(), std::fstream::out);

    if(0 != readVocabulary(vocabularyPath.c_str(), vocabularies))
        EXIT;
    bowDescriptor->setVocabulary(vocabularies);
    allDescriptions = new Mat(0, bowDescriptor->descriptorSize(), bowDescriptor->descriptorType());

    if(!fileList.is_open())
    {
        std::cerr<<"ERROR:: can't open the file:" <<fileListPath.c_str()<<std::endl;
        EXIT;
    }

    for (; !im.empty(); im = imBase.getNextImage())
    {
        string descriptionFileName;
        Ptr<Mat> descriptions(new Mat);        
        Ptr<Mat> sub_im = imBase.getNextObject(*im);
      
        for(; !sub_im.empty(); sub_im = imBase.getNextObject(*im))
        {
            memset(str,0,10); 
            sprintf(str,"%d", count++);
            descriptionFileName = outputPath + '/' + imBase.getCurrentImageName()+'_'+imBase.getCurrentObjectName()+str+".yml";
            descriptions = describe_roi_bow(*sub_im, detector, bowDescriptor);
            if (!descriptions->empty()) 
            {
                writeDescriptions(descriptionFileName, *descriptions, imBase.getCurrentObjectName());   
                fileList<< descriptionFileName.c_str()<<std::endl;
            }     
        }
    }
    ret = allDescriptions;
    fileList.close();

    __SAM_END__;

    return ret;
}


////////////////////////////////////////////////////////////////////////////////
/////////////////////////         Test            //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdlib.h>

void usage()
{
    cout << "describe-images - \n"
         << " describe-images [--useVoc subclass] [--imageSetFileList aeroplane_train.txt] [-a <algorithm> [--vPath  <path>]] [-t <detector>] [-T <detector adapter] [-s <descriptor>] [-S <descriptor adapter>] [-m <matcher>] [-o <output-dir>] [-i <inputDir>]\n"
         << " DESCRIPTION\n"
         << "--useVoc :  Use the voc data: whole, subclass, className"
         << "  -a:  algorithm. If not specified, normal, otherwise \"bow\" only now.\n"
         << "  -t:  detector type\n"
         << "  -T:  detector adapter type\n"
         << "  -s:  descriptor type\n"
         << "  -S:  descriptor adapter type\n"
         << "  -m:  matcher type\n"
         << "  -h:  help\n"
         << "\n"
         << " Detectors:\n"
         << "   0:SIFT, 1:SURF, 2:ORB, 3:FAST, 4:STAR, 5:MSER, 6:GFTT, 7:HARRIS, 8:Dense, 9:SimpleBlob\n"
         << " Detector Adapters:\n" 
         << "   0: , 1:Grid, 2:Pyramid\n"
         << " Descriptors:\n"
         << "   0:SIFT, 1:SURF, 2:ORB, 3:BRIEF, 4:BOW\n"
         << " Descriptor Adapters\n"
         << "   0: , 1:Opponent\n"
         << " Matchers:\n"
         << "   0:BruteForce, 1:BruteForce-L1, 2:BruteForce-Hamming, 3:BruteForce-HammingLUT, 4:FlannBased\n"
         << "\n"
         << "   Sample file: Please refer to src/samples_example.yml.\n"
         << endl;
}

int main (int argc, char **argv)
{
    BowParam params;
    const char *algorithm = "";
    char *vocLabel = NULL;    
    char *inputPath = NULL;
    char *fileList = NULL;
    string outputPath;
    string vPath;
// Parse Parameters 
    char *cmdPara = NULL;
    while(NULL != (cmdPara=getCmdPara(argc, argv)))
    {
        if (0==strcmp(cmdPara, "--useVoc"))
            vocLabel = getCmdParaValue();            
        else if ((0==strcmp(cmdPara, "-a")) || (0==strcmp(cmdPara, "--algorithm")))
            algorithm = getCmdParaValue();        
        else if (0==strcmp(cmdPara, "--vPath"))
            vPath = getCmdParaValue(); 
        else if ((0==strcmp(cmdPara, "-t")) || (0==strcmp(cmdPara, "--detectorType"))) 
           params.detectorType = atoi(getCmdParaValue());
        else if ((0==strcmp(cmdPara, "-s")) || (0==strcmp(cmdPara, "--descriptorType"))) 
            params.descriptorType = atoi(getCmdParaValue());
        else if ((0==strcmp(cmdPara, "-T")) || (0==strcmp(cmdPara, "--dtcAdapterType"))) 
            params.dtcAdapterType = atoi(getCmdParaValue());
        else if ((0==strcmp(cmdPara, "-S")) || (0==strcmp(cmdPara, "--dscAdapterType"))) 
            params.dscAdapterType = atoi(getCmdParaValue());
        else if ((0==strcmp(cmdPara, "-m")) || (0==strcmp(cmdPara, "--matcherType")))
            params.matcherType = atoi(getCmdParaValue());         
        else if ((0==strcmp(cmdPara, "-i")) || (0==strcmp(cmdPara, "--inputPath")))
            inputPath  = getCmdParaValue();
        else if ((0==strcmp(cmdPara, "-o")) || (0==strcmp(cmdPara, "--outputPath")))
            outputPath = getCmdParaValue();
        else if (0==strcmp(cmdPara, "--imageSetFileList"))
            fileList = getCmdParaValue();

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

    if ((NULL==inputPath)||(outputPath.empty()) || (NULL == fileList))
    {
        cerr << "ERROR: <samples-filename> must be specified." << endl;
        return 1;
    }
    
///
/// MAIN
///
    time_stamp(0, NULL);
    initModule_nonfree(); // It should be called to register SURF and SIFT, otherwise
                          // there will be assert failed.
    imageDataSet *imageSet = NULL;
    if(NULL != vocLabel)
        imageSet = new vocDataSet(inputPath, vocLabel, fileList);
    else 
    {
        cerr<<"ERROR: Only support vocdta now"<<endl;
        return 1;
    }
    if(0 != imageSet->initial())
    {
        cerr<<"ERROR: Get image data failed"<<endl;
        return 1;
    } 
    Ptr<Mat> descs;
   
    if (strcmp(algorithm, "bow") == 0)
    {
        if(vPath.empty())
        {
            cerr<<"ERROR: Please specify the variable path when useing the vow description"<<endl;
        }
        descs = describe_images_bow(*imageSet, params, vPath,outputPath);
    }
    else
        descs = describe_images_local(*imageSet,  params, outputPath);


    if (descs.empty()) 
    {
        cerr << "Warning: There is none descriptions." << endl;
        return 1;
    }
    delete imageSet;
    time_stamp(1, "describeImage");
    cout<<"Finished \n";
    return 0;
}

//#endif//BUILD_APP
