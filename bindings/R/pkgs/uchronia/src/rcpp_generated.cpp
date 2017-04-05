////////////////////////////////////
// 
// *** THIS FILE IS GENERATED ****
// DO NOT MODIFY IT MANUALLY, AS YOU ARE VERY LIKELY TO LOSE WORK
// 
////////////////////////////////////

#include "uchronia_wrapper_setup.h"
#include "cinterop/rcpp_interop.hpp"
#include "uchronia_r_exports.h"
#include "uchronia_struct_interop.h"

using namespace Rcpp;
using namespace cinterop::utils;

//////////// End of preamble ////////////
// [[Rcpp::export]]
CharacterVector GetLastStdExceptionMessage_Rcpp()
{
    auto result = GetLastStdExceptionMessage();
    auto x = CharacterVector(result);
    DeleteAnsiString(result);
    return x;
}

// [[Rcpp::export]]
void RegisterExceptionCallback_Rcpp(XPtr<OpaquePointer> callback)
{
    RegisterExceptionCallback(callback->Get());
}

// [[Rcpp::export]]
void DisposeSharedPointer_Rcpp(XPtr<OpaquePointer> ptr)
{
    DisposeSharedPointer(ptr->Get());
}

// [[Rcpp::export]]
void DeleteDoubleArray_Rcpp(NumericVector value)
{
    DeleteDoubleArray(&(value[0]));
}

// [[Rcpp::export]]
void SetTimeSeriesMissingValueValue_Rcpp(NumericVector missingValueValue)
{
    SetTimeSeriesMissingValueValue(as<double>(missingValueValue));
}

// [[Rcpp::export]]
XPtr<OpaquePointer> LoadEnsembleDataset_Rcpp(CharacterVector filename, CharacterVector dataPath)
{
    auto result = LoadEnsembleDataset(filename[0], dataPath[0]);
    auto x = XPtr<OpaquePointer>(new OpaquePointer(result));
    return x;
}

// [[Rcpp::export]]
XPtr<OpaquePointer> CreateEnsembleDataset_Rcpp(CharacterVector type)
{
    auto result = CreateEnsembleDataset(type[0]);
    auto x = XPtr<OpaquePointer>(new OpaquePointer(result));
    return x;
}


// [[Rcpp::export]]
CharacterVector GetEnsembleDatasetDataIdentifiers_Rcpp(XPtr<OpaquePointer> dataLibrary)
{
	int size; 
        char** values = GetEnsembleDatasetDataIdentifiers(dataLibrary->Get(),  &size);
        return to_custom_character_vector<CharacterVector>(values, size, true);
}


// [[Rcpp::export]]
CharacterVector GetEnsembleDatasetDataSubIdentifiers_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector dataCollectionId)
{
	int size; 
        char** values = GetEnsembleDatasetDataSubIdentifiers(dataLibrary->Get(), dataCollectionId[0],  &size);
        return to_custom_character_vector<CharacterVector>(values, size, true);
}


// [[Rcpp::export]]
CharacterVector GetEnsembleDatasetDataSummaries_Rcpp(XPtr<OpaquePointer> dataLibrary)
{
	int size; 
        char** values = GetEnsembleDatasetDataSummaries(dataLibrary->Get(),  &size);
        return to_custom_character_vector<CharacterVector>(values, size, true);
}

// [[Rcpp::export]]
XPtr<OpaquePointer> CreateEnsembleForecastTimeSeries_Rcpp(Rcpp::Datetime start, IntegerVector length, CharacterVector timeStepName)
{
    date_time_to_second start_datetime = to_date_time_to_second<Rcpp::Datetime>(start);
    auto result = CreateEnsembleForecastTimeSeries(start_datetime, as<int>(length), timeStepName[0]);
    // start_datetime - no cleanup needed
    auto x = XPtr<OpaquePointer>(new OpaquePointer(result));
    return x;
}

// [[Rcpp::export]]
XPtr<OpaquePointer> GetDatasetSingleTimeSeries_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector dataId)
{
    auto result = GetDatasetSingleTimeSeries(dataLibrary->Get(), dataId[0]);
    auto x = XPtr<OpaquePointer>(new OpaquePointer(result));
    return x;
}

// [[Rcpp::export]]
XPtr<OpaquePointer> GetDatasetEnsembleTimeSeries_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector dataEnsembleId)
{
    auto result = GetDatasetEnsembleTimeSeries(dataLibrary->Get(), dataEnsembleId[0]);
    auto x = XPtr<OpaquePointer>(new OpaquePointer(result));
    return x;
}

// [[Rcpp::export]]
XPtr<OpaquePointer> GetEnsembleTimeSeriesData_Rcpp(XPtr<OpaquePointer> ensSeries)
{
    auto result = GetEnsembleTimeSeriesData(ensSeries->Get());
    auto x = XPtr<OpaquePointer>(new OpaquePointer(result));
    return x;
}

// [[Rcpp::export]]
XPtr<OpaquePointer> GetDatasetEnsembleForecastTimeSeries_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector dataId)
{
    auto result = GetDatasetEnsembleForecastTimeSeries(dataLibrary->Get(), dataId[0]);
    auto x = XPtr<OpaquePointer>(new OpaquePointer(result));
    return x;
}

// [[Rcpp::export]]
void GetTimeSeriesGeometry_Rcpp(XPtr<OpaquePointer> timeSeries, const Rcpp::S4& geom)
{
    regular_time_series_geometry* geom_tsgeom = toMarshalledTsinfoPtr(geom);
    GetTimeSeriesGeometry(timeSeries->Get(), geom_tsgeom);
    delete geom_tsgeom;
}

// [[Rcpp::export]]
void GetEnsembleForecastTimeSeriesGeometry_Rcpp(XPtr<OpaquePointer> timeSeries, const Rcpp::S4& geom)
{
    regular_time_series_geometry* geom_tsgeom = toMarshalledTsinfoPtr(geom);
    GetEnsembleForecastTimeSeriesGeometry(timeSeries->Get(), geom_tsgeom);
    delete geom_tsgeom;
}

// [[Rcpp::export]]
void GetTimeSeriesValues_Rcpp(XPtr<OpaquePointer> timeSeries, NumericVector values, IntegerVector arrayLength)
{
    GetTimeSeriesValues(timeSeries->Get(), &(values[0]), as<int>(arrayLength));
}

// [[Rcpp::export]]
IntegerVector GetNumTimeSeries_Rcpp()
{
    auto result = GetNumTimeSeries();
    auto x = Rcpp::wrap(result);
    return x;
}

// [[Rcpp::export]]
void GetProviderTsGeometry_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector variableIdentifier, const Rcpp::S4& geom)
{
    regular_time_series_geometry* geom_tsgeom = toMarshalledTsinfoPtr(geom);
    GetProviderTsGeometry(dataLibrary->Get(), variableIdentifier[0], geom_tsgeom);
    delete geom_tsgeom;
}

// [[Rcpp::export]]
void GetProviderTimeSeriesValues_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector variableIdentifier, NumericVector values, IntegerVector arrayLength)
{
    GetProviderTimeSeriesValues(dataLibrary->Get(), variableIdentifier[0], &(values[0]), as<int>(arrayLength));
}


// [[Rcpp::export]]
CharacterVector GetProviderTimeSeriesIdentifiers_Rcpp(XPtr<OpaquePointer> dataLibrary)
{
	int size; 
        char** values = GetProviderTimeSeriesIdentifiers(dataLibrary->Get(),  &size);
        return to_custom_character_vector<CharacterVector>(values, size, true);
}

