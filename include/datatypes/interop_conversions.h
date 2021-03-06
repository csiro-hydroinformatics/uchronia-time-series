#pragma once

#include "cinterop/c_cpp_interop.hpp"
#include "datatypes/common.h"
#include "datatypes/time_series_io.hpp"
#include "datatypes/internals_c_api.hpp"

using namespace cinterop::utils;
using namespace datatypes::timeseries;

DATATYPES_DLL_LIB TimeSeries CreateTimeSeries(double * values, const regular_time_series_geometry& g);
DATATYPES_DLL_LIB TimeSeries CreateTimeSeries(double * values, TS_GEOMETRY_PTR geom);
DATATYPES_DLL_LIB TimeSeries CreateTimeSeries(const multi_regular_time_series_data& g);
DATATYPES_DLL_LIB TimeSeriesEnsemble<TimeSeries> ToTimeSeriesEnsemble(const multi_regular_time_series_data& rawData);
DATATYPES_DLL_LIB TimeSeriesEnsemble<TimeSeries>* ToTimeSeriesEnsemblePtr(const multi_regular_time_series_data& rawData);
DATATYPES_DLL_LIB multi_regular_time_series_data* ToMultiTimeSeriesDataPtr(const TimeSeriesEnsemble<TimeSeries>& mts);
DATATYPES_DLL_LIB multi_regular_time_series_data* ToMultiTimeSeriesDataPtr(const TimeSeries& ts);
DATATYPES_DLL_LIB TimeSeries* SingleTsPtrFromMultiTimeSeriesData(const multi_regular_time_series_data& ts);
DATATYPES_DLL_LIB TimeSeriesEnsemble<TimeSeries>* MultiTsPtrFromMultiTimeSeriesData(const multi_regular_time_series_data& ts);
DATATYPES_DLL_LIB TimeSeries SingleTsFromMultiTimeSeriesData(const multi_regular_time_series_data& ts);
DATATYPES_DLL_LIB TimeSeriesEnsemble<TimeSeries> MultiTsFromMultiTimeSeriesData(const multi_regular_time_series_data& ts);
DATATYPES_DLL_LIB time_series_dimensions_description* ToTimeSeriesDimensionDescriptions(vector<DataDimensionDescriptor>& mts);
DATATYPES_DLL_LIB void CopyToMultiTimeSeriesData(const TimeSeriesEnsemble<TimeSeries>& mts, multi_regular_time_series_data& result);
DATATYPES_DLL_LIB void CopyToMultiTimeSeriesData(const TimeSeries& ts, multi_regular_time_series_data& result);
DATATYPES_DLL_LIB void CopyFromMultiTimeSeriesData(const multi_regular_time_series_data& interopdata, TimeSeries& ts);
DATATYPES_DLL_LIB void CopyFromMultiTimeSeriesData(const multi_regular_time_series_data& interopdata, TimeSeriesEnsemble<TimeSeries>& mts);
DATATYPES_DLL_LIB double** ToRawData(const TimeSeriesEnsemble<TimeSeries>& mts);
DATATYPES_DLL_LIB double* ToRawData(const TimeSeries& ts);
DATATYPES_DLL_LIB void DisposeMultiTimeSeriesData(multi_regular_time_series_data* d);
DATATYPES_DLL_LIB void DisposeTimeSeriesDimensionDescriptions(time_series_dimensions_description* d);
