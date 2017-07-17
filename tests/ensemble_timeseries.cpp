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

	EnsembleForecastTimeSeries<>* backEndEnsTs = tsensts->GetSeries("");

	EnsembleForecastTimeSeries<> ensTs = DTH::CreateTsEnsembleTs(
		tsEtsLength,
		ensSize,
		tsLength,
		start,
		etsTstep,
		fcastTstep);

	writableTs = ensTs;

	REQUIRE(s->Size() == tsEtsLength);
	auto p = s->GetProxy(0);
	auto blah = writableTs.GetValue(0);
	REQUIRE(datatypes::timeseries::TimeSeriesOperations<>::AreTimeSeriesEnsembleTimeSeriesEqual(*backEndEnsTs, ensTs));

	REQUIRE(datatypes::timeseries::TimeSeriesOperations<>::AreTimeSeriesEnsembleTimeSeriesEqual(writableTs, ensTs));

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

TEST_CASE("Ensemble input provider")
{
	using namespace datatypes::timeseries::io;

	string rainObsId("rain_obs");
	string petObsId("pet_obs");

	string rainNcVarname("rain_der");
	string petNcVarname("pet_der");

	string historicalSeriesNcIdentifier("1");

	string rainFcastNcVarname("rain_fcast_ens");
	string rainFcastId("rain_fcast_ens");

	string petFcastNcVarname(petNcVarname);
	string petFcastId("pet_fcast_ens");

	std::string testDir = TestDataLocationHelper::ReadEnvironmentVariable("SWIFT_TEST_DIR");

	TimeSeriesLibraryDescription tsld;
	tsld.AddSingle(rainObsId, TimeSeriesLibraryFactory::CreateNetcdfSourceInfo(rainObsId, NetCdfSourceInfo::StorageTypeSingleNetcdfFile, TestDataLocationHelper::BuildPath({ testDir, "Obs_data", "Upper_Murray_rain_1hr.nc" }), rainNcVarname, historicalSeriesNcIdentifier));
	tsld.AddSingle(petObsId, TimeSeriesLibraryFactory::CreateNetcdfSourceInfo(petObsId, NetCdfSourceInfo::StorageTypeSingleNetcdfFile, TestDataLocationHelper::BuildPath({ testDir, "Obs_data", "Upper_Murray_pet_24h_89_2012.nc" }), petNcVarname, historicalSeriesNcIdentifier));
	tsld.AddSingle(petFcastId, TimeSeriesLibraryFactory::CreateNetcdfSourceInfo(petFcastId, NetCdfSourceInfo::StorageTypeSingleNetcdfFile, TestDataLocationHelper::BuildPath({ testDir, "Fct_Data", "Upper_Murray_pet_clim_1990_2010.nc" }), petNcVarname, historicalSeriesNcIdentifier));

	string forecastDataFiles(TestDataLocationHelper::BuildPath({ testDir, "Fct_Data", "Upper_Murray_F1_1_{0}21_shuffle.nc" })); // where {0} is e.g. 20100801
	tsld.AddTsEnsembleTs(rainFcastId, TimeSeriesLibraryFactory::CreateNetcdfSourceInfo(rainFcastId, NetCdfSourceInfo::StorageTypeMultipleNetcdfFiles, forecastDataFiles, rainFcastNcVarname, string("1"), 0,
		"daily", "20100801T210000", 0, 0, 0, "hourly"));

	boost::filesystem::path p = FileSystemHelper::GetTempFile();
	ConfigFileHelper::SaveTimeSeriesLibraryDescription(tsld, p.generic_string());
	auto loaded = ConfigFileHelper::LoadTimeSeriesLibraryDescription(p.generic_string());
	FileSystemHelper::Remove(p);

	auto skeys = loaded.GetDataIdSingle();
	REQUIRE_EQUAL(3, skeys.size());
	REQUIRE(datatypes::utils::StringProcessing::EqualsAny(rainObsId, skeys));
	REQUIRE(datatypes::utils::StringProcessing::EqualsAny(petObsId, skeys));
	REQUIRE(datatypes::utils::StringProcessing::EqualsAny(petFcastId, skeys));

	//REQUIRE_EQUAL(TestDataLocationHelper::BuildPath({ testDir, "Obs_data", "Upper_Murray_rain_1hr.nc"} ), loaded.GetFilename(rainObsId));
	//REQUIRE_EQUAL(-1, loaded.GetIndex(rainObsId));

	auto ekeys = loaded.GetDataIdTsEnsembleTs();
	REQUIRE_EQUAL(1, ekeys.size());
	//REQUIRE_EQUAL(TestDataLocationHelper::BuildPath({ testDir, "Fct_Data", "Upper_Murray_F1_1_{0}21_shuffle.nc"} ), loaded.GetFilename(rainFcastId));
	//REQUIRE_EQUAL(0, loaded.GetIndex(rainFcastId));

	//datatypes::timeseries::io::ConfigFileHelper::SaveTimeSeriesLibraryDescription(tsld, string("f:\\tmp\\tsld.yaml"));

	// Tease out an API for ensemble of inputs that may be on disk in a variety of forms.
	// See usage of 		TimeSeriesLibrary dataLibrary in previous FACTs
	// 
	// MultifileNetCDFTimeSeriesStore<double> store(DataDirectory);

	/*
	PtrSeriesType obsPetTimeSeries = TsOps::TrimTimeSeries(fullHourlyObsPetTimeSeries, ptime(from_iso_string("19900101T000000")), ptime(from_iso_string("20100805T200000")));

	Catchment* c = CatchmentTestHelper::CreateOneMegPerDayLineNetwork(1);
	ModelRunner mr(c);
	ptime historicStart = obsPetTimeSeries->GetStartDate();
	int historicLength = obsPetTimeSeries->GetLength();
	CatchmentTestHelper::SetHourlyTimeStepSpan(mr, historicStart, historicLength);

	mr.Play(rainVarId, *obsRainTimeSeries);
	mr.Play(petVarId, *obsPetTimeSeries);

	mr.InitializeSimulation();

	// Run from historic start to historic end minus 4 days (96 hours)

	mr.RunTimeSteps(historicLength - (24 * 4));

	EnsembleForecastModelRunner efmr = EnsembleForecastModelRunner(mr);

	efmr.Setup(mr.GetSimulationTime(), TimeStep::GetHourly(), 5, 1000, 231, 24);

	efmr.Play(rainfallEnsembles, rainVarId);
	efmr.PlaySingle(petEnsembles, petVarId);

	efmr.Record(runoffVarId);
	efmr.Record(memoryStateVarId);

	efmr.RunEnsembles();


	*/


}

