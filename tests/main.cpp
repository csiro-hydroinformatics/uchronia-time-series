#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <boost/filesystem.hpp>
#include "datatypes/tests/datatypes_test_helpers.hpp"
#include "datatypes/tests/datatypes_test_helpers.h"
#include "datatypes/time_series.hpp"
#include "datatypes/time_series_store.hpp"
#include "datatypes/time_series_io.hpp"
#include "catch_macros.hpp"

using namespace datatypes::tests;

std::string DatatypesTestPath(const string& relativePath) {
	return TestDataLocationHelper::BuildPath({ TestDataLocationHelper::ReadEnvironmentVariable("SWIFT_TEST_DIR"), relativePath });
}

using namespace boost::gregorian;
using namespace datatypes::tests;
using namespace datatypes::timeseries;
using namespace std;

TEST_CASE("Basic access - single and double precision")
{
	double inputValues[6] = { 0, 1, 2, 22, 4, 5 };
	auto td = DataTestHelper<double>::Create(inputValues, 6);
	ptime start = td.GetStartDate();
	auto threeHoursLater = start + hours(3);
	REQUIRE_EQUAL(22.0, td[3]);
	REQUIRE_EQUAL(22.0, td[threeHoursLater]);

	float floatInputValues[6] = { 0, 1, 2, 42, 4, 5 };
	auto tf = DataTestHelper<float>::Create(floatInputValues, 6);
	REQUIRE_EQUAL(42.0f, tf[3]);
	REQUIRE_EQUAL(42.0f, tf[threeHoursLater]);

}

TEST_CASE("Time step calculation and number of items in the series are consistent")
{
	// After refactoring time series/time step; got an issue.
	// https://jira.csiro.au/browse/WIRADA-172
	// This ended up related to the calculation of time steps and elements and inconsistencies of length 
	// btween model runner and time series.
	TimeSeries ts;
	REQUIRE_EQUAL(TimeStep::GetHourly(), ts.GetTimeStep());
	ptime start = ts.GetStartDate();
	auto newStart = start + hours(1);

	ts.Reset(1, newStart);
	REQUIRE_EQUAL(TimeStep::GetHourly(), ts.GetTimeStep());
	REQUIRE_EQUAL(newStart, ts.GetStartDate());
	REQUIRE_EQUAL(newStart, ts.GetEndDate());

	size_t numSteps = 4;
	ts.Reset(numSteps, newStart);

	REQUIRE_EQUAL(0, ts.LowerIndexForTime(newStart));
	REQUIRE_EQUAL(0, ts.IndexForTime(newStart));
	REQUIRE_EQUAL(0, ts.UpperIndexForTime(newStart));
	REQUIRE_EQUAL(0, ts.IndexForTime(newStart + seconds(42)));
	REQUIRE_EQUAL(1, ts.UpperIndexForTime(newStart + seconds(42)));

	REQUIRE_EQUAL(1, ts.IndexForTime(newStart + hours(1)));
	REQUIRE_EQUAL(1, ts.UpperIndexForTime(newStart + hours(1)));
	REQUIRE_EQUAL(1, ts.IndexForTime(newStart + hours(1) + seconds(42)));
	REQUIRE_EQUAL(2, ts.IndexForTime(newStart + hours(2)));

	REQUIRE_EQUAL(TimeStep::GetHourly(), ts.GetTimeStep());
	REQUIRE_EQUAL(newStart, ts.GetStartDate());
	REQUIRE_EQUAL(newStart + hours(numSteps - 1), ts.GetEndDate());

	ts.SetTimeStep(TimeStep::GetDaily());
	REQUIRE_EQUAL(TimeStep::GetDaily(), ts.GetTimeStep());
	REQUIRE_EQUAL(newStart, ts.GetStartDate());
	REQUIRE_EQUAL(newStart + days(numSteps - 1), ts.GetEndDate());

}

// what about the cases where the time series falls between two known instants

TEST_CASE("Time series of forecasts")
{
	ForecastTimeSeries<TimeSeries> forecast;
	ptime s(date(2009, 1, 1));
	forecast.Reset(3, s, nullptr);
	REQUIRE_NULL(forecast[0]);
	REQUIRE(forecast.IsMissingValue(forecast[0]));
	REQUIRE_NULL(forecast[2]);
	REQUIRE(forecast.IsMissingValue(forecast[2]));
}

