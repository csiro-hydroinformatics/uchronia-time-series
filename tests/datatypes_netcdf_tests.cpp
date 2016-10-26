#include "common_datatypes.h"

TEST_CASE("Read operations from a netCDF SWIFT data file")
{
	/*
	See file
	create_swift_netcdf_ens_data.R
	in the same directory as the present file
	*/
	std::string testDataFile = TestDataLocationHelper::BuildPath({ TestDataLocationHelper::ReadEnvironmentVariable("SWIFT_TEST_DIR"), "netcdf", "testswift.nc" });
	NetCdfSingleSeriesStore<double> store(testDataFile);

	//stationIds <-c(123, 456)
	int nEns = 3;
	int nLead = 4;
	int nTimeSteps = 48;
	// units: hours since 2010-08-01 14:00:00 +0000"
	auto startDate = ptime(date(2010, 8, 1)) + hours(14);

	REQUIRE_EQUAL(nEns, store.GetEnsembleSize());
	REQUIRE_EQUAL(nLead, store.GetLeadTimeCount());
	auto timeDim = store.GetTimeDim();
	REQUIRE_EQUAL(startDate, store.GetStart());
	REQUIRE_EQUAL(startDate, timeDim[0]);
	REQUIRE_EQUAL(startDate + hours(2), timeDim[2]);


	// Check single realization time series

	NetCdfSingleSeriesStore<double> singleTsStore(testDataFile, "var2_obs" /*variable name*/, "456"/*catchment*/);
	REQUIRE_EQUAL(0, singleTsStore.GetEnsembleSize());
	REQUIRE_EQUAL(0, singleTsStore.GetLeadTimeCount());

	auto singleTsPtr = singleTsStore.Read();

	/*
	> head(snc$getSingleSeries('var2_obs', 456))
	[,1]
	2010-08-01 14:00:00 1.212
	2010-08-01 15:00:00 2.222
	2010-08-01 16:00:00 3.232
	2010-08-01 17:00:00 4.242
	2010-08-01 18:00:00 5.252
	2010-08-01 19:00:00 6.262
	Warning message:
	timezone of object (UTC) is different than current timezone ().
	> tail(snc$getSingleSeries('var2_obs', 456))
	[,1]
	2010-08-03 08:00:00 43.632
	2010-08-03 09:00:00 44.642
	2010-08-03 10:00:00 45.652
	2010-08-03 11:00:00 46.662
	2010-08-03 12:00:00 47.672
	2010-08-03 13:00:00 48.682
	*/

	REQUIRE_EQUAL(1.212, (*singleTsPtr)[0]);
	REQUIRE_EQUAL(48, singleTsPtr->GetLength());
	REQUIRE_EQUAL(48.682, (*singleTsPtr)[47]);
	REQUIRE_EQUAL(startDate, singleTsPtr->GetStartDate());

	delete singleTsPtr;

	NetCdfEnsembleTimeSeriesStore<double> ensTsStore(testDataFile, "var2_ens" /*variable name*/, "456"/*catchment*/);
	REQUIRE_EQUAL(nEns, ensTsStore.GetEnsembleSize());
	REQUIRE_EQUAL(0, ensTsStore.GetLeadTimeCount());

	auto ensTs = ensTsStore.Read();

	REQUIRE_EQUAL(3, ensTs->Size());
	REQUIRE_EQUAL(startDate, ensTs->GetStartDate());

	singleTsPtr = ensTs->Get(0);

	REQUIRE_EQUAL(0.55, (*singleTsPtr)[0]);
	REQUIRE_EQUAL(0.80, (*singleTsPtr)[1]);
	REQUIRE_EQUAL(48, singleTsPtr->GetLength());
	REQUIRE_EQUAL(12.3, (*singleTsPtr)[47]);
	REQUIRE_EQUAL(startDate, singleTsPtr->GetStartDate());

	delete singleTsPtr;
	singleTsPtr = ensTs->Get(1);

	REQUIRE_EQUAL(0.80, (*singleTsPtr)[0]);
	REQUIRE_EQUAL(48, singleTsPtr->GetLength());
	REQUIRE_EQUAL(12.55, (*singleTsPtr)[47]);
	REQUIRE_EQUAL(startDate, singleTsPtr->GetStartDate());

	delete singleTsPtr;

	/*
	> head(snc$getEnsSeries('var1_ens', 123))
	[,1] [,2] [,3]
	2010-08-01 14:00:00  2.1  3.1  4.1
	2010-08-01 15:00:00  3.1  4.1  5.1
	2010-08-01 16:00:00  4.1  5.1  6.1
	2010-08-01 17:00:00  5.1  6.1  7.1
	2010-08-01 18:00:00  6.1  7.1  8.1
	2010-08-01 19:00:00  7.1  8.1  9.1
	Warning message:
	timezone of object (UTC) is different than current timezone ().
	> head(snc$getEnsSeries('var2_ens', 123))
	[,1]  [,2]  [,3]
	2010-08-01 14:00:00 0.525 0.775 1.025
	2010-08-01 15:00:00 0.775 1.025 1.275
	2010-08-01 16:00:00 1.025 1.275 1.525
	2010-08-01 17:00:00 1.275 1.525 1.775
	2010-08-01 18:00:00 1.525 1.775 2.025
	2010-08-01 19:00:00 1.775 2.025 2.275
	Warning message:
	timezone of object (UTC) is different than current timezone ().
	> head(snc$getEnsSeries('var1_ens', 456))
	[,1] [,2] [,3]
	2010-08-01 14:00:00  2.2  3.2  4.2
	2010-08-01 15:00:00  3.2  4.2  5.2
	2010-08-01 16:00:00  4.2  5.2  6.2
	2010-08-01 17:00:00  5.2  6.2  7.2
	2010-08-01 18:00:00  6.2  7.2  8.2
	2010-08-01 19:00:00  7.2  8.2  9.2
	Warning message:
	timezone of object (UTC) is different than current timezone ().
	> head(snc$getEnsSeries('var2_ens', 456))
	[,1] [,2] [,3]
	2010-08-01 14:00:00 0.55 0.80 1.05
	2010-08-01 15:00:00 0.80 1.05 1.30
	2010-08-01 16:00:00 1.05 1.30 1.55
	2010-08-01 17:00:00 1.30 1.55 1.80
	2010-08-01 18:00:00 1.55 1.80 2.05
	2010-08-01 19:00:00 1.80 2.05 2.30
	Warning message:
	timezone of object (UTC) is different than current timezone ().
	> tail(snc$getEnsSeries('var2_ens', 456))
	[,1]  [,2]  [,3]
	2010-08-03 08:00:00 11.05 11.30 11.55
	2010-08-03 09:00:00 11.30 11.55 11.80
	2010-08-03 10:00:00 11.55 11.80 12.05
	2010-08-03 11:00:00 11.80 12.05 12.30
	2010-08-03 12:00:00 12.05 12.30 12.55
	2010-08-03 13:00:00 12.30 12.55 12.80
	>
	*/

	// Check ensemble forecast time series 


	NetCdfTimeSeriesEnsembleTimeSeriesStore<double> ensts(testDataFile, "var2_fcast_ens" /*variable name*/, "456"/*catchment*/);
	// The returned value needs is a forecast ensemble with 3 forecasts for each time stamp, of lead length 4
	REQUIRE_EQUAL(nEns, ensts.GetEnsembleSize());
	REQUIRE_EQUAL(nLead, ensts.GetLeadTimeCount());

	int offset = 22;
	auto t = store.GetStart() + hours(offset);

	MultiTimeSeries<TimeSeries*> * mts = ensts.GetForecasts(offset);

	REQUIRE_EQUAL(nEns, mts->Size());
	REQUIRE_EQUAL(t, mts->GetStartDate());

	int replicateNum = 3; // R index
	int replicateIndex = replicateNum - 1; // C index
	int mult = 2; // because var2_fcast_ens
	int offsetR = offset + 1;
	int leadTimeIndex = 1;
	int leadTimeIndexR = leadTimeIndex + 1;
	int stationNum = 2; // "456" is the second station

	auto ts = mts->Get(replicateIndex);
	REQUIRE_EQUAL(nLead, ts->GetLength());
	REQUIRE_EQUAL(t, ts->GetStartDate());

	// finally, a check on a value in the time series
	REQUIRE_EQUAL(mult * (offsetR + 0.1 * replicateNum + leadTimeIndexR*0.01 + stationNum*0.1), ts->GetValue(leadTimeIndex));
	// We do need to reclaim the memory of these time series.

	delete mts;
}

