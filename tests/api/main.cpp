#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <vector>
#include <string>

#include "boost/date_time/posix_time/posix_time.hpp"
#include "catch/catch.hpp"
#include "catch/catch_macros.hpp"

#include "cinterop/c_cpp_interop.hpp"
#include "cinterop/c_boost_posix_time_interop.hpp"

#include <moirai/reference_handle_test_helper.hpp>

// We first include the extern_c_api as a user of the external library via opaque pointers:
#define DATATYPES_USE_OPAQUE_POINTERS
#define USING_DATATYPES
#include "datatypes/extern_c_api.h"

// Then we also include the macros for transparent pointers.
#define FORCE_OPAQUE_PTR_TYPECAST

#undef DATATYPES_USE_OPAQUE_POINTERS
#define DATATYPES_USE_CPP_POINTERS

#include "datatypes/internals_c_api.hpp"
#undef FORCE_OPAQUE_PTR_TYPECAST
#undef  DATATYPES_USE_CPP_POINTERS
#define DATATYPES_USE_OPAQUE_POINTERS


#include "datatypes/datatypes_test_helpers.h"
#include "datatypes/datatypes_test_helpers.hpp"

using namespace std;

template<typename TTo>
TTo* scast(void* ref_hndl) {
	return moirai::tests::unchecked_cast_to_raw_ptr<TTo>(ref_hndl);
}


void delete_ansi_string_array(char** values, int arrayLength)
{
	if (values == nullptr)
		throw std::logic_error("delete_ansi_string_array: values cannot be a nullptr");
	for (int i = 0; i < arrayLength; i++)
		delete[] values[i];
	delete[] values;
}

TEST_CASE("Retrieving", "[uchronia]")
{

	using datatypes::tests::TestDataLocationHelper;
	using datatypes::utils::StringProcessing;
	using namespace cinterop::utils;
	TimeSeriesLibrary* dataLibrary = TestDataLocationHelper::CreateTestTimeSeriesLibrary();

	auto ds = WRAP_ENSEMBLE_DATA_SET_PTR(dataLibrary);

	int size;
	char** cids = GetEnsembleDatasetDataIdentifiers(ds, &size);
	vector<string> ids = to_cpp_string_vector(cids, size, false);
	DeleteAnsiStringArray(cids, size);

	REQUIRE(StringProcessing::SetEquals(ids, TestDataLocationHelper::TestTsLibraryIdentifiers()));

	auto streq = [&](const char* a, const char* b) 
	{
		REQUIRE(string(a) == string(b));		
	};

	time_series_dimensions_description* tsdim = GetDataDimensionsDescription(ds, TestDataLocationHelper::kIdentifier1Obs.c_str());
	REQUIRE(tsdim->num_dimensions == 1);
	streq(tsdim->dimensions[0].dimension_type, TIME_DIM_TYPE_DATA_DIMENSION);
	DisposeDataDimensionsDescriptions(tsdim);

	tsdim = GetDataDimensionsDescription(ds, TestDataLocationHelper::kIdentifier1Ens.c_str());
	REQUIRE(tsdim->num_dimensions == 2);
	streq(tsdim->dimensions[0].dimension_type, ENSEMBLE_DIM_TYPE_DATA_DIMENSION);
	streq(tsdim->dimensions[1].dimension_type, TIME_DIM_TYPE_DATA_DIMENSION);
	DisposeDataDimensionsDescriptions(tsdim);

	tsdim = GetDataDimensionsDescription(ds, TestDataLocationHelper::kIdentifier1FcastEns.c_str());
	REQUIRE(tsdim->num_dimensions == 3);
	streq(tsdim->dimensions[0].dimension_type, TIME_DIM_TYPE_DATA_DIMENSION);
	streq(tsdim->dimensions[1].dimension_type, ENSEMBLE_DIM_TYPE_DATA_DIMENSION);
	streq(tsdim->dimensions[2].dimension_type, TIME_DIM_TYPE_DATA_DIMENSION);
	DisposeDataDimensionsDescriptions(tsdim);

	delete ds;
}

using DTH = datatypes::tests::DataTestHelper<double>;
using EFTS = datatypes::timeseries::EnsembleForecastTimeSeries<>;
using MTSPTR = EFTS::ElementType;
using MTS = std::remove_pointer<MTSPTR>::type;
using TS = datatypes::timeseries::TimeSeries;
using TSPTR = TS*;
using TSL = datatypes::timeseries::TimeSeriesLibrary;

