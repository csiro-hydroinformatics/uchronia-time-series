#include "moirai/error_reporting.h"
#include "cinterop/c_cpp_interop.hpp"
#include "datatypes/extern_c_api.h"
#include "datatypes/internals_c_api.hpp"
#include "datatypes/shared_pointer_conversions.hpp"
#include "datatypes/time_series_io.hpp"
#include "datatypes/interop_conversions.h"
#include "datatypes/interop_conversions.hpp"

string error_log::last_exception_message = string("");
std::recursive_mutex error_log::mutex;
error_log::exception_callback error_log::callback = nullptr;

#define TRY_START try {
#define INTERCEPT_STD_EXCEPTION } catch (std::exception& e) { error_log::handle_std_exception(e); }

/***********************
* FUNCTIONS NOT EXPORTED
************************/
using namespace cinterop::utils;

char** StrVecToChars(int* size, const std::vector<std::string>& names)
{
	TRY_START
		return cinterop::utils::vector_to_c_array<string, char*>(names, cinterop::utils::to_ansi_string<string>, size);
	INTERCEPT_STD_EXCEPTION
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
	if (callback == nullptr)
		error_log::callback = nullptr;
	else
	{
		error_log::callback = (error_log::exception_callback) callback;
	}
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
		auto t = TIME_SERIES_DYNCAST(timeSeries);
	FillTsGeometry(t, nullptr, geom);
	INTERCEPT_STD_EXCEPTION
}

void GetTimeSeriesValues(TIME_SERIES_PTR timeSeries, double * values, int arrayLength)
{
	TRY_START
		auto t = TIME_SERIES_DYNCAST(timeSeries);
	CopyTimeSeriesValues(t, values, arrayLength);
	INTERCEPT_STD_EXCEPTION
}

void DeleteAnsiStringArray(char** values, int arrayLength)
{
	TRY_START
		delete_ansi_string_array( values, arrayLength );
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
		char* res = STRDUP(error_log::last_exception_message.c_str());
	return res;
	INTERCEPT_STD_EXCEPTION
}

int GetNumTimeSeries(){ return datatypes::timeseries::TTimeSeries<double>::NumInstances(); }

void GetProviderTsGeometry(TIME_SERIES_PROVIDER_PTR dataLibrary, const char* variableIdentifier, TS_GEOMETRY_PTR geom)
{
	TRY_START
		string varId(variableIdentifier);
	TimeSeries* ts = dataLibrary->get_ptr()->GetSingle(varId);
	FillTsGeometry(ts, variableIdentifier, geom);
	delete ts;
	INTERCEPT_STD_EXCEPTION
}

void GetProviderTimeSeriesValues(TIME_SERIES_PROVIDER_PTR dataLibrary, const char* variableIdentifier, double * values, int arrayLength)
{
	TRY_START
		string varId(variableIdentifier);
	TimeSeries* ts = dataLibrary->get_ptr()->GetSingle(varId);
	CopyTimeSeriesValues(ts, values, arrayLength);
	delete ts;
	INTERCEPT_STD_EXCEPTION
}

char** GetProviderTimeSeriesIdentifiers(TIME_SERIES_PROVIDER_PTR dataLibrary, int* size)
{
	TRY_START
		vector<string> ids = dataLibrary->get_ptr()->GetIdentifiers();
	return StrVecToChars(size, ids);
	INTERCEPT_STD_EXCEPTION
}

using datatypes::timeseries::TimeSeriesLibraryFactory;

ENSEMBLE_DATA_SET_PTR LoadEnsembleDataset(const char* filename, const char* dataPath)
{
	TRY_START
		TimeSeriesLibrary* x = TimeSeriesLibraryFactory::LoadTimeSeriesLibraryPtr(string(filename), string(dataPath));
	return WRAP_ENSEMBLE_DATA_SET_PTR(x);
	INTERCEPT_STD_EXCEPTION
}

ENSEMBLE_DATA_SET_PTR CreateEnsembleDataset(const char* type)
{
	TRY_START
		 auto x = TimeSeriesLibraryFactory::CreateTimeSeriesLibraryPtr(string(type));
	return WRAP_ENSEMBLE_DATA_SET_PTR(x);
	INTERCEPT_STD_EXCEPTION
}