TEST_CASE("Time dimensions in the netCDF SWIFT data file")
{
	using datatypes::utils::SeqVec;
	TimeStep hourly = TimeStep::GetHourly();
	TimeStep etsTstep = hourly * 6;
	TimeStep fcastTstep = hourly / 4;

	TimeStep refAxisTimeStep = SwiftNetCDFAccess::CreateTimeUnits(etsTstep);
	const double fcastStepMult = 0.25; // base axis unit should be hourly, and fcastTstep is a 1/4 hour
	REQUIRE((refAxisTimeStep * fcastStepMult) == fcastTstep);

	ptime start = ptime(date(2003, 4, 5));
	size_t tsLength = 5;
	size_t leadTimeSize = 24;
	std::pair<vector<double>, vector<double>> dims;

	int fcastOffset = 1;
	dims = SwiftNetCDFAccess::CreateTimeVectors(start, etsTstep, tsLength, fcastTstep, leadTimeSize);
	REQUIRE(VectorEqual<double>(SeqVec<double>(0, 6, tsLength), dims.first));
	// expect the default start of forecast to be offset by one time step of the lead time axis compared to the forecast issue time
	REQUIRE(VectorEqual<double>(SeqVec<double>(fcastOffset*fcastStepMult, fcastStepMult, leadTimeSize), dims.second));

	fcastOffset = 1;
	dims = SwiftNetCDFAccess::CreateTimeVectors(start, etsTstep, tsLength, fcastTstep, leadTimeSize, fcastOffset);
	REQUIRE(VectorEqual<double>(SeqVec<double>(0, 6, tsLength), dims.first));
	REQUIRE(VectorEqual<double>(SeqVec<double>(fcastOffset*fcastStepMult, fcastStepMult, leadTimeSize), dims.second));

	fcastOffset = 0;
	dims = SwiftNetCDFAccess::CreateTimeVectors(start, etsTstep, tsLength, fcastTstep, leadTimeSize, fcastOffset);
	REQUIRE(VectorEqual<double>(SeqVec<double>(0, 6, tsLength), dims.first));
	REQUIRE(VectorEqual<double>(SeqVec<double>(fcastOffset*fcastStepMult, fcastStepMult, leadTimeSize), dims.second));

	fcastOffset = 2;
	dims = SwiftNetCDFAccess::CreateTimeVectors(start, etsTstep, tsLength, fcastTstep, leadTimeSize, fcastOffset);
	REQUIRE(VectorEqual<double>(SeqVec<double>(fcastOffset*fcastStepMult, fcastStepMult, leadTimeSize), dims.second));

	// And reconstructing axes:
	dims = SwiftNetCDFAccess::CreateTimeVectors(start, etsTstep, tsLength, fcastTstep, leadTimeSize);
	//vector<ptime> expectedTimeIndex = etsTstep.AddSteps(start, SeqVec<double>(0, 1, tsLength));
	//vector<ptime> expectedLeadTimeIndex = fcastTstep.AddSteps(start, SeqVec<double>(0, 1, leadTimeSize));
	vector<double> timeVec = dims.first;
	vector<double> leadTimeVec = dims.second;

	string refAxisUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(start, etsTstep);
	auto mainTimeIndex = SwiftNetCDFAccess::CreateTimeGeometry(refAxisUnits, timeVec);
	REQUIRE(mainTimeIndex.first == start);
	REQUIRE(mainTimeIndex.second == etsTstep);

	auto leadTimeIndex = SwiftNetCDFAccess::CreateTimeGeometry(refAxisUnits, leadTimeVec);
	REQUIRE(leadTimeIndex.first == fcastTstep.AddSteps(start, 1));
	REQUIRE(leadTimeIndex.second == fcastTstep);

	fcastOffset = 1;
	for (fcastOffset = -1; fcastOffset < 4; fcastOffset++)
	{
		leadTimeIndex = SwiftNetCDFAccess::CreateTimeGeometry(refAxisUnits, SeqVec<double>(fcastOffset*fcastStepMult, fcastStepMult, leadTimeSize));
		REQUIRE(leadTimeIndex.first == fcastTstep.AddSteps(start, fcastOffset));
		REQUIRE(leadTimeIndex.second == fcastTstep);
	}
}

