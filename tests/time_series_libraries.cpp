#include "common_datatypes.h"

TEST_CASE("High level library of time series")
{
	// I am reluctant to introduce a dependency on YAML from this library.
	// Will keep it for the time being in SWIFT.
	// const auto filepath = DatatypesTestPath("time_series_library_ut.yaml");
	TimeSeriesLibrary tsl;

	auto startDate = ptime(date(2010, 8, 1)) + hours(14);
	vector<double> values({ 1,2,3,4,5.0 });
	TimeSeries singleTs = TimeSeries(values, startDate, TimeStep::GetHourly());
	SingleTimeSeriesStore<double> * single = new TestSingleTimeSeriesStore(singleTs);
	tsl.AddSource("single", single);

	auto mkMts = [&](vector<double> values, size_t ensSize, const TimeStep& tStep) {
		TimeSeries ts = TimeSeries(values, startDate, tStep);
		vector<vector<double>> v(ensSize);
		for (size_t i = 0; i < ensSize; i++)
			v[i] = (ts + 0.1*(i + 1)).GetValuesVector();
		MultiTimeSeries<> mts(v, startDate, tStep);
		return mts;
	};

	auto addVec = [&](const vector<double>& values, double added) {
		vector<double> v(values.size());
		for (size_t i = 0; i < values.size(); i++)
			v[i] += added;
		return v;
	};

	size_t ensSize = 5;

	auto ensembleTs = mkMts(values, ensSize, TimeStep::GetHourly());
	EnsembleTimeSeriesStore<double>* ensts = new TestEnsembleTimeSeriesStore(ensembleTs);
	tsl.AddSource("ensemble", ensts);

	auto startEnsTs = ptime(date(2011, 1, 1)) + hours(9);
	EnsembleForecastTimeSeries<> ensFts(31, startEnsTs, TimeStep::GetDaily());

	ensSize = 3;
	for (size_t i = 0; i < ensFts.GetLength(); i++)
	{
		ensFts.SetValue(i, mkMts(addVec(values, i), ensSize, TimeStep::GetHourly()).AsPointerSeries());
	}

	string tsEnsTsId("ts_ensemble");
	TimeSeriesEnsembleTimeSeriesStore<double>* tsensts = new TestTimeSeriesEnsembleTimeSeriesStore(ensFts, tsEnsTsId);
	tsl.AddSource(tsEnsTsId, tsensts);

	auto hasItem = [&](const string& s, const vector<string>& v)
	{
		return (std::find(v.begin(), v.end(), s) != v.end());
	};

	auto ids = tsl.GetIdentifiers();
	REQUIRE(ids.size() == 3);
	REQUIRE(hasItem("single", ids));
	REQUIRE(hasItem("ensemble", ids));
	REQUIRE(hasItem("ts_ensemble", ids));

	auto tsActual = tsl.GetSingle("single");
	REQUIRE_EQUAL_COMPARER(singleTs, *tsActual, TimeSeriesOperations<TimeSeries>::AreTimeSeriesEqual);
	delete tsActual;

	auto mtsActualPtr = tsl.GetEnsembleTimeSeries("ensemble");
	MultiTimeSeries<> mtsActual = mtsActualPtr->AsValueSeries();
	REQUIRE_EQUAL_COMPARER(ensembleTs, mtsActual, TimeSeriesOperations<TimeSeries>::AreEnsembleTimeSeriesEqual);
	delete mtsActualPtr;

	auto entsActualPtr = tsl.GetTimeSeriesEnsembleTimeSeries("ts_ensemble");
	REQUIRE_EQUAL_COMPARER(ensFts, *entsActualPtr, TimeSeriesOperations<TimeSeries>::AreTimeSeriesEnsembleTimeSeriesEqual);
	delete entsActualPtr;

}

