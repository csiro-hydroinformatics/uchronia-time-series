#pragma once

#include <string.h>
#include "datatypes/setup_exports.h"
#include "cinterop/common_c_interop.h"

#ifdef _WIN32
#define STRDUP _strdup
#else
#define STRDUP strdup
#endif

typedef struct _regular_time_series_geometry
{
	date_time_to_second start;
	int time_step_seconds;
	int length;
} regular_time_series_geometry;

typedef struct _multi_regular_time_series_data
{
	regular_time_series_geometry time_series_geometry;
	int ensemble_size;
	double** numeric_data;
} multi_regular_time_series_data;

