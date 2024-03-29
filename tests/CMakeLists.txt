cmake_minimum_required(VERSION 3.12)

SET (this_target datatypes_tests)
PROJECT(${this_target})

if (WIN32)
    set(BOOST_INCLUDEDIR "c:/local/include")
    set(BOOST_LIBRARYDIR "c:/local/libs/64")
else()
    # list(APPEND CMAKE_MODULE_PATH "/usr/local/share/cmake/Modules")
    # list(APPEND CMAKE_MODULE_PATH "~/share/cmake/Modules") # Pearcey
    # list(APPEND CMAKE_MODULE_PATH "/data/projects/hydroforecast/usr/local/share/cmake/Modules") # Pearcey
endif()

# # (Julien Lerat)
# set(NETCDF_INCLUDE_DIR "/usr/local/include/")
# set(NETCDF_LIBRARY "/usr/local/lib64/libnetcdf.so")
find_package (NetCDF REQUIRED)

find_package(Boost 1.48.0 COMPONENTS filesystem system date_time REQUIRED)

find_path (datatypes_INCLUDE_DIRS NAMES datatypes/time_series.hpp)
# Find catch.hpp
find_path (CATCH_INCLUDE_DIRS NAMES catch/catch.hpp PATHS )
find_path (MOIRAI_INCLUDE_DIRS moirai/reference_handle.hpp)
FIND_LIBRARY(MOIRAI_LIBRARIES NAMES moirai)
message ("The path to the moirai library is ${MOIRAI_LIBRARIES}")

find_package (YAMLCPP REQUIRED)
# message ("The path to the yamlcpp headers is ${YAMLCPP_INCLUDE_DIRS}")

FIND_LIBRARY(UCHRONIA_LIBRARIES NAMES datatypes)

SET (${this_target}_SOURCE_FILES
    main.cpp
	basic_timeseries.cpp
	common_datatypes.cpp
	datatypes_netcdf_tests.cpp
	ensemble_timeseries.cpp
)

SET_SOURCE_FILES_PROPERTIES(${this_target}_HEADER_FILES
                            PROPERTIES HEADER_FILE_ONLY TRUE)
LIST(APPEND ${this_target}_SOURCE_FILES ${${this_target}_HEADER_FILES})

include_directories(
    ${Boost_INCLUDE_DIRS}
   ${datatypes_INCLUDE_DIRS}
   ${CATCH_INCLUDE_DIRS}
   ${MOIRAI_INCLUDE_DIRS}
   ${NETCDF_INCLUDE_DIRS}
   ${YAMLCPP_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")  #  required on some linux clusters 

IF (ENABLE_CODECOVERAGE)
    INCLUDE(CodeCoverage)
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_html(
        NAME ${this_target}_coverage
        EXECUTABLE ${this_target}
        # DEPENDENCIES datatypes? 
        BASE_DIRECTORY "../datatypes" # so that the datatypes shared library is included and (if available) its coverage files are found. 
    )
ENDIF()


if(DEFINED ENV{BOOST_LIBRARYDIR})
	message("Detected environment variable BOOST_LIBRARYDIR, adding to CMAKE_CXX_FLAGS")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L$ENV{BOOST_LIBRARYDIR}")  #  required on some linux clusters 
endif()

ADD_EXECUTABLE(${this_target} ${${this_target}_SOURCE_FILES})
TARGET_LINK_LIBRARIES( ${this_target}
    ${Boost_LIBRARIES}
 	${UCHRONIA_LIBRARIES}
    ${NETCDF_LIBRARIES}
    ${YAMLCPP_LIBRARIES}
    ${MOIRAI_LIBRARIES}
)

# Files to be installed
INSTALL(TARGETS ${this_target}
  DESTINATION bin
)
