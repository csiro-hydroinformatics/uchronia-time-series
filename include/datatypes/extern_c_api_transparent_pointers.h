// This file deliberately does not have a #pragma once directive

#include "datatypes/setup_exports.h"
#include "datatypes/interop_struct.h"
#include "datatypes/time_series_store.hpp"
#include "moirai/reference_handle.hpp"

using namespace datatypes::timeseries;

#define DATATYPES_TIME_SERIES_DOUBLE TimeSeries
#define DATATYPES_TIME_SERIES_DOUBLE_PTR DATATYPES_TIME_SERIES_DOUBLE*
#define DATATYPES_ENSEMBLE_TIME_SERIES_DOUBLE MultiTimeSeries<DATATYPES_TIME_SERIES_DOUBLE>
#define DATATYPES_ENSEMBLE_PTR_TIME_SERIES_DOUBLE MultiTimeSeries<DATATYPES_TIME_SERIES_DOUBLE_PTR>
#define ENSEMBLE_FORECAST_TIME_SERIES_DOUBLE EnsembleForecastTimeSeries<DATATYPES_TIME_SERIES_DOUBLE>

#define TS_GEOMETRY_PTR  regular_time_series_geometry*
#define DATE_TIME_INFO_PTR  date_time_to_second*

///////////////////////////////////////////////
// Definitions of macros for reference_handle<>
///////////////////////////////////////////////

using moirai::reference_handle;

#define TIME_SERIES_TRANSPARENT_PTR  reference_handle<DATATYPES_TIME_SERIES_DOUBLE>*
#define ENSEMBLE_DATA_SET_TRANSPARENT_PTR  reference_handle<TimeSeriesLibrary>*
#define ENSEMBLE_FORECAST_TIME_SERIES_TRANSPARENT_PTR  reference_handle<ENSEMBLE_FORECAST_TIME_SERIES_DOUBLE>*
#define ENSEMBLE_TIME_SERIES_TRANSPARENT_PTR  reference_handle<DATATYPES_ENSEMBLE_TIME_SERIES_DOUBLE>*
#define ENSEMBLE_PTR_TIME_SERIES_TRANSPARENT_PTR  reference_handle<DATATYPES_ENSEMBLE_PTR_TIME_SERIES_DOUBLE>*
#define TIME_SERIES_PROVIDER_TRANSPARENT_PTR  reference_handle<TimeSeriesProvider<double>>*


