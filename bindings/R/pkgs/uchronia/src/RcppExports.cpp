// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "../inst/include/uchronia.h"
#include <Rcpp.h>
// following line added by R function custom_compileAttributes
#ifdef _WIN32
#define RcppExport extern "C" __declspec(dllexport)
#endif



using namespace Rcpp;

// RegisterExceptionCallback_Pkg
void RegisterExceptionCallback_Pkg();
RcppExport SEXP uchronia_RegisterExceptionCallback_Pkg() {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    RegisterExceptionCallback_Pkg();
    return R_NilValue;
END_RCPP
}
// GetLastStdExceptionMessage_Rcpp
CharacterVector GetLastStdExceptionMessage_Rcpp();
RcppExport SEXP uchronia_GetLastStdExceptionMessage_Rcpp() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(GetLastStdExceptionMessage_Rcpp());
    return rcpp_result_gen;
END_RCPP
}
// RegisterExceptionCallback_Rcpp
void RegisterExceptionCallback_Rcpp(XPtr<OpaquePointer> callback);
RcppExport SEXP uchronia_RegisterExceptionCallback_Rcpp(SEXP callbackSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type callback(callbackSEXP);
    RegisterExceptionCallback_Rcpp(callback);
    return R_NilValue;
END_RCPP
}
// DisposeSharedPointer_Rcpp
void DisposeSharedPointer_Rcpp(XPtr<OpaquePointer> ptr);
RcppExport SEXP uchronia_DisposeSharedPointer_Rcpp(SEXP ptrSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type ptr(ptrSEXP);
    DisposeSharedPointer_Rcpp(ptr);
    return R_NilValue;
END_RCPP
}
// DeleteDoubleArray_Rcpp
void DeleteDoubleArray_Rcpp(NumericVector value);
RcppExport SEXP uchronia_DeleteDoubleArray_Rcpp(SEXP valueSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericVector >::type value(valueSEXP);
    DeleteDoubleArray_Rcpp(value);
    return R_NilValue;
END_RCPP
}
// SetTimeSeriesMissingValueValue_Rcpp
void SetTimeSeriesMissingValueValue_Rcpp(NumericVector missingValueValue);
RcppExport SEXP uchronia_SetTimeSeriesMissingValueValue_Rcpp(SEXP missingValueValueSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericVector >::type missingValueValue(missingValueValueSEXP);
    SetTimeSeriesMissingValueValue_Rcpp(missingValueValue);
    return R_NilValue;
END_RCPP
}
// LoadEnsembleDataset_Rcpp
XPtr<OpaquePointer> LoadEnsembleDataset_Rcpp(CharacterVector filename, CharacterVector dataPath);
RcppExport SEXP uchronia_LoadEnsembleDataset_Rcpp(SEXP filenameSEXP, SEXP dataPathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataPath(dataPathSEXP);
    rcpp_result_gen = Rcpp::wrap(LoadEnsembleDataset_Rcpp(filename, dataPath));
    return rcpp_result_gen;
END_RCPP
}
// CreateEnsembleDataset_Rcpp
XPtr<OpaquePointer> CreateEnsembleDataset_Rcpp(CharacterVector type);
RcppExport SEXP uchronia_CreateEnsembleDataset_Rcpp(SEXP typeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type type(typeSEXP);
    rcpp_result_gen = Rcpp::wrap(CreateEnsembleDataset_Rcpp(type));
    return rcpp_result_gen;
END_RCPP
}
// GetEnsembleDatasetDataIdentifiers_Rcpp
CharacterVector GetEnsembleDatasetDataIdentifiers_Rcpp(XPtr<OpaquePointer> dataLibrary);
RcppExport SEXP uchronia_GetEnsembleDatasetDataIdentifiers_Rcpp(SEXP dataLibrarySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type dataLibrary(dataLibrarySEXP);
    rcpp_result_gen = Rcpp::wrap(GetEnsembleDatasetDataIdentifiers_Rcpp(dataLibrary));
    return rcpp_result_gen;
END_RCPP
}
// GetEnsembleDatasetDataSubIdentifiers_Rcpp
CharacterVector GetEnsembleDatasetDataSubIdentifiers_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector dataCollectionId);
RcppExport SEXP uchronia_GetEnsembleDatasetDataSubIdentifiers_Rcpp(SEXP dataLibrarySEXP, SEXP dataCollectionIdSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataCollectionId(dataCollectionIdSEXP);
    rcpp_result_gen = Rcpp::wrap(GetEnsembleDatasetDataSubIdentifiers_Rcpp(dataLibrary, dataCollectionId));
    return rcpp_result_gen;
END_RCPP
}
// GetEnsembleDatasetDataSummaries_Rcpp
CharacterVector GetEnsembleDatasetDataSummaries_Rcpp(XPtr<OpaquePointer> dataLibrary);
RcppExport SEXP uchronia_GetEnsembleDatasetDataSummaries_Rcpp(SEXP dataLibrarySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type dataLibrary(dataLibrarySEXP);
    rcpp_result_gen = Rcpp::wrap(GetEnsembleDatasetDataSummaries_Rcpp(dataLibrary));
    return rcpp_result_gen;
END_RCPP
}
// CreateEnsembleForecastTimeSeries_Rcpp
XPtr<OpaquePointer> CreateEnsembleForecastTimeSeries_Rcpp(Rcpp::Datetime start, IntegerVector length, CharacterVector timeStepName);
RcppExport SEXP uchronia_CreateEnsembleForecastTimeSeries_Rcpp(SEXP startSEXP, SEXP lengthSEXP, SEXP timeStepNameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::Datetime >::type start(startSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type length(lengthSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type timeStepName(timeStepNameSEXP);
    rcpp_result_gen = Rcpp::wrap(CreateEnsembleForecastTimeSeries_Rcpp(start, length, timeStepName));
    return rcpp_result_gen;
END_RCPP
}
// GetDatasetSingleTimeSeries_Rcpp
XPtr<OpaquePointer> GetDatasetSingleTimeSeries_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector dataId);
RcppExport SEXP uchronia_GetDatasetSingleTimeSeries_Rcpp(SEXP dataLibrarySEXP, SEXP dataIdSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataId(dataIdSEXP);
    rcpp_result_gen = Rcpp::wrap(GetDatasetSingleTimeSeries_Rcpp(dataLibrary, dataId));
    return rcpp_result_gen;
END_RCPP
}
// GetDatasetEnsembleTimeSeries_Rcpp
XPtr<OpaquePointer> GetDatasetEnsembleTimeSeries_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector dataEnsembleId);
RcppExport SEXP uchronia_GetDatasetEnsembleTimeSeries_Rcpp(SEXP dataLibrarySEXP, SEXP dataEnsembleIdSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataEnsembleId(dataEnsembleIdSEXP);
    rcpp_result_gen = Rcpp::wrap(GetDatasetEnsembleTimeSeries_Rcpp(dataLibrary, dataEnsembleId));
    return rcpp_result_gen;
END_RCPP
}
// GetEnsembleTimeSeriesData_Rcpp
XPtr<OpaquePointer> GetEnsembleTimeSeriesData_Rcpp(XPtr<OpaquePointer> ensSeries);
RcppExport SEXP uchronia_GetEnsembleTimeSeriesData_Rcpp(SEXP ensSeriesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type ensSeries(ensSeriesSEXP);
    rcpp_result_gen = Rcpp::wrap(GetEnsembleTimeSeriesData_Rcpp(ensSeries));
    return rcpp_result_gen;
END_RCPP
}
// GetDatasetEnsembleForecastTimeSeries_Rcpp
XPtr<OpaquePointer> GetDatasetEnsembleForecastTimeSeries_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector dataId);
RcppExport SEXP uchronia_GetDatasetEnsembleForecastTimeSeries_Rcpp(SEXP dataLibrarySEXP, SEXP dataIdSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataId(dataIdSEXP);
    rcpp_result_gen = Rcpp::wrap(GetDatasetEnsembleForecastTimeSeries_Rcpp(dataLibrary, dataId));
    return rcpp_result_gen;
END_RCPP
}
// GetTimeSeriesGeometry_Rcpp
void GetTimeSeriesGeometry_Rcpp(XPtr<OpaquePointer> timeSeries, const Rcpp::S4& geom);
RcppExport SEXP uchronia_GetTimeSeriesGeometry_Rcpp(SEXP timeSeriesSEXP, SEXP geomSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type timeSeries(timeSeriesSEXP);
    Rcpp::traits::input_parameter< const Rcpp::S4& >::type geom(geomSEXP);
    GetTimeSeriesGeometry_Rcpp(timeSeries, geom);
    return R_NilValue;
END_RCPP
}
// GetEnsembleForecastTimeSeriesGeometry_Rcpp
void GetEnsembleForecastTimeSeriesGeometry_Rcpp(XPtr<OpaquePointer> timeSeries, const Rcpp::S4& geom);
RcppExport SEXP uchronia_GetEnsembleForecastTimeSeriesGeometry_Rcpp(SEXP timeSeriesSEXP, SEXP geomSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type timeSeries(timeSeriesSEXP);
    Rcpp::traits::input_parameter< const Rcpp::S4& >::type geom(geomSEXP);
    GetEnsembleForecastTimeSeriesGeometry_Rcpp(timeSeries, geom);
    return R_NilValue;
END_RCPP
}
// GetTimeSeriesValues_Rcpp
void GetTimeSeriesValues_Rcpp(XPtr<OpaquePointer> timeSeries, NumericVector values, IntegerVector arrayLength);
RcppExport SEXP uchronia_GetTimeSeriesValues_Rcpp(SEXP timeSeriesSEXP, SEXP valuesSEXP, SEXP arrayLengthSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type timeSeries(timeSeriesSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type values(valuesSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type arrayLength(arrayLengthSEXP);
    GetTimeSeriesValues_Rcpp(timeSeries, values, arrayLength);
    return R_NilValue;
END_RCPP
}
// GetNumTimeSeries_Rcpp
IntegerVector GetNumTimeSeries_Rcpp();
RcppExport SEXP uchronia_GetNumTimeSeries_Rcpp() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(GetNumTimeSeries_Rcpp());
    return rcpp_result_gen;
END_RCPP
}
// GetProviderTsGeometry_Rcpp
void GetProviderTsGeometry_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector variableIdentifier, const Rcpp::S4& geom);
RcppExport SEXP uchronia_GetProviderTsGeometry_Rcpp(SEXP dataLibrarySEXP, SEXP variableIdentifierSEXP, SEXP geomSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type variableIdentifier(variableIdentifierSEXP);
    Rcpp::traits::input_parameter< const Rcpp::S4& >::type geom(geomSEXP);
    GetProviderTsGeometry_Rcpp(dataLibrary, variableIdentifier, geom);
    return R_NilValue;
END_RCPP
}
// GetProviderTimeSeriesValues_Rcpp
void GetProviderTimeSeriesValues_Rcpp(XPtr<OpaquePointer> dataLibrary, CharacterVector variableIdentifier, NumericVector values, IntegerVector arrayLength);
RcppExport SEXP uchronia_GetProviderTimeSeriesValues_Rcpp(SEXP dataLibrarySEXP, SEXP variableIdentifierSEXP, SEXP valuesSEXP, SEXP arrayLengthSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type variableIdentifier(variableIdentifierSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type values(valuesSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type arrayLength(arrayLengthSEXP);
    GetProviderTimeSeriesValues_Rcpp(dataLibrary, variableIdentifier, values, arrayLength);
    return R_NilValue;
END_RCPP
}
// GetProviderTimeSeriesIdentifiers_Rcpp
CharacterVector GetProviderTimeSeriesIdentifiers_Rcpp(XPtr<OpaquePointer> dataLibrary);
RcppExport SEXP uchronia_GetProviderTimeSeriesIdentifiers_Rcpp(SEXP dataLibrarySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<OpaquePointer> >::type dataLibrary(dataLibrarySEXP);
    rcpp_result_gen = Rcpp::wrap(GetProviderTimeSeriesIdentifiers_Rcpp(dataLibrary));
    return rcpp_result_gen;
END_RCPP
}