TEST_CASE("Time Series Library which can create new netCDF backed store for time series")
{
	//https://jira.csiro.au/browse/WIRADA-387
	// Note that this test relates strongly to a testswiftapi test:
	// TEST_CASE("Recording ensemble forecast time series to a recording library", "[simulation]")
	using namespace boost::filesystem;
	using DTH = datatypes::tests::DataTestHelper<double>;

	TimeSeriesLibrary library;
	REQUIRE_FALSE(library.CanCreateTimeSeriesEnsembleSeriesStore("blah"));
	path temp = FileSystemHelper::GetTempDir();
	create_directories(temp);
	string dataId("var_1_ts_ens_ts");
	DataGeometryProvider * dgp = nullptr;
	library = TimeSeriesLibrary(new SwiftNetcdfStoreFactory(temp.generic_string(), dgp));
	REQUIRE(library.CanCreateTimeSeriesEnsembleSeriesStore(dataId));
	library.CreateTimeSeriesEnsembleSeriesStore(dataId);
	auto pts = library.GetTimeSeriesEnsembleTimeSeries(dataId);

	size_t tsEtsLength = 4;
	size_t ensSize = 5;
	size_t tsLength = 24;
	ptime start(date(2008, 3, 4));
	auto hourly = TimeStep::GetHourly();
	TimeStep daily = TimeStep::GetDaily();
	TimeStep etsTstep = hourly * 6;
	TimeStep fcastTstep = hourly / 4;

	EnsembleForecastTimeSeries<> ensTs = DTH::CreateTsEnsembleTs(
		tsEtsLength,
		ensSize,
		tsLength,
		start,
		etsTstep,
		fcastTstep);

	(*pts) = ensTs;

	library.Close();

	auto ncFile = temp / (dataId + ".nc");
	REQUIRE(exists(ncFile));

	TempFileCleaner cleaner(ncFile.generic_string());

	auto readTs = TimeSeriesLibraryFactory::LoadTsEnsTs(ncFile.generic_string(), dataId, "", dataId);
	REQUIRE(datatypes::timeseries::TimeSeriesOperations<>::AreTimeSeriesEnsembleTimeSeriesEqual(*readTs, ensTs));
	delete readTs;
}

TEST_CASE("Time series of forecast read from multiple netcdf file storage") {

	// Unit test related to, covering a superset of, https://jira.csiro.au/browse/WIRADA-349

	string rainObsId;
	string petObsId;
	string rainFcastId;
	string petFcastId;

	TimeSeriesLibrary dataLibrary = TestDataLocationHelper::CreateEnsembleTimeSeriesLibrary(rainObsId, petObsId, rainFcastId, petFcastId);

	REQUIRE_THROWS(dataLibrary.GetSingle(rainFcastId));
	REQUIRE_THROWS(dataLibrary.GetTimeSeriesEnsembleTimeSeries("some_unknown_identifier"));

	auto fcastSeries = dataLibrary.GetTimeSeriesEnsembleTimeSeries(rainFcastId);
	size_t n = fcastSeries->GetLength();

	auto eStart = TimeStep::CreatePtime(2010, 8, 1);
	auto eEnd = eStart + days(5-1); //	5, // to "Upper_Murray_F1_1_2010080521_shuffle.nc"
	//	1000, // int ensembleSize, 
	//	231, // int ensembleLength, 
	//		 // const TimeStep& ensembleTimeStep
	//	TimeStep::GetHourly());
	auto s = fcastSeries->GetStartDate();
	REQUIRE_FALSE(s.is_not_a_date_time());
	REQUIRE(s == eStart);
	auto e = fcastSeries->GetEndDate();
	REQUIRE_FALSE(e.is_not_a_date_time());
	REQUIRE(e == eEnd);
	auto tstep = fcastSeries->GetTimeStep();

	REQUIRE(n == tstep.GetNumSteps(s, e));
	MultiTimeSeriesPtr<>* mts = nullptr;
	REQUIRE_NOTHROW(mts = fcastSeries->GetValue(0));
	REQUIRE_NOTHROW(mts = fcastSeries->GetValue(0));
	REQUIRE_NOTHROW(mts = fcastSeries->GetValue(n-1));
	REQUIRE_THROWS(mts = fcastSeries->GetValue(-1));
	REQUIRE_THROWS(mts = fcastSeries->GetValue(n));

	delete fcastSeries;
	fcastSeries = dataLibrary.GetTimeSeriesEnsembleTimeSeries(rainFcastId);
	delete fcastSeries;
}