TEST_CASE("Reading single instance time series caters for degenerate dimensions")
{
	/*
	See file
	create_swift_netcdf_ens_data.R
	in the same directory as the present file
	*/
	std::string testDataFile = TestDataLocationHelper::BuildPath({ TestDataLocationHelper::ReadEnvironmentVariable("SWIFT_TEST_DIR"), "netcdf", "testswift_w407.nc" });
	NetCdfSingleSeriesStore<double> store(testDataFile);

	//stationIds <-c(123, 456)
	int nEns = 1;
	int nLead = 1;
	int nTimeSteps = 48;
	// units: hours since 2010-08-01 14:00:00 +0000"
	auto startDate = ptime(date(2010, 8, 1)) + hours(14);

	REQUIRE_EQUAL(nEns, store.GetEnsembleSize());
	REQUIRE_EQUAL(nLead, store.GetLeadTimeCount());

	// Check single realization time series

	NetCdfSingleSeriesStore<double> singleTsStore(testDataFile, "var1_obs_3d" /*variable name*/, "456"/*catchment*/);
	//REQUIRE_EQUAL(0, singleTsStore.GetEnsembleSize());
	//REQUIRE_EQUAL(0, singleTsStore.GetLeadTimeCount());
	auto singleTsPtr = singleTsStore.Read();

	/*
	double var1_obs_4d[lead_time,station,ens_member,time]
	units: mm
	_FillValue: -9999
	long_name: var1_obs_4d synthetic data
	type: 2
	type_description: accumulated over the preceding interval
	Location_Type: Point
	double var2_obs_4d[lead_time,station,ens_member,time]
	units: mm
	_FillValue: -9999
	long_name: var2_obs_4d synthetic data
	type: 2
	type_description: accumulated over the preceding interval
	Location_Type: Point
	double var1_obs_3d[station,ens_member,time]
	units: mm
	_FillValue: -9999
	long_name: var1_obs_3d synthetic data
	type: 2
	type_description: accumulated over the preceding interval
	Location_Type: Point
	double var2_obs_3d[station,ens_member,time]
	units: mm
	_FillValue: -9999
	long_name: var2_obs_3d synthetic data
	type: 2
	type_description: accumulated over the preceding interval
	Location_Type: Point
	> head(snc$getSingleSeries('var1_obs_3d', 456))
	456
	2010-08-01 14:00:00 2.2
	2010-08-01 15:00:00 3.2
	2010-08-01 16:00:00 4.2
	2010-08-01 17:00:00 5.2
	2010-08-01 18:00:00 6.2
	2010-08-01 19:00:00 7.2
	Warning message:
	timezone of object (UTC) is different than current timezone ().
	> head(snc$getSingleSeries('var2_obs_3d', 456))
	456
	2010-08-01 14:00:00 0.55
	2010-08-01 15:00:00 0.80
	2010-08-01 16:00:00 1.05
	2010-08-01 17:00:00 1.30
	2010-08-01 18:00:00 1.55
	2010-08-01 19:00:00 1.80
	Warning message:
	timezone of object (UTC) is different than current timezone ().
	> head(snc$getSingleSeries('var1_obs_4d', 456))
	456
	2010-08-01 14:00:00 1.31
	2010-08-01 15:00:00 2.31
	2010-08-01 16:00:00 3.31
	2010-08-01 17:00:00 4.31
	2010-08-01 18:00:00 5.31
	2010-08-01 19:00:00 6.31
	Warning message:
	timezone of object (UTC) is different than current timezone ().
	> head(snc$getSingleSeries('var2_obs_4d', 456))
	456
	2010-08-01 14:00:00  2.62
	2010-08-01 15:00:00  4.62
	2010-08-01 16:00:00  6.62
	2010-08-01 17:00:00  8.62
	2010-08-01 18:00:00 10.62
	2010-08-01 19:00:00 12.62
	Warning message:
	timezone of object (UTC) is different than current timezone ().
	>
	*/

	TimeSeries& ts = *singleTsPtr;
	REQUIRE_EQUAL(2.2, ts[0]);
	REQUIRE_EQUAL(3.2, ts[1]);
	REQUIRE_EQUAL(48, ts.GetLength());
	REQUIRE_EQUAL(49.2, ts[47]);
	REQUIRE_EQUAL(startDate, ts.GetStartDate());

	delete singleTsPtr;

	singleTsStore = NetCdfSingleSeriesStore<double>(testDataFile, "var1_obs_4d" /*variable name*/, "456"/*catchment*/);
	//REQUIRE_EQUAL(0, singleTsStore.GetEnsembleSize());
	//REQUIRE_EQUAL(0, singleTsStore.GetLeadTimeCount());
	singleTsPtr = singleTsStore.Read();
	ts = *singleTsPtr;
	REQUIRE_EQUAL(1.31, ts[0]);
	REQUIRE_EQUAL(2.31, ts[1]);
	REQUIRE_EQUAL(48, ts.GetLength());
	REQUIRE_EQUAL(48.31, ts[47]);
	REQUIRE_EQUAL(startDate, ts.GetStartDate());

}

