
#include "common_datatypes.h"

using namespace datatypes::tests;
using namespace datatypes::timeseries;
using namespace boost::posix_time;

TEST_CASE("Date time operations")
{
	REQUIRE_THROWS(TimeStepImplementation::CheckIsDateTime(not_a_date_time));
}

TEST_CASE("Time step handling")
{
	auto parse = [](const string& s) {return TimeStep::Parse(s);};
	auto tot_secs = [](const string& s) {return TimeStep::Parse(s).GetRegularStepDuration().total_seconds();};
	REQUIRE_EQUAL((3600 * 24), tot_secs("daily"));
	REQUIRE_EQUAL((3600 * 24), tot_secs("24:00:00"));
	REQUIRE_EQUAL((3600), tot_secs("hourly"));
	REQUIRE_EQUAL((3600 * 3), tot_secs("03:00:00"));
	REQUIRE_EQUAL((3600 * 3 + 45), tot_secs("03:00:45"));
	REQUIRE_EQUAL((43200), tot_secs("43200"));

	auto getName = [](const string& s) {return TimeStep::Parse(s).GetName();};
	REQUIRE_EQUAL("daily", getName("daily"));
	REQUIRE_EQUAL("daily", getName("24:00:00"));
	REQUIRE_EQUAL("hourly", getName("hourly"));
	REQUIRE_EQUAL("030000", getName("03:00:00"));
	REQUIRE_EQUAL("030045", getName("03:00:45"));
	REQUIRE_EQUAL("120000", getName("43200"));

	auto hourly = TimeStep::GetHourly();
	ptime s = from_iso_string("20000124T010203");
	REQUIRE_EQUAL(3600, hourly.GetTimeStepDuration(s).total_seconds());
	hourly.Increment(&s);
	REQUIRE_EQUAL(from_iso_string("20000124T020203"), s);

	// arithmetic
	auto threeHourly = hourly * 3;
	REQUIRE_EQUAL((3600 * 3), threeHourly.GetRegularStepDuration().total_seconds());
	auto sixMinutes = hourly / 10;
	REQUIRE_EQUAL((360), sixMinutes.GetRegularStepDuration().total_seconds());

	// test assignments operators
	auto a = TimeStep::GetDaily();
	auto b = TimeStep::GetHourly();
	a = b;
	REQUIRE_EQUAL("hourly", a.GetName());
	a = time_duration(3, 30, 29);
	REQUIRE_EQUAL("033029", a.GetName());

	auto parse_2 = [](const string& s) {TimeStep ts = TimeStep::GetDaily() ; ts = s ; return ts.GetName();};
	REQUIRE_EQUAL("daily", parse_2("daily"));
	REQUIRE_EQUAL("daily", parse_2("24:00:00"));
	REQUIRE_EQUAL("hourly", parse_2("hourly"));
	REQUIRE_EQUAL("030000", parse_2("03:00:00"));
	REQUIRE_EQUAL("030045", parse_2("03:00:45"));
	REQUIRE_EQUAL("120000", parse_2("43200"));

	vector<ptime> v = {from_iso_string("20001104T010203"), from_iso_string("20110228T233000")};
	auto w = hourly.AddSteps(v, 2.0);
	REQUIRE_EQUAL(from_iso_string("20001104T030203"), w[0]);
	REQUIRE_EQUAL(from_iso_string("20110301T013000"), w[1]);

	auto d = from_iso_string("19871104T010203");
	REQUIRE_EQUAL("1987-11-04 01:02:03", TimeStep::ToString(d, "YYYY-MM-DD hh:mm:ss"));
	REQUIRE_EQUAL("04/11/1987 01:02:03", TimeStep::ToString(d, "DD/MM/YYYY hh:mm:ss"));

	// Modulo operations
	auto hl = TimeStep::GetHourly();
	auto dl = TimeStep::GetDaily();

	// Equivalent of the integer division (TODO: was that a wise choice??? would reconsidering break something?)
	REQUIRE_EQUAL( 24, dl / hl);
	REQUIRE_EQUAL( 8, dl / TimeStep::Parse("03:00:00"));

	REQUIRE_EQUAL( 1, dl / dl);
	REQUIRE_EQUAL( 1, (dl*1.01) / dl);
	REQUIRE_EQUAL( 1, (dl*1.49) / dl);
	REQUIRE_EQUAL( 1, (dl*1.51) / dl);
	REQUIRE_EQUAL( 1, (dl*1.99) / dl);
	REQUIRE_EQUAL( 2, (dl*2) / dl);


	auto doMod = [parse](const string& a, const string& b) {time_duration td = (parse(a) % parse(b)); return td;};
	REQUIRE_EQUAL( seconds(0), doMod("10", "10"));
	REQUIRE_EQUAL( seconds(0), doMod("10", "5"));
	REQUIRE_EQUAL( seconds(1), doMod("10", "9"));
	REQUIRE_EQUAL( seconds(2), doMod("10", "8"));
	REQUIRE_EQUAL( seconds(28), doMod("00:10:00", "00:09:32"));

}