TEST_CASE("Time Series of ensemble forecasts")
{
	EnsembleForecastTimeSeries<> forecasts;
	ptime s(date(2009, 1, 1));
	forecasts.Reset(3, s, nullptr);
	REQUIRE_NULL(forecasts[0]);
	REQUIRE(forecasts.IsMissingValue(forecasts[0]));
	REQUIRE_NULL(forecasts[2]);
	REQUIRE(forecasts.IsMissingValue(forecasts[2]));
}

TEST_CASE("MultiTimeSeries - template argument is TimeSeries i.e. not a pointer")
{
	MultiTimeSeries<TimeSeries> mts;
	ptime s(date(2009, 1, 1));
	mts.ResetSeries(3, 4, s, TimeStep::GetDaily());
	double mypi = 3.1415;
	mts.Set(0, 1, mypi);
	TimeSeries ts = mts.Get(0);
	REQUIRE_EQUAL(mypi, ts[1]);
	mts.Set(0, 1, mypi * 2);
	REQUIRE_EQUAL(mypi, ts[1]);
}

TEST_CASE("MultiTimeSeries - template argument is a pointer to a TimeSeries")
{
	ptime s(date(2009, 1, 1));
	MultiTimeSeries<TimeSeries*> mtsPtr;
	double mypi = 3.1415;
	mtsPtr.ResetSeries(3, 4, s, TimeStep::GetDaily());
	mtsPtr.Set(0, 1, mypi);
	TimeSeries * pts = mtsPtr.Get(0);
	REQUIRE_EQUAL(mypi, (*pts)[1]);
	mtsPtr.Set(0, 1, mypi * 2);
	REQUIRE_EQUAL(mypi * 2, (*pts)[1]);

}

TEST_CASE("Ensembles of time series")
{
	ptime start = ptime(date(2000, 1, 1));

	vector<vector<double>> d = {
		{ 1.0, 1.1, 1.2, 1.3 },
		{ 2.0, 2.1, 2.2, 2.3 },
		{ 3.0, 3.1, 3.2, 3.3 }
	};
	TimeSeriesEnsemble<TimeSeries> ensTs(d, start, TimeStep::GetHourly());

	REQUIRE_EQUAL(3, ensTs.Size());
	for (int i = 0; i < 3; i++)
	{
		REQUIRE_EQUAL(4, ensTs.GetLength(i));
		REQUIRE_EQUAL(start, ensTs.Get(i)->GetStartDate());
	}
	REQUIRE_EQUAL(1.0, ensTs.Get(0, 0));
	REQUIRE_EQUAL(2.1, ensTs.Get(1, 1));
	REQUIRE_EQUAL(3.3, ensTs.Get(2, 3));


}

TEST_CASE("Different time steps")
{
	double inputValues[6] = { 0, 1, 2, 22, 4, 5 };
	ptime s(date(2009, 1, 1));

	boost::function<time_duration(int increment)> stepIncrementer;

	boost::function<void(TimeSeries&, boost::function<time_duration(int increment)>)> testFun =
		[&](TimeSeries& td, boost::function<time_duration(int increment)> incrementer)
	{
		REQUIRE_EQUAL(s, td.GetStartDate());
		REQUIRE_EQUAL(s + incrementer(6 - 1), td.GetEndDate());
		auto threeStepsLater = s + incrementer(3);
		REQUIRE_EQUAL(22.0, td[3]);
		REQUIRE_EQUAL(22.0, td[threeStepsLater]);
	};

	auto td = DataTestHelper<double>::Create(inputValues, 6, s, TimeStep::GetHourly());
	testFun(td, [&](long n) {return hours(n); });
	td = DataTestHelper<double>::Create(inputValues, 6, s, TimeStep::GetDaily());
	testFun(td, [&](long n) {return hours(24 * n); });
}

