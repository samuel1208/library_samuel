#!/home/fshen/Install_samuel/Python/python3.2.3/bin/python3.2

import os
import sys
import subprocess


cmd="./describeImage --useVoc whole -t 1 -T 0 -s 1 -S 1 \
	-i ~/samuel/project_self/VOC2010 \
	-o ~/samuel/project_self/bow-experiment/description/whole \
	--imageSetFileList aeroplane_train.txt"


#add opencheck for build
cmd= "buildVocabulary  -t 1 -T 0 -n 1000  \
	  -i ~/samuel/project_self/bow-experiment/description/whole/fileList_whole.txt \
	  -o ~/samuel/project_self/bow-experiment/vocabulary/vocabulary_whole "
	  
cmd="./describeImage --useVoc subclass -a bow -t 1 -T 0 -s 1 -S 1 \
	-i ~/samuel/project_self/VOC2010 \
	-o ~/samuel/project_self/bow-experiment/bow-description/ \
	--imageSetFileList aeroplane_train.txt  \
	--vPath  ~/samuel/project_self/bow-experiment/vocabulary/vocabularies"
	
#generate the  svm file for train 
cmd="./generateCSVData 	-i ~/samuel/project_self/bow-experiment/bow-description/fileList_bow.txt \
	-o ~/samuel/project_self/bow-experiment/svm/person.csv \
	-c person"