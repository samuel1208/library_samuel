#ifndef __VOC_DATA_SET_HPP__
#define __VOC_DATA_SET_HPP__

#include"imageDataSet.hpp"
#include <vector>

typedef struct __VOCObject
{
    std::string name;
    int xmin;
    int xmax;
    int ymin;
    int ymax;
}VOCObject;

class vocDataSet : public imageDataSet
{
public:
    vocDataSet(){}
    vocDataSet(char *path, char *label, char *imageSetFileList);
    ~vocDataSet();
public:
    virtual int  initial();
    virtual cv::Ptr<cv::Mat> getNextImage();
    virtual cv::Ptr<cv::Mat> getNextObject(cv::Mat &mat);
    

private:
    int _getObjectsOfVoc(const char *filePath);
    int _extractXMLBlock(const std::string& src, const std::string& tag, std::string& tag_contents, int  searchPos);
    int _readFileToString(const char *filename, std::string& file_contents);
    

private:
    std::string _JPGFolder;
    std::string _annotationFolder;
    std::string _imageSetsFile;
    std::string _imageExtentionName;
    std::vector<VOCObject> _vocObjects;
};

#endif
