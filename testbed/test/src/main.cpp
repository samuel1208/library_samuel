
#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
    const char *keymap =  
      "{ f | filename | default_value | file name }"
      "{ o | option   | 100           | option }" ;
    cv::CommandLineParser parser(argc, argv, keymap) ;

    std:: string file = parser.get<std::string>("filename",0);
    int o = parser.get<int>("option");
    
    printf("%s, %d", file.c_str() , o);
    printf("ok");
    return 0;
}