TEST_CASE("Missing values")
{
	int n = 20;
	int naIndex = 3;
	int naIndex2 = 5;
	double naCode = -9999;
	auto td = DataTestHelper<double>::Ramp(n);
	auto td2 = DataTestHelper<double>::Ramp(n);

	td[naIndex] = naCode;
	td2[naIndex2] = naCode;

	auto added = td + td2;

	REQUIRE(added.IsMissingValue(added[naIndex]));
	REQUIRE(added.IsMissingValue(added[naIndex2]));

	REQUIRE_EQUAL(0, added[0]);
	REQUIRE_EQUAL(2.0, added[1]);
	REQUIRE_EQUAL(4.0, added[2]);
	REQUIRE_EQUAL(naCode, added[3]);
	REQUIRE_EQUAL(8.0, added[4]);
	REQUIRE_EQUAL(naCode, added[5]);
	REQUIRE_EQUAL(12.0, added[6]);
	REQUIRE_EQUAL(20.0, added[10]);

	// This initial time series is not modified (weak check this was not a shallow copy)
	REQUIRE_EQUAL(10.0, td[10]);

}

TEST_CASE("Time series subsetting")
{
	int n = 20;
	auto td = DataTestHelper<double>::Ramp(n);
	auto valArray = td.GetValues(3, 6);
	REQUIRE_EQUAL(3, valArray[0]);
	REQUIRE_EQUAL(6, valArray[3]);
	delete[] valArray;

	TimeSeries tssub = td.Subset(3, 6);
	REQUIRE_EQUAL(4, tssub.GetLength());
	REQUIRE_EQUAL(3, tssub[0]);
	REQUIRE_EQUAL(6, tssub[3]);

}

TEST_CASE("Time series multiplication")
{
	int n = 20;
	int naIndex = 3;
	double naCode = -9999;
	auto td = TimeSeries(DataTestHelper<double>::Ramp(n));
	//td.SetNA(naCode);
	td[naIndex] = naCode;

	auto result = td * 1.1;
	REQUIRE_EQUAL(naCode, result[naIndex]);
	REQUIRE_EQUAL(0, result[0]);
	REQUIRE_EQUAL(1.1, result[1]);
	REQUIRE_EQUAL(11.0, result[10]);

	// This initial time series is not modified (weak check this was not a shallow copy)
	REQUIRE_EQUAL(10.0, td[10]);

}

//TEST_CASE("Multiple variables")
//{
//	// TODO
//	// There may be advantages in having a class that represent multivariate time series
//	// and/or with quality code for time series data. 
//	// This is a feature that needs several well defined use cases. 
//	// Meanwhile the fact that netCDF data storage is multi-variate need 
//	// not be reflected in the high-level concept of time series 
//}

//TEST_CASE("Disaggregation of time series to finer time step using semantics")
//{
//}

//TEST_CASE("Transparently write to a back end that is not an array")
//{
//}

// maybe the netCDF handling should be in a separate test suite.
TEST_CASE("Read operations from a netCDF SWIFT data file")
{


	/*
	See file
	create_swift_netcdf_ens_data.R
	in the same directory as the present file
	*/
	std::string testDataFile = TestDataLocationHelper::BuildPath({ TestDataLocationHelper::ReadEnvironmentVariable("SWIFT_TEST_DIR"), "netcdf", "testswift.nc" });
	SwiftNetCDFTimeSeriesStore<double> store(testDataFile);

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


	// Check single realixzation time series

	auto singleTsStore = store.Get("var2_obs" /*variable name*/, "456"/*catchment*/);
	REQUIRE_EQUAL(0, singleTsStore->GetEnsembleSize());
	REQUIRE_EQUAL(0, singleTsStore->GetLeadTimeCount());

	auto singleTsPtr = singleTsStore->GetSeries();

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
	delete singleTsStore;


	auto ensTsStore = store.Get("var2_ens" /*variable name*/, "456"/*catchment*/);
	REQUIRE_EQUAL(nEns, ensTsStore->GetEnsembleSize());
	REQUIRE_EQUAL(0, ensTsStore->GetLeadTimeCount());

	auto ensTs = ensTsStore->GetEnsembleSeries();

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

	auto ensts = store.Get("var2_fcast_ens" /*variable name*/, "456"/*catchment*/);
	// The returned value needs is a forecast ensemble with 3 forecasts for each time stamp, of lead length 4
	REQUIRE_EQUAL(nEns, ensts->GetEnsembleSize());
	REQUIRE_EQUAL(nLead, ensts->GetLeadTimeCount());

	int offset = 22;

	MultiTimeSeries<TimeSeries*> * mts = ensts->GetForecasts(offset);

	REQUIRE_EQUAL(nEns, mts->Size());

	int replicateNum = 3; // R index
	int replicateIndex = replicateNum - 1; // C index
	int mult = 2; // because var2_fcast_ens
	int offsetR = offset + 1;
	int leadTimeIndex = 1;
	int leadTimeIndexR = leadTimeIndex + 1;
	int stationNum = 2; // "456" is the second station

	auto ts = mts->Get(replicateIndex);
	REQUIRE_EQUAL(nLead, ts->GetLength());

	ptime start = ts->GetStartDate();
	REQUIRE_EQUAL(store.GetStart() + hours(offset), start);

	// finally, a check on a value in the time series
	REQUIRE_EQUAL(mult * (offsetR + 0.1 * replicateNum + leadTimeIndexR*0.01 + stationNum*0.1), ts->GetValue(leadTimeIndex));
	// We do need to reclaim the memory of these time series.

	delete mts;
	delete ensts;
}

