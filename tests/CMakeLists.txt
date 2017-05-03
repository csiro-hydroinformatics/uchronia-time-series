cmake_minimum_required(VERSION 2.8)

SET (this_target datatypes_tests)
PROJECT(${this_target})

find_package(Boost 1.48.0 COMPONENTS filesystem system date_time REQUIRED)

find_path (datatypes_INCLUDE_DIRS NAMES datatypes/time_series.hpp PATHS ".." )

SET (${this_target}_SOURCE_FILES
    main.cpp
	basic_timeseries.cpp
	common_datatypes.cpp
	datatypes_netcdf_tests.cpp
	ensemble_timeseries.cpp
)

SET(${this_target}_HEADER_FILES
    catch.hpp
    catch_macros.hpp
	common_datatypes.h
)

SET_SOURCE_FILES_PROPERTIES(${this_target}_HEADER_FILES
                            PROPERTIES HEADER_FILE_ONLY TRUE)
LIST(APPEND ${this_target}_SOURCE_FILES ${${this_target}_HEADER_FILES})

INCLUDE_DIRECTORIES(
    ${datatypes_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

ADD_EXECUTABLE(${this_target} ${${this_target}_SOURCE_FILES})
TARGET_LINK_LIBRARIES( ${this_target}
    ${Boost_LIBRARIES}
    datatypes
)

# Files to be installed
INSTALL(TARGETS ${this_target}
  DESTINATION bin
)
