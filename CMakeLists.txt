cmake_minimum_required(VERSION 2.8)

#define the SHARED_LIBRARY to generate shared library
if(SHARED_LIBRARY)
    message("build shared library")
    set(lib_status SHARED)
	add_definitions(-DDLL_LIBRARY)
else()
	message("build static library")
    set(lib_status STATIC)
endif()

#check the system type
if(UNIX)
    add_definitions(-DUNIX)
	find_package(OpenCV PATHS "/home/fshen/Install_samuel/OpenCV/OpenCV-2.4.2/Debug/share/OpenCV" NO_DEFAULT_PATH)
elseif(WIN32)
    add_definitions(-DWIN32)
	find_package(OpenCV)
else()
    message("this library only support on windows and unix")
	return()	
endif()

message("The OpenCV path is as follow:")
message(${OpenCV_INCLUDE_DIRS})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(modules)
add_subdirectory(testbed)