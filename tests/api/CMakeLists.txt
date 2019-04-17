cmake_minimum_required(VERSION 3.1)

SET (this_target datatypes_tests_api)
PROJECT(${this_target})

if (WIN32)
    set(BOOST_INCLUDEDIR "c:/local/include")
    set(BOOST_LIBRARYDIR "c:/local/libs/64")
else()
    list(APPEND CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules")
    list(APPEND CMAKE_MODULE_PATH "~/share/cmake/Modules") # Pearcey
    list(APPEND CMAKE_MODULE_PATH "/data/projects/hydroforecast/usr/local/share/cmake/Modules") # Pearcey
endif()

# # (Julien Lerat)
# set(NETCDF_INCLUDE_DIR "/usr/local/include/")
# set(NETCDF_LIBRARY "/usr/local/lib64/libnetcdf.so")

find_package (NetCDF REQUIRED)
find_package(Boost 1.48.0 COMPONENTS filesystem system date_time REQUIRED)


find_path (datatypes_INCLUDE_DIRS NAMES datatypes/time_series.hpp)
# Find catch.hpp
find_path (catch_INCLUDE_DIRS NAMES catch/catch.hpp PATHS )
find_path (MOIRAI_INCLUDE_DIRS moirai/reference_handle.hpp)

SET (${this_target}_SOURCE_FILES
    main.cpp
)

SET_SOURCE_FILES_PROPERTIES(${this_target}_HEADER_FILES
                            PROPERTIES HEADER_FILE_ONLY TRUE)
LIST(APPEND ${this_target}_SOURCE_FILES ${${this_target}_HEADER_FILES})

include_directories(
    ${Boost_INCLUDE_DIRS}
   ${catch_INCLUDE_DIRS}
   ${datatypes_INCLUDE_DIRS}
   ${MOIRAI_INCLUDE_DIRS}
    ${NETCDF_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L$ENV{BOOST_LIBRARYDIR} -std=c++11")  #  required on some linux clusters 

ADD_EXECUTABLE(${this_target} ${${this_target}_SOURCE_FILES})
TARGET_LINK_LIBRARIES( ${this_target}
    ${Boost_LIBRARIES}
    ${NETCDF_LIBRARIES}
    datatypes
    yaml-cpp
    moirai
)

# Files to be installed
INSTALL(TARGETS ${this_target}
  DESTINATION bin
)
