#include "datatypes/extern_c_api.h"
#include "moirai/error_reporting.h"

#include "cinterop/c_cpp_interop.hpp"
#include "cinterop/c_boost_posix_time_interop.hpp"
#include "cinterop/timeseries_interop.hpp"
#include "datatypes/internals_c_api.hpp"
#include "datatypes/shared_pointer_conversions.hpp"
#include "datatypes/time_series_io.hpp"
#include "datatypes/interop_conversions.h"
#include "datatypes/interop_conversions.hpp"

#define TRY_START try {
#define INTERCEPT_STD_EXCEPTION } catch (std::exception& e) { moirai::error_handling::error_log::handle_std_exception(e); }

/**
 * @brief macro for intercepting exceptions at C API call but avoiding 
 * "warning: control reaches end of non-void function [-Wreturn-type]"
 * 
 */
#define INTERCEPT_STD_EXCEPTION_RET(dummyReturned) } catch (std::exception& e) { moirai::error_handling::error_log::handle_std_exception(e); } return dummyReturned;

#define INTERCEPT_STD_EXCEPTION_RETPTR INTERCEPT_STD_EXCEPTION_RET(nullptr)
#define INTERCEPT_STD_EXCEPTION_BOOL INTERCEPT_STD_EXCEPTION_RET(false)
#define INTERCEPT_STD_EXCEPTION_DOUBLE INTERCEPT_STD_EXCEPTION_RET(0.0)
#define INTERCEPT_STD_EXCEPTION_INT INTERCEPT_STD_EXCEPTION_RET(0)

using namespace cinterop::utils;

// Using a few type aliases here:

using EFTS = EnsembleForecastTimeSeries<>;
using EFTSPTR = EFTS*;
using MTSPTR = EFTS::ElementType;
using MTS = std::remove_pointer<MTSPTR>::type;
using TS = TimeSeries;
using TSPTR = TS*;
using TSL = TimeSeriesLibrary;


// As we use the macro INTERCEPT_STD_EXCEPTION at compilation time we get lots of warnings.
#pragma warning( disable: 4715)