char** GetEnsembleDatasetDataIdentifiers(ENSEMBLE_DATA_SET_PTR dataLibrary, int* size)
{
	TRY_START
		auto ids = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary)->GetIdentifiers();
	return StrVecToChars(size, ids);
	INTERCEPT_STD_EXCEPTION
}

char** GetEnsembleDatasetDataSubIdentifiers(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataCollectionId, int* size)
{
	TRY_START
		auto ids = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary)->GetIdentifiers(string(dataCollectionId));
	return StrVecToChars(size, ids);
	INTERCEPT_STD_EXCEPTION
}

char** GetEnsembleDatasetDataSummaries(ENSEMBLE_DATA_SET_PTR dataLibrary, int* size)
{
	TRY_START
		vector<string> res;
		auto ds = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary);
	auto ids = ds->GetIdentifiers();
	for(auto id : ids)
		res.push_back(ds->GetDataSummary(id));
	return StrVecToChars(size, res);
	INTERCEPT_STD_EXCEPTION
}

ENSEMBLE_FORECAST_TIME_SERIES_PTR CreateEnsembleForecastTimeSeries(date_time_to_second start, int length, const char* timeStepName)
{
	TRY_START
		TimeStep tstep = TimeStep::Parse(string(timeStepName));
	auto x = new ENSEMBLE_FORECAST_TIME_SERIES_DOUBLE(length, to_ptime(start), tstep);
	return WRAP_ENSEMBLE_FORECAST_TIME_SERIES_PTR(x);
	INTERCEPT_STD_EXCEPTION
}

void SetItemEnsembleForecastTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i, multi_regular_time_series_data* values)
{
	TRY_START
		auto x = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(series);
	x->SetValue(i, ToTimeSeriesEnsemblePtr(*values));
	INTERCEPT_STD_EXCEPTION
}

multi_regular_time_series_data* GetItemEnsembleForecastTimeSeries(ENSEMBLE_FORECAST_TIME_SERIES_PTR series, int i)
{
	TRY_START
		auto x = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(series);
	auto mts = x->GetValue(i);
	return ToMultiTimeSeriesDataPtr(*mts);
	INTERCEPT_STD_EXCEPTION
}

void DisposeMultiTimeSeriesData(multi_regular_time_series_data* data)
{
	TRY_START
	DisposeMultiTimeSeriesData(*data);
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
		auto t = ENSEMBLE_FORECAST_TIME_SERIES_DYNCAST(timeSeries);
	FillTsGeometry(t, nullptr, geom);
	INTERCEPT_STD_EXCEPTION
}

ENSEMBLE_FORECAST_TIME_SERIES_PTR GetDatasetEnsembleForecastTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId)
{
	TRY_START
		auto d = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary);
		auto res = d->GetTimeSeriesEnsembleTimeSeries(dataId);
	return WRAP_ENSEMBLE_FORECAST_TIME_SERIES_PTR(res);
	INTERCEPT_STD_EXCEPTION
}

TIME_SERIES_PTR GetDatasetSingleTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataId)
{
	TRY_START
		auto d = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary);
	auto res = d->GetSingle(string(dataId));
	return WRAP_TIME_SERIES_PTR(res);
	INTERCEPT_STD_EXCEPTION
}

ENSEMBLE_PTR_TIME_SERIES_PTR GetDatasetEnsembleTimeSeries(ENSEMBLE_DATA_SET_PTR dataLibrary, const char* dataEnsembleId)
{
	TRY_START
		auto d = ENSEMBLE_DATA_SET_DYNCAST(dataLibrary);
	auto res = d->GetEnsembleTimeSeries(string(dataEnsembleId));
	return WRAP_ENSEMBLE_TIME_SERIES_PTR(*res);
	INTERCEPT_STD_EXCEPTION
}

multi_regular_time_series_data* GetEnsembleTimeSeriesData(ENSEMBLE_PTR_TIME_SERIES_PTR ensSeries)
{
	TRY_START
	auto mts = ENSEMBLE_PTR_TIME_SERIES_DYNCAST(ensSeries);
	return ToMultiTimeSeriesDataPtr(*mts);
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
