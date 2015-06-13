#include "xUnit++.h"
#include "datatypes_test_helpers.hpp"
#include "../datatypes/datatypes/time_series.h"
#include "../datatypes/datatypes/time_series_store.h"
#include <boost/filesystem.hpp>

using xUnitpp::xUnitAssert;
using namespace boost::gregorian;
using namespace datatypes::tests;
using namespace std;

SUITE("Time Series")
{

	const string TestDataDir("\\\\clw-02-mel.it.csiro.au\\OSM_MEL_CLW_HYDROFORECAST_common\\Staff\\per202\\sample_data\\netcdf\\");
	//const string TestDataDir("F:\\STSF\\tmp\\");



	FACT("Basic access - single and double precision")
	{
		double inputValues[6] = { 0, 1, 2, 22, 4, 5 };
		auto td = DataTestHelper<double>::Create(inputValues, 6);
		ptime start = td.GetStartDate();
		auto threeHoursLater = start + hours(3);
		Assert.Equal(22.0, td[3]);
		Assert.Equal(22.0, td[threeHoursLater]);

		float floatInputValues[6] = { 0, 1, 2, 42, 4, 5 };
		auto tf = DataTestHelper<float>::Create(floatInputValues, 6);
		Assert.Equal(42.0f, tf[3]);
		Assert.Equal(42.0f, tf[threeHoursLater]);

	}

	FACT("Time step calculation and number of items in the series are consistent")
	{
		// After refactoring time series/time step; got an issue.
		// https://jira.csiro.au/browse/WIRADA-172
		// This ended up related to the calculation of time steps and elements and inconsistencies of length 
		// btween model runner and time series.
		TimeSeries ts;
		Assert.Equal(TimeStep::GetHourly(), ts.GetTimeStep());
		ptime start = ts.GetStartDate();
		auto newStart = start + hours(1);

		ts.Reset(1, newStart);
		Assert.Equal(TimeStep::GetHourly(), ts.GetTimeStep());
		Assert.Equal(newStart, ts.GetStartDate());
		Assert.Equal(newStart, ts.GetEndDate());

		size_t numSteps = 4;
		ts.Reset(numSteps, newStart);

		Assert.Equal(0, ts.LowerIndexForTime(newStart));
		Assert.Equal(0, ts.IndexForTime(newStart));
		Assert.Equal(0, ts.UpperIndexForTime(newStart));
		Assert.Equal(0, ts.IndexForTime(newStart + seconds(42)));
		Assert.Equal(1, ts.UpperIndexForTime(newStart + seconds(42)));

		Assert.Equal(1, ts.IndexForTime(newStart + hours(1)));
		Assert.Equal(1, ts.UpperIndexForTime(newStart + hours(1)));
		Assert.Equal(1, ts.IndexForTime(newStart + hours(1) + seconds(42)));
		Assert.Equal(2, ts.IndexForTime(newStart + hours(2)));

		Assert.Equal(TimeStep::GetHourly(), ts.GetTimeStep());
		Assert.Equal(newStart, ts.GetStartDate());
		Assert.Equal(newStart + hours(numSteps - 1), ts.GetEndDate());

		ts.SetTimeStep(TimeStep::GetDaily());
		Assert.Equal(TimeStep::GetDaily(), ts.GetTimeStep());
		Assert.Equal(newStart, ts.GetStartDate());
		Assert.Equal(newStart + days(numSteps - 1), ts.GetEndDate());

	}

	// what about the cases where the time series falls between two known instants

	FACT("Multiple realizations")
	{
		// I think I meant by that a time series that is an ensemble of realizations. Really necessary?
	}

	FACT("Different time steps")
	{
		double inputValues[6] = { 0, 1, 2, 22, 4, 5 };
		ptime s(date(2009, 1, 1));

		boost::function<time_duration(int increment)> stepIncrementer;

		boost::function<void(TimeSeries&, boost::function<time_duration(int increment)>)> testFun =
			[&](TimeSeries& td, boost::function<time_duration(int increment)> incrementer)
		{
			Assert.Equal(s, td.GetStartDate());
			Assert.Equal(s + incrementer(6 - 1), td.GetEndDate());
			auto threeStepsLater = s + incrementer(3);
			Assert.Equal(22.0, td[3]);
			Assert.Equal(22.0, td[threeStepsLater]);
		};

		auto td = DataTestHelper<double>::Create(inputValues, 6, s, TimeStep::GetHourly());
		testFun(td, [&](long n){return hours(n); });
		td = DataTestHelper<double>::Create(inputValues, 6, s, TimeStep::GetDaily());
		testFun(td, [&](long n){return hours(24 * n); });
	}

	FACT("Missing values")
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

		Assert.True(added.IsMissingValue(added[naIndex]));
		Assert.True(added.IsMissingValue(added[naIndex2]));

		Assert.Equal(0, added[0]);
		Assert.Equal(2.0, added[1]);
		Assert.Equal(4.0, added[2]);
		Assert.Equal(naCode, added[3]);
		Assert.Equal(8.0, added[4]);
		Assert.Equal(naCode, added[5]);
		Assert.Equal(12.0, added[6]);
		Assert.Equal(20.0, added[10]);

		// This initial time series is not modified (weak check this was not a shallow copy)
		Assert.Equal(10.0, td[10]);

	}

	FACT("Time series subsetting")
	{
		int n = 20;
		auto td = DataTestHelper<double>::Ramp(n);
		auto valArray = td.GetValues(3, 6);
		Assert.Equal(3, valArray[0]);
		Assert.Equal(6, valArray[3]);
		delete[] valArray;

		TimeSeries tssub = td.Subset(3, 6);
		Assert.Equal(4, tssub.GetLength());
		Assert.Equal(3, tssub[0]);
		Assert.Equal(6, tssub[3]);

	}

	FACT("Time series multiplication")
	{
		int n = 20;
		int naIndex = 3;
		double naCode = -9999;
		auto td = TimeSeries(DataTestHelper<double>::Ramp(n));
		//td.SetNA(naCode);
		td[naIndex] = naCode;

		auto result = td * 1.1;
		Assert.Equal(naCode, result[naIndex]);
		Assert.Equal(0, result[0]);
		Assert.Equal(1.1, result[1]);
		Assert.Equal(11.0, result[10]);

		// This initial time series is not modified (weak check this was not a shallow copy)
		Assert.Equal(10.0, td[10]);

	}

	FACT("Multiple variables")
	{
		// TODO
		// There may be advantages in having a class that represent multivariate time series
		// and/or with quality code for time series data. 
		// This is a feature that needs several well defined use cases. 
		// Meanwhile the fact that netCDF data storage is multi-variate need 
		// not be reflected in the high-level concept of time series 
	}

	FACT("Disaggregation of time series to finer time step using semantics")
	{
	}

	FACT("Transparently write to a back end that is not an array")
	{
	}

	FACT("Ensemble forecast over a lead time")
	{
		// That is to say, the collation of hindcast and a forecast ensemble.
	}

	// maybe the netCDF handling should be in a separate test suite.
	FACT("Read operations from a netCDF SWIFT data file")
	{


		/*
		See file
		create_swift_netcdf_ens_data.R
		in the same directory as the present file
		*/

		// 2014-10 You should also find the test netcdf file for reference at 
		// \\clw-02-mel.it.csiro.au\OSM_MEL_CLW_HYDROFORECAST_common\Staff\per202\sample_data\netcdf\testswift.nc
		// SwiftNetCDFTimeSeriesStore<double> store(string("f:\\STSF\\tmp\\testswift.nc"));
		SwiftNetCDFTimeSeriesStore<double> store(TestDataDir + "testswift.nc");

		//stationIds <-c(123, 456)
		int nEns = 3;
		int nLead = 4;
		int nTimeSteps = 48;
		// units: hours since 2010-08-01 14:00:00 +0000"
		auto startDate = ptime(date(2010, 8, 1)) + hours(14);

		Assert.Equal(nEns, store.GetEnsembleSize());
		Assert.Equal(nLead, store.GetLeadTimeCount());
		auto timeDim = store.GetTimeDim();
		Assert.Equal(startDate, store.GetStart());


		// Check single realixzation time series

		auto singleTsStore = store.Get("var2_obs" /*variable name*/, "456"/*catchment*/);
		Assert.Equal(0, singleTsStore->GetEnsembleSize());
		Assert.Equal(0, singleTsStore->GetLeadTimeCount());

		auto singleTs = singleTsStore->GetSeries();

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

		Assert.Equal(1.212, (*singleTs)[0]);
		Assert.Equal(48, singleTs->GetLength());
		Assert.Equal(48.682, (*singleTs)[47]);
		Assert.Equal(startDate, singleTs->GetStartDate());

		delete singleTs;
		delete singleTsStore;


		auto ensTsStore = store.Get("var2_ens" /*variable name*/, "456"/*catchment*/);
		Assert.Equal(nEns, ensTsStore->GetEnsembleSize());
		Assert.Equal(0, ensTsStore->GetLeadTimeCount());

		auto ensTs = ensTsStore->GetEnsembleSeries();

		Assert.Equal(3, ensTs->Size());
		Assert.Equal(startDate, ensTs->GetStartDate());
		singleTs = ensTs->Get(0);

		Assert.Equal(0.55, (*singleTs)[0]);
		Assert.Equal(0.80, (*singleTs)[1]);
		Assert.Equal(48, singleTs->GetLength());
		Assert.Equal(12.3, (*singleTs)[47]);
		Assert.Equal(startDate, singleTs->GetStartDate());

		singleTs = ensTs->Get(1);

		Assert.Equal(0.80, (*singleTs)[0]);
		Assert.Equal(48, singleTs->GetLength());
		Assert.Equal(12.55, (*singleTs)[47]);
		Assert.Equal(startDate, singleTs->GetStartDate());

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
		Assert.Equal(nEns, ensts->GetEnsembleSize());
		Assert.Equal(nLead, ensts->GetLeadTimeCount());

		int offset = 22;

		MultiTimeSeries<TimeSeries> * mts = ensts->GetForecasts(offset);

		Assert.Equal(nEns, mts->Size());

		int replicateNum = 3; // R index
		int replicateIndex = replicateNum - 1; // C index
		int mult = 2; // because var2_fcast_ens
		int offsetR = offset + 1;
		int leadTimeIndex = 1;
		int leadTimeIndexR = leadTimeIndex + 1;
		int stationNum = 2; // "456" is the second station

		auto ts = mts->Get(replicateIndex);
		Assert.Equal(nLead, ts->GetLength());

		ptime start = ts->GetStartDate();
		Assert.Equal(store.GetStart() + hours(offset), start);

		// finally, a check on a value in the time series
		Assert.Equal(mult * (offsetR + 0.1 * replicateNum + leadTimeIndexR*0.01 + stationNum*0.1), ts->GetValue(leadTimeIndex));
		// We do need to reclaim the memory of these time series.
		delete ts;

		delete mts;
		delete ensts;
	}

	FACT("Write and read a single netcdf time series file")
	{
		const string testDirectory = "\\\\clw-02-mel.it.csiro.au\\OSM_MEL_CLW_HYDROFORECAST_common\\Staff\\bri246\\SWIFTv2 Unit Test Data\\";

		const string testFilePath = testDirectory + "single.nc";

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

		Assert.Equal(0, readSeries->GetEnsembleSize());
		Assert.Equal(0, readSeries->GetLeadTimeCount());
		Assert.Equal(numberOfTimeSteps, readSeries->GetTimeLength());

		TTimeSeries<double>* outputTs = readSeries->GetSeries();

		Assert.Equal(numberOfTimeSteps, outputTs->GetLength());
		Assert.Equal(startDate, outputTs->GetStartDate());
		Assert.Equal(timeStep.GetName(), outputTs->GetTimeStep().GetName());

		for (int i = 0; i < numberOfTimeSteps; i++)
			Assert.Equal(i, outputTs->GetValue(i));

		delete outputTs;

		delete readSeries;
		delete readAccess;

		remove(testFilePath.c_str());
	}

	FACT("Write and read an ensemble netcdf file")
	{
		const string testDirectory = "\\\\clw-02-mel.it.csiro.au\\OSM_MEL_CLW_HYDROFORECAST_common\\Staff\\bri246\\SWIFTv2 Unit Test Data\\";

		const string testFilePath = testDirectory + "ensemble.nc";

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

		MultiTimeSeries<TimeSeries> inputMts(inputTimeSeriesVec, numberOfTimeSteps, startDate, timeStep);

		writeSeries->SetEnsemble(&inputMts);

		for (double* data : inputTimeSeriesVec)
			delete[] data;
		inputTimeSeriesVec.clear();

		delete writeSeries;
		delete writeAccess;



		SwiftNetCDFAccess* readAccess = new SwiftNetCDFAccess(testFilePath);

		SwiftNetCDFTimeSeries<double>* readSeries = new SwiftNetCDFTimeSeries<double>(readAccess, varOneName, stationOneId);

		Assert.Equal(numberOfEnsembles, readSeries->GetEnsembleSize());
		Assert.Equal(0, readSeries->GetLeadTimeCount());
		Assert.Equal(numberOfTimeSteps, readSeries->GetTimeLength());

		MultiTimeSeries<TimeSeries>* outputMts = readSeries->GetEnsembleSeries();

		Assert.Equal(numberOfEnsembles, outputMts->Size());

		for (int i = 0; i < numberOfEnsembles; i++)
		{
			TTimeSeries<double>* outputTs = outputMts->Get(i);

			Assert.Equal(numberOfTimeSteps, outputTs->GetLength());
			Assert.Equal(startDate, outputTs->GetStartDate());
			Assert.Equal(timeStep.GetName(), outputTs->GetTimeStep().GetName());

			for (int j = 0; j < numberOfTimeSteps; j++)
				Assert.Equal(numberOfEnsembles * i + j, outputTs->GetValue(j));

			delete outputTs;
		}

		delete outputMts;

		delete readSeries;
		delete readAccess;

		remove(testFilePath.c_str());
	}

	FACT("Write and read a forecast ensemble netcdf time series file")
	{
		const string testDirectory = "\\\\clw-02-mel.it.csiro.au\\OSM_MEL_CLW_HYDROFORECAST_common\\Staff\\bri246\\SWIFTv2 Unit Test Data\\";

		const string testFilePath = testDirectory + "forecast.nc";

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

			MultiTimeSeries<TimeSeries>* inputMts = new MultiTimeSeries<TimeSeries>(inputTimeSeriesDataVec, numberOfTimeSteps, sDate, timeStep);
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

		Assert.Equal(numberOfEnsembles, readSeries->GetEnsembleSize());
		Assert.Equal(leadTime, readSeries->GetLeadTimeCount());
		Assert.Equal(numberOfTimeSteps, readSeries->GetTimeLength());

		ptime sDate = startDate;

		for (int i = 0; i < leadTime; i++)
		{
			MultiTimeSeries<TimeSeries>* outputMts = readSeries->GetForecasts(i);

			Assert.Equal(numberOfEnsembles, outputMts->Size());

			for (int j = 0; j < numberOfEnsembles; j++)
			{
				TTimeSeries<double>* outputTs = outputMts->Get(j);

				Assert.Equal(numberOfTimeSteps, outputTs->GetLength());
				Assert.Equal(sDate, outputTs->GetStartDate());
				Assert.Equal(timeStep.GetName(), outputTs->GetTimeStep().GetName());

				for (int k = 0; k < numberOfTimeSteps; k++)
					Assert.Equal((leadTime * i) + (numberOfEnsembles * j) + k, outputTs->GetValue(k));

				delete outputTs;
			}

			delete outputMts;

			sDate = timeStep.AddSteps(sDate, 1);
		}

		delete readSeries;
		delete readAccess;

		remove(testFilePath.c_str());
	}

	FACT("Time Series written to netCDF")
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

		SwiftNetCDFTimeSeriesStore<double> store(fname, nEns, nLead, timeUnits, timeVar, stationIds, varNames);

		Assert.Equal(nEns, store.GetEnsembleSize());
		Assert.Equal(nLead, store.GetLeadTimeCount());
		Assert.Equal(startDate, store.GetStart());

		auto ensts = store.Get("var2_fcast_ens" /*variable name*/, "456"/*catchment*/);
		//Assert.Equal(nEns, ensts->GetEnsembleSize());
		//Assert.Equal(nLead, ensts->GetLeadTimeCount());

		int offset = 22;

		// The data, if retrieved now, returns everything with default values (FillValue)
		//ensts->GetForecasts

		std::vector<double*>* values = DataTestHelper<double>::Seq(0, 1, nLead, nEns);
		MultiTimeSeries<TimeSeries> mts(*values, nLead, ensts->TimeForIndex(offset), timeStep);
		ensts->SetForecasts(offset, &mts);

		MultiTimeSeries<TimeSeries> * mtsGet = ensts->GetForecasts(offset);


		DataTestHelper<double>::DeleteElements(*values);
		delete values;
		delete mtsGet;
		store.Close();

		FileSystemHelper::Remove(fPath);
	}

	FACT("High level library of time series")
	{

		string ensVarId("var2_fcast_id");
		string ensVarname("var2_fcast_ens");

		//TimeSeriesLibrary<double> tsl;
		//tsl.AddSwiftNetCDFSource(ensVarId, TestDataDir + "testswift.nc", ensVarname, "some_identifier");


		// Get a simplest time series. 
		// Gets a multi time series
		// Gets an ensemble forecast. 
		// Gets multiple series of EnsFcast
	}

}