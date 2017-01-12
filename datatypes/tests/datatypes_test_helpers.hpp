#pragma once

#include <boost/filesystem.hpp>
#include "../common.h"
#include "../time_series.hpp"
#include "../time_series_io.hpp"

#define TEST_START_TIME ptime(date(2010, 8, 1)) + hours(14)

using namespace boost::gregorian;
using namespace datatypes::timeseries;
using namespace std;

namespace datatypes {
	namespace tests {

		template<typename T>
		bool AllEqual(const vector<T>& values, T testValue)
		{
			for (int i = 0; i < values.size(); i++)
				if (values[i] != testValue)
					return false;
			return true;
		}

		template<typename T>
		bool VectorEqual(const vector<T>& a, const vector<T>& b)
		{
			if (a.size() != b.size())
				return false;
			for (int i = 0; i < a.size(); i++)
				if (a[i] != b[i])
					return false;
			return true;
		}

		template <typename T>
		class  DataTestHelper
		{
		public:

			using SeriesType = typename CommonTypes<T>::SeriesType;
			using PtrSeriesType = typename CommonTypes<T>::PtrSeriesType;
			using EnsembleType = typename CommonTypes<T>::EnsembleType;
			using EnsemblePtrType = typename CommonTypes<T>::EnsemblePtrType;
			using PtrEnsemblePtrType = typename CommonTypes<T>::PtrEnsemblePtrType;
			using TSeriesEnsemblePtrType = typename CommonTypes<T>::TSeriesEnsemblePtrType;
			using PtrTSeriesEnsemblePtrType = typename CommonTypes<T>::PtrTSeriesEnsemblePtrType;


			static TTimeSeries<T> Create(T * data, int num, const ptime& start = ptime(date(2000, 1, 1)), const TimeStep& timeStep = TimeStep::GetHourly());
			static TTimeSeries<T> Ramp(int num, const ptime& start = ptime(date(2000, 1, 1)), double from = 0.0, double increment = 1.0);
			static TTimeSeries<T> Pulse(int length, T value = 1, int firstPulse = 0, int period = 2, const ptime& start = ptime(date(2000, 1, 1)), const TimeStep& timeStep = TimeStep::GetHourly());

			const static size_t kTimeSeriesLength = 48;

			static TTimeSeries<T> GetExpectedTestSingleTimeSeries(size_t indexInEnsemble, size_t length = kTimeSeriesLength, double constOffset = 1, const ptime& startDate = TEST_START_TIME, const TimeStep& timeStep = TimeStep::GetHourly());

			static T* Seq(T from, T by, size_t num);
			static vector<T>  SeqVec(T from, T by, size_t num);
			static vector<T> Rep(T value, size_t num);
			static vector<T> Add(const vector<T>& a, const vector<T>& b);
			static vector<T> Add(const vector<T>& a, const T& b);
			static vector<T> Add(const T& a, const vector<T>& b);
			static vector<T> Mult(const vector<T>& a, const vector<T>& b);
			static vector<T> Mult(const vector<T>& a, const T& b);
			static vector<T> Mult(const T& a, const vector<T>& b);
			static vector<T> Neg(const vector<T>& a);
			static vector<T*>* Seq(T from, T by, size_t num, size_t vecSize);

			static void DeleteElements(vector<T*>& vec);

			static bool AreEqual(PtrSeriesType expected, PtrSeriesType actual);
			static bool AreEqual(PtrSeriesType actual, T expected);

			// A function that generates a value in an ensemble fcast time series:  (fcastIndex, ensIndex, seriesIndex) -> T
			typedef std::function<T(size_t /*fcastIndex*/, size_t /*ensIndex*/, size_t /*seriesIndex*/)> FullElementValueFunc;
			// A function that generates an item for a time series:  (seriesIndex) -> T
			typedef std::function<T(size_t /*seriesIndex*/)> ElementValueFunc;
			// A function that generates a time series for an index in an ensemble:  (ensembleIndex) -> TS
			typedef std::function<SeriesType(size_t /*ensembleIndex*/)> EnsembleValueFunc;
			// A function that generates an ensemble given an index in the ens fcst TS:  (index) -> EnsemblePtrType
			typedef std::function<EnsemblePtrType(size_t /*tsEnsTsItemIndex*/)> TsEnsembleValueFunc;

			static T DecimalRamp(size_t fcastIndex, size_t ensIndex, size_t seriesIndex)
			{
				return (T)(fcastIndex + 0.1 * ensIndex + 0.01 * seriesIndex);
			}

			static ElementValueFunc CreateValueGen(size_t fcastIndex, size_t ensIndex, FullElementValueFunc ffun = &DecimalRamp)
			{
				ElementValueFunc valueGen = [=](size_t seriesIndex)
				{
					return ffun(fcastIndex, ensIndex, seriesIndex);
				};
				return valueGen;
			}

			static T Identity(size_t i) { return (T)i; }

