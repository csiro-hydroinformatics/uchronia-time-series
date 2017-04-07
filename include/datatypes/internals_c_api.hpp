#pragma once

#include "moirai/reference_handle.hpp"
#include "datatypes/extern_c_api_transparent_pointers.h"

#define TIME_SERIES_DYNCAST(x) CHECKED_RETRIEVE_PTR(DATATYPES_TIME_SERIES_DOUBLE, x)
#define TIME_SERIES_PROVIDER_DYNCAST CHECKED_RETRIEVE_PTR(TimeSeriesProvider<double>, x)
#define ENSEMBLE_DATA_SET_DYNCAST(x) CHECKED_RETRIEVE_PTR(TimeSeriesLibrary, x)
#define ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(x) CHECKED_RETRIEVE_PTR(ENSEMBLE_FORECAST_TIME_SERIES_DOUBLE, x)
#define ENSEMBLE_PTR_TIME_SERIES_DYNCAST(x) CHECKED_RETRIEVE_PTR(DATATYPES_ENSEMBLE_PTR_TIME_SERIES_DOUBLE, x)

#define DATE_TIME_INFO_DYNCAST(x) (date_time_to_second*)x
#define TS_GEOMETRY_DYNCAST(x) x

#define FREE_ARRAY(x) delete[] x

#define WRAP_DATE_TIME_INFO_PTR(x)  new reference_handle<date_time_to_second>(x)
#define WRAP_TIME_SERIES_PROVIDER_PTR(x)  new reference_handle<TimeSeriesProvider<double>>(x)
#define WRAP_ENSEMBLE_DATA_SET_PTR(x)  new reference_handle<TimeSeriesLibrary>(x)
#define WRAP_TIME_SERIES_PTR(x)   new reference_handle<DATATYPES_TIME_SERIES_DOUBLE>(x)
#define WRAP_ENSEMBLE_FORECAST_TIME_SERIES_PTR(x)  new reference_handle<EnsembleForecastTimeSeries<DATATYPES_TIME_SERIES_DOUBLE>>(x)
#define WRAP_ENSEMBLE_TIME_SERIES_PTR(x)  new reference_handle<DATATYPES_ENSEMBLE_PTR_TIME_SERIES_DOUBLE>(x)
