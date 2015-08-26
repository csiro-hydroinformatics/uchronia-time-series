cmake_minimum_required(VERSION 2.8)

## section: Macro
MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledBinary}\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

## start setting
SET (this_target datatypes)
PROJECT(${this_target})



## section: include directory
INCLUDE_DIRECTORIES(
  
  )

## section: source files
# Add your source files here (one file per line), please SORT in alphabetical order for future maintenance
SET (${this_target}_SOURCE_FILES
    time_series.cpp
	time_series_store.cpp
	time_step.cpp
    )

## section: header files
# Add your header files here(one file per line), please SORT in alphabetical order for future maintenance!
SET(${this_target}_HEADER_FILES
    exception_utilities.h
	common.h
	time_series.h
	time_series_store.h
	time_step.h
    )

SOURCE_GROUP("Header Files" FILES 
	
	)
SOURCE_GROUP("Source Files" FILES 
	
	)
SOURCE_GROUP("Resource Files" FILES 
	
	)


## section: precompiled header	
#ADD_MSVC_PRECOMPILED_HEADER("precompiled.h" "precompiled.cpp" MySources)
#ADD_LIBRARY(MyLibrary ${MySources})

SET_SOURCE_FILES_PROPERTIES(${this_target}_HEADER_FILES
                            PROPERTIES HEADER_FILE_ONLY TRUE)
LIST(APPEND ${this_target}_SOURCE_FILES ${${this_target}_HEADER_FILES})

## section: add definitions
#   add prefix -D. example> -DSHP
#  - DO NOT add  the following definitions(already defined in ${OSP_DEFINITIONS}:
#   -DSHP, -DWIN32, -D_WINDOWS, -D_DEBUG, -D_USRDLL, -D_CRT_SECURE_NO_DEPRECATE
ADD_DEFINITIONS(
    # -DCMAKE_CXX_FLAGS:STRING=' -std=c++11 '    
    )

# JM needed to use this as of 2015-02, after an upgrade to latest Debian setup. The Add_definitions macro was not working, somehow.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

## section: add target

ADD_LIBRARY(${this_target} SHARED ${${this_target}_SOURCE_FILES} )

## per202 comments out the following - no such thing for Linux.
## section: add dependency
# dependency determines overall build order.
# ADD_DEPENDENCIES(${this_target} 
#    netcdf.lib
# 	)

## section: set link libraries
TARGET_LINK_LIBRARIES( ${this_target}
        netcdf.so
        boost_system.so
		boost_date_time.so
        )
