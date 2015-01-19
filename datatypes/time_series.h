#pragma once

#include "common.h"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace datatypes::sys;
using namespace boost::posix_time;

namespace datatypes
{
	namespace timeseries
	{
		template <typename T>
		class DLL_LIB TTimeSeries
		{
		public:
			TTimeSeries();
			TTimeSeries(T default_value, int length, const ptime& startDate);
			TTimeSeries(T * values, int length, const ptime& startDate);
			TTimeSeries(const TTimeSeries<T>& src); // Copy constructor

			TTimeSeries(TTimeSeries<T>&& src); // Move constructor
			// C++ for the Impatient
			// Appendix A. A Painless Introduction to Rvalue References (C++11)
			// See also http://stackoverflow.com/a/3109981/2752565 for information on move semantic

			virtual ~TTimeSeries();
			T GetValue(int index);
			void CopyTo(T * dest, int from = 0, int to = -1);
			T * GetValues(int from = 0, int to = -1);
			void SetValue(int index, T value);
			void Reset(int length, const ptime& startDate, T * value = nullptr);
			int GetLength();
			ptime GetStartDate();
			ptime GetEndDate();
			ptime TimeForIndex(int timeIndex);
			int GetTimeStepSeconds();

			T& operator[](const ptime& instant);
			T& operator[](const int i);
			TTimeSeries<T> operator+(const TTimeSeries<T>& rhs) const;

			void SetNA(T& naCode);
			T GetNA();

		protected:
			int length;
			T * data = nullptr;
			T naCode;
		private:
			int indexForTime(const ptime& instant);
			ptime AddTimeStep(ptime& start, int numSteps);
			void CopyNonData(const TTimeSeries<T>& src, bool deep);
			void SetDefaults();
			ptime * startDate;
			ptime * endDate;
			void DeleteInnerData()
			{
				if (data != nullptr)
				{
					delete[] data;
					data = nullptr;
				}
			}
			void DeleteInnerTimeSpan()
			{
				if (this->startDate == nullptr) delete (this->startDate);
				if (this->endDate == nullptr) delete (this->endDate);
			}
		};


		class DLL_LIB TimeSeries
			: public TTimeSeries < double >
		{
		public:
			TimeSeries() : TTimeSeries() {}
			TimeSeries(double default_value, int length, const ptime& startDate) : TTimeSeries(default_value, length, startDate) {}
			TimeSeries(double * values, int length, const ptime& startDate) : TTimeSeries(values, length, startDate) {}

			// TODO: generalize. Expediency, fbut a refactor to generic types is cascading to a lot more. Basically, no TimeSeries anymore. 
			TimeSeries(const TTimeSeries<double>& src) : TTimeSeries(src) {}
			TimeSeries operator*(double multiplicator) const;

		};


		template <typename T>
		class DLL_LIB MultiTimeSeries // This may become an abstract class with specializations for lazy loading time series from the data store.
		{
		public:
			MultiTimeSeries(const std::vector<T*>& values, int length, const ptime& startDate);
			MultiTimeSeries(const MultiTimeSeries<T>& src);
			~MultiTimeSeries();
			TTimeSeries<T> * Get(int i);
			void Set(int i, int tsIndex, T val);
			std::vector<T*>* GetValues();
			int Size();
			ptime GetStart();

		private:
			std::vector<TTimeSeries<T>*> * series;
			ptime startDate;
		};
	}
}
