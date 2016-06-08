include(../datatypes/version.cmake)

SET (this_target datatypes_tests)
PROJECT(${this_target})

#include(../../FindxUnitpp.cmake)

find_package(Boost 1.48.0 COMPONENTS filesystem system date_time REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/..")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../datatypes")
find_package (NetCDF REQUIRED)

find_path (datatypes_INCLUDE_DIRS NAMES datatypes/time_series.hpp PATHS ".." )
find_library (datatypes_LIBRARIES NAMES datatypes )

INCLUDE_DIRECTORIES(
    ${datatypes_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${NETCDF_INCLUDE_DIRS}
)

SET (${this_target}_SOURCE_FILES
    main.cpp
    test_dataset_location.cpp
)

SET(${this_target}_HEADER_FILES
    catch.hpp
    catch_macros.hpp
    test_dataset_location.h
)

SET_SOURCE_FILES_PROPERTIES(${this_target}_HEADER_FILES
                            PROPERTIES HEADER_FILE_ONLY TRUE)
LIST(APPEND ${this_target}_SOURCE_FILES ${${this_target}_HEADER_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__stdcall='' -std=c++11")

ADD_EXECUTABLE(${this_target} ${${this_target}_SOURCE_FILES})
TARGET_LINK_LIBRARIES( ${this_target}
    ${NETCDF_LIBRARIES}
    ${Boost_LIBRARIES}
    ${datatypes_LIBRARIES}
)
#ADD_LIBRARY(${this_target} SHARED ${${this_target}_SOURCE_FILES} )

# Files to be installed
#INSTALL(TARGETS ${this_target} 
#  LIBRARY DESTINATION lib
#)