			static EnsembleValueFunc CreateTsGen(
				const ptime& start, size_t tsLength,
				size_t fcastIndex,
				const TimeStep& timeStep = TimeStep::GetHourly(),
				FullElementValueFunc ffun = &DecimalRamp
			)
			{
				return [=](size_t ensIndex)
				{
					ElementValueFunc valueGen = CreateValueGen(fcastIndex, ensIndex, ffun);
					return TTimeSeries<T>(valueGen, tsLength, start, timeStep);
				};
			}


			static TsEnsembleValueFunc CreateMtsGen(
				size_t ensSize, size_t tsLength,
				const ptime& start = ptime(date(2000, 1, 1)),
				const TimeStep& timeStep = TimeStep::GetDaily(),
				const TimeStep& timeStepFcasts = TimeStep::GetHourly(),
				FullElementValueFunc ffun = &DecimalRamp,
				int forecastStartOffset = 1
			)
			{
				auto result = [=](size_t fcastIndex)
				{
					ptime fcastIssueDate = timeStep.AddSteps(start, fcastIndex);
					ptime fcastStart = timeStepFcasts.AddSteps(fcastIssueDate, forecastStartOffset);
					auto tsGen = CreateTsGen(fcastStart, tsLength, fcastIndex, timeStepFcasts, ffun);
					EnsemblePtrType mts(tsGen, ensSize, fcastStart, timeStepFcasts);
					return mts;
				};
				return result;
			}

			static EnsembleValueFunc DefaultTsGen()
			{
				return CreateTsGen(ptime(date(2000, 1, 1)), 5 /*tsLength*/, 0 /*fcastIndex*/);
			}

			static EnsemblePtrType CreateEnsembleTs(
				size_t ensSize, size_t length, const ptime& start, const TimeStep& timeStep,
				size_t fcastIndex = 0,
				FullElementValueFunc ffun = &DecimalRamp
			)
			{
				EnsembleValueFunc tsGen = CreateTsGen(start, length /*tsLength*/, fcastIndex /*fcastIndex*/, timeStep, ffun);
				vector<TTimeSeries<T>> values(ensSize);
				for (size_t i = 0; i < ensSize; i++)
				{
					values[i] = tsGen(i);
				}
				return EnsemblePtrType(values, start, timeStep);
			}

			static EnsemblePtrType CreateEnsembleTs(
				size_t ensSize, size_t length, const ptime& start = ptime(date(2000, 1, 1)), const TimeStep& timeStep = TimeStep::GetDaily(),
				EnsembleValueFunc tsGen = DefaultTsGen())
			{
				vector<TTimeSeries<T>> values(ensSize);
				for (size_t i = 0; i < ensSize; i++)
				{
					values[i] = tsGen(i);
				}
				return EnsemblePtrType(values, start, timeStep);
			}

			static TsEnsembleValueFunc DefaultMtsGen()
			{
				return CreateMtsGen(3 /*ensSize*/, 5 /*tsLength*/);
			}

			static TSeriesEnsemblePtrType CreateTsEnsembleTs(
				size_t length,
				const ptime& start = ptime(date(2000, 1, 1)), const TimeStep& timeStep = TimeStep::GetDaily(),
				TsEnsembleValueFunc mtsGen = DefaultMtsGen()
			)
			{
				TSeriesEnsemblePtrType e(length, start, timeStep);
				for (size_t i = 0; i < length; i++)
				{
					EnsemblePtrType v = mtsGen(i);
					e[i] = new EnsemblePtrType(v);
				}
				return e;
			}

			static TSeriesEnsemblePtrType CreateTsEnsembleTs(
				size_t length,
				size_t ensSize,
				size_t tsLength,
				const ptime& start = ptime(date(2000, 1, 1)), const TimeStep& timeStep = TimeStep::GetDaily(),
				const TimeStep& timeStepFcasts = TimeStep::GetHourly(),
				FullElementValueFunc ffun = &DecimalRamp,
				int forecastStartOffset = 1
			)
			{
				TsEnsembleValueFunc mtsGen = CreateMtsGen(ensSize, tsLength, start, timeStep, timeStepFcasts, ffun, forecastStartOffset);
				return CreateTsEnsembleTs(length, start, timeStep, mtsGen);
			}
		};

		template <typename T>
		TTimeSeries<T> DataTestHelper<T>::Pulse(int length, T value, int firstPulse, int period, const ptime& start, const TimeStep& timeStep)
		{
			vector<T> values(length);
			for (size_t i = 0; i < length; i++)
				values[i] = (((i - firstPulse) % period) == 0 ? value : 0);
			return TTimeSeries<T>(values, start, timeStep);
		}

		template <typename T>
		TTimeSeries<T> DataTestHelper<T>::Create(T * data, int num, const ptime& start, const TimeStep& timeStep)
		{
			return TTimeSeries<T>(data, num, start, timeStep);
		}

