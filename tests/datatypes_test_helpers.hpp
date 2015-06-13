#pragma once

#include "../datatypes/time_series.h"
#include <boost/filesystem.hpp>

using namespace boost::gregorian;
using namespace datatypes::timeseries;
using namespace std;

namespace datatypes {
	namespace tests {

		class FileSystemHelper{
		public:
			static boost::filesystem::path GetTempFile()
			{
				boost::filesystem::path tempFname = boost::filesystem::unique_path();
				boost::filesystem::path tempDir = boost::filesystem::temp_directory_path();
				boost::filesystem::path fPath = tempDir / tempFname; // operator/ is overloaded
				return fPath;
			}

			static void Remove(const boost::filesystem::path& p)
			{
				if (boost::filesystem::exists(p))
					boost::filesystem::remove(p);
			}

		};

		template <typename T>
		class  DataTestHelper
		{
		public:
			static TTimeSeries<T> Create(T * data, int num, const ptime& start = ptime(date(2000, 1, 1)), const TimeStep& timeStep = TimeStep::GetHourly());
			static TTimeSeries<T> Ramp(int num, const ptime& start = ptime(date(2000, 1, 1)), double from = 0.0, double increment = 1.0);
			static T* Seq(T from, T to, T by);
			static T* Seq(T from, T by, int num);
			static std::vector<T>  SeqVec(T from, T by, int num);
			static std::vector<T*>* Seq(T from, T by, int num, int vecSize);
			static void DeleteElements(std::vector<T*>& vec);
		};

		template <typename T>
		TTimeSeries<T> DataTestHelper<T>::Create(T * data, int num, const ptime& start, const TimeStep& timeStep)
		{
			return TTimeSeries<T>(data, num, start, timeStep);
		}

		template <typename T>
		void DataTestHelper<T>::DeleteElements(std::vector<T*>& vec)
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
		std::vector<T>  DataTestHelper<T>::SeqVec(T from, T by, int num)
		{
			std::vector<T> data;
			data.resize(num);
			for (size_t i = 0; i < num; i++)
				data[i] = from + i*by;
			return data;
		}

		template <typename T>
		std::vector<T*> * DataTestHelper<T>::Seq(T from, T by, int num, int vecSize)
		{
			std::vector<T*> * result = new std::vector<T*>();
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