#ifndef __SAK_BOW_HPP__
#define __SAK_BOW_HPP__

/*#include "sakcore.h"
#include "mylibc.h"
#include "image-data-set.hpp"
#include "LearningDataBase.hpp"
*/

#include <imageDataSet.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <set>

#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/features2d/features2d.hpp>

#define IN
#define OUT

namespace sam_bow {


////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Bow Parameters      ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

///
/// BowParam
///

class BowParam {
public:
    BowParam() {
        detectorType = 1;
        dtcAdapterType = 0;
        descriptorType = 1;
        dscAdapterType = 1;
        matcherType = 0;
        maxVisualWordNum = 100;
        modelFileName = NULL;
        enableClassBalance = false;
        enableUpdated = true;
        memoryAvailable = 300;
        descProportion = 0.3;
    }

public:
    int detectorType;
    int dtcAdapterType;
    int descriptorType;
    int dscAdapterType;
    int matcherType;

    int  maxVisualWordNum;
    bool enableClassBalance;
    bool enableUpdated;        // Enable increament learning from a learned model.

    const char* modelFileName;
    const char* dataFileName;

    int memoryAvailable;
    float descProportion;

public:
    size_t countOfDetectors() const { return _kCountOfDetectors; }
    size_t countOfDtcAdapters() const { return _kCountOfDtcAdapters; }
    size_t countOfDescriptors() const { return _kCountOfDescriptors; }
    size_t countOfDscAdapters() const { return _kCountOfDscAdapters; }
    size_t countOfMatchers() const { return _kCountOfMatchers; }
    const char* detectorName() const { return _kAllDetectorNames[detectorType]; }
    const char* dtcAdapterName() const { return _kAllDtcAdapterNames[dtcAdapterType]; }
    const char* descriptorName() const { return _kAllDescriptorNames[descriptorType]; }
    const char* dscAdapterName() const { return _kAllDscAdapterNames[dscAdapterType]; }
    const char* matcherName() const {return _kAllMatcherNames[matcherType]; }
    const char** detectorNames() const { return _kAllDetectorNames; }
    const char** dtcAdapterNames() const { return _kAllDtcAdapterNames; }
    const char** descriptorNames() const { return _kAllDescriptorNames; }
    const char** dscAdapterNames() const { return _kAllDscAdapterNames; }
    const char** matcherNames() const {return _kAllMatcherNames; }

private:
    static const size_t _kCountOfDetectors;
    static const char* _kAllDetectorNames[];
    static const size_t _kCountOfDtcAdapters;
    static const char* _kAllDtcAdapterNames[];
    static const size_t _kCountOfDescriptors;
    static const char* _kAllDescriptorNames[];
    static const size_t _kCountOfDscAdapters;
    static const char* _kAllDscAdapterNames[];
    static const size_t _kCountOfMatchers;
    static const char* _kAllMatcherNames[];
};

class bowPath
{
public:
    static const std::string  description;
    static const std::string  vocabulary;  
    static const std::string  bow_description;
    static const std::string  train;
};

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////      APIs      ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


cv::Ptr<cv::Mat> describe_images_local(imageDataSet &imBase, sam_bow::BowParam& params);
cv::Ptr<cv::Mat> describe_roi_local(const cv::Mat& roi, const cv::Ptr<cv::FeatureDetector>& detector, const cv::Ptr<cv::DescriptorExtractor>& descriptor);

cv::Ptr<cv::FeatureDetector> createDetector(const std::string& detectorName, const std::string& adapterName);
cv::Ptr<cv::DescriptorExtractor> createDescriptor(const std::string& descriptorName, const std::string& adapterName);
/*
cv::Ptr<cv::Mat> describe_images_local(std::string className, sam_bow::ImageDataSet &imBase, sam_bow::BowParam& params, sam_bow::BowDataBase *dataBase=knull, bool inMemory = false);
cv::Ptr<cv::Mat> describe_images_bow (std::string className, sam_bow::ImageDataSet &imBase, sam_bow::BowParam& params, sam_bow::BowDataBase *bowBase=knull, bool inMemory = false);
cv::Ptr<cv::Mat> describe_roi_local(const cv::Mat& roi, const cv::Ptr<cv::FeatureDetector>& detector, const cv::Ptr<cv::DescriptorExtractor>& descriptor);
cv::Ptr<cv::Mat> describe_roi_bow(cv::Mat &roi, cv::Ptr<cv::FeatureDetector> detector, cv::Ptr<cv::BOWImgDescriptorExtractor> descriptor);

cv::Ptr<cv::FeatureDetector> createDetector(const std::string& detectorName, const std::string& adapterName);
cv::Ptr<cv::DescriptorExtractor> createDescriptor(const std::string& descriptorName, const std::string& adapterName);
cv::Ptr<cv::BOWImgDescriptorExtractor> createBOWDescriptor(const std::string& descriptorName, const std::string& adapterName, const std::string& matcherName);

cv::Ptr<cv::Mat> build_vocabularies(std::string className, sam_bow::ImageDataSet &imBase, sam_bow::BowParam& params, sam_bow::BowDataBase *bowBase);


bool train(std::string& type, CvMLData *data, sam_bow::BowParam& params);
void test(std::string&  type, CvMLData *data, sam_bow::BowParam& params, sam_bow::BowDataBase *bowBase);
*/
#endif//__SAK_BOW_HPP__
