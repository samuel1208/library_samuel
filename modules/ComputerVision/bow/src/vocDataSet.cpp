#include"vocDataSet.hpp"
#include<iostream>
#include<fstream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<ctype.h>
using namespace std;


vocDataSet::vocDataSet(char *path, char *label, char *imageSetFileList):imageDataSet(path, label)
{   
	std::string temp=imageSetFileList;
    _JPGFolder = "JPEGImages";
    _annotationFolder = "Annotations";
    _imageSetsFile = "ImageSets/Main/" + temp;
    _imageExtentionName = "jpg";
    _vocObjects.clear();
}
vocDataSet::~vocDataSet()
{
    _vocObjects.clear();
}
int  vocDataSet:: initial()
{
    //get the fileName    
    string fileName = _path+'/'+_imageSetsFile;
    if (_label.size()<1)
    {
        cerr<<"please input voc label: whole , subclass, or className"<<endl;
        return 1;
    }
    ifstream file(fileName.c_str(), ifstream::in);
    if(!file.is_open())
    {
        cerr<<"The file is not exist"<<endl;
        return 1;
    }
    char line[100]={0};
    while(!file.eof())
    {
        file.getline(line, 100);
        string str  = line;
        if(!isalnum(line[0]))
            continue;
        string sub_str = str.substr(0, str.find_first_of(' '));
        _imageNames.push_back(sub_str);
    }
    if(_imageNames.size() < 1)
        return 1;
    return 0;
}
cv::Ptr<cv::Mat> vocDataSet::getNextImage()
{
    if(_imageNames.size()<1)
        return NULL;
    _currentImageName = _imageNames.back();
    _imageNames.pop_back();
   
    //get the content of xml
    string annotationFileName =  _path+'/'+ _annotationFolder + '/'+ _currentImageName +".xml";
    _getObjectsOfVoc(annotationFileName.c_str());       
    //get the image Mat
    string filePath = _path+'/'+_JPGFolder+'/'+ _currentImageName +'.'+_imageExtentionName;
    return  new cv::Mat(cv::imread(filePath.c_str()));
}


cv::Ptr<cv::Mat> vocDataSet::getNextObject(cv::Mat &mat)
{
    if(_vocObjects.size()<1)
        return NULL;
    VOCObject temp = _vocObjects.back();
    _vocObjects.pop_back();
    _currentObjectName = temp.name;
    return new cv::Mat(mat, cv::Rect(temp.xmin,temp.ymin,temp.xmax,temp.ymax));
}

int vocDataSet::_getObjectsOfVoc(const char *filePath)
{    
    string file_contents;
    if( 0!= _readFileToString(filePath, file_contents))
    {
        cerr<<"read annotation xml file failed"<<endl;
        return 1;
    }
    VOCObject vocObject;
    string tag_contents;
    int searchPos = 0;
    string value;
	_vocObjects.clear();
    if( 0 == strcmp(_label.c_str(), "whole"))
    {
        if (-1 ==  _extractXMLBlock(file_contents, "size", tag_contents,searchPos))
            return 1;
        if(-1 == _extractXMLBlock(tag_contents, "width", value,0))
            return 1;
        vocObject.xmax = atoi(value.c_str());
        
        if(-1 == _extractXMLBlock(tag_contents, "height", value,0))
            return 1;
        vocObject.ymax = atoi(value.c_str());
        vocObject.xmin = vocObject.ymin = 0;
        vocObject.name = "whole"; 
        _vocObjects.push_back(vocObject);          
        return 0;
    }
        
        
    while(-1 != ( searchPos =  _extractXMLBlock(file_contents, "object", tag_contents,searchPos)))
    {
        if(-1 == _extractXMLBlock(tag_contents, "name", value,0))
            continue;
        if( 0 != strcmp(_label.c_str(), "subclass"))
        {
            if( 0 != strcmp(value.c_str(), _label.c_str()))
                continue;
        }
        vocObject.name = value;
        if(-1 == _extractXMLBlock(tag_contents, "xmin", value,0))
            continue;
        vocObject.xmin = atoi(value.c_str());
        if(-1 == _extractXMLBlock(tag_contents, "xmax", value,0))
            continue;
        vocObject.xmax = atoi(value.c_str())-vocObject.xmin;
        if(-1 == _extractXMLBlock(tag_contents, "ymin", value,0))
            continue;
        vocObject.ymin = atoi(value.c_str());
        if(-1 == _extractXMLBlock(tag_contents, "ymax", value,0))
            continue;
        vocObject.ymax = atoi(value.c_str())-vocObject.ymin;
        _vocObjects.push_back(vocObject);                        
    }
    if(_vocObjects.size() < 1)
        return 1;
    return 0;
}


int vocDataSet::_extractXMLBlock(const string& src, const string& tag, string& tag_contents, int  searchPos)
{
    size_t startPos=0 ,endPos=0;
    startPos = src.find("<" + tag + ">", searchPos);
     if (startPos == string::npos)
         return -1;
    startPos += tag.size()+2;
    endPos = src.find("</" + tag + ">", startPos);
    if (endPos ==string::npos)
        return -1;
    tag_contents = src.substr(startPos, endPos-startPos);    
    return endPos;
}
int vocDataSet::_readFileToString(const char *filename, string& file_contents)
{
    std::ifstream ifs(filename);
    if (!ifs.is_open()) 
    {
        cerr<<"can't open the file:"<<filename<<endl;
        return 1;
    }
    stringstream oss;
    oss << ifs.rdbuf();
    file_contents = oss.str();
    return 0;
}
