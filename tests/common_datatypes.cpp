#include "common_datatypes.h"

using namespace datatypes::tests;

std::string DatatypesTestPath(const string& relativePath) {
	return TestDataLocationHelper::BuildPath({ TestDataLocationHelper::ReadEnvironmentVariable("SWIFT_TEST_DIR"), relativePath });
}

EnsembleForecastTimeSeries<> CreateTestTsEnsTs()
{
	size_t tsEtsLength = 31 * 6;
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
	return ensTs;
}

using namespace boost::gregorian;
using namespace datatypes::tests;
using namespace datatypes::timeseries;
using namespace std;

string var1_fcast_ens = ("var1_fcast_ens");
string var2_fcast_ens = ("var2_fcast_ens");
string       var1_obs = ("var1_obs");
string       var2_obs = ("var2_obs");
string       var1_ens = ("var1_ens");
string       var2_ens = ("var2_ens");
vector<string> stationIds({ "123", "456" });
string tDesc("accumulated over the preceding interval");
string dType("der");
string dDesc("AWAP data interpolated from observations");
string pt("Point");

SwiftNetCDFAccess* CreateTestSwiftFile(string testFilePath,
	size_t nEns,
	size_t leadTimeLength,
	size_t mainTimeLength,
	ptime startDate,
	TimeStep timeStep)
{
	// units: hours since 2010-08-01 14:00:00 +0000"

	string timeUnits = SwiftNetCDFAccess::CreateTimeUnitsAttribute(startDate, timeStep);
	vector<double> timeVar = DTH::SeqVec(0.0, 1.0, mainTimeLength);
	vector<double> leadTimeVar = DTH::SeqVec(1.0, 1.0, leadTimeLength);

	map<string, VariableDefinition> varDefs;

	double fillVal = TEST_FILL_VALUE;
	int tsType = 2;
	varDefs[var1_fcast_ens] = VariableDefinition(var1_fcast_ens, string(DATATYPES_DOUBLE_PRECISION_ID), string(DATATYPES_FOUR_DIMENSIONS_DATA), var1_fcast_ens, string("mm"), fillVal, tsType, tDesc, dType, dDesc, pt);
	varDefs[var2_fcast_ens] = VariableDefinition(var2_fcast_ens, string(DATATYPES_DOUBLE_PRECISION_ID), string(DATATYPES_FOUR_DIMENSIONS_DATA), var2_fcast_ens, string("mm"), fillVal, tsType, tDesc, dType, dDesc, pt);
	varDefs[var1_obs] = VariableDefinition(var1_obs, string(DATATYPES_DOUBLE_PRECISION_ID), DATATYPES_TWO_DIMENSIONS_DATA, var1_obs, string("mm"), fillVal, tsType, tDesc, dType, dDesc, pt);
	varDefs[var2_obs] = VariableDefinition(var2_obs, string(DATATYPES_DOUBLE_PRECISION_ID), DATATYPES_TWO_DIMENSIONS_DATA, var2_obs, string("mm"), fillVal, tsType, tDesc, dType, dDesc, pt);
	varDefs[var1_ens] = VariableDefinition(var1_ens, string(DATATYPES_DOUBLE_PRECISION_ID), string(DATATYPES_THREE_DIMENSIONS_DATA), var1_ens, string("mm"), fillVal, tsType, tDesc, dType, dDesc, pt);
	varDefs[var2_ens] = VariableDefinition(var2_ens, string(DATATYPES_DOUBLE_PRECISION_ID), string(DATATYPES_THREE_DIMENSIONS_DATA), var2_ens, string("mm"), fillVal, tsType, tDesc, dType, dDesc, pt);
	GlobalAttributes globAtts = GlobalAttributes::CreateDefault();
	SwiftNetCDFAccess* access = new SwiftNetCDFAccess(testFilePath, nEns, leadTimeVar, timeUnits, timeVar, stationIds, varDefs, globAtts);
	return access;
}