TEST_CASE("Write and read a single netcdf time series file")
{
	boost::filesystem::path temp = boost::filesystem::unique_path(
		boost::filesystem::temp_directory_path() / "single_%%%%%%%%%%%%.nc"
		);
	std::string testFilePath = temp.generic_string();

	ptime startDate = from_iso_string("20000101T000000");

	int numberOfEnsembles = 1;
	int leadTime = 1;
	int numberOfTimeSteps = 100;
	TimeStep timeStep = TimeStep::GetDaily();

	string timeUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(startDate, timeStep);

	vector<double> timeVar;
	for (int i = 0; i < numberOfTimeSteps; i++)
		timeVar.push_back(i);

	string stationOneId = "1";

	vector<string> stationIds;
	stationIds.push_back(stationOneId);

	string varOneName = "rain_der";

	vector<string> varNames;
	varNames.push_back(varOneName);

	map<string, VariableAttributes> varAttribs;
	varAttribs[varOneName].Units = "mm";
	varAttribs[varOneName].FillValue = -9999;
	varAttribs[varOneName].Type = "9";
	varAttribs[varOneName].LongName = "test data";

	SwiftNetCDFAccess* writeAccess = new SwiftNetCDFAccess(testFilePath, numberOfEnsembles, leadTime, timeUnits, timeVar, stationIds, varNames, varAttribs);

	SwiftNetCDFTimeSeries<double>* writeSeries = new SwiftNetCDFTimeSeries<double>(writeAccess, varOneName, stationOneId);

	double* inputTimeSeriesData = new double[numberOfTimeSteps];
	for (int i = 0; i < numberOfTimeSteps; i++)
		inputTimeSeriesData[i] = i;
	TTimeSeries<double>* inputTs = new TTimeSeries<double>(inputTimeSeriesData, numberOfTimeSteps, startDate, timeStep);

	writeSeries->SetSeries(inputTs);

	delete[] inputTimeSeriesData;
	delete inputTs;

	delete writeSeries;
	delete writeAccess;



	SwiftNetCDFAccess* readAccess = new SwiftNetCDFAccess(testFilePath);

	SwiftNetCDFTimeSeries<double>* readSeries = new SwiftNetCDFTimeSeries<double>(readAccess, varOneName, stationOneId);

	REQUIRE_EQUAL(0, readSeries->GetEnsembleSize());
	REQUIRE_EQUAL(0, readSeries->GetLeadTimeCount());
	REQUIRE_EQUAL(numberOfTimeSteps, readSeries->GetTimeLength());

	TTimeSeries<double>* outputTs = readSeries->GetSeries();

	REQUIRE_EQUAL(numberOfTimeSteps, outputTs->GetLength());
	REQUIRE_EQUAL(startDate, outputTs->GetStartDate());
	REQUIRE_EQUAL(timeStep.GetName(), outputTs->GetTimeStep().GetName());

	for (int i = 0; i < numberOfTimeSteps; i++)
		REQUIRE_EQUAL(i, outputTs->GetValue(i));

	delete outputTs;

	delete readSeries;
	delete readAccess;

	remove(testFilePath.c_str());
}

