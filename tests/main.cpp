#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

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

	TimeSeriesEnsembleTimeSeriesStore<double>* tsensts = new TestTimeSeriesEnsembleTimeSeriesStore(ensFts);
	tsl.AddSource("ts_ensemble", tsensts);

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

//template <typename Tts = TimeSeries>
//class TestEnsembleForecastTimeSeries : public EnsembleForecastTimeSeries<Tts>
//{
//public:
//	bool IsLoaded(int i);
//};
//
//TEST_CASE("Lazy loading time series") {
//
//
//	TestEnsembleForecastTimeSeries<> ts;
//
//	using datatypes::SharedPointerProxy;
//
//	using MTS = SharedPointerProxy<MultiTimeSeriesPtr<>>;
//
//	REQUIRE_FALSE(ts.IsLoaded(0));
//	MTS first = ts[0];
//	REQUIRE(ts.IsLoaded(0));
//
//	REQUIRE_FALSE(ts.IsLoaded(1));
//	MTS second = ts[1];
//	REQUIRE(ts.IsLoaded(1));
//
//	REQUIRE_FALSE(ts.IsLoaded(2));
//	MTS third = ts[2];
//	REQUIRE(ts.IsLoaded(2));
//	REQUIRE(ts.IsLoaded(0));
//	first = third;
//	REQUIRE_FALSE(ts.IsLoaded(0));
//
//}

void TestTimeStepAddSteps(const std::string& start_iso_str, const std::string& expected_iso_str, int nSteps)
{
	TimeStep ts = TimeStep::GetMonthlyQpp();

	ptime start = from_iso_string(start_iso_str);
	ptime actual = ts.AddSteps(start, nSteps);
	ptime expected = from_iso_string(expected_iso_str);
	REQUIRE_EQUAL(expected, actual);
}

TEST_CASE("Monthly QPP TimeStep AddSteps Test")
{
	// Positive

	// [dd < 24]
	TestTimeStepAddSteps("20000123T101112", "20000123T101112", 0);
	TestTimeStepAddSteps("20000123T101112", "20000223T101112", 1);
	TestTimeStepAddSteps("20000123T101112", "20010623T101112", 17);

	// [dd >= 24] w/seven day offset
	TestTimeStepAddSteps("20000124T101112", "20000124T101112", 0);
	TestTimeStepAddSteps("20000124T101112", "20000222T101112", 1);
	TestTimeStepAddSteps("20000124T101112", "20010623T101112", 17);

	// Negative

	// [dd < 24]
	TestTimeStepAddSteps("20000123T101112", "20000123T101112", -0);
	TestTimeStepAddSteps("20000123T101112", "19991223T101112", -1);
	TestTimeStepAddSteps("20000123T101112", "19980823T101112", -17);

	// [dd >= 24] w/seven day offset
	TestTimeStepAddSteps("20000124T101112", "20000124T101112", -0);
	TestTimeStepAddSteps("20000124T101112", "19991224T101112", -1);
	TestTimeStepAddSteps("20000124T101112", "19980824T101112", -17);
}

void TestTimeStepNumSteps(const std::string& start_iso_str, const std::string& end_iso_str, int exp_upper, int exp_steps, int exp_offset)
{
	TimeStep ts = TimeStep::GetMonthlyQpp();

	ptime s = from_iso_string(start_iso_str);
	ptime e = from_iso_string(end_iso_str);
	int act_upper = ts.GetUpperNumSteps(s, e);
	REQUIRE_EQUAL(exp_upper, act_upper);
	int act_steps = ts.GetNumSteps(s, e);
	REQUIRE_EQUAL(exp_steps, act_steps);
	int act_offset = ts.GetOffset(s, e);
	REQUIRE_EQUAL(exp_offset, act_offset);
}

TEST_CASE("Monthly QPP TimeStep NumSteps Test")
{
	// [dd < 24]

	// Difference between start and end is a fraction (one second) less than one month
	TestTimeStepNumSteps("20000123T000000", "20000222T235959", 2, 1, 0);
	
	// Difference between start and end exactly one month
	TestTimeStepNumSteps("20000123T000000", "20000223T000000", 2, 2, 1);

	// Difference between start and end is a fraction (one second) more than one month
	TestTimeStepNumSteps("20000123T000000", "20000223T000001", 3, 2, 1);

	// [dd >= 24] w/seven day offset

	// Difference between start and end is a fraction (one second) less than one month
	TestTimeStepNumSteps("20000124T000000", "20000221T235959", 2, 1, 0);

	// Difference between start and end exactly one month
	TestTimeStepNumSteps("20000124T000000", "20000222T000000", 2, 2, 1);

	// Difference between start and end is a fraction (one second) more than one month
	TestTimeStepNumSteps("20000124T000000", "20000222T000001", 3, 2, 1);
}
