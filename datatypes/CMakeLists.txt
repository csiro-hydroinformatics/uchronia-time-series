cmake_minimum_required(VERSION 3.10
)

## start setting
set(UCHRONIA_LIB_NAME uchronia)
set(TARGET_LIB_NAME datatypes)
SET (this_target ${TARGET_LIB_NAME})

set(TARGET_MAJOR 2)
set(TARGET_MINOR 4)
set(TARGET_RELEASE 1)
# set(TARGET_NAME libmoirai${TARGET_MAJOR}${TARGET_MINOR})
set(TARGET_VERSION ${TARGET_MAJOR}.${TARGET_MINOR}.${TARGET_RELEASE})
SET (VERSION ${TARGET_VERSION})

PROJECT(${this_target} VERSION ${TARGET_VERSION} DESCRIPTION "C++ library designed to handle multidimensional time series")

include(GNUInstallDirs)
# Always add the path to the standard (?) linux filesystem location
list(APPEND CMAKE_MODULE_PATH "/usr/share/cmake/Modules")

find_package(Boost 1.48.0 COMPONENTS system date_time regex filesystem REQUIRED)

find_package (NetCDF REQUIRED)
find_package (YAMLCPP REQUIRED)

find_path (MOIRAI_INCLUDE_DIRS moirai/reference_handle.hpp)
FIND_LIBRARY(MOIRAI_LIBRARIES NAMES moirai)

IF(DEFINED ENV{UCHRONIA_CMAKE_VERBOSE})
	message ("The path to the boost headers is ${Boost_INCLUDE_DIRS}")
	message ("The path to the boost libraries is ${Boost_LIBRARIES}")
	message ("The path to the netcdf headers is ${NETCDF_INCLUDE_DIRS}")
	message ("The path to the netcdf libraries is ${NETCDF_LIBRARIES}")
	message ("The path to the yaml-cpp headers is ${YAMLCPP_INCLUDE_DIRS}")
	message ("The path to the moirai headers is ${MOIRAI_INCLUDE_DIRS}")
ENDIF()

## section: include directory
INCLUDE_DIRECTORIES(
	./include
	${Boost_INCLUDE_DIRS}
	${NETCDF_INCLUDE_DIRS}
	${MOIRAI_INCLUDE_DIRS}
	${YAMLCPP_INCLUDE_DIRS}
)

## section: library directories
LINK_DIRECTORIES(
# use target_link_libraries instead
)

## section: source files
# Add your source files here (one file per line), please SORT in alphabetical order for future maintenance
SET (${this_target}_SOURCE_FILES
	src/common.cpp
	src/extern_c_api.cpp
	src/interop_conversions.cpp
	src/io_helper.cpp
	src/tests/datatypes_test_helpers.cpp
	src/time_series.cpp
	src/time_series_io.cpp
	src/time_series_store.cpp
	src/time_step.cpp
	src/time_step_implementation.cpp

    )

## section: header files
# Add your header files here(one file per line), please SORT in alphabetical order for future maintenance!
SET(${this_target}_HEADER_FILES
	./include/datatypes/common.h
	./include/datatypes/datatypes_test_helpers.h
	./include/datatypes/datatypes_test_helpers.hpp
	./include/datatypes/exception_utilities.h
	./include/datatypes/extern_c_api_as_opaque.h
	./include/datatypes/extern_c_api_as_transparent.h
	./include/datatypes/extern_c_api.h
	./include/datatypes/extern_c_api_transparent_pointers.h
	./include/datatypes/internals_c_api.hpp
	./include/datatypes/interop_conversions.h
	./include/datatypes/interop_conversions.hpp
	./include/datatypes/interop_struct.h
	./include/datatypes/io_helper.h
	./include/datatypes/setup_exports.h
	./include/datatypes/shared_pointer_conversions.hpp
	./include/datatypes/time_series.hpp
	./include/datatypes/time_series_io.hpp
	./include/datatypes/time_series_store.hpp
	./include/datatypes/time_series_strategies.hpp
	./include/datatypes/time_step.h
	./include/datatypes/time_step_implementation.h
)

IF (ENABLE_CODECOVERAGE)
    INCLUDE(CodeCoverage)
    append_coverage_compiler_flags()
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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")  #  required on some linux clusters 

if(DEFINED ENV{BOOST_LIBRARYDIR})
	message("Detected environment variable BOOST_LIBRARYDIR, adding to CMAKE_CXX_FLAGS")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L$ENV{BOOST_LIBRARYDIR}")  #  required on some linux clusters 
endif()

## section: add target
ADD_LIBRARY(${this_target} SHARED ${${this_target}_SOURCE_FILES} )
set_target_properties(${this_target} PROPERTIES
  OUTPUT_NAME ${TARGET_LIB_NAME}
  VERSION ${PROJECT_VERSION}
  SOVERSION ${TARGET_MAJOR}
  # CLEAN_DIRECT_OUTPUT 1
)

## section: set link libraries
TARGET_LINK_LIBRARIES( ${this_target}
    ${NETCDF_LIBRARIES}
    ${Boost_LIBRARIES}
    ${MOIRAI_LIBRARIES}
    ${YAMLCPP_LIBRARIES}
)

# Files to be installed
INSTALL(FILES ${${this_target}_HEADER_FILES} DESTINATION include/datatypes)
configure_file(${UCHRONIA_LIB_NAME}.pc.in ${UCHRONIA_LIB_NAME}.pc @ONLY)

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
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
  )
else()
INSTALL(TARGETS ${this_target}
	# EXPORT libmoirai # apparently not in favor
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	# LIBRARY DESTINATION lib
	)
	install(FILES ${CMAKE_BINARY_DIR}/${UCHRONIA_LIB_NAME}.pc
    # DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig) from examplar, but this appears not prevalent, most I see are under /usr/lib/x86_64-linux-gnu/pkgconfig, so:
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    # The above also prevents a lintian error
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