TEST_CASE("Tease out facilities to retrieve data from time series libraries in a convenient dimensionality")
{
	// We want to have enough conveniences to retrieve time series as ensembles or 
	// single items ("item" may mean an ensemble itself, note)
	//https://jira.csiro.au/browse/WIRADA-417
	TimeSeriesLibrary dataLibrary = TestDataLocationHelper::GetTestTimeSeriesLibrary();

	string varNameSingle(TestDataLocationHelper::kVarSingleStation);
	string varNameMultiple(TestDataLocationHelper::kVarMultiStations);

	using TS = TimeSeriesLibrary::TS;
	TS* ts = nullptr;
	MultiTimeSeries<TS*>* mts = nullptr;
	//Single time series, simplest test
	REQUIRE_NOTHROW(ts = dataLibrary.GetSingle(varNameSingle));
	delete ts; ts = nullptr;

	//6 variables (excluding dimension variables):
	//double var_multi_stations[station,time]   
	//station  Size:2
	//time  Size:48   *** is unlimited ***
	//units: hours since 2010-08-01 14:00:00 +0000
	//long_name: time
	//standard_name: time
	//time_standard: UTC
	//axis: t

	//Single main id, but collection of time series as one per station identifier
	REQUIRE_NOTHROW(ts = dataLibrary.GetSingle(StringProcessing::BuildIdentifier(varNameMultiple, TestDataLocationHelper::kStationIdOne)));
	delete ts; ts = nullptr;

	// 2017-02
	// Retrieving collections of time series via GetEnsembleTimeSeries used to not be allowed. 
	// Other use cases have changed this since, but this may be revised yet.
	// REQUIRE_THROWS(mts = dataLibrary.GetEnsembleTimeSeries(varNameMultiple));
	REQUIRE_NOTHROW(mts = dataLibrary.GetEnsembleTimeSeries(varNameMultiple));
	delete mts; mts = nullptr;
	REQUIRE_NOTHROW(mts = dataLibrary.GetAllTimeSeries(varNameMultiple));
	REQUIRE(mts->Size() == TestDataLocationHelper::TestStationIds().size());
	for (size_t i = 0; i < mts->Size(); i++)
	{
		REQUIRE(mts->GetLength(i) == TestDataLocationHelper::kTimeSeriesLength);
		ts = mts->Get(i);
		TS expected = DTH::GetExpectedTestSingleTimeSeries(i);
		REQUIRE_EQUAL_COMPARER(*ts, expected, TimeSeriesOperations<TimeSeries>::AreTimeSeriesEqual);
	}
	delete mts; mts = nullptr;
	//Ensemble of time series, different entities
	//Ensemble of time series, different entities AND more than one station id too.
	//Ensemble of ensemble forecast time series, different entities
	//Ensemble of ensemble forecast time series, different entities AND more than one station id too.
}

TEST_CASE("Transform time series inplace in an ensemble")
{
	using DTH = datatypes::tests::DataTestHelper<double>;
	using EFTS = datatypes::timeseries::EnsembleForecastTimeSeries<>;
	using TSPTR = CommonTypes<double>::PtrSeriesType;

	size_t tsEnsTsLength = 7;
	size_t nEns = 3;
	size_t leadLength = 240;
	TimeStep leadTstep = TimeStep::GetHourly();
	ptime start(date(2008, 3, 4));

	// Test the aggregation
	EFTS efts = DTH::CreateTsEnsembleTs(tsEnsTsLength, nEns, leadLength, start, TimeStep::GetDaily(), leadTstep);

	int testedItem = 3;

	auto fcastStart = efts.GetValue(testedItem)->GetStartDate();
	EFTS::ElementType beforeEns = efts.GetValue(testedItem);
	TSPTR beforeTsptr = beforeEns->Get(0);
	vector<double> expectedAccumulated = { 0,0 };
	for (size_t i = 0; i < 3; i++)
	{
		expectedAccumulated[0] += beforeTsptr->GetValue(i);
		expectedAccumulated[1] += beforeTsptr->GetValue(i+3);
	}

	datatypes::timeseries::TimeSeriesOperations<>::TransformEachItem(efts, "accumulate", "03:00:00");

	EFTS::ElementType fcasts = efts.GetValue(testedItem);
	REQUIRE(fcasts->GetStartDate() == fcastStart + hours(2));
	REQUIRE(fcasts->Size() == nEns);

	TSPTR tsptr = fcasts->Get(0);
	REQUIRE(tsptr->GetLength() == (leadLength / 3));
	REQUIRE(tsptr->GetStartDate() == (fcastStart + hours(2)));
	REQUIRE(tsptr->GetTimeStep() == leadTstep * 3);
	REQUIRE(tsptr->GetValue(0) == expectedAccumulated[0]);
	REQUIRE(tsptr->GetValue(1) == expectedAccumulated[1]);


	// Test the disaggregation from 3 hourly to hourly
	efts = DTH::CreateTsEnsembleTs(tsEnsTsLength, nEns, leadLength / 3, start, TimeStep::GetDaily(), leadTstep * 3);

	fcastStart = efts.GetValue(testedItem)->GetStartDate();
	beforeEns = efts.GetValue(testedItem);
	beforeTsptr = beforeEns->Get(0);
	vector<double> expectedDisag(9);
	for (size_t i = 0; i < expectedDisag.size(); i++)
		expectedDisag[i] = beforeTsptr->GetValue(i/3) / 3;


	datatypes::timeseries::TimeSeriesOperations<>::TransformEachItem(efts, "disaggregate", "01:00:00");

	fcasts = efts.GetValue(testedItem);
	tsptr = fcasts->Get(0);
	REQUIRE(tsptr->GetLength() == (leadLength));
	REQUIRE(tsptr->GetStartDate() == (fcastStart - hours(2)));
	REQUIRE(tsptr->GetTimeStep() == leadTstep);
	REQUIRE(fcasts->GetStartDate() == fcastStart - hours(2));
	REQUIRE(fcasts->Size() == nEns);
	for (size_t i = 0; i < expectedDisag.size(); i++)
		REQUIRE(tsptr->GetValue(i) == expectedDisag[i]);

}

