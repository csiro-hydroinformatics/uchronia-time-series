#pragma once

#include "uchronia_wrapper_setup.h"

// Pragmatic: these are generated in rcpp_generated.cpp but declarations are needed for custom. 
// Ideally should there be a .h file generated?

XPtr<opaque_pointer_handle> GetDatasetSingleTimeSeries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataId);
XPtr<opaque_pointer_handle> GetDatasetEnsembleTimeSeries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataEnsembleId);
XPtr<opaque_pointer_handle> GetDatasetEnsembleForecastTimeSeries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataEnsembleId);