TEST_CASE("Invalid time step Operations")
{
	TimeStep ts = TimeStep::GetDaily();

	ptime s = from_iso_string("20000124T000000");
	ptime e = from_iso_string("20000120T000000"); // end before start
	REQUIRE_THROWS(ts.GetUpperNumSteps(s, e));
	REQUIRE_THROWS(ts.GetNumSteps(s, e));
	REQUIRE_THROWS(ts / 0);
}

TEST_CASE("Basic access - single and double precision")
{
	double inputValues[6] = { 0, 1, 2, 22, 4, 5 };
	auto td = DTH::Create(inputValues, 6);
	ptime start = td.GetStartDate();
	auto threeHoursLater = start + hours(3);
	REQUIRE_EQUAL(22.0, td[3]);
	REQUIRE_EQUAL(22.0, td[threeHoursLater]);

	float floatInputValues[6] = { 0, 1, 2, 42, 4, 5 };
	auto tf = DataTestHelper<float>::Create(floatInputValues, 6);
	REQUIRE_EQUAL(42.0f, tf[3]);
	REQUIRE_EQUAL(42.0f, tf[threeHoursLater]);

}

template<typename E>
void CheckSeriesPtrData()
{
	// https://jira.csiro.au/browse/WIRADA-420
	using T = TTimeSeries<E>;
	E inputValues[6] = { 0, 1, 2, 22, 4, 5 };
	size_t tslen = 4;
	ptime s(date(2014, 8, 1));
	auto tstep = TimeStep::GetDaily();
	E* d = inputValues;
	T ts(d, tslen, s, tstep);
	for (size_t i = 0; i < tslen; i++)
		REQUIRE(ts[i] == inputValues[i]);

	d = &(inputValues[1]);
	ts = T(d, tslen, s, tstep);
	for (size_t i = 0; i < tslen; i++)
		REQUIRE(ts[i] == inputValues[i + 1]);

}

TEST_CASE("Time series construction from pointers to data values")
{
	CheckSeriesPtrData<double>();
	CheckSeriesPtrData<float>();
	CheckSeriesPtrData<int>();
}

TEST_CASE("Time step calculation and number of items in the series are consistent")
{
	// After refactoring time series/time step; got an issue.
	// https://jira.csiro.au/browse/WIRADA-172
	// This ended up related to the calculation of time steps and elements and inconsistencies of length 
	// btween model runner and time series.
	ptime start = TimeStep::CreatePtime(2016, 7, 14);
	TimeSeries ts(1, start);
	REQUIRE_EQUAL(TimeStep::GetHourly(), ts.GetTimeStep());
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
	REQUIRE_EQUAL((newStart + hours(numSteps - 1)), ts.GetEndDate());

	ts.SetTimeStep(TimeStep::GetDaily());
	REQUIRE_EQUAL(TimeStep::GetDaily(), ts.GetTimeStep());
	REQUIRE_EQUAL(newStart, ts.GetStartDate());
	REQUIRE_EQUAL((newStart + days(numSteps - 1)), ts.GetEndDate());

}

// what about the cases where the time series falls between two known instants

TEST_CASE("Time series of forecasts")
{
	ForecastTimeSeries<TimeSeries> forecast;
	ptime s(date(2009, 1, 1));
	TimeSeries* value = nullptr;
	forecast.Reset(3, s, value);
	REQUIRE_NULL(forecast[0]);
	REQUIRE(forecast.IsMissingValue(forecast[0]));
	REQUIRE_NULL(forecast[2]);
	REQUIRE(forecast.IsMissingValue(forecast[2]));
}