TEST_CASE("Write and read an ensemble netcdf file")
{
	boost::filesystem::path temp = boost::filesystem::unique_path(
		boost::filesystem::temp_directory_path() / "ensemble_%%%%%%%%%%%%.nc"
		);
	std::string testFilePath = temp.generic_string();

	ptime startDate = from_iso_string("20000101T000000");

	int numberOfEnsembles = 5;
	int leadTime = 1;
	int numberOfTimeSteps = 100;
	TimeStep timeStep = TimeStep::GetHourly();

	string timeUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(startDate, timeStep);

	vector<double> timeVar;
	for (int i = 0; i < numberOfTimeSteps; i++)
		timeVar.push_back(i);

	string stationOneId = "1";

	vector<string> stationIds;
	stationIds.push_back(stationOneId);

	string varOneName = "rain_ens";

	vector<string> varNames;
	varNames.push_back(varOneName);

	map<string, VariableAttributes> varAttribs;
	varAttribs[varOneName].Units = "mm";
	varAttribs[varOneName].FillValue = -9999;
	varAttribs[varOneName].Type = "9";
	varAttribs[varOneName].LongName = "test data";

	SwiftNetCDFAccess* writeAccess = new SwiftNetCDFAccess(testFilePath, numberOfEnsembles, leadTime, timeUnits, timeVar, stationIds, varNames, varAttribs);

	SwiftNetCDFTimeSeries<double>* writeSeries = new SwiftNetCDFTimeSeries<double>(writeAccess, varOneName, stationOneId);

	vector<double*> inputTimeSeriesVec;

	for (int i = 0; i < numberOfEnsembles; i++)
	{
		double* inputTimeSeriesData = new double[numberOfTimeSteps];
		for (int j = 0; j < numberOfTimeSteps; j++)
			inputTimeSeriesData[j] = numberOfEnsembles * i + j;
		inputTimeSeriesVec.push_back(inputTimeSeriesData);
	}

	MultiTimeSeries<TimeSeries*> inputMts(inputTimeSeriesVec, numberOfTimeSteps, startDate, timeStep);

	writeSeries->SetEnsemble(&inputMts);

	for (double* data : inputTimeSeriesVec)
		delete[] data;
	inputTimeSeriesVec.clear();

	delete writeSeries;
	delete writeAccess;



	SwiftNetCDFAccess* readAccess = new SwiftNetCDFAccess(testFilePath);

	SwiftNetCDFTimeSeries<double>* readSeries = new SwiftNetCDFTimeSeries<double>(readAccess, varOneName, stationOneId);

	REQUIRE_EQUAL(numberOfEnsembles, readSeries->GetEnsembleSize());
	REQUIRE_EQUAL(0, readSeries->GetLeadTimeCount());
	REQUIRE_EQUAL(numberOfTimeSteps, readSeries->GetTimeLength());

	MultiTimeSeries<TimeSeries*>* outputMts = readSeries->GetEnsembleSeries();

	REQUIRE_EQUAL(numberOfEnsembles, outputMts->Size());

	for (int i = 0; i < numberOfEnsembles; i++)
	{
		TimeSeries outputTs = outputMts->Get(i);

		REQUIRE_EQUAL(numberOfTimeSteps, outputTs.GetLength());
		REQUIRE_EQUAL(startDate, outputTs.GetStartDate());
		REQUIRE_EQUAL(timeStep.GetName(), outputTs.GetTimeStep().GetName());

		for (int j = 0; j < numberOfTimeSteps; j++)
			REQUIRE_EQUAL(numberOfEnsembles * i + j, outputTs.GetValue(j));

	}

	delete outputMts;

	delete readSeries;
	delete readAccess;

	remove(testFilePath.c_str());
}

