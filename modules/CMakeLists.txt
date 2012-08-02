
macro(define_module module)
    #should at least input the mudule's name
    if(${ARGC} LESS "1")
        message("please specify the mudule's name")
    endif()

    include_directories(inc)
    
    #get the include and source files
    file(GLOB  inc_files "inc/*.h" "inc/*hpp")
    aux_source_directory(src src_files)

    #generate the library
    add_library(${module} ${lib_status} ${src_files} ${inc_files})

    #link the dependent libraries if has
    if( ${ARGC} GREATER 1)
        target_link_libraries(${module} ${ARGN})
    endif()
    
    #define the DLL macro for target	
	if(SHARED_LIBRARY)
	    set_target_properties(${module} PROPERTIES DEFINE_SYMBOL DLL_API)
	endif()
endmacro(define_module)

#the include files 
include_directories(../common/include)
include_directories(./MManage/inc)

#generate the MMange library
add_subdirectory(MManage)

#generate the OCLCore library
if(OCL_ENABLE)
    find_package(CUDA)
	message("\n------------------------------------")
    message("The OCL path is as follow:")
    message(${CUDA_TOOLKIT_ROOT_DIR})
    message("------------------------------------\n")
    include_directories(${CUDA_TOOLKIT_ROOT_DIR}/include/CL)
	link_directories(${CUDA_TOOLKIT_ROOT_DIR}/lib/Win32)
	
    include_directories(./OCLCore/inc)	
    add_subdirectory(OCLCore)
	add_subdirectory(OCLLibrary)	
endif()


