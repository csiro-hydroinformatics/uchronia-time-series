#pragma once

//#define USING_SOMETHING

#ifdef _WIN32
#ifdef USING_DATATYPES
#define DATATYPES_DLL_LIB __declspec(dllimport)
#define TEMPLATE_SPECIALIZATION_EXTERN extern
#else
#define DATATYPES_DLL_LIB __declspec(dllexport)
#define TEMPLATE_SPECIALIZATION_EXTERN

// TO prevent warnings such as:
// Warning	C4251	'datatypes::io::IoHelper::DefaultFilePattern' : class 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>' needs to have dll - interface to be used by clients of class 'datatypes::io::IoHelper'
#pragma warning (disable : 4251)

#endif
#else
#define DATATYPES_DLL_LIB // nothing
#define TEMPLATE_SPECIALIZATION_EXTERN
#endif

// You can define the following to activate the Visual Leak Detector tool for visual C++
// https://vld.codeplex.com

//#define DATATYPES_USE_VLD

#ifdef DATATYPES_USE_VLD
#include<vld.h>
#endif

