#include "bow.hpp"
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace sam_bow;


const unsigned int BowParam::_kCountOfDetectors = 10;
const char* BowParam::_kAllDetectorNames[] = { "SIFT", "SURF", "ORB", "FAST", "STAR", 
                                               "MSER", "GFTT", "HARRIS", "Dense", "SimpleBlob"};
const unsigned int BowParam::_kCountOfDtcAdapters = 3;
const char* BowParam::_kAllDtcAdapterNames[] = { "", "Grid", "Pyramid" };
const unsigned int BowParam::_kCountOfDescriptors = 5;
const char* BowParam::_kAllDescriptorNames[] = { "SIFT", "SURF", "ORB", "BRIEF" "BOW" };
const unsigned int BowParam::_kCountOfDscAdapters = 2;
const char* BowParam::_kAllDscAdapterNames[] = { "", "Opponent" };
const unsigned int BowParam::_kCountOfMatchers = 5;
const char* BowParam::_kAllMatcherNames[] = { "BruteForce", "BruteForce-L1",
                                              "BruteForce-Hamming", "BruteForce-HammingLUT",
                                              "FlannBased" };


//set the path for bow
const std::string bowPath::description = "description";
const std::string bowPath::vocabulary = "vocabulary";
const std::string bowPath::bow_description="bow-description";
const std::string bowPath::train="train";
