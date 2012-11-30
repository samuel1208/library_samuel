
include_directories(inc)

#generate the describe Image target    
set(target "describeImage")
set(src_files "src/describeImages.cpp" "src/bowParam.cpp" "src/imageDataSet.cpp" "src/vocDataSet.cpp")
add_executable(${target}  ${src_files})
target_link_libraries(${target} ${OpenCV_LIBS} SamCore)

#generate the build vacabulary target    
set(target "buildVocabulary")
set(src_files "src/buildVocabulary.cpp" "src/bowParam.cpp" "src/imageDataSet.cpp"  "src/vocDataSet.cpp")
add_executable(${target}  ${src_files})
target_link_libraries(${target} ${OpenCV_LIBS} SamCore)

set(target "generateCSVData")
set(src_files "src/generateCSVData.cpp"  "src/imageDataSet.cpp" )
add_executable(${target}  ${src_files})
target_link_libraries(${target} ${OpenCV_LIBS} SamCore)

