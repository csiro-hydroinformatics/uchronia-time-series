
// This file was GENERATED
//Do NOT modify it manually, as you are very likely to lose work

#include <string>
#include <vector>
#include "moirai/opaque_pointers.hpp"
using moirai::opaque_pointer_handle;
#include "cinterop/c_cpp_interop.hpp"
#include "cinterop/timeseries_c_interop.h"
std::string GetLastStdExceptionMessage_cpp()
;
void RegisterExceptionCallback_cpp(opaque_pointer_handle* callback)
;
void DisposeSharedPointer_cpp(opaque_pointer_handle* ptr)
;
void SetTimeSeriesMissingValueValue_cpp(double missingValueValue)
;
opaque_pointer_handle* LoadEnsembleDataset_cpp(const std::string libraryIdentifier, const std::string dataPath)
;
opaque_pointer_handle* CreateEnsembleDataset_cpp(const std::string type)
;
std::vector<std::string> GetEnsembleDatasetDataIdentifiers_cpp(opaque_pointer_handle* dataLibrary)
;

std::vector<std::string> GetEnsembleDatasetDataSubIdentifiers_cpp(opaque_pointer_handle* dataLibrary, const std::string dataCollectionId)
;

std::vector<std::string> GetEnsembleDatasetDataSummaries_cpp(opaque_pointer_handle* dataLibrary);
int EnsembleSizeEnsembleTimeSeries_cpp(opaque_pointer_handle* ensSeries)
;
opaque_pointer_handle* CreateEnsembleForecastTimeSeries_cpp(date_time_to_second start, int length, const std::string timeStepName)
;
opaque_pointer_handle* GetDatasetSingleTimeSeries_cpp(opaque_pointer_handle* dataLibrary, const std::string dataId)
;
opaque_pointer_handle* GetDatasetEnsembleTimeSeries_cpp(opaque_pointer_handle* dataLibrary, const std::string dataEnsembleId)
;
opaque_pointer_handle* GetDatasetEnsembleForecastTimeSeries_cpp(opaque_pointer_handle* dataLibrary, const std::string dataId)
;
void SaveSingleTimeSeriesToNetcdf_cpp(opaque_pointer_handle* timeSeries, const std::string filename, bool overwrite)
;
void SaveEnsembleTimeSeriesToNetcdf_cpp(opaque_pointer_handle* collection, const std::string filename, bool overwrite)
;
void SaveEnsembleForecastTimeSeriesToNetcdf_cpp(opaque_pointer_handle* tsEnsTs, const std::string filename, bool overwrite)
;
bool IsMissingValueItemEnsembleForecastTimeSeries_cpp(opaque_pointer_handle* series, int i)
;
opaque_pointer_handle* TimeSeriesFromEnsembleOfTimeSeries_cpp(opaque_pointer_handle* collectionTs, int index)
;
opaque_pointer_handle* TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_cpp(opaque_pointer_handle* efts, int indexInIssueTime, int indexInForecastTime)
;
double GetValueFromUnivariateTimeSeries_cpp(opaque_pointer_handle* ts, int index)
;
void TransformEachItem_cpp(opaque_pointer_handle* tsEnsTs, const std::string method, const std::string methodArgument)
;
void SetValueToUnivariateTimeSeries_cpp(opaque_pointer_handle* ts, int index, double value)
;
opaque_pointer_handle* GetItemEnsembleTimeSeriesAsStructure_cpp(opaque_pointer_handle* series, int i)
;
void SetItemEnsembleTimeSeriesAsStructure_cpp(opaque_pointer_handle* collection, int i, multi_regular_time_series_data& values)
;
opaque_pointer_handle* CreatePerfectForecastTimeSeries_cpp(opaque_pointer_handle* observations, date_time_to_second start, int length, const std::string timeStepName, int offsetForecasts, int leadTime)
;
opaque_pointer_handle* ToStructEnsembleTimeSeriesData_cpp(opaque_pointer_handle* ensSeries)
;
opaque_pointer_handle* ToStructSingleTimeSeriesData_cpp(opaque_pointer_handle* timeSeries)
;
opaque_pointer_handle* CreateEnsembleTimeSeriesDataFromStruct_cpp(multi_regular_time_series_data& ensSeries)
;
opaque_pointer_handle* CreateSingleTimeSeriesDataFromStruct_cpp(multi_regular_time_series_data& timeSeries)
;
void GetTimeSeriesGeometry_cpp(opaque_pointer_handle* timeSeries, regular_time_series_geometry& geom)
;
void GetEnsembleForecastTimeSeriesGeometry_cpp(opaque_pointer_handle* timeSeries, regular_time_series_geometry& geom)
;
int GetNumTimeSeries_cpp()
;
void GetProviderTsGeometry_cpp(opaque_pointer_handle* dataLibrary, const std::string variableIdentifier, regular_time_series_geometry& geom)
;
std::vector<std::string> GetProviderTimeSeriesIdentifiers_cpp(opaque_pointer_handle* dataLibrary)
;
opaque_pointer_handle* TimeSeriesFromProviderTs_cpp(opaque_pointer_handle* dataLibrary, const std::string variableIdentifier)
;
