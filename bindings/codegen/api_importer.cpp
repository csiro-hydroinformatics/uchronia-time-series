// This file is an entry point to extract a simpler C header file of the uchronia API 
// more amenable to be used for compiling bindings (Python, Matlab, ...)

#ifndef USING_DATATYPES_CORE
#define USING_DATATYPES_CORE
#endif 

#ifdef _WIN32
#define STRDUP _strdup
#else
#define STRDUP strdup
#endif

#define DATATYPES_USE_OPAQUE_POINTERS

#include "../../include/datatypes/extern_c_api.h"

