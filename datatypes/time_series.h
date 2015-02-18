#pragma once

#include "common.h"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace datatypes::sys;
using namespace boost::posix_time;
using namespace boost::gregorian;

namespace datatypes
{
	namespace timeseries
	{
		/**
		 * \brief	A template for univariate, single realisasion time series
		 *
		 * \tparam	T	Elemental type, typically double or float.
		 */
		template <typename T>
		class /*DATATYPES_DLL_LIB*/ TTimeSeries
		{
		public:
			TTimeSeries();
			TTimeSeries(T default_value, int length, const ptime& startDate);
			TTimeSeries(T * values, int length, const ptime& startDate);
			TTimeSeries(const TTimeSeries<T>& src); // Copy constructor

			/**
			 * \brief	Constructor using the move semantics
			 *
			 * \param [in,out]	src	time series from which to move data.
			 * \remarks 		C++ for the Impatient
			 * 					Appendix A. A Painless Introduction to Rvalue References (C++11)
			 * 					See also http://stackoverflow.com/a/3109981/2752565 for information on move semantic
			 */
			TTimeSeries(TTimeSeries<T>&& src); // Move constructor

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
			template<typename M>
			TTimeSeries<T> operator*(M multiplicator) const;

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


		//class DATATYPES_DLL_LIB TimeSeries
		//	: public TTimeSeries < double >
		//{
		//public:
		//	TimeSeries() : TTimeSeries() {}
		//	TimeSeries(double default_value, int length, const ptime& startDate) : TTimeSeries(default_value, length, startDate) {}
		//	TimeSeries(double * values, int length, const ptime& startDate) : TTimeSeries(values, length, startDate) {}

		//	// TODO: generalize. Expediency, fbut a refactor to generic types is cascading to a lot more. Basically, no TimeSeries anymore. 
		//	TimeSeries(const TTimeSeries<double>& src) : TTimeSeries(src) {}
		//	TimeSeries operator*(double multiplicator) const;

		//};

		/**
		 * \brief	Template for time series with multiple values at time point; e.g. to hold multiple realizations of time series in an ensemble
		 *
		 * \tparam	T	Elemental type, typically double or float.
		 */
		template <typename T>
		class DATATYPES_DLL_LIB MultiTimeSeries // This may become an abstract class with specializations for lazy loading time series from the data store.
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


		typedef TTimeSeries<MultiTimeSeries<double>> EnsembleForecastTimeSeries;

		template <typename T>
		class DATATYPES_DLL_LIB TimeSeriesOperations
		{
		public:
			static TTimeSeries<T>* TrimTimeSeries(TTimeSeries<T>* timeSeries, const ptime& startDate, const ptime& endDate);
			static TTimeSeries<T>* DailyToHourly(TTimeSeries<T>* dailyTimeSeries);
			static TTimeSeries<T>* JoinTimeSeries(TTimeSeries<T>* head, TTimeSeries<T>* tail);
			static bool AreTimeSeriesEqual(TTimeSeries<T>* a, TTimeSeries<T>* b);
		};

		template <typename T>
		class DATATYPES_DLL_LIB TimeWindow
		{
		public:
			TimeWindow(const ptime& startDate, const ptime& endDate);
			TTimeSeries<T>* Trim(TTimeSeries<T>* timeSeries) const;
			TTimeSeries<T>* ReadDailyToHourly(const std::string& netCdfFilePath, const std::string& varName, const std::string& identifier) const;

		private:
			ptime startDate;
			ptime endDate;

		};


		/*******************
		Below are implementations of the template code; they would normally be found in a .cpp file, but as 
		templates putting them here makes it more reusable from other programs.
		/*******************/


		template <class T>
		void TTimeSeries<T>::SetDefaults()
		{
			naCode = std::numeric_limits<T>::min();
		}

