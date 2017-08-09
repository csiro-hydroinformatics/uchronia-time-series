// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "../inst/include/uchronia.h"
#include <Rcpp.h>
// following line added by R function custom_rcpp_compile_attributes
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
// GetTimeSeriesGeometry_Pkg
Rcpp::S4 GetTimeSeriesGeometry_Pkg(XPtr<opaque_pointer_handle> timeSeries);
RcppExport SEXP uchronia_GetTimeSeriesGeometry_Pkg(SEXP timeSeriesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type timeSeries(timeSeriesSEXP);
    rcpp_result_gen = Rcpp::wrap(GetTimeSeriesGeometry_Pkg(timeSeries));
    return rcpp_result_gen;
END_RCPP
}
// GetEnsembleForecastTimeSeriesGeometry_Pkg
Rcpp::S4 GetEnsembleForecastTimeSeriesGeometry_Pkg(XPtr<opaque_pointer_handle> timeSeries);
RcppExport SEXP uchronia_GetEnsembleForecastTimeSeriesGeometry_Pkg(SEXP timeSeriesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type timeSeries(timeSeriesSEXP);
    rcpp_result_gen = Rcpp::wrap(GetEnsembleForecastTimeSeriesGeometry_Pkg(timeSeries));
    return rcpp_result_gen;
END_RCPP
}
// GetItemEnsembleForecastTimeSeries_Pkg
Rcpp::S4 GetItemEnsembleForecastTimeSeries_Pkg(XPtr<opaque_pointer_handle> series, IntegerVector i);
RcppExport SEXP uchronia_GetItemEnsembleForecastTimeSeries_Pkg(SEXP seriesSEXP, SEXP iSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type series(seriesSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type i(iSEXP);
    rcpp_result_gen = Rcpp::wrap(GetItemEnsembleForecastTimeSeries_Pkg(series, i));
    return rcpp_result_gen;
END_RCPP
}
// SetItemEnsembleForecastTimeSeries_Pkg
void SetItemEnsembleForecastTimeSeries_Pkg(XPtr<opaque_pointer_handle> series, IntegerVector i, Rcpp::S4& timeSeriesEnsemble);
RcppExport SEXP uchronia_SetItemEnsembleForecastTimeSeries_Pkg(SEXP seriesSEXP, SEXP iSEXP, SEXP timeSeriesEnsembleSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type series(seriesSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type i(iSEXP);
    Rcpp::traits::input_parameter< Rcpp::S4& >::type timeSeriesEnsemble(timeSeriesEnsembleSEXP);
    SetItemEnsembleForecastTimeSeries_Pkg(series, i, timeSeriesEnsemble);
    return R_NilValue;
END_RCPP
}
// GetEnsembleTimeSeries_Pkg
Rcpp::S4 GetEnsembleTimeSeries_Pkg(XPtr<opaque_pointer_handle> series);
RcppExport SEXP uchronia_GetEnsembleTimeSeries_Pkg(SEXP seriesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type series(seriesSEXP);
    rcpp_result_gen = Rcpp::wrap(GetEnsembleTimeSeries_Pkg(series));
    return rcpp_result_gen;
END_RCPP
}
// TimeSeriesToTsInfo_Pkg
List TimeSeriesToTsInfo_Pkg(XPtr<opaque_pointer_handle> timeSeries);
RcppExport SEXP uchronia_TimeSeriesToTsInfo_Pkg(SEXP timeSeriesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type timeSeries(timeSeriesSEXP);
    rcpp_result_gen = Rcpp::wrap(TimeSeriesToTsInfo_Pkg(timeSeries));
    return rcpp_result_gen;
END_RCPP
}
// GetDatasetFromLibrary_Pkg
Rcpp::S4 GetDatasetFromLibrary_Pkg(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataIdentifier);
RcppExport SEXP uchronia_GetDatasetFromLibrary_Pkg(SEXP dataLibrarySEXP, SEXP dataIdentifierSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataIdentifier(dataIdentifierSEXP);
    rcpp_result_gen = Rcpp::wrap(GetDatasetFromLibrary_Pkg(dataLibrary, dataIdentifier));
    return rcpp_result_gen;
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
void RegisterExceptionCallback_Rcpp(XPtr<opaque_pointer_handle> callback);
RcppExport SEXP uchronia_RegisterExceptionCallback_Rcpp(SEXP callbackSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type callback(callbackSEXP);
    RegisterExceptionCallback_Rcpp(callback);
    return R_NilValue;
END_RCPP
}
// DisposeSharedPointer_Rcpp
void DisposeSharedPointer_Rcpp(XPtr<opaque_pointer_handle> ptr);
RcppExport SEXP uchronia_DisposeSharedPointer_Rcpp(SEXP ptrSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type ptr(ptrSEXP);
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
XPtr<opaque_pointer_handle> LoadEnsembleDataset_Rcpp(CharacterVector libraryIdentifier, CharacterVector dataPath);
RcppExport SEXP uchronia_LoadEnsembleDataset_Rcpp(SEXP libraryIdentifierSEXP, SEXP dataPathSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< CharacterVector >::type libraryIdentifier(libraryIdentifierSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataPath(dataPathSEXP);
    rcpp_result_gen = Rcpp::wrap(LoadEnsembleDataset_Rcpp(libraryIdentifier, dataPath));
    return rcpp_result_gen;
END_RCPP
}
// CreateEnsembleDataset_Rcpp
XPtr<opaque_pointer_handle> CreateEnsembleDataset_Rcpp(CharacterVector type);
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
CharacterVector GetEnsembleDatasetDataIdentifiers_Rcpp(XPtr<opaque_pointer_handle> dataLibrary);
RcppExport SEXP uchronia_GetEnsembleDatasetDataIdentifiers_Rcpp(SEXP dataLibrarySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type dataLibrary(dataLibrarySEXP);
    rcpp_result_gen = Rcpp::wrap(GetEnsembleDatasetDataIdentifiers_Rcpp(dataLibrary));
    return rcpp_result_gen;
END_RCPP
}
// GetEnsembleDatasetDataSubIdentifiers_Rcpp
CharacterVector GetEnsembleDatasetDataSubIdentifiers_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataCollectionId);
RcppExport SEXP uchronia_GetEnsembleDatasetDataSubIdentifiers_Rcpp(SEXP dataLibrarySEXP, SEXP dataCollectionIdSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataCollectionId(dataCollectionIdSEXP);
    rcpp_result_gen = Rcpp::wrap(GetEnsembleDatasetDataSubIdentifiers_Rcpp(dataLibrary, dataCollectionId));
    return rcpp_result_gen;
END_RCPP
}
// GetEnsembleDatasetDataSummaries_Rcpp
CharacterVector GetEnsembleDatasetDataSummaries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary);
RcppExport SEXP uchronia_GetEnsembleDatasetDataSummaries_Rcpp(SEXP dataLibrarySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type dataLibrary(dataLibrarySEXP);
    rcpp_result_gen = Rcpp::wrap(GetEnsembleDatasetDataSummaries_Rcpp(dataLibrary));
    return rcpp_result_gen;
END_RCPP
}
// CreateEnsembleForecastTimeSeries_Rcpp
XPtr<opaque_pointer_handle> CreateEnsembleForecastTimeSeries_Rcpp(Rcpp::Datetime start, IntegerVector length, CharacterVector timeStepName);
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
XPtr<opaque_pointer_handle> GetDatasetSingleTimeSeries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataId);
RcppExport SEXP uchronia_GetDatasetSingleTimeSeries_Rcpp(SEXP dataLibrarySEXP, SEXP dataIdSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataId(dataIdSEXP);
    rcpp_result_gen = Rcpp::wrap(GetDatasetSingleTimeSeries_Rcpp(dataLibrary, dataId));
    return rcpp_result_gen;
END_RCPP
}
// GetDatasetEnsembleTimeSeries_Rcpp
XPtr<opaque_pointer_handle> GetDatasetEnsembleTimeSeries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataEnsembleId);
RcppExport SEXP uchronia_GetDatasetEnsembleTimeSeries_Rcpp(SEXP dataLibrarySEXP, SEXP dataEnsembleIdSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataEnsembleId(dataEnsembleIdSEXP);
    rcpp_result_gen = Rcpp::wrap(GetDatasetEnsembleTimeSeries_Rcpp(dataLibrary, dataEnsembleId));
    return rcpp_result_gen;
END_RCPP
}
// GetDatasetEnsembleForecastTimeSeries_Rcpp
XPtr<opaque_pointer_handle> GetDatasetEnsembleForecastTimeSeries_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector dataId);
RcppExport SEXP uchronia_GetDatasetEnsembleForecastTimeSeries_Rcpp(SEXP dataLibrarySEXP, SEXP dataIdSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type dataId(dataIdSEXP);
    rcpp_result_gen = Rcpp::wrap(GetDatasetEnsembleForecastTimeSeries_Rcpp(dataLibrary, dataId));
    return rcpp_result_gen;
END_RCPP
}
// TimeSeriesFromEnsembleOfTimeSeries_Rcpp
XPtr<opaque_pointer_handle> TimeSeriesFromEnsembleOfTimeSeries_Rcpp(XPtr<opaque_pointer_handle> collectionTs, IntegerVector index);
RcppExport SEXP uchronia_TimeSeriesFromEnsembleOfTimeSeries_Rcpp(SEXP collectionTsSEXP, SEXP indexSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type collectionTs(collectionTsSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type index(indexSEXP);
    rcpp_result_gen = Rcpp::wrap(TimeSeriesFromEnsembleOfTimeSeries_Rcpp(collectionTs, index));
    return rcpp_result_gen;
END_RCPP
}
// TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_Rcpp
XPtr<opaque_pointer_handle> TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_Rcpp(XPtr<opaque_pointer_handle> efts, IntegerVector indexInIssueTime, IntegerVector indexInForecastTime);
RcppExport SEXP uchronia_TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_Rcpp(SEXP eftsSEXP, SEXP indexInIssueTimeSEXP, SEXP indexInForecastTimeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type efts(eftsSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type indexInIssueTime(indexInIssueTimeSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type indexInForecastTime(indexInForecastTimeSEXP);
    rcpp_result_gen = Rcpp::wrap(TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_Rcpp(efts, indexInIssueTime, indexInForecastTime));
    return rcpp_result_gen;
END_RCPP
}
// GetValueFromUnivariateTimeSeries_Rcpp
NumericVector GetValueFromUnivariateTimeSeries_Rcpp(XPtr<opaque_pointer_handle> ts, IntegerVector index);
RcppExport SEXP uchronia_GetValueFromUnivariateTimeSeries_Rcpp(SEXP tsSEXP, SEXP indexSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type ts(tsSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type index(indexSEXP);
    rcpp_result_gen = Rcpp::wrap(GetValueFromUnivariateTimeSeries_Rcpp(ts, index));
    return rcpp_result_gen;
END_RCPP
}
// SetValueToUnivariateTimeSeries_Rcpp
void SetValueToUnivariateTimeSeries_Rcpp(XPtr<opaque_pointer_handle> ts, IntegerVector index, NumericVector value);
RcppExport SEXP uchronia_SetValueToUnivariateTimeSeries_Rcpp(SEXP tsSEXP, SEXP indexSEXP, SEXP valueSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type ts(tsSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type index(indexSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type value(valueSEXP);
    SetValueToUnivariateTimeSeries_Rcpp(ts, index, value);
    return R_NilValue;
END_RCPP
}
// GetItemEnsembleTimeSeriesAsStructure_Rcpp
XPtr<opaque_pointer_handle> GetItemEnsembleTimeSeriesAsStructure_Rcpp(XPtr<opaque_pointer_handle> series, IntegerVector i);
RcppExport SEXP uchronia_GetItemEnsembleTimeSeriesAsStructure_Rcpp(SEXP seriesSEXP, SEXP iSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type series(seriesSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type i(iSEXP);
    rcpp_result_gen = Rcpp::wrap(GetItemEnsembleTimeSeriesAsStructure_Rcpp(series, i));
    return rcpp_result_gen;
END_RCPP
}
// SetItemEnsembleTimeSeriesAsStructure_Rcpp
void SetItemEnsembleTimeSeriesAsStructure_Rcpp(XPtr<opaque_pointer_handle> collection, IntegerVector i, const Rcpp::S4& values);
RcppExport SEXP uchronia_SetItemEnsembleTimeSeriesAsStructure_Rcpp(SEXP collectionSEXP, SEXP iSEXP, SEXP valuesSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type collection(collectionSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type i(iSEXP);
    Rcpp::traits::input_parameter< const Rcpp::S4& >::type values(valuesSEXP);
    SetItemEnsembleTimeSeriesAsStructure_Rcpp(collection, i, values);
    return R_NilValue;
END_RCPP
}
// CreatePerfectForecastTimeSeries_Rcpp
XPtr<opaque_pointer_handle> CreatePerfectForecastTimeSeries_Rcpp(XPtr<opaque_pointer_handle> observations, Rcpp::Datetime start, IntegerVector length, CharacterVector timeStepName, IntegerVector offsetForecasts, IntegerVector leadTime);
RcppExport SEXP uchronia_CreatePerfectForecastTimeSeries_Rcpp(SEXP observationsSEXP, SEXP startSEXP, SEXP lengthSEXP, SEXP timeStepNameSEXP, SEXP offsetForecastsSEXP, SEXP leadTimeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type observations(observationsSEXP);
    Rcpp::traits::input_parameter< Rcpp::Datetime >::type start(startSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type length(lengthSEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type timeStepName(timeStepNameSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type offsetForecasts(offsetForecastsSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type leadTime(leadTimeSEXP);
    rcpp_result_gen = Rcpp::wrap(CreatePerfectForecastTimeSeries_Rcpp(observations, start, length, timeStepName, offsetForecasts, leadTime));
    return rcpp_result_gen;
END_RCPP
}
// ToStructEnsembleTimeSeriesData_Rcpp
XPtr<opaque_pointer_handle> ToStructEnsembleTimeSeriesData_Rcpp(XPtr<opaque_pointer_handle> ensSeries);
RcppExport SEXP uchronia_ToStructEnsembleTimeSeriesData_Rcpp(SEXP ensSeriesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type ensSeries(ensSeriesSEXP);
    rcpp_result_gen = Rcpp::wrap(ToStructEnsembleTimeSeriesData_Rcpp(ensSeries));
    return rcpp_result_gen;
END_RCPP
}
// ToStructSingleTimeSeriesData_Rcpp
XPtr<opaque_pointer_handle> ToStructSingleTimeSeriesData_Rcpp(XPtr<opaque_pointer_handle> timeSeries);
RcppExport SEXP uchronia_ToStructSingleTimeSeriesData_Rcpp(SEXP timeSeriesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type timeSeries(timeSeriesSEXP);
    rcpp_result_gen = Rcpp::wrap(ToStructSingleTimeSeriesData_Rcpp(timeSeries));
    return rcpp_result_gen;
END_RCPP
}
// CreateEnsembleTimeSeriesDataFromStruct_Rcpp
XPtr<opaque_pointer_handle> CreateEnsembleTimeSeriesDataFromStruct_Rcpp(const Rcpp::S4& ensSeries);
RcppExport SEXP uchronia_CreateEnsembleTimeSeriesDataFromStruct_Rcpp(SEXP ensSeriesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const Rcpp::S4& >::type ensSeries(ensSeriesSEXP);
    rcpp_result_gen = Rcpp::wrap(CreateEnsembleTimeSeriesDataFromStruct_Rcpp(ensSeries));
    return rcpp_result_gen;
END_RCPP
}
// CreateSingleTimeSeriesDataFromStruct_Rcpp
XPtr<opaque_pointer_handle> CreateSingleTimeSeriesDataFromStruct_Rcpp(const Rcpp::S4& timeSeries);
RcppExport SEXP uchronia_CreateSingleTimeSeriesDataFromStruct_Rcpp(SEXP timeSeriesSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const Rcpp::S4& >::type timeSeries(timeSeriesSEXP);
    rcpp_result_gen = Rcpp::wrap(CreateSingleTimeSeriesDataFromStruct_Rcpp(timeSeries));
    return rcpp_result_gen;
END_RCPP
}
// GetTimeSeriesGeometry_Rcpp
void GetTimeSeriesGeometry_Rcpp(XPtr<opaque_pointer_handle> timeSeries, const Rcpp::S4& geom);
RcppExport SEXP uchronia_GetTimeSeriesGeometry_Rcpp(SEXP timeSeriesSEXP, SEXP geomSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type timeSeries(timeSeriesSEXP);
    Rcpp::traits::input_parameter< const Rcpp::S4& >::type geom(geomSEXP);
    GetTimeSeriesGeometry_Rcpp(timeSeries, geom);
    return R_NilValue;
END_RCPP
}
// GetEnsembleForecastTimeSeriesGeometry_Rcpp
void GetEnsembleForecastTimeSeriesGeometry_Rcpp(XPtr<opaque_pointer_handle> timeSeries, const Rcpp::S4& geom);
RcppExport SEXP uchronia_GetEnsembleForecastTimeSeriesGeometry_Rcpp(SEXP timeSeriesSEXP, SEXP geomSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type timeSeries(timeSeriesSEXP);
    Rcpp::traits::input_parameter< const Rcpp::S4& >::type geom(geomSEXP);
    GetEnsembleForecastTimeSeriesGeometry_Rcpp(timeSeries, geom);
    return R_NilValue;
END_RCPP
}
// GetTimeSeriesValues_Rcpp
void GetTimeSeriesValues_Rcpp(XPtr<opaque_pointer_handle> timeSeries, NumericVector values, IntegerVector arrayLength);
RcppExport SEXP uchronia_GetTimeSeriesValues_Rcpp(SEXP timeSeriesSEXP, SEXP valuesSEXP, SEXP arrayLengthSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type timeSeries(timeSeriesSEXP);
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
void GetProviderTsGeometry_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector variableIdentifier, const Rcpp::S4& geom);
RcppExport SEXP uchronia_GetProviderTsGeometry_Rcpp(SEXP dataLibrarySEXP, SEXP variableIdentifierSEXP, SEXP geomSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type variableIdentifier(variableIdentifierSEXP);
    Rcpp::traits::input_parameter< const Rcpp::S4& >::type geom(geomSEXP);
    GetProviderTsGeometry_Rcpp(dataLibrary, variableIdentifier, geom);
    return R_NilValue;
END_RCPP
}
// GetProviderTimeSeriesValues_Rcpp
void GetProviderTimeSeriesValues_Rcpp(XPtr<opaque_pointer_handle> dataLibrary, CharacterVector variableIdentifier, NumericVector values, IntegerVector arrayLength);
RcppExport SEXP uchronia_GetProviderTimeSeriesValues_Rcpp(SEXP dataLibrarySEXP, SEXP variableIdentifierSEXP, SEXP valuesSEXP, SEXP arrayLengthSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type dataLibrary(dataLibrarySEXP);
    Rcpp::traits::input_parameter< CharacterVector >::type variableIdentifier(variableIdentifierSEXP);
    Rcpp::traits::input_parameter< NumericVector >::type values(valuesSEXP);
    Rcpp::traits::input_parameter< IntegerVector >::type arrayLength(arrayLengthSEXP);
    GetProviderTimeSeriesValues_Rcpp(dataLibrary, variableIdentifier, values, arrayLength);
    return R_NilValue;
END_RCPP
}
// GetProviderTimeSeriesIdentifiers_Rcpp
CharacterVector GetProviderTimeSeriesIdentifiers_Rcpp(XPtr<opaque_pointer_handle> dataLibrary);
RcppExport SEXP uchronia_GetProviderTimeSeriesIdentifiers_Rcpp(SEXP dataLibrarySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtr<opaque_pointer_handle> >::type dataLibrary(dataLibrarySEXP);
    rcpp_result_gen = Rcpp::wrap(GetProviderTimeSeriesIdentifiers_Rcpp(dataLibrary));
    return rcpp_result_gen;
END_RCPP
}