/***********************
* FUNCTIONS NOT EXPORTED
************************/
char** StrVecToChars(int* size, const std::vector<std::string>& names)
{
	TRY_START
		return cinterop::utils::vector_to_c_array<string, char*>(names, cinterop::utils::to_ansi_string<string>, size);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

template<typename Tts>
void FillTsGeometry(Tts* ts, const char* variableIdentifier, TS_GEOMETRY_PTR geom)
{
	TRY_START
		auto g = TS_GEOMETRY_DYNCAST(geom);
	// TODO: what about variableIdentifier? Name?
	ToTimeSeriesGeomStruct<Tts>(*ts, *g);
	INTERCEPT_STD_EXCEPTION
}

using datatypes::interop::MissingValueHandling;

void CopyTimeSeriesValues(DATATYPES_TIME_SERIES_DOUBLE_PTR timeSeries, double * values, int arrayLength)
{
	if (timeSeries->GetLength() != arrayLength)
		datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument(string("Time series length is not equal to the number of elements expected by the caller"));
	timeSeries->CopyWithMissingValueTo(values, MissingValueHandling::TimeSeriesMissingValueValue);
}

/***********************
* START OF THE API
************************/

void RegisterExceptionCallback(const void* callback)
{
	using moirai::error_handling::error_log;
	if(callback == nullptr) // Allow the deregistration, at least for unit test purposes.
		error_log::register_exception_callback(callback);
	else if(!error_log::has_callback_registered())
		error_log::register_exception_callback(callback);
}

void DisposeSharedPointer(VOID_PTR_PROVIDER_PTR ptr)
{
	delete ptr;
}

void CopyPosixTime(ptime& from,  DATE_TIME_INFO_PTR to)
{
	TRY_START
		auto tt = DATE_TIME_INFO_DYNCAST(to);
	to_date_time_to_second(from, *tt);
	INTERCEPT_STD_EXCEPTION
}

void GetTimeSeriesGeometry(TIME_SERIES_PTR timeSeries, TS_GEOMETRY_PTR geom)
{
	TRY_START
		TSPTR t = TIME_SERIES_DYNCAST(timeSeries);
	FillTsGeometry(t, nullptr, geom);
	INTERCEPT_STD_EXCEPTION
}

void GetTimeSeriesValues(TIME_SERIES_PTR timeSeries, double * values, int arrayLength)
{
	TRY_START
		TSPTR t = TIME_SERIES_DYNCAST(timeSeries);
	CopyTimeSeriesValues(t, values, arrayLength);
	INTERCEPT_STD_EXCEPTION
}

void DeleteAnsiStringArray(char** values, int arrayLength)
{
	TRY_START
		datatypes::utils::datatypes_delete_ansi_string_array(values, arrayLength);
	INTERCEPT_STD_EXCEPTION
}

void DeleteAnsiString(const char* value)
{
	TRY_START
		delete[] value;
	INTERCEPT_STD_EXCEPTION
}

char* GetLastStdExceptionMessage()
{
	TRY_START
		char* res = STRDUP(moirai::error_handling::error_log::get_last_exception_message().c_str());
	return res;
	INTERCEPT_STD_EXCEPTION_RETPTR
}

int GetNumTimeSeries(){ return datatypes::timeseries::TTimeSeries<double>::NumInstances(); }

void GetProviderTsGeometry(TIME_SERIES_PROVIDER_PTR dataLibrary, const char* variableIdentifier, TS_GEOMETRY_PTR geom)
{
	TRY_START
		string varId(variableIdentifier);
	TimeSeries* ts = TIME_SERIES_PROVIDER_DYNCAST(dataLibrary)->GetSingle(varId);
	FillTsGeometry(ts, variableIdentifier, geom);
	delete ts;
	INTERCEPT_STD_EXCEPTION
}

void GetProviderTimeSeriesValues(TIME_SERIES_PROVIDER_PTR dataLibrary, const char* variableIdentifier, double * values, int arrayLength)
{
	TRY_START
		string varId(variableIdentifier);
	TimeSeries* ts = TIME_SERIES_PROVIDER_DYNCAST(dataLibrary)->GetSingle(varId);
	CopyTimeSeriesValues(ts, values, arrayLength);
	delete ts;
	INTERCEPT_STD_EXCEPTION
}

char** GetProviderTimeSeriesIdentifiers(TIME_SERIES_PROVIDER_PTR dataLibrary, int* size)
{
	TRY_START
		vector<string> ids = TIME_SERIES_PROVIDER_DYNCAST(dataLibrary)->GetIdentifiers();
	return StrVecToChars(size, ids);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

TIME_SERIES_PTR TimeSeriesFromProviderTs(TIME_SERIES_PROVIDER_PTR dataLibrary, const char* variableIdentifier)
{
	TRY_START
		TimeSeriesProvider<double>* mts = TIME_SERIES_PROVIDER_DYNCAST(dataLibrary);
	TS* tsPtr = mts->GetSingle(string(variableIdentifier));
	return WRAP_TIME_SERIES_PTR(tsPtr);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

using datatypes::timeseries::TimeSeriesLibraryFactory;

ENSEMBLE_DATA_SET_PTR LoadEnsembleDataset(const char* filename, const char* dataPath)
{
	TRY_START
		TSL* x = TimeSeriesLibraryFactory::LoadTimeSeriesLibraryPtr(string(filename), string(dataPath));
	return WRAP_ENSEMBLE_DATA_SET_PTR(x);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

ENSEMBLE_DATA_SET_PTR CreateEnsembleDataset(const char* type)
{
	TRY_START
		TSL* x = TimeSeriesLibraryFactory::CreateTimeSeriesLibraryPtr(string(type));
	return WRAP_ENSEMBLE_DATA_SET_PTR(x);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

char** GetEnsembleDatasetDataIdentifiers(ENSEMBLE_DATA_SET_PTR dataLibrary, int* size)
{
	TRY_START
		auto ids = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary)->GetIdentifiers();
	return StrVecToChars(size, ids);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

char** GetEnsembleDatasetDataSubIdentifiers(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataCollectionId, int* size)
{
	TRY_START
		auto ids = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary)->GetIdentifiers(string(dataCollectionId));
	return StrVecToChars(size, ids);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

char** GetEnsembleDatasetDataSummaries(ENSEMBLE_DATA_SET_PTR dataLibrary, int* size)
{
	TRY_START
		vector<string> res;
		TSL* ds = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary);
	auto ids = ds->GetIdentifiers();
	for(auto id : ids)
		res.push_back(ds->GetDataSummary(id));
	return StrVecToChars(size, res);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

ENSEMBLE_FORECAST_TIME_SERIES_PTR CreateEnsembleForecastTimeSeries(date_time_to_second start, int length, const char* timeStepName)
{
	TRY_START
		TimeStep tstep = TimeStep::Parse(string(timeStepName));
	auto x = new ENSEMBLE_FORECAST_TIME_SERIES_DOUBLE(length, to_ptime(start), tstep);
	return WRAP_ENSEMBLE_FORECAST_TIME_SERIES_PTR(x);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

ENSEMBLE_FORECAST_TIME_SERIES_PTR CreatePerfectForecastTimeSeries(TIME_SERIES_PTR observations, date_time_to_second start, int length, const char* timeStepName, int offsetForecasts, int leadTime)
{
	TRY_START
		TSPTR obs = TIME_SERIES_DYNCAST(observations);
		TimeStep tstep = TimeStep::Parse(string(timeStepName));
		ptime s = to_ptime(start);
		EFTSPTR result = datatypes::timeseries::TimeSeriesOperations<>::CreateForecastPtr(*obs, s, length, tstep, leadTime, offsetForecasts);
	return WRAP_ENSEMBLE_FORECAST_TIME_SERIES_PTR(result);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

void SetItemEnsembleForecastTimeSeriesAsStructure(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i, const multi_regular_time_series_data*  values)
{
	TRY_START
		EFTSPTR x = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(series);
	x->SetValue(i, ToTimeSeriesEnsemblePtr(*values));
	INTERCEPT_STD_EXCEPTION
}

void SetItemEnsembleTimeSeriesAsStructure(ENSEMBLE_PTR_TIME_SERIES_PTR collection, int i, const multi_regular_time_series_data*  values)
{
	TRY_START
		MTSPTR x = ENSEMBLE_PTR_TIME_SERIES_DYNCAST(collection);
	x->Set(i, SingleTsFromMultiTimeSeriesData(*values));
	INTERCEPT_STD_EXCEPTION
}

time_series_dimensions_description* GetDataDimensionsDescription(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId)
{
	TRY_START
		TSL* d = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary);
	auto dims = d->GetDataDimensionsDescription(string(dataId));
	auto res = ToTimeSeriesDimensionDescriptions(dims);
	return res;
	INTERCEPT_STD_EXCEPTION_RETPTR
}

int EnsembleSizeEnsembleTimeSeries(ENSEMBLE_PTR_TIME_SERIES_PTR ensSeries)
{
	TRY_START
		MTSPTR d = ENSEMBLE_PTR_TIME_SERIES_DYNCAST(ensSeries);
	return static_cast<int>(d->Size());
	INTERCEPT_STD_EXCEPTION_INT
}

void DisposeDataDimensionsDescriptions(time_series_dimensions_description* data)
{
	TRY_START
		if (data == nullptr)
			throw std::logic_error("DisposeDataDimensionsDescriptions: data cannot be a nullptr");
	DisposeTimeSeriesDimensionDescriptions(data);
	INTERCEPT_STD_EXCEPTION
}

bool IsMissingValueItemEnsembleForecastTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i)
{
	TRY_START
		EFTSPTR x = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(series);
	MTSPTR mts = x->GetValue(i);
	return(x->IsMissingValue(mts));
	INTERCEPT_STD_EXCEPTION_BOOL
}

ENSEMBLE_PTR_TIME_SERIES_PTR GetItemEnsembleForecastTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i)
{
	TRY_START
		EFTSPTR x = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(series);
	MTSPTR mts = x->GetValue(i);
	if (mts == nullptr)
		datatypes::exceptions::ExceptionUtilities::ThrowNotSupported("GetItemEnsembleForecastTimeSeries cannot yet deal with missing values");
	MTSPTR result = new MTS(*mts);
	return WRAP_ENSEMBLE_TIME_SERIES_PTR(result);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

multi_regular_time_series_data* GetItemEnsembleForecastTimeSeriesAsStructure(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i)
{
	TRY_START
		EFTSPTR x = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(series);
	auto mts = x->GetValue(i);
	multi_regular_time_series_data* result;
	if(mts!=nullptr)
		result = ToMultiTimeSeriesDataPtr(*mts);
	else
	{
		// handle "missing value" in conventions by passing "degenerate" sizes rather than returning nullptr;
		result = new multi_regular_time_series_data();
		result->ensemble_size = -1;
		result->numeric_data = nullptr;
		result->time_series_geometry.length = 0;
		result->time_series_geometry.time_step_seconds = 0;
		result->time_series_geometry.time_step_code = time_step_code::strictly_regular;
		// use R's lubridate::origin
		result->time_series_geometry.start.year = 1970;
		result->time_series_geometry.start.month = 1;
		result->time_series_geometry.start.day = 1;
		result->time_series_geometry.start.hour = 0;
		result->time_series_geometry.start.minute = 0;
		result->time_series_geometry.start.second = 0;
	}
	return result;
	INTERCEPT_STD_EXCEPTION_RETPTR
}

multi_regular_time_series_data* GetItemEnsembleTimeSeriesAsStructure(ENSEMBLE_PTR_TIME_SERIES_PTR series, int i)
{
	TRY_START
		MTSPTR x = ENSEMBLE_PTR_TIME_SERIES_DYNCAST(series);
	auto ts = x->Get(i);
	multi_regular_time_series_data* result = ToMultiTimeSeriesDataPtr(*ts);
	return result;
	INTERCEPT_STD_EXCEPTION_RETPTR
}

double GetValueFromUnivariateTimeSeries(TIME_SERIES_PTR ts, int index)
{
	TRY_START
		TSPTR tsptr = TIME_SERIES_DYNCAST(ts);
	return tsptr->GetValue(index);
	INTERCEPT_STD_EXCEPTION_DOUBLE
}

void SetValueToUnivariateTimeSeries(TIME_SERIES_PTR ts, int index, double value)
{
	TRY_START
		TSPTR tsptr = TIME_SERIES_DYNCAST(ts);
	return tsptr->SetValue(index, value);
	INTERCEPT_STD_EXCEPTION
}

TIME_SERIES_PTR TimeSeriesFromEnsembleOfTimeSeries(ENSEMBLE_PTR_TIME_SERIES_PTR collectionTs, int index)
{
	TRY_START
		MTSPTR mts = ENSEMBLE_PTR_TIME_SERIES_DYNCAST(collectionTs);
	TS* tsPtr = new TS(mts->Get(index));
	return WRAP_TIME_SERIES_PTR(tsPtr);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

TIME_SERIES_PTR TimeSeriesFromTimeSeriesOfEnsembleOfTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR efts, int indexInIssueTime, int indexInForecastTime)
{
	TRY_START
		EFTSPTR e = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(efts);
	MTSPTR mts = e->GetValue(indexInIssueTime);
	TS* tsPtr = new TS(mts->Get(indexInForecastTime));
	return WRAP_TIME_SERIES_PTR(tsPtr);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

multi_regular_time_series_data* ToStructEnsembleTimeSeriesData(ENSEMBLE_PTR_TIME_SERIES_PTR ensSeries)
{
	TRY_START
		MTSPTR mts = ENSEMBLE_PTR_TIME_SERIES_DYNCAST(ensSeries);
	multi_regular_time_series_data* result = ToMultiTimeSeriesDataPtr(*mts);
	return result;
	INTERCEPT_STD_EXCEPTION_RETPTR
}

multi_regular_time_series_data* ToStructSingleTimeSeriesData(TIME_SERIES_PTR timeSeries)
{
	TRY_START
		TSPTR ts = TIME_SERIES_DYNCAST(timeSeries);
	multi_regular_time_series_data* result = ToMultiTimeSeriesDataPtr(*ts);
	return result;
	INTERCEPT_STD_EXCEPTION_RETPTR
}

ENSEMBLE_PTR_TIME_SERIES_PTR CreateEnsembleTimeSeriesDataFromStruct(const multi_regular_time_series_data* ensSeries)
{
	TRY_START
		return WRAP_ENSEMBLE_TIME_SERIES_PTR(MultiTsPtrFromMultiTimeSeriesData(*ensSeries));
	INTERCEPT_STD_EXCEPTION_RETPTR
}

TIME_SERIES_PTR CreateSingleTimeSeriesDataFromStruct(const multi_regular_time_series_data* timeSeries)
{
	TRY_START
		return WRAP_TIME_SERIES_PTR(SingleTsPtrFromMultiTimeSeriesData(*timeSeries));
	INTERCEPT_STD_EXCEPTION_RETPTR
}

void DisposeMultiTimeSeriesData(multi_regular_time_series_data* data)
{
	TRY_START
		if (data == nullptr) return;
	cinterop::disposal::dispose_of<multi_regular_time_series_data>(*data);
	delete data;
	INTERCEPT_STD_EXCEPTION
}

void SetTimeSeriesMissingValueValue(double missingValueValue)
{
	MissingValueHandling::TimeSeriesMissingValueValue = missingValueValue;
}

void DeleteDoubleArray(double* values)
{
	TRY_START
		if (values == nullptr)
			throw std::logic_error("DeleteDoubleArray: value cannot be a nullptr");
	delete[] values;
	INTERCEPT_STD_EXCEPTION
}

void GetEnsembleForecastTimeSeriesGeometry(ENSEMBLE_FORECAST_TIME_SERIES_PTR timeSeries, TS_GEOMETRY_PTR geom)
{
	TRY_START
		EFTSPTR t = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(timeSeries);
	FillTsGeometry(t, nullptr, geom);
	INTERCEPT_STD_EXCEPTION
}

ENSEMBLE_FORECAST_TIME_SERIES_PTR GetDatasetEnsembleForecastTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId)
{
	TRY_START
		TSL* d = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary);
		EFTSPTR res = d->GetTimeSeriesEnsembleTimeSeries(dataId);
	return WRAP_ENSEMBLE_FORECAST_TIME_SERIES_PTR(res);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

TIME_SERIES_PTR GetDatasetSingleTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId)
{
	TRY_START
		TSL* d = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary);
	auto res = d->GetSingle(string(dataId));
	return WRAP_TIME_SERIES_PTR(res);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

ENSEMBLE_PTR_TIME_SERIES_PTR GetDatasetEnsembleTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataEnsembleId)
{
	TRY_START
		TSL* d = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary);
	auto res = d->GetEnsembleTimeSeries(string(dataEnsembleId));
	return WRAP_ENSEMBLE_TIME_SERIES_PTR(*res);
	INTERCEPT_STD_EXCEPTION_RETPTR
}

multi_regular_time_series_data* GetEnsembleTimeSeriesData(ENSEMBLE_PTR_TIME_SERIES_PTR ensSeries)
{
	TRY_START
	MTSPTR mts = ENSEMBLE_PTR_TIME_SERIES_DYNCAST(ensSeries);
	return ToMultiTimeSeriesDataPtr(*mts);
	INTERCEPT_STD_EXCEPTION_RETPTR
}



void TransformEachItem(ENSEMBLE_FORECAST_TIME_SERIES_PTR tsEnsTs, const char* method, const char* methodArgument)
{
	TRY_START
		EFTSPTR efts = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(tsEnsTs);
	datatypes::timeseries::TimeSeriesOperations<>::TransformEachItem(*efts, string(method), string(methodArgument));
	INTERCEPT_STD_EXCEPTION
}


// Below are evil temporary things tied to a particular backend storage;
void SaveSingleTimeSeriesToNetcdf(TIME_SERIES_PTR timeSeries, const char* filename, bool overwrite)
{
	TRY_START
		throw std::logic_error("SaveSingleTimeSeriesToNetcdf not implemented");
		INTERCEPT_STD_EXCEPTION
}
void SaveEnsembleTimeSeriesToNetcdf(ENSEMBLE_PTR_TIME_SERIES_PTR collection, const char* filename, bool overwrite)
{
	TRY_START
	throw std::logic_error("SaveEnsembleTimeSeriesToNetcdf not implemented");

	//MTSPTR ens = ENSEMBLE_PTR_TIME_SERIES_DYNCAST(collection);
	//datatypes::timeseries::DimensionsFromSeries(*ens);
	//string fn(filename);
	//size_t nEns = ens->Size();
	//vector<double> leadTimeVar;
	//string timeUnits;
	//vector<double> timeVar;
	//vector<string> stationIds;
	//std::map < string, VariableDefinition >  varDefinitions;
	//GlobalAttributes globalAttributes;
	//string ncVarName;
	//string identifier = "";
	//string leadTimeUnits = "";

	//NetCdfEnsembleTimeSeriesStore<double> ensTsSrc(const string& filename, const size_t nEns, const vector<double>& leadTimeVar, const string& timeUnits, const vector<double>& timeVar, const vector<string>& stationIds,
	//	const std::map<string, VariableDefinition>& varDefinitions, const GlobalAttributes& globalAttributes, const string& ncVarName, const string& identifier = "", const string& leadTimeUnits = "");
	//(ncFilename, ncVarName, ncIdentifier);
	//TimeSeriesLibraryFactory::CreateEnsTsSource(fileAllDataCases, TestDataLocationHelper::kVar1Ens, multiStationIds[0]);

		INTERCEPT_STD_EXCEPTION
}

void SaveEnsembleForecastTimeSeriesToNetcdf(ENSEMBLE_FORECAST_TIME_SERIES_PTR tsEnsTs, const char* filename, bool overwrite)
{
	TRY_START
		EFTSPTR ens = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(tsEnsTs);
	DimensionsDefinitions dims = DimensionsFromSeries(*ens);
	string fn(filename);
	//size_t nEns = ens->Size();
	//vector<double> leadTimeVar;
	//string timeUnits;
	//vector<double> timeVar;
	//vector<string> stationIds;
	//std::map < string, VariableDefinition >  varDefinitions;
	//GlobalAttributes globalAttributes;
	//string ncVarName;
	//string identifier = "";
	//string leadTimeUnits = "";

	string units(""); // shall come from tsEnsTs "later"
	string name("data_name");
	string longName("long name");
	auto v = VariableDefinition::TimeSeriesEnsembleTimeSeries(name, units, longName);
	map<string, VariableDefinition> varDef;
	varDef[name] = v;

	GlobalAttributes gattr = GetMetadataFrom<EFTS, GlobalAttributes>(*ens);
	string ncVarName("TODO");
	NetCdfTimeSeriesEnsembleTimeSeriesStore<double> store(fn, dims, varDef, gattr, ncVarName);
	store.SetSeries("", ens);
		INTERCEPT_STD_EXCEPTION
}

#undef DATATYPES_TIME_SERIES_DOUBLE
#undef DATATYPES_TIME_SERIES_DOUBLE_PTR
#undef SWIFT_ENSEMBLE_TIME_SERIES_DOUBLE
#undef SWIFT_ENSEMBLE_TIME_SERIES_DOUBLE_PTR

#undef TS_GEOMETRY_PTR
#undef DATE_TIME_INFO_PTR
#undef TIME_SERIES_PROVIDER_PTR 
#undef ENSEMBLE_DATA_SET_PTR 
#undef ENSEMBLE_FORECAST_TIME_SERIES_PTR
#undef TIME_SERIES_PTR
#undef ENSEMBLE_TIME_SERIES_PTR

#undef TS_GEOMETRY_DYNCAST
#undef DATE_TIME_INFO_DYNCAST
#undef ENSEMBLE_DATA_SET_DYNCAST
#undef OBJECT_STATE_INITIALIZER_DYNCAST