TEST_CASE("Write and read a forecast ensemble netcdf time series file")
{
	boost::filesystem::path temp = boost::filesystem::unique_path(
		boost::filesystem::temp_directory_path() / "forecast_%%%%%%%%%%%%.nc"
		);
	std::string testFilePath = temp.generic_string();

	ptime startDate = from_iso_string("20000101T000000");

	int numberOfEnsembles = 5;
	int leadTime = 10;
	int numberOfTimeSteps = 10;
	TimeStep timeStep = TimeStep::GetHourly();

	string timeUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(startDate, timeStep);

	vector<double> timeVar;
	for (int i = 0; i < numberOfTimeSteps; i++)
		timeVar.push_back(i);

	string stationOneId = "1";

	vector<string> stationIds;
	stationIds.push_back(stationOneId);

	string varOneName = "rain_ens_fcast";

	vector<string> varNames;
	varNames.push_back(varOneName);

	map<string, VariableAttributes> varAttribs;
	varAttribs[varOneName].Units = "mm";
	varAttribs[varOneName].FillValue = -9999;
	varAttribs[varOneName].Type = "9";
	varAttribs[varOneName].LongName = "test data";

	SwiftNetCDFAccess* writeAccess = new SwiftNetCDFAccess(testFilePath, numberOfEnsembles, leadTime, timeUnits, timeVar, stationIds, varNames, varAttribs);

	SwiftNetCDFTimeSeries<double>* writeSeries = new SwiftNetCDFTimeSeries<double>(writeAccess, varOneName, stationOneId);

	for (int i = 0; i < leadTime; i++)
	{
		ptime sDate = startDate;

		vector<double*> inputTimeSeriesDataVec;

		for (int j = 0; j < numberOfEnsembles; j++)
		{
			double* inputData = new double[numberOfTimeSteps];

			for (int k = 0; k < numberOfTimeSteps; k++)
				inputData[k] = (leadTime * i) + (numberOfEnsembles * j) + k;

			inputTimeSeriesDataVec.push_back(inputData);
		}

		MultiTimeSeries<TimeSeries*>* inputMts = new MultiTimeSeries<TimeSeries*>(inputTimeSeriesDataVec, numberOfTimeSteps, sDate, timeStep);
		writeSeries->SetForecasts(i, inputMts);

		for (double* d : inputTimeSeriesDataVec)
			delete[] d;
		delete inputMts;

		sDate = timeStep.AddSteps(sDate, 1);
	}

	delete writeSeries;
	delete writeAccess;



	SwiftNetCDFAccess* readAccess = new SwiftNetCDFAccess(testFilePath);

	SwiftNetCDFTimeSeries<double>* readSeries = new SwiftNetCDFTimeSeries<double>(readAccess, varOneName, stationOneId);

	REQUIRE_EQUAL(numberOfEnsembles, readSeries->GetEnsembleSize());
	REQUIRE_EQUAL(leadTime, readSeries->GetLeadTimeCount());
	REQUIRE_EQUAL(numberOfTimeSteps, readSeries->GetTimeLength());

	ptime sDate = startDate;

	for (int i = 0; i < leadTime; i++)
	{
		MultiTimeSeries<TimeSeries*>* outputMts = readSeries->GetForecasts(i);

		REQUIRE_EQUAL(numberOfEnsembles, outputMts->Size());

		for (int j = 0; j < numberOfEnsembles; j++)
		{
			TimeSeries outputTs = outputMts->Get(j);

			REQUIRE_EQUAL(numberOfTimeSteps, outputTs.GetLength());
			REQUIRE_EQUAL(sDate, outputTs.GetStartDate());
			REQUIRE_EQUAL(timeStep.GetName(), outputTs.GetTimeStep().GetName());

			for (int k = 0; k < numberOfTimeSteps; k++)
				REQUIRE_EQUAL((leadTime * i) + (numberOfEnsembles * j) + k, outputTs.GetValue(k));

		}

		delete outputMts;

		sDate = timeStep.AddSteps(sDate, 1);
	}

	delete readSeries;
	delete readAccess;

	remove(testFilePath.c_str());
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
	auto timeVar = DataTestHelper<double>::SeqVec(0, 1, nTimeSteps);

	boost::filesystem::path fPath = FileSystemHelper::GetTempFile();

	const string fname = fPath.generic_string();

	std::vector<std::string> stationIds;
	stationIds.push_back(string("123"));
	stationIds.push_back(string("456"));
	int nStations = 2;

	std::vector<std::string> varNames;
	varNames.push_back(string("var1_fcast_ens"));
	varNames.push_back(string("var2_fcast_ens"));
	int nVars = 2;

	std::map<std::string, VariableAttributes> varAttrs;
	varAttrs[string("var1_fcast_ens")] = VariableAttributes();
	varAttrs[string("var2_fcast_ens")] = VariableAttributes();

	SwiftNetCDFTimeSeriesStore<double> store(fname, nEns, nLead, timeUnits, timeVar, stationIds, varNames, varAttrs);

	REQUIRE_EQUAL(nEns, store.GetEnsembleSize());
	REQUIRE_EQUAL(nLead, store.GetLeadTimeCount());
	REQUIRE_EQUAL(startDate, store.GetStart());

	auto ensts = store.Get("var2_fcast_ens" /*variable name*/, "456"/*catchment*/);
	//REQUIRE_EQUAL(nEns, ensts->GetEnsembleSize());
	//REQUIRE_EQUAL(nLead, ensts->GetLeadTimeCount());

	int offset = 22;

	// The data, if retrieved now, returns everything with default values (FillValue)
	//ensts->GetForecasts

	std::vector<double*>* values = DataTestHelper<double>::Seq(0, 1, nLead, nEns);
	MultiTimeSeries<TimeSeries*> mts(*values, nLead, ensts->TimeForIndex(offset), timeStep);
	ensts->SetForecasts(offset, &mts);

	MultiTimeSeries<TimeSeries*> * mtsGet = ensts->GetForecasts(offset);

	DataTestHelper<double>::DeleteElements(*values);
	delete values;
	delete mtsGet;
	store.Close();

	FileSystemHelper::Remove(fPath);
}