TEST_CASE("Write and read the geometry of a netcdf time series file")
{
	auto temp = FileSystemHelper::GetTempFile("geometry_%%%%%%%%%%%%.nc");
	std::string testFilePath = temp.generic_string();
	TempFileCleaner cleaner(testFilePath);

	size_t nEns = 3;
	size_t leadTimeLength = 4;
	size_t mainTimeLength = 5;

	// units: hours since 2010-08-01 14:00:00 +0000"
	ptime startDate = ptime(date(2010, 8, 1)) + hours(14);
	TimeStep timeStep = TimeStep::GetHourly();

	string timeUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(startDate, timeStep);
	vector<double> timeVar = DTH::SeqVec(0.0, 1.0, mainTimeLength);

	map<string, VariableDefinition> varDefs;

	SwiftNetCDFAccess* access = CreateTestSwiftFile(testFilePath,
		nEns,
		leadTimeLength,
		mainTimeLength,
		startDate,
		timeStep);

	delete access;

	NetCdfSingleSeriesStore<double> singleTsStore(testFilePath, var2_obs /*variable name*/, "456"/*catchment*/);
	REQUIRE_EQUAL(0, singleTsStore.GetEnsembleSize());
	REQUIRE_EQUAL(0, singleTsStore.GetLeadTimeCount());
	REQUIRE_EQUAL(mainTimeLength, singleTsStore.GetTimeLength());
	NetCdfEnsembleTimeSeriesStore<double> ensTsStore(testFilePath, var2_ens /*variable name*/, "456"/*catchment*/);
	REQUIRE_EQUAL(nEns, ensTsStore.GetEnsembleSize());
	REQUIRE_EQUAL(0, ensTsStore.GetLeadTimeCount());
	REQUIRE_EQUAL(mainTimeLength, ensTsStore.GetTimeLength());
	NetCdfTimeSeriesEnsembleTimeSeriesStore<double> ensts(testFilePath, "var2_fcast_ens" /*variable name*/, "456"/*catchment*/);
	// The returned value needs is a forecast ensemble with 3 forecasts for each time stamp, of lead length 4
	REQUIRE_EQUAL(nEns, ensts.GetEnsembleSize());
	REQUIRE_EQUAL(leadTimeLength, ensts.GetLeadTimeCount());
	REQUIRE_EQUAL(mainTimeLength, ensts.GetTimeLength());

	// We need to explicitly close the stores, to allow for File.Remove.
	singleTsStore.Close();
	ensTsStore.Close();
	ensts.Close();

}

