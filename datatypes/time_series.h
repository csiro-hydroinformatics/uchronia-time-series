#pragma once

#include "boost/date_time/posix_time/posix_time.hpp"
#include "common.h"
#include "time_step.h"
#include "exception_utilities.h"

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
		 * \tparam	T	Element type, typically double or float, but possibly a more complicated type such as another TTimeSeries.
		 */
		template <typename T>
		class TTimeSeries
		{
		public:
			TTimeSeries();

			/**
			 * \brief	Time Series Constructor.
			 *
			 * \param	default_value	The default value.
			 * \param	length		 	The length.
			 * \param	startDate	 	The start date.
			 * \param	timeStep	 	The time step of the time series.
			 */
			TTimeSeries(T default_value, int length, const ptime& startDate, const TimeStep& timeStep = TimeStep::GetHourly());
			TTimeSeries(T * values, int length, const ptime& startDate, const TimeStep& timeStep = TimeStep::GetHourly());
			TTimeSeries(const TTimeSeries<T>& src); // Copy constructor

			TTimeSeries<T>& operator=(const TTimeSeries<T>& rhs);

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
			TimeStep GetTimeStep();
			ptime TimeForIndex(int timeIndex);
			int IndexForTime(const ptime& instant);
			int UpperIndexForTime(const ptime& instant);
			int LowerIndexForTime(const ptime& instant);

			T& operator[](const ptime& instant);
			T& operator[](const int i);
			TTimeSeries<T> operator+(const TTimeSeries<T>& rhs) const;
			template<typename M>
			TTimeSeries<T> operator*(M multiplicator) const;

			void SetNA(T& naCode);
			T GetNA();

			string Tag;

		protected:
			int length;
			T * data = nullptr;
			T naCode;
		private:
			ptime AddTimeStep(const ptime& start, int numSteps);
			void DeepCopyFrom(const TTimeSeries<T>& src);
			void CopyNonData(const TTimeSeries<T>& src);
			void SetDefaults();
			ptime startDate;
			ptime endDate;
			TimeStep timeStep;
			void DeleteInnerData()
			{
				if (data != nullptr)
				{
					delete[] data;
					data = nullptr;
				}
			}
			//void DeleteInnerTimeSpan()
			//{
			//	if (this->startDate == nullptr) delete (this->startDate);
			//	if (this->endDate == nullptr) delete (this->endDate);
			//}
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
			timeStep = TimeStep::GetHourly();
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

			this->startDate = startDate;
			this->endDate = AddTimeStep((this->startDate), length);
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries()
		{
			SetDefaults();
			Reset(1, ptime(date(2000, 1, 1)));
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(T default_value, int length, const ptime& startDate, const TimeStep& timeStep)
		{
			SetDefaults();
			this->timeStep = timeStep;
			Reset(length, startDate);
			for (int i = 0; i < length; i++)
			{
				data[i] = default_value;
			}
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(T * values, int length, const ptime& startDate, const TimeStep& timeStep)
		{
			SetDefaults();
			this->timeStep = timeStep;
			Reset(length, startDate);
			for (int i = 0; i < length; i++)
			{
				data[i] = values[i];
			}
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(TTimeSeries<T>&& src) {   // Move constructor.
			CopyNonData(src);
			data = src.data;
			src.data = nullptr;
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(const TTimeSeries<T>& src) {   // (Deep) Copy constructor.
			DeepCopyFrom(src);
		}

		template <class T>
		void TTimeSeries<T>::DeepCopyFrom(const TTimeSeries<T>& src) { 
			CopyNonData(src);
			data = new T[length];
			if (length > 0)
				memcpy(data, src.data, length * sizeof(T));
		}

		template <class T>
		TTimeSeries<T>& TTimeSeries<T>::operator=(const TTimeSeries<T>& rhs)
		{
			DeepCopyFrom(rhs);
			return *this;
		}

		template <class T>
		void TTimeSeries<T>::CopyNonData(const TTimeSeries<T>& src) {
			naCode = src.naCode;
			length = src.length;
			timeStep = src.timeStep;
			startDate = src.startDate;
			endDate = src.endDate;
			Tag = src.Tag;
		}

		template <class T>
		TTimeSeries<T>::~TTimeSeries()
		{
			DeleteInnerData();
		}

		template <class T>
		T TTimeSeries<T>::GetValue(int index)
		{
			if (index < 0 || index >= length)
				datatypes::exceptions::ExceptionUtilities::ThrowOutOfRange("Trying to access a time series value with an index outside of its bounds");
			return data[index];
		}

		template <class T>
		int TTimeSeries<T>::IndexForTime(const ptime& instant) {
			return LowerIndexForTime(instant);
		}

		template <class T>
		int TTimeSeries<T>::UpperIndexForTime(const ptime& instant)
		{
			return timeStep.GetUpperNumSteps(startDate, instant);
		}

		template <class T>
		int TTimeSeries<T>::LowerIndexForTime(const ptime& instant)
		{
			return timeStep.GetNumSteps(startDate, instant);
		}

		template <class T>
		T& TTimeSeries<T>::operator[](const ptime& instant) {
			return data[IndexForTime(instant)];
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
		ptime TTimeSeries<T>::AddTimeStep(const ptime& start, int numSteps)
		{
			return timeStep.AddSteps(start, numSteps);
		}

		template <class T>
		ptime TTimeSeries<T>::GetStartDate()
		{
			return ptime(startDate);
		}

		template <class T>
		ptime TTimeSeries<T>::GetEndDate()
		{
			return ptime(endDate);
		}

		template <class T>
		ptime TTimeSeries<T>::TimeForIndex(int timeIndex)
		{
			return timeStep.AddSteps(this->startDate, timeIndex);
		}

		template <class T>
		int TTimeSeries<T>::GetLength()
		{
			return this->length;
		}

		template <class T>
		TimeStep TTimeSeries<T>::GetTimeStep()
		{
			return this->timeStep;
		}

		typedef TTimeSeries < double > TimeSeries;		

	}
}