TEST_CASE("Test Time Series Masking")
{
	int len = 10;
	double* d = new double[len];
	ptime start = from_iso_string("20000101T000000");
	TimeStep ts = TimeStep::GetDaily();

	for (int i = 0; i < len; i++)
		d[i] = i;

	TimeSeries timeSeries = TimeSeries(d, len, start, ts);
	delete[] d;

	double maskingValue = -9999.0;

	ptime mStart = from_iso_string("20000103T000000");
	ptime mEnd = from_iso_string("20000109T000000");

	TimeSeries* result = TimeSeriesOperations<TimeSeries>::MaskTimeSeries(timeSeries, mStart, mEnd, maskingValue);

	for (int i = 0; i < len; i++)
	{
		if (i >= 2 && i <= 8)
			REQUIRE_EQUAL(maskingValue, result->GetValue(i));
		else
			REQUIRE_EQUAL(timeSeries.GetValue(i), result->GetValue(i));
	}

	delete result;
}

TEST_CASE("TimeStep operations")
{
	auto hourly = TimeStep::GetHourly();

	auto threeHourly = hourly * 3;
	REQUIRE_EQUAL(3600 * 3, threeHourly.GetRegularStepDuration().total_seconds());
}

class TestSingleTimeSeriesStore :
	public SingleTimeSeriesStore<double>
{
	using string = std::string;
public:

	TestSingleTimeSeriesStore(const vector<double>& values,
		const ptime& startDate, const TimeStep& timeStep = TimeStep::GetHourly())
	{
		innerTs = TimeSeries(values, startDate, timeStep);
	}

	TestSingleTimeSeriesStore(const TimeSeries& series)
	{
		innerTs = series;
	}

	string GetDataSummary() const
	{
		auto start = innerTs.GetStartDate();
		auto end = innerTs.GetEndDate();
		string result =
			string(", start: ") + to_iso_extended_string(start) +
			string(", end: ") + to_iso_extended_string(end) +
			string(", time step: ") + innerTs.GetTimeStep().GetName();
		return result;
	}

	TTimeSeries<double>* Read()
	{
		return new TTimeSeries<double>(innerTs);
	}

	TTimeSeries<double>* Read(const string& blah)
	{
		return new TTimeSeries<double>(innerTs);
	}
	std::vector<std::string> GetIdentifiers() const { datatypes::exceptions::ExceptionUtilities::ThrowNotImplemented();  std::vector<std::string> x; return x; }

private:
	TTimeSeries<double> innerTs;

};