TEST_CASE("Converting to and from uchronia time series")
{

	//sampleDataDir <- "c:/STSF/RPP/data"
	//
	//# One way to define a catalogue of a time series library is a YAML definition
	//x <- uchronia::LoadEnsembleDataset_R(file.path(sampleDataDir, 'Stanley.yaml'), dataPath='')
	//obsStationIds <- uchronia::GetEnsembleDatasetDataSubIdentifiers_R(x, "rain_obs_collection")
	//
	//tsEnsTs <- uchronia::getDataSet(x, "rain_fcast_ens")
	//fcasts <- ToRppSeriesVectors_R(tsEnsTs, subset=FALSE, -1, -1, FALSE)
	//
	//tsObservations <- uchronia::getDataSet(x, "rain_obs_collection")
	//#  This should be supported:
	//# uchronia::getItem(tsObservations, "1")
	//# TODO: needs a better uchronia interface. Starting with extracting from a collection of TS.
	//tsObs <- uchronia::TimeSeriesFromEnsembleOfTimeSeries_R(tsObservations, 0)
	//
	ptime start(date(2008, 3, 4));

	size_t nFcasts = 4;
	size_t offsetForecasts = 1;
	size_t lenFcasts = 24;
	size_t obsLen = nFcasts * lenFcasts * 3;
	double from = 0.0, increment = 1.0;
	auto tsObs = new TimeSeries(DTH::Ramp(obsLen, start, from, increment));
	auto tIndex = tsObs->TimeIndices();

	TIME_SERIES_PTR tsPtr = WRAP_TIME_SERIES_PTR(tsObs);
	//fcastsTimeIndex <- uchronia::timeIndex(tsInfo=tsEnsTs)

	int offsetFcast = 3;
	auto startFcast = tIndex[offsetFcast];
	auto st = cinterop::utils::to_date_time_to_second(startFcast);

	auto fcastObs = CreatePerfectForecastTimeSeries(tsPtr, st, nFcasts, "daily", offsetForecasts, lenFcasts);
	EFTS* ensFts = scast<EFTS>(fcastObs);

	REQUIRE(ensFts->GetLength() == nFcasts);
	for (size_t i = 0; i < nFcasts; i++)
	{
		auto mts = ensFts->GetValue(i);
		REQUIRE(mts->Size() == 1);
		auto ts = mts->Get(0);
		REQUIRE(ts->GetLength() == lenFcasts);
		// Further testing for values is covered in the other unit test application and not duplicated here.
	}

	delete (scast<TS>(tsPtr));
	delete (scast<EFTS>(fcastObs));

}
TEST_CASE("time series of ensembles of time series: items with missing values")
{
	ptime start(date(2008, 3, 4));

	size_t nFcasts = 4;
	size_t offsetForecasts = 1;
	size_t lenFcasts = 24;
	size_t obsLen = nFcasts * lenFcasts * 3;
	double from = 0.0, increment = 1.0;
	auto tsObs = new TimeSeries(DTH::Ramp(obsLen, start, from, increment));
	auto tIndex = tsObs->TimeIndices();

	date_time_to_second dt;
	dt.year = 2010;
	dt.day = 1; dt.month = 1; dt.hour = 0; dt.minute = 0; dt.second = 0;

	int tsLen = 3;
	ENSEMBLE_FORECAST_TIME_SERIES_PTR tsEnsTs = CreateEnsembleForecastTimeSeries(dt, tsLen, "daily");

	EFTS* ensFts = scast<EFTS>(tsEnsTs);

	REQUIRE(IsMissingValueItemEnsembleForecastTimeSeries(tsEnsTs, 0));

	multi_regular_time_series_data* mdt = GetItemEnsembleForecastTimeSeriesAsStructure(tsEnsTs, 0);
	REQUIRE(mdt->numeric_data == nullptr);
	REQUIRE(mdt->ensemble_size == -1);
	DisposeMultiTimeSeriesData(mdt);

	int ensSize = 2;
	auto mts = new MTS(DTH::CreateEnsembleTs(ensSize, 3));
	ensFts->SetValue(0, mts);

	REQUIRE(!IsMissingValueItemEnsembleForecastTimeSeries(tsEnsTs, 0));
	mdt = GetItemEnsembleForecastTimeSeriesAsStructure(tsEnsTs, 0);
	REQUIRE(mdt->numeric_data != nullptr);
	REQUIRE(mdt->ensemble_size == ensSize);
	DisposeMultiTimeSeriesData(mdt);

	delete (scast<TS>(tsEnsTs));

}