		template <typename T>
		vector<T> DataTestHelper<T>::Add(const vector<T>& a, const vector<T>& b)
		{
			if (a.size() != b.size())
				ExceptionUtilities::ThrowInvalidOperation("vector addition: vectors must be of the same size");
			auto c = a;
			for (size_t i = 0; i < a.size(); i++)
			{
				c[i] += b[i];
			}
			return c;
		}
		template <typename T>
		vector<T> DataTestHelper<T>::Add(const vector<T>& a, const T& b)
		{
			auto c = a;
			for (size_t i = 0; i < a.size(); i++)
			{
				c[i] += b;
			}
			return c;
		}

		template <typename T>
		vector<T> DataTestHelper<T>::Add(const T& a, const vector<T>& b)
		{
			return Add(b, a);
		}

		template <typename T>
		vector<T> DataTestHelper<T>::Rep(T value, size_t num)
		{
			vector<T> x;
			x.assign(num, value);
			return x;
		}
		template <typename T>
		vector<T> DataTestHelper<T>::Mult(const vector<T>& a, const vector<T>& b)
		{
			if (a.size() != b.size())
				ExceptionUtilities::ThrowInvalidOperation("vector addition: vectors must be of the same size");
			auto c = a;
			for (size_t i = 0; i < a.size(); i++)
			{
				c[i] *= b[i];
			}
			return c;
		}
		template <typename T>
		vector<T> DataTestHelper<T>::Mult(const vector<T>& a, const T& b)
		{
			auto c = a;
			for (size_t i = 0; i < a.size(); i++)
			{
				c[i] *= b;
			}
			return c;
		}
		template <typename T>
		vector<T> DataTestHelper<T>::Mult(const T& a, const vector<T>& b)
		{
			return Mult(b, a);
		}
		template <typename T>
		vector<T> DataTestHelper<T>::Neg(const vector<T>& a)
		{
			auto c = a;
			for (size_t i = 0; i < a.size(); i++)
			{
				c[i] = -c[i];
			}
			return c;
		}


	}
}
using DTH = datatypes::tests::DataTestHelper<double>;

namespace datatypes {
	namespace tests {


		template <typename T>
		TTimeSeries<T> DataTestHelper<T>::GetExpectedTestSingleTimeSeries(size_t indexInEnsemble, size_t length, double constOffset, const ptime& startDate, const TimeStep& timeStep)
		{


	//if(length(v2)>0) {
 //     for (k in 1:length(v2)) {
 //       # dummy <- k %% 2 # KLUDGE just for backward compat reasons - existing unit tests.
 //       for (j in 1:length(stationIds)) {
 //         varValues <- k + 0.1*j + 0.01 * timeSteps
 //         station <- stationIds[j]
 //         snc$putSingleSeries(varValues, varName = v2[k], identifier = station)
 //       }
 //     }
 //   }
			T k = constOffset;
			int j = indexInEnsemble + 1;
			auto varValues = DTH::Rep(k, length);
			varValues = DTH::Add(varValues, DTH::Rep(j * 0.1, length));
			auto timeSteps = DTH::SeqVec(1.0, 1.0, length);
			varValues = DTH::Add(varValues, DTH::Mult(timeSteps, 0.01));
			return TTimeSeries<T>(varValues, startDate, timeStep);
		}

		template <typename T>
		void DataTestHelper<T>::DeleteElements(vector<T*>& vec)
		{
			for (int i = 0; i < vec.size(); i++)
				delete[] vec[i];
		}

		template <typename T>
		T*  DataTestHelper<T>::Seq(T from, T by, size_t num)
		{
			T * data = new T[num];
			for (size_t i = 0; i < num; i++)
				data[i] = from + i*by;
			return data;
		}

		template <typename T>
		vector<T>  DataTestHelper<T>::SeqVec(T from, T by, size_t num)
		{
			return datatypes::utils::SeqVec<T>(from, by, num);
		}

		template <typename T>
		vector<T*> * DataTestHelper<T>::Seq(T from, T by, size_t num, size_t vecSize)
		{
			vector<T*> * result = new vector<T*>();
			for (size_t i = 0; i < vecSize; i++)
				result->push_back(Seq(from + i*(by*num), by, num));
			return result;
		}


		template <typename T>
		TTimeSeries<T> DataTestHelper<T>::Ramp(int num, const ptime& start, double from, double increment)
		{
			T * data = Seq(from, increment, num);
			auto ts = Create(data, num, start);
			delete data;
			return ts;
		}

		template <typename T>
		bool DataTestHelper<T>::AreEqual(PtrSeriesType expected, PtrSeriesType actual)
		{
			if (actual == nullptr) return false;
			if (expected == nullptr) return false;
			return datatypes::timeseries::TimeSeriesOperations<TTimeSeries<T>>::AreTimeSeriesEqual(*expected, *actual);
		}

		template <typename T>
		bool DataTestHelper<T>::AreEqual(PtrSeriesType actual, T expected)
		{
			if (actual == nullptr) return false;
			return datatypes::timeseries::TimeSeriesOperations<TTimeSeries<T>>::template AreEqual<T>(*actual, expected);
		}
	}
}