TEST_CASE("Low level IO operations of SWIFT netcdf file")
{
	auto temp = FileSystemHelper::GetTempFile("lowlevel_io_%%%%%%%%%%%%.nc");
	std::string testFilePath = temp.generic_string();
	TempFileCleaner cleaner(testFilePath);

	size_t nEns = 3;
	size_t leadTimeLength = 4;
	size_t mainTimeLength = 5;

	// units: hours since 2010-08-01 14:00:00 +0000"
	ptime startDate = ptime(date(2010, 8, 1)) + hours(14);
	TimeStep timeStep = TimeStep::GetHourly();
	string timeUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(startDate, timeStep);
	vector<double> timeVar = DTH::SeqVec(0.0, 1.0, mainTimeLength);
	vector<ptime> timeIndex = timeStep.AddSteps(startDate, timeVar);

	map<string, VariableDefinition> varDefs;

	SwiftNetCDFAccess* access = CreateTestSwiftFile(testFilePath,
		nEns,
		leadTimeLength,
		mainTimeLength,
		startDate,
		timeStep);

	vector<double> values = access->GetValues<double>(var1_obs, 0);
	REQUIRE(values.size() == mainTimeLength);
	double missValue = TEST_FILL_VALUE;
	REQUIRE(AllEqual<double>(values, missValue));

	vector<double> expected = DTH::SeqVec(0, 1, mainTimeLength);
	access->SetValues(var1_obs, 0, expected);
	values = access->GetValues<double>(var1_obs, 0);
	REQUIRE(values.size() == mainTimeLength);
	REQUIRE(VectorEqual<double>(expected, values));
	REQUIRE(VectorEqual<ptime>(timeIndex, access->GetTimeDim()));

	delete access;

	access = new SwiftNetCDFAccess(testFilePath);
	REQUIRE(VectorEqual<ptime>(timeIndex, access->GetTimeDim()));

	values = access->GetValues<double>(var1_obs, 0);
	REQUIRE(values.size() == mainTimeLength);

	expected = DTH::SeqVec(0, 1, mainTimeLength);
	values = access->GetValues<double>(var1_obs, 0);
	REQUIRE(values.size() == mainTimeLength);
	REQUIRE(VectorEqual<double>(expected, values));

	delete access;

}

