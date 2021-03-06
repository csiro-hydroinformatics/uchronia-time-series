#pragma once

// Must define the switch so that datatypes constructs are declared as IMPORT definitions
#define USING_DATATYPES

#pragma warning (disable : 4251)

#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <queue>
#include <boost/filesystem.hpp>
#include "datatypes/datatypes_test_helpers.hpp"
#include "datatypes/datatypes_test_helpers.h"
#include "datatypes/time_series.hpp"
#include "datatypes/time_series_store.hpp"
#include "datatypes/time_series_io.hpp"
#include "catch/catch_macros.hpp"

using namespace datatypes::tests;
//using DTH = datatypes::tests::DataTestHelper<double>;

using namespace boost::gregorian;
using namespace datatypes::tests;
using namespace datatypes::timeseries;
using namespace std;

std::string DatatypesTestPath(const string& relativePath);

EnsembleForecastTimeSeries<> CreateTestTsEnsTs();

template <typename T = double>
class TestWindowedStorage
	: public MemoryCachingStorageWriter<T>
{
private:
	TestWindowedStorage(const TestWindowedStorage& src) :
		MemoryCachingStorageWriter<T>(src)
	{
	}
public:
	TestWindowedStorage(size_t bufferSize) :
		MemoryCachingStorageWriter<T>(bufferSize, new StlVectorStorage<T>())
	{
	}
	StoragePolicy<T>* Clone() const { return new TestWindowedStorage<T>(*this); }
};

class NcFileDef
{
public:
	DimensionsDefinitions DimDefs;
	map<string, VariableDefinition> VarDefs;
	datatypes::timeseries::GlobalAttributes GlobAtts;
};

template<typename TStore>
NcFileDef CreateNcIo(string& timeUnits, const vector<string>& stationIds,
	const size_t tsLength, const size_t ensembleSize, const size_t leadTimeSize, const TimeStep& timeStepLead,
	const ptime& startDate, const TimeStep& timeStep, const vector<string>& ncVarnames,
	const string& longName = "test data",
	const string& units = "mm",
	double fillValue = DEFAULT_MISSING_DATA_VALUE,
	int type = 9,
	const string& typeDescription = "Aggregation type description",
	const string& datType = "der",
	const string& datDescription = "Data type description")
{
	NcFileDef result;
	timeUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(startDate, timeStep);
	std::pair<vector<double>, vector<double>> timeVars = SwiftNetCDFAccess::CreateTimeVectors(startDate, timeStep, tsLength, timeStepLead, leadTimeSize);

	vector<double> timeVar = timeVars.first;
	vector<double> leadTimeVar = timeVars.second;

	result.DimDefs = DimensionsDefinitions(ensembleSize, leadTimeVar, timeUnits, timeVar, stationIds);
	map<string, VariableDefinition> v;
	for (auto& ncVarname : ncVarnames)
		v[ncVarname] = VariableDefinition(ncVarname, DATATYPES_DOUBLE_PRECISION_ID, TStore::Dimensions(), longName, units, fillValue, type, typeDescription, datType, datDescription, "Point");

	result.VarDefs = v;
	result.GlobAtts = GlobalAttributes::CreateDefault();
	return result;
}

template<typename TStore>
NcFileDef CreateNcIo(string& timeUnits, const vector<string>& stationIds,
	const size_t tsLength, const size_t ensembleSize, const size_t leadTimeSize, const TimeStep& timeStepLead,
	const ptime& startDate, const TimeStep& timeStep, string& ncVarname,
	const string& longName = "test data",
	const string& units = "mm",
	double fillValue = DEFAULT_MISSING_DATA_VALUE,
	int type = 9,
	const string& typeDescription = "Aggregation type description",
	const string& datType = "der",
	const string& datDescription = "Data type description")
{
	return CreateNcIo<TStore>(timeUnits, stationIds,
		tsLength, ensembleSize, leadTimeSize, timeStepLead, startDate, timeStep, vector<string> {ncVarname},
		longName, units, fillValue, type, typeDescription, datType, datDescription);
}

#define TEST_FILL_VALUE -9999.9

extern string var1_fcast_ens;
extern string var2_fcast_ens;
extern string       var1_obs;
extern string       var2_obs;
extern string       var1_ens;
extern string       var2_ens;
extern vector<string> stationIds;
extern string tDesc;
extern string dType;
extern string dDesc;
extern string pt;

SwiftNetCDFAccess* CreateTestSwiftFile(string testFilePath,
	size_t nEns = 3,
	size_t leadTimeLength = 4,
	size_t mainTimeLength = 5,
	ptime startDate = ptime(date(2010, 8, 1)) + hours(14),
	TimeStep timeStep = TimeStep::GetHourly());


