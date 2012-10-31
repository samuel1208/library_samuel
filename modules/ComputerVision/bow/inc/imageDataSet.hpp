#ifndef __IMAGE_DATA_SET_HPP__
#define __IMAGE_DATA_SET_HPP__

#include<string>

#include <opencv2/core/core.hpp>

class imageDataSet
{
public:
    imageDataSet(){}
    imageDataSet(char* path, char *label);
    ~imageDataSet();

public:
    virtual int  initial() = 0;
    virtual cv::Ptr<cv::Mat> getNextImage()=0;
    virtual cv::Ptr<cv::Mat> getNextObject(cv::Mat &mat)=0;
    std::string getCurrentImageName(){return _currentImageName;}
    std::string getCurrentObjectName(){return _currentObjectName;}
	std::string getLabel(){return _label;}
    //int getCurrentObjectID(){return _currentObjectID;}

protected:
    std::string _path;    
    std::vector<std::string>  _imageNames;
    std::string _currentImageName;
    std::string _currentObjectName;
	std::string _label;
    int _currentObjectID;
    
};

int  writeDescriptions( const std::string& filename,  const cv::Mat& descriptions, const std::string& className);
int  readDescriptions ( const std::string& filename,  cv::Mat& descriptions, std::string& className);
int  readVocabulary ( const std::string& filename,  cv::Mat& vocabularies);
int  writeVocabulary (const std::string& filename,  const cv::Mat& vocabulary );
#endif