		template <class T>
		void TTimeSeries<T>::Reset(int length, const ptime& startDate, T * value = nullptr)
		{
			DeleteInnerData();
			T val = value == nullptr ? naCode : *value;
			data = new T[length];
			this->length = length;
			for (int i = 0; i < length; i++)
				data[i] = val;

			DeleteInnerTimeSpan();
			this->startDate = new ptime(startDate);
			this->endDate = new ptime(AddTimeStep(*(this->startDate), length));
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries()
		{
			SetDefaults();
			Reset(1, ptime(date(2000, 1, 1)));
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(T default_value, int length, const ptime& startDate)
		{
			SetDefaults();
			Reset(length, startDate);
			for (int i = 0; i < length; i++)
			{
				data[i] = default_value;
			}
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(T * values, int length, const ptime& startDate)
		{
			SetDefaults();
			Reset(length, startDate);
			for (int i = 0; i < length; i++)
			{
				data[i] = values[i];
			}
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(TTimeSeries<T>&& src) {   // Move constructor.
			CopyNonData(src, false);
			data = src.data;
			src.data = nullptr;
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(const TTimeSeries<T>& src) {   // (Deep) Copy constructor.
			CopyNonData(src, true);
			data = new T[length];
			memcpy(data, src.data, length * sizeof(T));
		}

		template <class T>
		void TTimeSeries<T>::CopyNonData(const TTimeSeries<T>& src, bool deep) {
			naCode = src.naCode;
			length = src.length;
			startDate = (deep) ? new ptime(*src.startDate) : src.startDate;
			endDate = (deep) ? new ptime(*src.endDate) : src.endDate;
		}

		template <class T>
		TTimeSeries<T>::~TTimeSeries()
		{
			DeleteInnerData();
			DeleteInnerTimeSpan();
		}

		template <class T>
		T TTimeSeries<T>::GetValue(int index)
		{
			if (index < 0 || index >= length)
				datatypes::exceptions::ExceptionUtilities::ThrowOutOfRange("Trying to access a time series value with an index outside of its bounds");
			return data[index];
		}

		template <class T>
		int TTimeSeries<T>::indexForTime(const ptime& instant) {
			// TODO: make this generic WRT time step
			return (instant - (*startDate)).hours();
		}


		template <class T>
		T& TTimeSeries<T>::operator[](const ptime& instant) {
			return data[indexForTime(instant)];
		}

		template <class T>
		T& TTimeSeries<T>::operator[](const int i) {
			return data[i];
		}

		template <class T>
		TTimeSeries<T> TTimeSeries<T>::operator+(const TTimeSeries<T>& rhs) const
		{
			// TODO check geometry compatibility
			T a, b;
			TTimeSeries<T> ts(*this);
			for (size_t i = 0; i < length; i++)
			{
				a = this->data[i];
				b = rhs.data[i];
				if (a == this->naCode || b == rhs.naCode)
					ts.data[i] = ts.naCode;
				else
					ts.data[i] = a + b;
			}
			return ts;
		}


		template <class T>
		template<typename M>
		TTimeSeries<T> TTimeSeries<T>::operator*(M multiplicator) const
		{
			double a;
			TTimeSeries<T> ts(*this);
			for (size_t i = 0; i < length; i++)
			{
				a = this->data[i];
				if (a == this->naCode)
					ts.data[i] = ts.naCode;
				else
					ts.data[i] = a * multiplicator;
			}
			return ts;
		}

		template <class T>
		T * TTimeSeries<T>::GetValues(int from, int to)
		{
			int tsLen = this->GetLength();
			datatypes::exceptions::ExceptionUtilities::CheckInRange(from, 0, tsLen - 1, "from");
			datatypes::exceptions::ExceptionUtilities::CheckInRange(to, -1, tsLen - 1, "to");
			if (to < 0) to = (tsLen - 1);

			int len = (to - from) + 1;
			T * values = new T[len];
			this->CopyTo(values, from, to);
			return values;
		}

		template <class T>
		void TTimeSeries<T>::CopyTo(T * dest, int from, int to)
		{
			int tsLen = this->GetLength();
			datatypes::exceptions::ExceptionUtilities::CheckInRange(from, 0, tsLen - 1, "from");
			datatypes::exceptions::ExceptionUtilities::CheckInRange(to, -1, tsLen - 1, "to");
			if (to < 0) to = (tsLen - 1);

			for (int i = from; i <= to; i++)
				dest[i - from] = data[i]; // TODO: faster options? memcopy?
		}

		template <class T>
		void TTimeSeries<T>::SetValue(int index, T value)
		{
			data[index] = value;
		}

		template <class T>
		void TTimeSeries<T>::SetNA(T& naCode)
		{
			this->naCode = naCode;
		}

		template <class T>
		T TTimeSeries<T>::GetNA()
		{
			return this->naCode;
		}

		template <class T>
		ptime TTimeSeries<T>::AddTimeStep(ptime& start, int numSteps)
		{
			// KLUDGE : this is an obviously temporary shortcut (TM)
			return start + hours(numSteps - 1);
		}

		template <class T>
		ptime TTimeSeries<T>::GetStartDate()
		{
			return ptime(*(this->startDate));
		}

		template <class T>
		ptime TTimeSeries<T>::GetEndDate()
		{
			return ptime(*(this->endDate));
		}

		template <class T>
		ptime TTimeSeries<T>::TimeForIndex(int timeIndex)
		{
			return AddTimeStep(*this->startDate, timeIndex);
		}

		template <class T>
		int TTimeSeries<T>::GetLength()
		{
			return this->length;
		}

		template <class T>
		int TTimeSeries<T>::GetTimeStepSeconds()
		{
			int n = GetLength();
			// KLUDGE - TODO what to do with degenerate cases.
			if (n <= 1) return 3600;
			return (*endDate - *startDate).total_seconds() / (n - 1);
		}


		typedef TTimeSeries < double > TimeSeries;
		

	}
}
