cmake_minimum_required(VERSION 2.8)

## start setting
SET (this_target datatypes)
PROJECT(${this_target})

find_package(Boost 1.48.0 COMPONENTS system date_time regex filesystem REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake_modules")
find_package (NetCDF REQUIRED)

## section: include directory
INCLUDE_DIRECTORIES(
    ${Boost_INCLUDE_DIRS}
    ${NETCDF_INCLUDE_DIRS}
	../include
)


## section: library directories
LINK_DIRECTORIES(
)


## section: source files
# Add your source files here (one file per line), please SORT in alphabetical order for future maintenance
SET (${this_target}_SOURCE_FILES
	common.cpp
	extern_c_api.cpp
	interop_conversions.cpp
	io_helper.cpp
	tests/datatypes_test_helpers.cpp
	time_series.cpp
	time_series_io.cpp
	time_series_store.cpp
	time_step.cpp
	time_step_implementation.cpp

    )

## section: header files
# Add your header files here(one file per line), please SORT in alphabetical order for future maintenance!
SET(${this_target}_HEADER_FILES
	../include/datatypes/common.h
	../include/datatypes/datatypes_test_helpers.h
	../include/datatypes/datatypes_test_helpers.hpp
	../include/datatypes/exception_utilities.h
	../include/datatypes/extern_c_api_as_opaque.h
	../include/datatypes/extern_c_api_as_transparent.h
	../include/datatypes/extern_c_api.h
	../include/datatypes/extern_c_api_transparent_pointers.h
	../include/datatypes/internals_c_api.hpp
	../include/datatypes/interop_conversions.h
	../include/datatypes/interop_conversions.hpp
	../include/datatypes/interop_struct.h
	../include/datatypes/io_helper.h
	../include/datatypes/setup_exports.h
	../include/datatypes/shared_pointer_conversions.hpp
	../include/datatypes/time_series.hpp
	../include/datatypes/time_series_io.hpp
	../include/datatypes/time_series_store.hpp
	../include/datatypes/time_series_strategies.hpp
	../include/datatypes/time_step.h
	../include/datatypes/time_step_implementation.h
)

IF (ENABLE_CODECOVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_COVERAGE}")
ENDIF()

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

# JM needed to use this as of 2016-07, after an upgrade to latest Debian setup. The Add_definitions macro was not working, somehow.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

## section: add target
ADD_LIBRARY(${this_target} SHARED ${${this_target}_SOURCE_FILES} )

## section: set link libraries
TARGET_LINK_LIBRARIES( ${this_target}
    ${NETCDF_LIBRARIES}
    ${Boost_LIBRARIES}
)

# Files to be installed - used by packaging stuff below
INSTALL(FILES ${${this_target}_HEADER_FILES} DESTINATION include/datatypes)

########### Add uninstall target ###############
# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
########### End uninstall target ###############

if(WIN32)
  INSTALL(TARGETS ${this_target}
    RUNTIME DESTINATION bin
  )
else()
  INSTALL(TARGETS ${this_target}
    LIBRARY DESTINATION lib
  )
endif()

# Packages - currently just an RPM
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR "RPM;TGZ")
set(CPACK_PACKAGE_NAME "swift-datatypes")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_RELEASE ${CPACK_PACKAGE_RELEASE})
set(CPACK_PACKAGE_CONTACT "David Kent")
set(CPACK_PACKAGE_VENDOR "BoM")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_RPM_PACKAGE_REQUIRES "${NETCDF_LIBRARIES}")
include(CPack)