TEST_CASE("Write and read a single netcdf time series file")
{
	auto temp = FileSystemHelper::GetTempFile("single_%%%%%%%%%%%%.nc");
	std::string testFilePath = temp.generic_string();
	TempFileCleaner cleaner(testFilePath);

	ptime startDate = from_iso_string("20000101T000000");

	size_t numberOfEnsembles = 1;
	size_t leadTime = 1;
	size_t numberOfTimeSteps = 100;
	TimeStep timeStep = TimeStep::GetDaily();

	string stationOneId = "1";
	vector<string> stationIds({ stationOneId });

	string varOneName = "rain_der";
	string timeUnits;
	NcFileDef args = CreateNcIo<NetCdfSingleSeriesStore<double>>(timeUnits, stationIds, numberOfTimeSteps, numberOfEnsembles, leadTime, timeStep, startDate, timeStep, varOneName);
	NetCdfSingleSeriesStore<double> writeSeries(testFilePath, args.first, args.second, varOneName, stationOneId);

	auto inputTimeSeriesData = DTH::SeqVec(0, 1, numberOfTimeSteps);
	TTimeSeries<double> inputTs(inputTimeSeriesData, startDate, timeStep);


	writeSeries.Write(&inputTs);

	TTimeSeries<double>* outputTs = nullptr;
	outputTs = writeSeries.Read();
	REQUIRE(DTH::AreEqual(outputTs, &inputTs));
	delete outputTs;

	// At this point, reading through the reader would fail! Known behavior though.
	// https://www.unidata.ucar.edu/software/netcdf/netcdf-4/newdocs/netcdf-c/nc_005fsync.html says/said:
	//An easier way to accomplish sharing(and what is now recommended) is to have the writer and readers 
	//open the dataset with the NC_SHARE flag, and then it will not be necessary to call nc_sync at all.
	//However, the nc_sync function still provides finer granularity than the NC_SHARE flag, if only a 
	//few netCDF accesses need to be synchronized among processes.
	//outputTs = readSeries.Read();
	//REQUIRE(DTH::AreEqual(outputTs, &inputTs));
	//delete outputTs;

	writeSeries.Close();

	NetCdfSingleSeriesStore<double> readSeries(testFilePath, varOneName, stationOneId);
	REQUIRE_EQUAL(0, readSeries.GetEnsembleSize());
	REQUIRE_EQUAL(0, readSeries.GetLeadTimeCount());
	REQUIRE_EQUAL(numberOfTimeSteps, readSeries.GetTimeLength());

	outputTs = readSeries.Read();
	REQUIRE(DTH::AreEqual(outputTs, &inputTs));
	REQUIRE_EQUAL(numberOfTimeSteps, outputTs->GetLength());
	REQUIRE_EQUAL(startDate, outputTs->GetStartDate());
	REQUIRE_EQUAL(timeStep.GetName(), outputTs->GetTimeStep().GetName());
	delete outputTs;
}

TEST_CASE("Write and read an ensemble netcdf file")
{
	auto temp = FileSystemHelper::GetTempFile("ensemble_%%%%%%%%%%%%.nc");
	std::string testFilePath = temp.generic_string();
	TempFileCleaner cleaner(testFilePath);

	ptime startDate = from_iso_string("20080506T000000");

	int numberOfEnsembles = 5;
	int leadTime = 1;
	int numberOfTimeSteps = 100;
	TimeStep timeStep = TimeStep::GetHourly();

	string stationOneId = "1";
	vector<string> stationIds({ stationOneId });

	string varOneName = "rain_ens";
	string timeUnits;
	vector<double> timeVar;
	vector<string> varNames;
	map<string, VariableAttributes> varAttribs;

	NcFileDef args = CreateNcIo<NetCdfEnsembleTimeSeriesStore<double>>(timeUnits, stationIds, numberOfTimeSteps, numberOfEnsembles, leadTime, timeStep, startDate, timeStep, varOneName);
	NetCdfEnsembleTimeSeriesStore<double> writeSeries(testFilePath, args.first, args.second, varOneName, stationOneId);

	vector<double*> inputTimeSeriesVec;

	for (int i = 0; i < numberOfEnsembles; i++)
	{
		double* inputTimeSeriesData = new double[numberOfTimeSteps];
		for (int j = 0; j < numberOfTimeSteps; j++)
			inputTimeSeriesData[j] = numberOfEnsembles * i + j;
		inputTimeSeriesVec.push_back(inputTimeSeriesData);
	}

	MultiTimeSeries<TimeSeries*> inputMts(inputTimeSeriesVec, numberOfTimeSteps, startDate, timeStep);

	writeSeries.Write(&inputMts);
	writeSeries.Close();

	for (double* data : inputTimeSeriesVec)
		delete[] data;
	inputTimeSeriesVec.clear();

	NetCdfEnsembleTimeSeriesStore<double> readSeries(testFilePath, varOneName, stationOneId);
	//REQUIRE(readAccess->TimeForIndex(0) == startDate);

	REQUIRE_EQUAL(numberOfEnsembles, readSeries.GetEnsembleSize());
	REQUIRE_EQUAL(0, readSeries.GetLeadTimeCount());
	REQUIRE_EQUAL(numberOfTimeSteps, readSeries.GetTimeLength());

	MultiTimeSeries<TimeSeries*>* outputMts = readSeries.Read();
	readSeries.Close();

	REQUIRE(TimeSeriesOperations<TimeSeries>::AreEnsembleTimeSeriesEqual(inputMts, *outputMts));

	delete outputMts;

}

