#include"imageDataSet.hpp"
#include <opencv2/core/core.hpp>

imageDataSet::imageDataSet(char* path, char *label)
{    
    _path = path;
    _currentImageName = "";
    _imageNames.clear();
	_label = label;
}


imageDataSet::~imageDataSet()
{
    _imageNames.clear();
}


int writeDescriptions(
    const std::string& filename, 
    const cv::Mat& descriptions, 
    const std:: string& className)
{
    if(filename.empty())
        return 1;
    cv::FileStorage fs(filename.c_str(), cv::FileStorage::WRITE);
    if (fs.isOpened()) {
        fs << "class" << className;
        fs << "descriptions" << descriptions;
        fs.release();
        
        return 0;
    }
    return 1;
}

int readDescriptions (
    const std::string& filename, 
    cv::Mat& descriptions,
    std::string& className)
{
    if(filename.empty())
        return 1;
    cv::FileStorage fs(filename, cv::FileStorage::READ );
    if (fs.isOpened()) {
        fs["class"] >> className;
        fs["descriptions"] >> descriptions;
        fs.release();
        
        return 0;
    }
    return 1;
}
int readVocabulary ( const std::string& filename,  cv::Mat& vocabularies)
{
    if(filename.empty())
        return 1;
    cv::FileStorage fs(filename, cv::FileStorage::READ );
    if( fs.isOpened() )
    {
        fs["vocabularies"] >> vocabularies;
        return 0;
    }
    return 1;
}

int  writeVocabulary (const std::string& filename,  const cv::Mat& vocabulary )
{
    if(filename.empty())
        return 1;
    cv::FileStorage fs( filename, cv::FileStorage::WRITE );
    if( fs.isOpened() )
    {
        fs << "vocabularies" << vocabulary;
        return 0;
    }
    return 1;
}
