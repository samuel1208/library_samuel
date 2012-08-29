
macro(define_testbed testbed MFC_ENABLE)
    #should at least input the mudule's name
    if(${ARGC} LESS "1")
        message("please specify the testbed's name")
    endif()

    include_directories(inc)
    
    #get the include and source files
    file(GLOB  inc_files "inc/*.h" "inc/*hpp")
    aux_source_directory(src src_files)

    #generate the library
	if(${MFC_ENABLE} EQUAL 1)
            add_executable(${testbed} WIN32 ${src_files} ${inc_files})
	else()
	    add_executable(${testbed} ${src_files} ${inc_files})
	endif()

    #link the dependent libraries if has
    if( ${ARGC} GREATER 1)
        target_link_libraries(${testbed} ${ARGN})
    endif()
    
endmacro(define_testbed)

#the include files 
include_directories(../modules/SamCore/inc)
include_directories(../modules/ImageProcess/inc)
add_subdirectory(test)
if(OGL_ENABLE)
add_subdirectory(test_ogl)


#generate the testbeds
if(OCL_ENABLE)
include_directories(../modules/OCLLibrary/inc)
include_directories(../modules/OCLCore/inc)
include_directories(${CUDA_TOOLKIT_ROOT_DIR}/include/CL)
link_directories(${CUDA_TOOLKIT_ROOT_DIR}/lib/Win32)
add_subdirectory(test_ocl)
endif()






