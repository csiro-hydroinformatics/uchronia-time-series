#pragma once

#include "uchronia_r_exports.h"
#include "datatypes/interop_struct.h"

void toMarshalledTsinfo(const Rcpp::S4& rTsInfo, regular_time_series_geometry& mts);
regular_time_series_geometry toMarshalledTsinfo(const Rcpp::S4& rTsInfo);
regular_time_series_geometry* toMarshalledTsinfoPtr(const Rcpp::S4& rTsInfo);
NumericMatrix toNumericMatrix(const multi_regular_time_series_data& mts);
multi_regular_time_series_data toMultiTimeSeriesData(const Rcpp::S4& timeSeriesEnsemble);
Rcpp::S4 toRMultiTimeSeriesData(const multi_regular_time_series_data& mts);
Rcpp::S4 fromMarshalledTsinfo(const regular_time_series_geometry& mts);
void PkgDisposeMultiTimeSeriesData(multi_regular_time_series_data& d);