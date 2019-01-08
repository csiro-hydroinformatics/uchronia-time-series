
// This file was GENERATED
//Do NOT modify it manually, as you are very likely to lose work

#include "../include/uchronia_pb_exports.h"
#include "../include/cpp_bindings_generated.h"

std::string GetLastStdExceptionMessage_cpp()
{
    auto result = GetLastStdExceptionMessage();
    auto x = std::string(result);
    DeleteAnsiString(result);
    return x;}

void RegisterExceptionCallback_cpp(opaque_pointer_handle* callback)
{
    RegisterExceptionCallback(callback->get());
}

void DisposeSharedPointer_cpp(opaque_pointer_handle* ptr)
{
    DisposeSharedPointer(ptr->get());
}

void SetTimeSeriesMissingValueValue_cpp(double missingValueValue)
{
    SetTimeSeriesMissingValueValue((double)missingValueValue);
}

opaque_pointer_handle* LoadEnsembleDataset_cpp(const std::string libraryIdentifier, const std::string dataPath)
{
    auto result = LoadEnsembleDataset(libraryIdentifier.c_str(), dataPath.c_str());
    auto x = new opaque_pointer_handle(result);
    return x;}

opaque_pointer_handle* CreateEnsembleDataset_cpp(const std::string type)
{
    auto result = CreateEnsembleDataset(type.c_str());
    auto x = new opaque_pointer_handle(result);
    return x;}


std::vector<std::string> GetEnsembleDatasetDataIdentifiers_cpp(opaque_pointer_handle* dataLibrary)
{
	int size; 
	char** names = GetEnsembleDatasetDataIdentifiers(dataLibrary->get(),  &size);
	return cinterop::utils::to_cpp_string_vector(names, size);
}


std::vector<std::string> GetEnsembleDatasetDataSubIdentifiers_cpp(opaque_pointer_handle* dataLibrary, const std::string dataCollectionId)
{
	int size; 
	char** names = GetEnsembleDatasetDataSubIdentifiers(dataLibrary->get(), dataCollectionId.c_str(),  &size);
	return cinterop::utils::to_cpp_string_vector(names, size);
}


std::vector<std::string> GetEnsembleDatasetDataSummaries_cpp(opaque_pointer_handle* dataLibrary)
{
	int size; 
	char** names = GetEnsembleDatasetDataSummaries(dataLibrary->get(),  &size);
	return cinterop::utils::to_cpp_string_vector(names, size);
}

int EnsembleSizeEnsembleTimeSeries_cpp(opaque_pointer_handle* ensSeries)
{
    auto result = EnsembleSizeEnsembleTimeSeries(ensSeries->get());
    auto x = result;
    return x;}

opaque_pointer_handle* CreateEnsembleForecastTimeSeries_cpp(date_time_to_second start, int length, const std::string timeStepName)
{
    auto result = CreateEnsembleForecastTimeSeries(start, (int)length, timeStepName.c_str());
    auto x = new opaque_pointer_handle(result);
    return x;}

opaque_pointer_handle* GetDatasetSingleTimeSeries_cpp(opaque_pointer_handle* dataLibrary, const std::string dataId)
{
    auto result = GetDatasetSingleTimeSeries(dataLibrary->get(), dataId.c_str());
    auto x = new opaque_pointer_handle(result);
    return x;}

opaque_pointer_handle* GetDatasetEnsembleTimeSeries_cpp(opaque_pointer_handle* dataLibrary, const std::string dataEnsembleId)
{
    auto result = GetDatasetEnsembleTimeSeries(dataLibrary->get(), dataEnsembleId.c_str());
    auto x = new opaque_pointer_handle(result);
    return x;}

opaque_pointer_handle* GetDatasetEnsembleForecastTimeSeries_cpp(opaque_pointer_handle* dataLibrary, const std::string dataId)
{
    auto result = GetDatasetEnsembleForecastTimeSeries(dataLibrary->get(), dataId.c_str());
    auto x = new opaque_pointer_handle(result);
    return x;}

void SaveSingleTimeSeriesToNetcdf_cpp(opaque_pointer_handle* timeSeries, const std::string filename, bool overwrite)
{
    SaveSingleTimeSeriesToNetcdf(timeSeries->get(), filename.c_str(), (bool)overwrite);
}

void SaveEnsembleTimeSeriesToNetcdf_cpp(opaque_pointer_handle* collection, const std::string filename, bool overwrite)
{
    SaveEnsembleTimeSeriesToNetcdf(collection->get(), filename.c_str(), (bool)overwrite);
}

void SaveEnsembleForecastTimeSeriesToNetcdf_cpp(opaque_pointer_handle* tsEnsTs, const std::string filename, bool overwrite)
{
    SaveEnsembleForecastTimeSeriesToNetcdf(tsEnsTs->get(), filename.c_str(), (bool)overwrite);
}

bool IsMissingValueItemEnsembleForecastTimeSeries_cpp(opaque_pointer_handle* series, int i)
{
    auto result = IsMissingValueItemEnsembleForecastTimeSeries(series->get(), (int)i);
    auto x = result;
    return x;}

opaque_pointer_handle* TimeSeriesFromEnsembleOfTimeSeries_cpp(opaque_pointer_handle* collectionTs, int index)
{
    auto result = TimeSeriesFromEnsembleOfTimeSeries(collectionTs->get(), (int)index);
    auto x = new opaque_pointer_handle(result);
    return x;}

