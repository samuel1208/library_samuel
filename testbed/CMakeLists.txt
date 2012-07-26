
macro(define_testbed testbed)
    #should at least input the mudule's name
    if(${ARGC} LESS "1")
        message("please specify the testbed's name")
    endif()

    include_directories(inc)
    
    #get the include and source files
    file(GLOB  inc_files "inc/*.h" "inc/*hpp")
    aux_source_directory(src src_files)

    #generate the library
    add_executable(${testbed} ${src_files} ${inc_files})

    #link the dependent libraries if has
    if( ${ARGC} GREATER 1)
        target_link_libraries(${testbed} ${ARGN} ${OpenCV_LIBS})
    endif()
    
endmacro(define_testbed)

#the include files 
include_directories(../common/include)
include_directories(../modules/MManage/inc)


#generate the testbeds
add_subdirectory(test)



