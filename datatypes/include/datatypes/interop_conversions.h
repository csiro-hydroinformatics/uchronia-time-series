#pragma once

#include "cinterop/c_cpp_interop.hpp"
#include "datatypes/common.h"
#include "datatypes/time_series_io.hpp"
#include "datatypes/internals_c_api.hpp"

using namespace cinterop::utils;
using namespace datatypes::timeseries;

/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB TimeSeries CreateTimeSeries(double * values, const regular_time_series_geometry& g);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB TimeSeries CreateTimeSeries(double * values, TS_GEOMETRY_PTR geom);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB TimeSeries CreateTimeSeries(const multi_regular_time_series_data& g);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB TimeSeriesEnsemble<TimeSeries> ToTimeSeriesEnsemble(const multi_regular_time_series_data& rawData);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB TimeSeriesEnsemble<TimeSeries>* ToTimeSeriesEnsemblePtr(const multi_regular_time_series_data& rawData);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB multi_regular_time_series_data* ToMultiTimeSeriesDataPtr(const TimeSeriesEnsemble<TimeSeries>& mts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB multi_regular_time_series_data* ToMultiTimeSeriesDataPtr(const TimeSeries& ts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB TimeSeries* SingleTsPtrFromMultiTimeSeriesData(const multi_regular_time_series_data& ts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB TimeSeriesEnsemble<TimeSeries>* MultiTsPtrFromMultiTimeSeriesData(const multi_regular_time_series_data& ts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB TimeSeries SingleTsFromMultiTimeSeriesData(const multi_regular_time_series_data& ts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB TimeSeriesEnsemble<TimeSeries> MultiTsFromMultiTimeSeriesData(const multi_regular_time_series_data& ts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB time_series_dimensions_description* ToTimeSeriesDimensionDescriptions(vector<DataDimensionDescriptor>& mts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB void CopyToMultiTimeSeriesData(const TimeSeriesEnsemble<TimeSeries>& mts, multi_regular_time_series_data& result);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB void CopyToMultiTimeSeriesData(const TimeSeries& ts, multi_regular_time_series_data& result);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB void CopyFromMultiTimeSeriesData(const multi_regular_time_series_data& interopdata, TimeSeries& ts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB void CopyFromMultiTimeSeriesData(const multi_regular_time_series_data& interopdata, TimeSeriesEnsemble<TimeSeries>& mts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB double** ToRawData(const TimeSeriesEnsemble<TimeSeries>& mts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB double* ToRawData(const TimeSeries& ts);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB void DisposeMultiTimeSeriesData(multi_regular_time_series_data* d);
/**
 * \brief Helper function for C API interop conversions
 */
DATATYPES_DLL_LIB void DisposeTimeSeriesDimensionDescriptions(time_series_dimensions_description* d);
