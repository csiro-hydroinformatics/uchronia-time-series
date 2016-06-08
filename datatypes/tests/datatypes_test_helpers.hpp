#pragma once

#include <boost/filesystem.hpp>
#include "../common.h"
#include "../time_series.hpp"
#include "../time_series_io.hpp"

using namespace boost::gregorian;
using namespace datatypes::timeseries;
using namespace std;

namespace datatypes {
	namespace tests {

		template <typename T>
		class  DataTestHelper
		{
		public:
			static TTimeSeries<T> Create(T * data, int num, const ptime& start = ptime(date(2000, 1, 1)), const TimeStep& timeStep = TimeStep::GetHourly());
			static TTimeSeries<T> Ramp(int num, const ptime& start = ptime(date(2000, 1, 1)), double from = 0.0, double increment = 1.0);
			static T* Seq(T from, T to, T by);
			static T* Seq(T from, T by, int num);
			static vector<T>  SeqVec(T from, T by, int num);
			static vector<T*>* Seq(T from, T by, int num, int vecSize);
			static void DeleteElements(vector<T*>& vec);
		};

		template <typename T>
		TTimeSeries<T> DataTestHelper<T>::Create(T * data, int num, const ptime& start, const TimeStep& timeStep)
		{
			return TTimeSeries<T>(data, num, start, timeStep);
		}

		template <typename T>
		void DataTestHelper<T>::DeleteElements(vector<T*>& vec)
		{
			for (int i = 0; i < vec.size(); i++)
				delete[] vec[i];
		}

		template <typename T>
		T*  DataTestHelper<T>::Seq(T from, T by, int num)
		{
			T * data = new T[num];
			for (size_t i = 0; i < num; i++)
				data[i] = from + i*by;
			return data;
		}

		template <typename T>
		vector<T>  DataTestHelper<T>::SeqVec(T from, T by, int num)
		{
			vector<T> data;
			data.resize(num);
			for (size_t i = 0; i < num; i++)
				data[i] = from + i*by;
			return data;
		}

		template <typename T>
		vector<T*> * DataTestHelper<T>::Seq(T from, T by, int num, int vecSize)
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

	}
}