opaque_pointer_handle* TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries_cpp(opaque_pointer_handle* efts, int indexInIssueTime, int indexInForecastTime)
{
    auto result = TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries(efts->get(), (int)indexInIssueTime, (int)indexInForecastTime);
    auto x = new opaque_pointer_handle(result);
    return x;}

double GetValueFromUnivariateTimeSeries_cpp(opaque_pointer_handle* ts, int index)
{
    auto result = GetValueFromUnivariateTimeSeries(ts->get(), (int)index);
    auto x = result;
    return x;}

void TransformEachItem_cpp(opaque_pointer_handle* tsEnsTs, const std::string method, const std::string methodArgument)
{
    TransformEachItem(tsEnsTs->get(), method.c_str(), methodArgument.c_str());
}

void SetValueToUnivariateTimeSeries_cpp(opaque_pointer_handle* ts, int index, double value)
{
    SetValueToUnivariateTimeSeries(ts->get(), (int)index, (double)value);
}

opaque_pointer_handle* GetItemEnsembleTimeSeriesAsStructure_cpp(opaque_pointer_handle* series, int i)
{
    auto result = GetItemEnsembleTimeSeriesAsStructure(series->get(), (int)i);
    auto x = new opaque_pointer_handle(result);
    return x;}

void SetItemEnsembleTimeSeriesAsStructure_cpp(opaque_pointer_handle* collection, int i, multi_regular_time_series_data& values)
{
    multi_regular_time_series_data* values_tsd_ptr = &values;
    SetItemEnsembleTimeSeriesAsStructure(collection->get(), (int)i, values_tsd_ptr);
    // no delection for values_tsd_ptr;
}

opaque_pointer_handle* CreatePerfectForecastTimeSeries_cpp(opaque_pointer_handle* observations, date_time_to_second start, int length, const std::string timeStepName, int offsetForecasts, int leadTime)
{
    auto result = CreatePerfectForecastTimeSeries(observations->get(), start, (int)length, timeStepName.c_str(), (int)offsetForecasts, (int)leadTime);
    auto x = new opaque_pointer_handle(result);
    return x;}

opaque_pointer_handle* ToStructEnsembleTimeSeriesData_cpp(opaque_pointer_handle* ensSeries)
{
    auto result = ToStructEnsembleTimeSeriesData(ensSeries->get());
    auto x = new opaque_pointer_handle(result);
    return x;}

opaque_pointer_handle* ToStructSingleTimeSeriesData_cpp(opaque_pointer_handle* timeSeries)
{
    auto result = ToStructSingleTimeSeriesData(timeSeries->get());
    auto x = new opaque_pointer_handle(result);
    return x;}

opaque_pointer_handle* CreateEnsembleTimeSeriesDataFromStruct_cpp(multi_regular_time_series_data& ensSeries)
{
    multi_regular_time_series_data* ensSeries_tsd_ptr = &ensSeries;
    auto result = CreateEnsembleTimeSeriesDataFromStruct(ensSeries_tsd_ptr);
    // no delection for ensSeries_tsd_ptr;
    auto x = new opaque_pointer_handle(result);
    return x;}

opaque_pointer_handle* CreateSingleTimeSeriesDataFromStruct_cpp(multi_regular_time_series_data& timeSeries)
{
    multi_regular_time_series_data* timeSeries_tsd_ptr = &timeSeries;
    auto result = CreateSingleTimeSeriesDataFromStruct(timeSeries_tsd_ptr);
    // no delection for timeSeries_tsd_ptr;
    auto x = new opaque_pointer_handle(result);
    return x;}

void GetTimeSeriesGeometry_cpp(opaque_pointer_handle* timeSeries, regular_time_series_geometry& geom)
{
    regular_time_series_geometry* geom_tsgeom = &geom;
    GetTimeSeriesGeometry(timeSeries->get(), geom_tsgeom);
    // no delection for geom_tsgeom;
}

void GetEnsembleForecastTimeSeriesGeometry_cpp(opaque_pointer_handle* timeSeries, regular_time_series_geometry& geom)
{
    regular_time_series_geometry* geom_tsgeom = &geom;
    GetEnsembleForecastTimeSeriesGeometry(timeSeries->get(), geom_tsgeom);
    // no delection for geom_tsgeom;
}

int GetNumTimeSeries_cpp()
{
    auto result = GetNumTimeSeries();
    auto x = result;
    return x;}

void GetProviderTsGeometry_cpp(opaque_pointer_handle* dataLibrary, const std::string variableIdentifier, regular_time_series_geometry& geom)
{
    regular_time_series_geometry* geom_tsgeom = &geom;
    GetProviderTsGeometry(dataLibrary->get(), variableIdentifier.c_str(), geom_tsgeom);
    // no delection for geom_tsgeom;
}


std::vector<std::string> GetProviderTimeSeriesIdentifiers_cpp(opaque_pointer_handle* dataLibrary)
{
	int size; 
	char** names = GetProviderTimeSeriesIdentifiers(dataLibrary->get(),  &size);
	return cinterop::utils::to_cpp_string_vector(names, size);
}

opaque_pointer_handle* TimeSeriesFromProviderTs_cpp(opaque_pointer_handle* dataLibrary, const std::string variableIdentifier)
{
    auto result = TimeSeriesFromProviderTs(dataLibrary->get(), variableIdentifier.c_str());
    auto x = new opaque_pointer_handle(result);
    return x;}