TEST_CASE("Default creation of a time series of ensemble forecasts")
{
	EnsembleForecastTimeSeries<> forecasts;
	ptime s(date(2009, 1, 1));
	MultiTimeSeriesPtr<>* value = nullptr;
	forecasts.Reset(3, s, value);
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
	REQUIRE_EQUAL((mypi * 2), (*pts)[1]);

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

class SpatialGrid
{
private:
	std::vector<double> data;
public:
	bool isMissing;
	SpatialGrid(bool isMissing = false) { this->isMissing = isMissing; }
	SpatialGrid& operator=(SpatialGrid&& src) {
		if (&src == this) {
			return *this;
		}
		std::swap(this->data, src.data);
		this->isMissing = src.isMissing;
		this->easting = src.easting;
		this->northing = src.northing;
		return *this;
	}
	SpatialGrid& operator=(const SpatialGrid& src) {
		if (&src == this) {
			return *this;
		}
		this->data = src.data;
		this->easting = src.easting;
		this->northing = src.northing;
		return *this;
	}
	SpatialGrid(SpatialGrid&& src) { *this = src; }
	SpatialGrid(const SpatialGrid& src) { *this = src; }
	double easting, northing;
	SpatialGrid operator+(const SpatialGrid& rhs) const {/*TODO*/ return SpatialGrid(); }
	SpatialGrid operator+(double rhs) const {/*TODO*/ return SpatialGrid(); }
};

class SpatialGridMissingValuePolicy
	: public MissingValuePolicy<SpatialGrid>
{
private:
	//const T missingValue = (T)(DEFAULT_MISSING_DATA_VALUE);
public:
	inline bool IsMissingValue(const SpatialGrid& a) const { return (a.isMissing); };
	inline SpatialGrid GetMissingValue() const { return SpatialGrid(true); };
	MissingValuePolicy<SpatialGrid>* Clone() const { return new SpatialGridMissingValuePolicy(); };
};



TEST_CASE("Proof of concept extensibility of templated time series to spatial data")
{
	// This is a placeholder for 
	// https://jira.csiro.au/browse/WIRADA-478
	//using SpatialTs = TTimeSeries<SpatialGrid>;
	//auto start = ptime(date(2017, 1, 1));
	//SpatialTs oneWeek(7, start, TimeStep::GetDaily(), nullptr, new SpatialGridMissingValuePolicy());
	//SpatialGrid val = oneWeek[start + days(2)];
	//SpatialTs other = oneWeek + val;

}

TEST_CASE("Check bounds when retrieving items in ensemble forecast time series")
{
	EnsembleForecastTimeSeries<> forecasts;
	ptime s(date(2009, 1, 1));
	MultiTimeSeriesPtr<>* value = nullptr;
	MultiTimeSeriesPtr<>* valueRead = nullptr;
	size_t n = 3;

	// Check that https ://jira.csiro.au/browse/WIRADA-348 is fixed
	REQUIRE_THROWS_EXCEPTION_TYPE(std::out_of_range, (valueRead = forecasts[0]));

	// then check that issue https://jira.csiro.au/browse/WIRADA-347 is fixed
	forecasts.Reset(n, s, value);
	for (size_t i = 0; i < n; i++)
	{
		REQUIRE_NOTHROW(valueRead = forecasts[i]);
	}
	REQUIRE_THROWS_EXCEPTION_TYPE(std::out_of_range, (valueRead = forecasts[-1]));
	REQUIRE_THROWS_EXCEPTION_TYPE(std::out_of_range, (valueRead = forecasts[n]));
	REQUIRE_THROWS_EXCEPTION_TYPE(std::out_of_range, (valueRead = forecasts[n+1]));

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
		REQUIRE_EQUAL((s + incrementer(6 - 1)), td.GetEndDate());
		auto threeStepsLater = s + incrementer(3);
		REQUIRE_EQUAL(22.0, td[3]);
		REQUIRE_EQUAL(22.0, td[threeStepsLater]);
	};

	auto td = DTH::Create(inputValues, 6, s, TimeStep::GetHourly());
	testFun(td, [&](long n) {return hours(n); });
	td = DTH::Create(inputValues, 6, s, TimeStep::GetDaily());
	testFun(td, [&](long n) {return hours(24 * n); });
}

