#include "common_datatypes.h"

TEST_CASE("Windowed write-buffered storage for time series of ensembles")
{
	// https://jira.csiro.au/browse/WIRADA-388
	using DTH = datatypes::tests::DataTestHelper<double>;
	using EFTS = EnsembleForecastTimeSeries<>;
	using MTSPTR = EFTS::ElementType;
	using MTS = std::remove_pointer<MTSPTR>::type;
	size_t windowSize = 6;
	auto startDate = ptime(date(2010, 8, 1));

	TestWindowedStorage<MTSPTR> * sp = new TestWindowedStorage<MTSPTR>(windowSize);
	EFTS efts(sp);
	size_t tsLen = 2 * windowSize + windowSize / 2;
	efts.Reset(tsLen, startDate, TimeStep::GetDaily());

	const MTS value = DTH::CreateEnsembleTs(6, 7);
	vector<MTS*> values(tsLen);
	for (size_t i = 0; i < tsLen; i++)
	{
		values[i] = new MTS(value);
	}
	for (size_t i = 0; i < windowSize; i++)
	{
		efts.SetValue(i, values[i]);
	}
	for (size_t i = 0; i < windowSize; i++)
	{
		REQUIRE(sp->GetWindowItem(i) == values[i]);
		REQUIRE(sp->GetBackendItem(i) == nullptr);
	}

	efts.SetValue(windowSize, values[windowSize]);

	REQUIRE(sp->GetWindowItem(0) == values[windowSize]);
	for (size_t i = 1; i < windowSize; i++)
		REQUIRE(sp->GetWindowItem(i) == nullptr);

	for (size_t i = 0; i < windowSize; i++)
	{
		REQUIRE(sp->GetBackendItem(i) == values[i]);
	}

	for (size_t i = 0; i < tsLen; i++)
		delete(values[i]);
}

TEST_CASE("Eager writer back end storage for time series of ensembles")
{
	size_t tsEtsLength = 31 * 6;
	size_t ensSize = 5;
	size_t tsLength = 24;
	ptime start(date(2008, 3, 4));
	auto hourly = TimeStep::GetHourly();
	TimeStep daily = TimeStep::GetDaily();
	TimeStep etsTstep = hourly * 6;
	TimeStep fcastTstep = hourly / 4;


	TestTimeSeriesEnsembleTimeSeriesStore* tsensts = new TestTimeSeriesEnsembleTimeSeriesStore();
	EagerWriter<TimeSeriesEnsembleTimeSeriesStore<double>::PtrEnsemblePtrType>* s =
		new EagerWriter<TimeSeriesEnsembleTimeSeriesStore<double>::PtrEnsemblePtrType>(tsensts);
	EnsembleForecastTimeSeries<> writableTs(s);

	EnsembleForecastTimeSeries<>* backEndEnsTs = tsensts->GetBackendSeries("");

	EnsembleForecastTimeSeries<> ensTs = DTH::CreateTsEnsembleTs(
		tsEtsLength,
		ensSize,
		tsLength,
		start,
		etsTstep,
		fcastTstep);

	writableTs = ensTs;

	REQUIRE(datatypes::timeseries::TimeSeriesOperations<>::AreTimeSeriesEnsembleTimeSeriesEqual(writableTs, ensTs));
	REQUIRE(datatypes::timeseries::TimeSeriesOperations<>::AreTimeSeriesEnsembleTimeSeriesEqual(*backEndEnsTs, ensTs));

	delete tsensts;

}

TEST_CASE("Creation of a NetCdfTimeSeriesEnsembleTimeSeriesStore as a back-end to write time series to")
{
	//https://jira.csiro.au/browse/WIRADA-387
	// What this unit test is for is testing the capacity to create 
	// a netCDF store for which we may not know the full geometry upfront. 
	using namespace boost::filesystem;
	using DTH = datatypes::tests::DataTestHelper<double>;
	path fPath = FileSystemHelper::GetTempFile();
	auto testFilePath = fPath.generic_string();
	TempFileCleaner cleaner(testFilePath);

	string dataId = "test_data_id";
	string stationId = "0";

	NetCdfTimeSeriesEnsembleTimeSeriesStore<double> writeSeries(testFilePath, dataId, stationId, true);

	//NcFileDef args = CreateNcIo<NetCdfTimeSeriesEnsembleTimeSeriesStore<double>>(timeUnits, stationIds, tsEtsLength, ensSize, tsLength, startDate, timeStep, varOneName);
	//NetCdfTimeSeriesEnsembleTimeSeriesStore<double> writeSeries(testFilePath, args.first, args.second, varOneName, stationOneId);

	EnsembleForecastTimeSeries< TTimeSeries<double> >* tsets = writeSeries.GetSeries(dataId);

	size_t tsEtsLength = 4;
	size_t ensSize = 5;
	size_t tsLength = 24;
	ptime start(date(2008, 3, 4));
	auto hourly = TimeStep::GetHourly();
	TimeStep etsTstep = hourly * 6; // Forecasts every 6 hours
	TimeStep fcastTstep = hourly; // forecast series are hourly time steps

	EnsembleForecastTimeSeries<> ensTs = DTH::CreateTsEnsembleTs(
		tsEtsLength,
		ensSize,
		tsLength,
		start,
		etsTstep,
		fcastTstep);

	(*tsets) = ensTs;

	writeSeries.Close();

	vector<string> warnings, errors;
	SwiftNetCDFAccess::CheckCompliance(testFilePath, 2, 0, warnings, errors);

	NetCdfTimeSeriesEnsembleTimeSeriesStore<double> readSeries(testFilePath, dataId, stationId, false);
	auto readEns = readSeries.GetSeries("");

	bool roundTrip = TimeSeriesOperations<TimeSeries>::AreTimeSeriesEnsembleTimeSeriesEqual(*readEns, ensTs);
	readSeries.Close();

	REQUIRE(roundTrip == true);

}

