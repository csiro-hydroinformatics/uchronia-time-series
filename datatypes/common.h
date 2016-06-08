#pragma once
#include <stdio.h>
#include <atomic>
#include <string> 
#include <vector>
#include <map> 

#ifdef _WIN32
  #ifdef USING_DATATYPES
    #define DATATYPES_DLL_LIB __declspec(dllimport)
	#define TEMPLATE_SPECIALIZATION_EXTERN extern
  #else
    #define DATATYPES_DLL_LIB __declspec(dllexport)
	#define TEMPLATE_SPECIALIZATION_EXTERN
  #endif
#else
  #define DATATYPES_DLL_LIB // nothing
  #define TEMPLATE_SPECIALIZATION_EXTERN
#endif

#define DEFAULT_MISSING_DATA_VALUE -9999.0

// You can define the following to activate the Visual Leak Detector tool for visual C++
// https://vld.codeplex.com
//#define DATATYPES_USE_VLD

#ifdef DATATYPES_USE_VLD
  #include<vld.h>
#endif

#ifdef _WIN32
  #define STRDUP _strdup
#else
  #define STRDUP strdup
#endif

#define STLSTR_TO_ANSISTR(x) STRDUP(x.c_str())

using std::string;
using std::vector;