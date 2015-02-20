#pragma once


#include "boost/date_time/posix_time/posix_time.hpp"
#include "common.h"
#include "time_step.h"
#include "exception_utilities.h"

#include <iterator>

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
			TTimeSeries(T default_value, size_t length, const ptime& startDate, const TimeStep& timeStep = TimeStep::GetHourly());
			TTimeSeries(T * values, size_t length, const ptime& startDate, const TimeStep& timeStep = TimeStep::GetHourly());
			TTimeSeries(const TTimeSeries<T>& src); // Copy constructor

			TTimeSeries<T>& operator=(const TTimeSeries<T>& rhs);
			TTimeSeries<T>& operator=(TTimeSeries<T>&& src);
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
			T GetValue(size_t index);
			void CopyTo(T * dest, size_t from = 0, size_t to = -1) const;
			T * GetValues(size_t from = 0, size_t to = -1) const;
			void SetValue(size_t index, T value);
			void Reset(size_t length, const ptime& startDate, T * value = nullptr);
			size_t GetLength() const;
			ptime GetStartDate() const;
			ptime GetEndDate() const;
			TimeStep GetTimeStep() const;
			ptime TimeForIndex(size_t timeIndex) const;
			size_t IndexForTime(const ptime& instant) const;
			size_t UpperIndexForTime(const ptime& instant) const;
			size_t LowerIndexForTime(const ptime& instant) const;

			T& operator[](const ptime& instant);
			T& operator[](const size_t i);
			const T& operator[](const ptime& instant) const;
			const T& operator[](const size_t i) const;
			TTimeSeries<T> operator+(const TTimeSeries<T>& rhs) const;
			template<typename M>
			TTimeSeries<T> operator*(M multiplicator) const;

			void SetNA(T& naCode);
			T GetNA() const;

			string Tag;

		protected:
			//size_t length;
			std::vector<T> data;
			//T * data = nullptr;
			T naCode;
		private:
			ptime AddTimeStep(const ptime& start, size_t numSteps);
			void DeepCopyFrom(const TTimeSeries<T>& src);
			void CopyNonData(const TTimeSeries<T>& src);
			void SetDefaults();
			ptime startDate;
			ptime endDate;
			TimeStep timeStep;
			//void DeleteInnerData()
			//{
			//	// do nothing, if data is a vector<T>
			//	//if (data != nullptr)
			//	//{
			//	//	delete[] data;
			//	//	data = nullptr;
			//	//}
			//}
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
		//	TimeSeries(double default_value, size_t length, const ptime& startDate) : TTimeSeries(default_value, length, startDate) {}
		//	TimeSeries(double * values, size_t length, const ptime& startDate) : TTimeSeries(values, length, startDate) {}

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
			MultiTimeSeries(const std::vector<T*>& values, size_t length, const ptime& startDate);
			MultiTimeSeries(const MultiTimeSeries<T>& src);
			~MultiTimeSeries();
			TTimeSeries<T> * Get(size_t i);
			void Set(size_t i, size_t tsIndex, T val);
			std::vector<T*>* GetValues();
			size_t Size();
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
			static TTimeSeries<T>* TrimTimeSeries(const TTimeSeries<T>& timeSeries, const ptime& startDate, const ptime& endDate);
			static TTimeSeries<T>* DailyToHourly(const TTimeSeries<T>& dailyTimeSeries);
			static TTimeSeries<T>* JoinTimeSeries(const TTimeSeries<T>& head, const TTimeSeries<T>& tail);
			static bool AreTimeSeriesEqual(const TTimeSeries<T>& a, const TTimeSeries<T>& b);
		};

		template <typename T>
		class DATATYPES_DLL_LIB TimeWindow
		{
		public:
			TimeWindow(const ptime& startDate, const ptime& endDate);
			TTimeSeries<T>* Trim(const TTimeSeries<T>& timeSeries) const;
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
		void TTimeSeries<T>::Reset(size_t length, const ptime& startDate, T * value)
		{
			if (value == nullptr)
				data = vector<T>(length, naCode);
			else
			{
				data.clear();
				data.reserve(length);
				data.assign(value, value + length);
			}
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
		TTimeSeries<T>::TTimeSeries(T default_value, size_t length, const ptime& startDate, const TimeStep& timeStep)
		{
			SetDefaults();
			this->timeStep = timeStep;
			Reset(length, startDate);
			data = vector<T>(length, default_value);
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(T * values, size_t length, const ptime& startDate, const TimeStep& timeStep)
		{
			SetDefaults();
			this->timeStep = timeStep;
			Reset(length, startDate, values);
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(TTimeSeries<T>&& src) {   // Move constructor.
			*this = std::move(src);
		}

		template <class T>
		TTimeSeries<T>& TTimeSeries<T>::operator=(TTimeSeries<T>&& src){
			// Avoid self assignment
			if (&src == this){
				return *this;
			}
			std::swap(naCode, src.naCode);
			std::swap(timeStep, src.timeStep);
			std::swap(startDate, src.startDate);
			std::swap(endDate, src.endDate);
			std::swap(Tag, src.Tag);
			std::swap(data, src.data);
			return *this;
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(const TTimeSeries<T>& src) {   // (Deep) Copy constructor.
			*this = src;
		}

		template <class T>
		TTimeSeries<T>& TTimeSeries<T>::operator=(const TTimeSeries<T>& src)
		{
			if (&src == this){
				return *this;
			}
			DeepCopyFrom(src);
			return *this;
		}

		template <class T>
		void TTimeSeries<T>::DeepCopyFrom(const TTimeSeries<T>& src) {
			naCode = src.naCode;
			timeStep = src.timeStep;
			startDate = src.startDate;
			endDate = src.endDate;
			Tag = src.Tag;
			data = src.data;
		}

		template <class T>
		TTimeSeries<T>::~TTimeSeries()
		{
			// nothing to do here after using std::vector for data.
		}

		template <class T>
		T TTimeSeries<T>::GetValue(size_t index)
		{
			if (index < 0 || index >= GetLength())
				datatypes::exceptions::ExceptionUtilities::ThrowOutOfRange("Trying to access a time series value with an index outside of its bounds");
			return data[index];
		}
		template <class T>
		size_t TTimeSeries<T>::IndexForTime(const ptime& instant) const {
			return LowerIndexForTime(instant);
		}

		template <class T>
		size_t TTimeSeries<T>::UpperIndexForTime(const ptime& instant) const
		{
			return timeStep.GetUpperNumSteps(startDate, instant);
		}

		template <class T>
		size_t TTimeSeries<T>::LowerIndexForTime(const ptime& instant) const
		{
			return timeStep.GetNumSteps(startDate, instant);
		}

		template <class T>
		T& TTimeSeries<T>::operator[](const ptime& instant) {
			return data[IndexForTime(instant)];
		}

		template <class T>
		T& TTimeSeries<T>::operator[](const size_t i) {
			T& vRef = data[i];
			return vRef;
		}

		template <class T>
		const T& TTimeSeries<T>::operator[](const ptime& instant) const {
			return this->operator[](IndexForTime(instant));
		}

		template <class T>
		const T& TTimeSeries<T>::operator[](const size_t i) const {
			return data[i];
		}

		template <class T>
		TTimeSeries<T> TTimeSeries<T>::operator+(const TTimeSeries<T>& rhs) const
		{
			// TODO check geometry compatibility
			T a, b;
			size_t length = rhs.GetLength();
			TTimeSeries<T> ts(naCode, length, startDate, timeStep);
			ts.naCode = this->naCode;
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
			auto length = GetLength();
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
		T * TTimeSeries<T>::GetValues(size_t from, size_t to) const
		{
			size_t tsLen = this->GetLength();
			if (to < 0) to = (tsLen - 1);
			datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(from, 0, tsLen - 1, "from");
			datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(to, -1, tsLen - 1, "to");

			size_t len = (to - from) + 1;
			T * values = new T[len];
			this->CopyTo(values, from, to);
			return values;
		}

		template <class T>
		void TTimeSeries<T>::CopyTo(T * dest, size_t from, size_t to) const
		{
			size_t tsLen = this->GetLength();
			if (to < 0) to = (tsLen - 1);
			datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(from, 0, tsLen - 1, "from");
			datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(to, -1, tsLen - 1, "to");
			std::copy(data.begin()+from, data.begin()+to, stdext::checked_array_iterator<T*>(dest, to - from + 1));
		}

		template <class T>
		void TTimeSeries<T>::SetValue(size_t index, T value)
		{
			data[index] = value;
		}

		template <class T>
		void TTimeSeries<T>::SetNA(T& naCode)
		{
			this->naCode = naCode;
		}

		template <class T>
		T TTimeSeries<T>::GetNA() const
		{
			return this->naCode;
		}

		template <class T>
		ptime TTimeSeries<T>::AddTimeStep(const ptime& start, size_t numSteps)
		{
			return timeStep.AddSteps(start, numSteps);
		}

		template <class T>
		ptime TTimeSeries<T>::GetStartDate() const
		{
			return ptime(startDate);
		}

		template <class T>
		ptime TTimeSeries<T>::GetEndDate() const
		{
			return ptime(endDate);
		}

		template <class T>
		ptime TTimeSeries<T>::TimeForIndex(size_t timeIndex) const
		{
			return timeStep.AddSteps(this->startDate, timeIndex);
		}

		template <class T>
		size_t TTimeSeries<T>::GetLength() const
		{
			return data.size();
		}

		template <class T>
		TimeStep TTimeSeries<T>::GetTimeStep() const
		{
			return this->timeStep;
		}

		typedef TTimeSeries < double > TimeSeries;		

	}
}