TEST_CASE("Write and read a forecast ensemble netcdf time series file")
{
	auto temp = FileSystemHelper::GetTempFile("forecast_%%%%%%%%%%%%.nc");
	std::string testFilePath = temp.generic_string();
	TempFileCleaner cleaner(testFilePath);

	ptime startDate = from_iso_string("20090203T000000");

	size_t tsEtsLength = 7;
	size_t ensSize = 5;
	size_t tsLength = 13;
	TimeStep hourly = TimeStep::GetHourly();
	TimeStep daily = TimeStep::GetDaily();
	TimeStep etsTstep = hourly * 6;
	TimeStep fcastTstep = hourly / 4;

	string stationOneId = "1";
	vector<string> stationIds;
	stationIds.push_back(stationOneId);

	string varOneName = "rain_ens_fcast";
	string timeUnits;
	vector<double> timeVar;
	NcFileDef args = CreateNcIo<NetCdfTimeSeriesEnsembleTimeSeriesStore<double>>(timeUnits, stationIds, tsEtsLength, ensSize, tsLength, fcastTstep, startDate, etsTstep, varOneName);
	NetCdfTimeSeriesEnsembleTimeSeriesStore<double> writeSeries(testFilePath, args.first, args.second, varOneName, stationOneId);

	using DTH = datatypes::tests::DataTestHelper<double>;
	EnsembleForecastTimeSeries<> ensTs = DTH::CreateTsEnsembleTs(
		tsEtsLength,
		ensSize,
		tsLength,
		startDate,
		etsTstep,
		fcastTstep,
		&DTH::DecimalRamp);

	for (int i = 0; i < tsEtsLength; i++)
		writeSeries.SetForecasts(i, ensTs.GetValue(i));


	auto entsActualPtr = writeSeries.GetSeries("varOneName");
	REQUIRE_EQUAL_COMPARER(ensTs, *entsActualPtr, TimeSeriesOperations<TimeSeries>::AreTimeSeriesEnsembleTimeSeriesEqual);
	delete entsActualPtr;

	writeSeries.Close();

	NetCdfTimeSeriesEnsembleTimeSeriesStore<double> readSeries(testFilePath, varOneName, stationOneId);

	REQUIRE_EQUAL(ensSize, readSeries.GetEnsembleSize());
	REQUIRE_EQUAL(tsLength, readSeries.GetLeadTimeCount());
	REQUIRE_EQUAL(tsEtsLength, readSeries.GetTimeLength());

	vector<double> expectedLeadTimeVec = DTH::SeqVec(0.25, 0.25, tsLength);

	REQUIRE(VectorEqual<double>(expectedLeadTimeVec, readSeries.GetLeadTimeDim()));

	entsActualPtr = readSeries.GetSeries("varOneName");
	REQUIRE_EQUAL_COMPARER(ensTs, *entsActualPtr, TimeSeriesOperations<TimeSeries>::AreTimeSeriesEnsembleTimeSeriesEqual);
	delete entsActualPtr;

	readSeries.Close();
}

TEST_CASE("Time Series written to netCDF")
{

	//stationIds <-c(123, 456)
	int nEns = 3;
	int nLead = 4;
	int nTimeSteps = 48;
	TimeStep timeStep = TimeStep::GetHourly();

	auto startDate = ptime(date(2010, 8, 1)) + hours(14);
	string timeUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(startDate, timeStep);
	// "hours since 2010-08-01 14:00:00 +0000");
	auto timeVar = DTH::SeqVec(0, 1, nTimeSteps);
	vector<double> leadTimeVar = DTH::SeqVec(1.0, 1.0, nLead);

	boost::filesystem::path fPath = FileSystemHelper::GetTempFile();

	const string fname = fPath.generic_string();
	TempFileCleaner cleaner(fname);

	int nVars = 2;

	std::map<std::string, VariableDefinition> varDefs;
	varDefs[var1_fcast_ens] = VariableDefinition(var1_fcast_ens, string(DATATYPES_DOUBLE_PRECISION_ID), string(DATATYPES_FOUR_DIMENSIONS_DATA), var1_fcast_ens, string("mm"), TEST_FILL_VALUE, string("2"), tDesc, pt);
	varDefs[var2_fcast_ens] = VariableDefinition(var2_fcast_ens, string(DATATYPES_DOUBLE_PRECISION_ID), string(DATATYPES_FOUR_DIMENSIONS_DATA), var2_fcast_ens, string("mm"), TEST_FILL_VALUE, string("2"), tDesc, pt);

	//SwiftNetCDFAccess* writeAccess = CreateNcIo(fname, startDate, timeStep, numberOfTimeSteps, numberOfEnsembles, leadTime, varOneName, stationIds);
	//SwiftNetCDFTimeSeriesStore<double> store(fname, nEns, nLead, timeUnits, timeVar, stationIds, varNames, varAttrs);
	//NetCdfSingleSeriesStore<double> store(writeAccess, varOneName, stationOneId);

	//REQUIRE_EQUAL(nEns, store.GetEnsembleSize());
	//REQUIRE_EQUAL(nLead, store.GetLeadTimeCount());
	//REQUIRE_EQUAL(startDate, store.GetStart());

	NetCdfTimeSeriesEnsembleTimeSeriesStore<double> ensts(fname, nEns, leadTimeVar, timeUnits, timeVar, stationIds,
		varDefs, var2_fcast_ens /*variable name*/, "456"/*catchment*/);
	REQUIRE_EQUAL(nEns, ensts.GetEnsembleSize());
	REQUIRE_EQUAL(nLead, ensts.GetLeadTimeCount());

	int offset = 22;

	// The data, if retrieved now, returns everything with default values (FillValue)
	//ensts.GetForecasts

	std::vector<double*>* values = DTH::Seq(0, 1, nLead, nEns);
	MultiTimeSeries<TimeSeries*> mts(*values, nLead, ensts.TimeForIndex(offset), timeStep);
	ensts.SetForecasts(offset, &mts);

	MultiTimeSeries<TimeSeries*> * mtsGet = ensts.GetForecasts(offset);

	DTH::DeleteElements(*values);
	delete values;
	delete mtsGet;
	ensts.Close();

}