TEST_CASE("Missing values handling in addition of time series")
{
	int n = 20;
	int naIndex = 3;
	int naIndex2 = 5;
	double naCode = DEFAULT_MISSING_DATA_VALUE;
	auto td = DTH::Ramp(n);
	auto td2 = DTH::Ramp(n);

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
	auto td = DTH::Ramp(n);
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
	double naCode = DEFAULT_MISSING_DATA_VALUE;
	auto td = TimeSeries(DTH::Ramp(n));
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

TEST_CASE("Design test helper methods for time series of ensemble time series") {

	using DTH = datatypes::tests::DataTestHelper<double>;
	using FullElementValueFunc = DTH::FullElementValueFunc;
	using ElementValueFunc = DTH::ElementValueFunc;
	using EnsembleValueFunc = DTH::EnsembleValueFunc;
	using TsEnsembleValueFunc = DTH::TsEnsembleValueFunc;

	FullElementValueFunc f(&DTH::DecimalRamp);
	const double tol = 1e-12;
	REQUIRE_WITHIN_ABSOLUTE_TOLERANCE(0.0, f(0, 0, 0), tol);
	REQUIRE_WITHIN_ABSOLUTE_TOLERANCE(1.23, f(1, 2, 3), tol);
	REQUIRE_WITHIN_ABSOLUTE_TOLERANCE(10.23, f(10, 2, 3), tol);

	size_t fcastIndex = 2, ensIndex = 3;
	ElementValueFunc evf = DTH::CreateValueGen(fcastIndex, ensIndex, f);
	REQUIRE_WITHIN_ABSOLUTE_TOLERANCE(2.34, evf(4), tol);
	fcastIndex = 0; ensIndex = 0;
	// Make sure the evf is self contained (has captured all its functional arguments)
	REQUIRE_WITHIN_ABSOLUTE_TOLERANCE(2.34, evf(4), tol); // and not 0.04

	size_t tsEtsLength = 31 * 6;
	size_t ensSize = 5;
	size_t tsLength = 24;
	ptime start(date(2008, 3, 4));
	auto hourly = TimeStep::GetHourly();
	TimeStep daily = TimeStep::GetDaily();
	TimeStep etsTstep = hourly * 6;
	TimeStep fcastTstep = hourly / 4;

	TsEnsembleValueFunc ensGen = DTH::CreateMtsGen(
		ensSize,
		tsLength,
		start,
		etsTstep,
		fcastTstep,
		f);

	DTH::EnsemblePtrType ensemble = ensGen(0);

	REQUIRE(ensemble.GetStartDate() == fcastTstep.AddSteps(start, 1));
	REQUIRE(ensemble.Size() == ensSize);
	REQUIRE(ensemble.GetLength(0) == tsLength);
	REQUIRE(ensemble.Get(0)->GetStartDate() == fcastTstep.AddSteps(start, 1));
	REQUIRE(ensemble.Get(0)->TimeForIndex(1) == fcastTstep.AddSteps(start, 2));

	EnsembleForecastTimeSeries<> ensTs = DTH::CreateTsEnsembleTs(
		tsEtsLength,
		ensSize,
		tsLength,
		start,
		etsTstep,
		fcastTstep,
		f);

	REQUIRE(ensTs.GetStartDate() == start);
	REQUIRE(ensTs.GetLength() == tsEtsLength);
	REQUIRE(ensTs[0]->Size() == ensSize);
	REQUIRE(ensTs[0]->GetLength(0) == tsLength);
	REQUIRE(ensTs[0]->Get(0)->GetStartDate() == fcastTstep.AddSteps(start, 1));
	REQUIRE(ensTs[0]->Get(0)->TimeForIndex(1) == fcastTstep.AddSteps(start, 2));
	auto s = etsTstep.AddSteps(start, 10);
	REQUIRE(ensTs[10]->Get(0)->TimeForIndex(1) == fcastTstep.AddSteps(s, 2));

	REQUIRE_WITHIN_ABSOLUTE_TOLERANCE(f(0, 0, 0), ensTs.GetValue(0)->Get(0)->GetValue(0), tol);
	REQUIRE_WITHIN_ABSOLUTE_TOLERANCE(f(1, 0, 0), ensTs.GetValue(1)->Get(0)->GetValue(0), tol);
	REQUIRE_WITHIN_ABSOLUTE_TOLERANCE(f(1, 2, 0), ensTs.GetValue(1)->Get(2)->GetValue(0), tol);
	REQUIRE_WITHIN_ABSOLUTE_TOLERANCE(f(1, 2, 3), ensTs.GetValue(1)->Get(2)->GetValue(3), tol);
	REQUIRE_WITHIN_ABSOLUTE_TOLERANCE(f(10, 2, 3), ensTs.GetValue(10)->Get(2)->GetValue(3), tol);

}

TEST_CASE("Basic operations on time series")
{
	using TsOps = TimeSeriesOperations<TTimeSeries<double>>;
	ptime s(date(2016, 10, 01));
	auto h = TimeStep::GetHourly();
	TimeSeries a(DTH::SeqVec(0, 1, 20), s, h);
	TimeSeries x = a << a;
	REQUIRE(TsOps::AreTimeSeriesEqual(x, a));
	TimeSeries b(DTH::SeqVec(0, 0.1, 20), h.AddSteps(s, 10), h);
	x = a << b;
	REQUIRE(TsOps::AreValueEqual(x, DTH::SeqVec(0, 1, 10), 0, 9));
	REQUIRE(TsOps::AreValueEqual(x, DTH::SeqVec(0, 0.1, 10), 10, 19));
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

	double maskingValue = DEFAULT_MISSING_DATA_VALUE;

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

