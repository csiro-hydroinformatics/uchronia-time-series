#pragma once
#include <stdio.h>
#include <atomic>
#include <string> 
#include <vector>
#include <map> 

#ifdef _WIN32
  #ifdef USING_SWIFT_CORE
    #define DLL_LIB __declspec(dllimport)
  #else
    #define DLL_LIB __declspec(dllexport)
  #endif
#else
  #define DLL_LIB // nothing
#endif

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

namespace datatypes
{
	namespace sys
	{
	}
}