TEST_CASE("Swift NetCDF variable attributes")
{
	using VD = vector<double>;
	const TimeStep hourly = TimeStep::GetHourly();
	const TimeStep daily = TimeStep::GetDaily();

	ptime d(date(2011, 2, 16));
	auto f = [](const ptime& d, const TimeStep& t) { return SwiftNetCDFAccess::CreateTimeUnitsAttribute(d, t); };
	string since(" since 2011-02-16T00:00:00 +0000");
	REQUIRE(f(d, daily) == "days" + since);
	REQUIRE(f(d, daily * 2) == "days" + since);
	REQUIRE(f(d, hourly) == "hours" + since);
	REQUIRE(f(d, hourly * 2) == "hours" + since);
	REQUIRE(f(d, hourly * 48) == "days" + since);
	REQUIRE(f(d, hourly / 4) == "minutes" + since);

	auto g = [](const ptime& d, const TimeStep& t, const size_t& length) { return SwiftNetCDFAccess::CreateTimeVector(d, t, length); };
	auto areEqual = [](const VD& a, const VD& b) { return datatypes::tests::AreEqual<double>(a, b); };

	REQUIRE(areEqual(g(d, daily, 2), VD({ 0, 1 })));
	REQUIRE(areEqual(g(d, daily, 3), VD({ 0, 1, 2 })));
	REQUIRE(areEqual(g(d, daily * 2, 2), VD({ 0, 2 })));
	REQUIRE(areEqual(g(d, hourly * 48, 2), VD({ 0, 2 })));

	// hours since
	REQUIRE(areEqual(g(d, daily / 2, 2), VD({ 0, 12 })));
	REQUIRE(areEqual(g(d, daily / 4, 2), VD({ 0, 6 })));

	REQUIRE(areEqual(g(d, hourly * 6, 2), VD({ 0, 6 })));
	REQUIRE(areEqual(g(d, hourly * 2, 2), VD({ 0, 2 })));
	REQUIRE(areEqual(g(d, hourly, 2), VD({ 0, 1 })));

	// minutes since
	REQUIRE(areEqual(g(d, hourly / 2, 2), VD({ 0, 30 })));
	REQUIRE(areEqual(g(d, hourly / 4, 2), VD({ 0, 15 })));
	REQUIRE(areEqual(g(d, hourly / 10, 2), VD({ 0, 6 })));

}

TEST_CASE("DimensionsDefinition")
{

	size_t tsEtsLength = 4;
	size_t ensSize = 5;
	size_t tsLength = 7;
	ptime start(date(2008, 3, 4));
	TimeStep hourly = TimeStep::GetHourly();
	TimeStep daily = TimeStep::GetDaily();
	TimeStep etsTstep = hourly * 6;
	TimeStep fcastTstep = hourly / 4;
	int forecastStartOffset = 1;

	EnsembleForecastTimeSeries<> ensTs = DTH::CreateTsEnsembleTs(
		tsEtsLength,
		ensSize,
		tsLength,
		start,
		etsTstep,
		fcastTstep,
		&DTH::DecimalRamp,
		forecastStartOffset);

	using datatypes::utils::SeqVec;

	//vector<EnsembleForecastTimeSeries<>::ElementType> values = ensTs.GetValuesVector();
	//TimeStep& tsEnsTstep = TimeStep::GetUnknown(), 
	//ptime& tsEnsStart = not_a_date_time, const vector<string>& stationIds = DEFAULT_STATION_IDENTIFIER)
	auto dimDef = DimensionsFromSeries<double>(ensTs);
	REQUIRE(dimDef.EnsembleSize == ensSize);
	REQUIRE(dimDef.TimeUnits == "hours since 2008-03-04T00:00:00 +0000");
	REQUIRE(VectorEqual<double>(dimDef.TimeVar, SeqVec<double>(0, 6, tsEtsLength)));
	REQUIRE(VectorEqual<double>(dimDef.LeadTimeVar, SeqVec<double>(0.25, 0.25, tsLength)));
}