TEST_CASE("Create a series of forecasts out of a time series of observation")
{
	using TSO = datatypes::timeseries::TimeSeriesOperations<>;
	size_t ndays = 7;
	size_t leadTime = 24;
	ptime startObs(date(2010, 2, 3));
	size_t obsLength = (ndays + 5) * leadTime;

	TimeStep hourly = TimeStep::GetHourly();
	TimeStep threeHourly = hourly * 3;
	ptime startFcast = hourly.AddSteps(startObs, 6);
	size_t nFcastIssues = 16; // every three hours, ~2 days
	size_t forecastStartOffset = 1;

	vector<double> values = DTH::SeqVec(0, 0.1, obsLength);
	TimeSeries observations(values, startObs, hourly);
	TSO::TSeriesEnsemblePtrType forecast = TSO::CreateForecast(observations, startFcast, nFcastIssues, threeHourly, leadTime, 1);
	REQUIRE(forecast.GetLength() == nFcastIssues);
	REQUIRE(forecast.GetStartDate() == startFcast);
	REQUIRE(forecast.GetTimeStep() == threeHourly);

	auto mts = forecast.GetValue(0);

	REQUIRE(mts->GetStartDate() == hourly.AddSteps(startFcast, forecastStartOffset));
	REQUIRE(mts->Size() == 1);
	auto ts = mts->Get(0);
	REQUIRE(ts->GetLength() == leadTime);
	REQUIRE(ts->GetStartDate() == hourly.AddSteps(startFcast, forecastStartOffset));
	REQUIRE(ts->GetTimeStep() == hourly);

	for (size_t i = 0; i < leadTime; i++)
		REQUIRE(ts->GetValue(i) == observations.GetValue(startFcast + hours(i + forecastStartOffset)));
}