class TestTimeSeriesEnsembleTimeSeriesStore : public TimeSeriesEnsembleTimeSeriesStore<double>
{
public:
	~TestTimeSeriesEnsembleTimeSeriesStore() { delete ensFts; }
	TestTimeSeriesEnsembleTimeSeriesStore(const EnsembleForecastTimeSeries<>& ensFts) {
		this->ensFts = new EnsembleForecastTimeSeries<>(ensFts);
	};
	MultiTimeSeries<TTimeSeries<double>*>* Read(const std::string& ensembleIdentifier)
	{
		int i = boost::lexical_cast<int>(ensembleIdentifier);
		MultiTimeSeries<TTimeSeries<double>*>* blah = ensFts->GetValue(i);
		return blah;
	}
	size_t GetLength() const
	{
		return ensFts->GetLength();
	}
	ptime GetStart() const
	{
		return ensFts->GetStartDate();
	}
	vector<string> GetItemIdentifiers() const
	{
		vector<string> result(ensFts->GetLength());
		for (size_t i = 0; i < result.size(); i++)
		{
			result[i] = boost::lexical_cast<string>(i);
		}
		return result;
	}
	string GetDataSummary() const
	{
		return "";
	}
	TimeStep GetTimeStep() const
	{
		return ensFts->GetTimeStep();
	}
private:
	EnsembleForecastTimeSeries<> * ensFts;

};
class TestEnsembleTimeSeriesStore : public EnsembleTimeSeriesStore<double> 
{
public:
	TestEnsembleTimeSeriesStore(const MultiTimeSeries<>& data)
	{
		this->data = data;
	}

	MultiTimeSeries<TTimeSeries<double>*>* Read()
	{
		return data.AsPointerSeries();
	}
	string GetDataSummary() const
	{
		return "";
	}

private:
	MultiTimeSeries<> data;
};

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

TEST_CASE("MultiFileTimeSeriesEnsembleTimeSeriesStore") {

	// https://jira.csiro.au/browse/WIRADA-349

	string rainObsId;
	string petObsId;
	string rainFcastId;
	string petFcastId;

	TimeSeriesLibrary dataLibrary = TestDataLocationHelper::CreateEnsembleTimeSeriesLibrary(rainObsId, petObsId, rainFcastId, petFcastId);

	REQUIRE_THROWS(dataLibrary.GetSingle(rainFcastId));
	REQUIRE_THROWS(dataLibrary.GetTimeSeriesEnsembleTimeSeries("some_unknown_identifier"));

	auto fcastSeries = dataLibrary.GetTimeSeriesEnsembleTimeSeries(rainFcastId);
	size_t n = fcastSeries->GetLength();
	auto s = fcastSeries->GetStartDate();
	auto e = fcastSeries->GetEndDate();
	auto tstep = fcastSeries->GetTimeStep();

	REQUIRE(n == tstep.GetNumSteps(s, e));
	MultiTimeSeriesPtr<>* mts = nullptr;
	REQUIRE_NOTHROW(mts = fcastSeries->GetValue(0));
	if (mts != nullptr) {
		delete mts; mts = nullptr;
	}
	// Calling again does not cause a double mem free
	REQUIRE_NOTHROW(mts = fcastSeries->GetValue(0));
	if (mts != nullptr) {
		delete mts; mts = nullptr;
	}
	REQUIRE_NOTHROW(mts = fcastSeries->GetValue(n-1));
	if (mts != nullptr) {
		delete mts; mts = nullptr;
	}

	REQUIRE_THROWS(mts = fcastSeries->GetValue(-1));
	if (mts != nullptr) {
		delete mts; mts = nullptr;
	}
	REQUIRE_THROWS(mts = fcastSeries->GetValue(n));
	if (mts != nullptr) {
		delete mts; mts = nullptr;
	}

	delete fcastSeries;

	fcastSeries = dataLibrary.GetTimeSeriesEnsembleTimeSeries(rainFcastId);
	delete fcastSeries;
}
