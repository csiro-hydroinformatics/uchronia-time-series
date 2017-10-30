#pragma once

#include <cinterop/rcpp_strict_r_headers.hpp>

#ifndef USING_DATATYPES_CORE
#define USING_DATATYPES_CORE
#endif 

#ifndef USING_DATATYPES
#define USING_DATATYPES
#endif 

#ifndef STRDUP
#ifdef _WIN32
#define STRDUP _strdup
#else
#define STRDUP strdup
#endif
#endif 

#define DATATYPES_USE_OPAQUE_POINTERS

#include "datatypes/extern_c_api.h"

using namespace Rcpp;

