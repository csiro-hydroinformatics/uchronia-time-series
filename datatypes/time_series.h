#pragma once


#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/function.hpp>
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


		template<typename T>
		class MissingValueCheck {
		private:

			// Could not find an easy if_then_else in the STL or Boost. IfThenElse will probably be replaced.
			// primary template: yield second or third argument depending on first argument 
			template<bool C, typename Ta, typename Tb>
			class IfThenElse;
			// Credits:
			// C++ templates : the complete guide / David Vandevoorde, Nicolai M. Josuttis.
			//				ISBN 0 - 201 - 73484 - 2 (alk.paper)

			// partial specialization: true yields second argument 
			template<typename Ta, typename Tb>
			class IfThenElse<true, Ta, Tb> {
			public:
				typedef Ta ResultT;
			};

			// partial specialization: false yields third argument 
			template<typename Ta, typename Tb>
			class IfThenElse<false, Ta, Tb> {
			public:
				typedef Tb ResultT;
			};

		public:
			typedef typename IfThenElse<std::is_arithmetic<T>::value,
				T,
				T*>::ResultT Type;
		};

		/**
		 * \brief	A template for univariate, single realisasion time series
		 *
		 * \tparam	T	Element type, typically double or float, but possibly a more complicated type such as another TTimeSeries.
		 */
		template <typename T = double>
		//template <typename T = double, typename MissingValuesHandler<T>>
		//template <typename T = double, bool isSimple = std::is_fundamental<T>::value>
		class TTimeSeries
		{
		public:
			typedef MissingValueCheck<T>::Type U;
			typedef boost::function<bool(U)> Predicate;

		protected:

			std::vector<T> data;

			Predicate missingValueTest;
			bool IsMissingValue(U value)
			{
				return false;
			}
			//std::enable_if_t<std::is_arithmetic<T>::value, T> naCode;
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
			void CopyTo(std::vector<T>& dest, size_t from = 0, size_t to = -1) const;
			T * GetValues(size_t from = 0, size_t to = std::numeric_limits<size_t>::max()) const;
			std::vector<T> GetValuesVector(size_t from = 0, size_t to = std::numeric_limits<size_t>::max()) const;
			TTimeSeries<T> Subset(size_t from = 0, size_t to = std::numeric_limits<size_t>::max()) const;
			void SetValue(size_t index, T value);

			//std::enable_if_t<std::is_arithmetic<T>::value, void>
			//	Reset(size_t length, const ptime& startDate, T * value = nullptr)
			void Reset(size_t length, const ptime& startDate, T * value = nullptr)
			{
				if (value == nullptr)
					data = vector<T>(length, 0);
				else
				{
					data.clear();
					data.reserve(length);
					data.assign(value, value + length);
				}
				this->startDate = startDate;
				UpdateEndDate();
			}

			size_t GetLength() const;
			ptime GetStartDate() const;
			ptime GetEndDate() const;
			TimeStep GetTimeStep() const;
			void SetTimeStep(const TimeStep& timeStep);
			void UpdateEndDate();
			ptime TimeForIndex(size_t timeIndex) const;
			size_t IndexForTime(const ptime& instant) const;
			size_t UpperIndexForTime(const ptime& instant) const;
			size_t LowerIndexForTime(const ptime& instant) const;
			string GetSummary() const;

			T& operator[](const ptime& instant);
			T& operator[](const size_t i);
			const T& operator[](const ptime& instant) const;
			const T& operator[](const size_t i) const;
			TTimeSeries<T> operator+(const TTimeSeries<T>& rhs) const;
			template<typename M>
			TTimeSeries<T> operator*(M multiplicator) const;

			//std::enable_if_t<std::is_arithmetic<T>::value, void>
			//	SetNA(T& naCode)
			//{
			//	this->naCode = naCode;
			//}
			//std::enable_if_t<std::is_arithmetic<T>::value, T>
			//	GetNA() const { return naCode; }

			string Tag;

		private:
			ptime EndForStart(const ptime& start, const size_t& numSteps) const;
			void CheckIntervalBounds(const size_t& from, size_t& to) const;
			void UncheckedCopyTo(T * dest, const size_t& from, const size_t& to) const;
			void DeepCopyFrom(const TTimeSeries<T>& src);
			void CopyNonData(const TTimeSeries<T>& src);
			
			ptime startDate;
			ptime endDate;
			TimeStep timeStep;
			static std::atomic<int> instances;

			//std::enable_if_t<std::is_arithmetic<T>::value, void>
			//	SetDefaults()
				void SetDefaults()
			{
				//naCode = std::numeric_limits<T>::min();
				timeStep = TimeStep::GetHourly();
			}

		public:
			static int NumInstances() { return TTimeSeries<T>::instances; };
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
			MultiTimeSeries(const std::vector<T*>& values, size_t length, const ptime& startDate, const TimeStep& timeStep);
			MultiTimeSeries(const MultiTimeSeries<T>& src);
			MultiTimeSeries();
			~MultiTimeSeries();
			TTimeSeries<T> * Get(size_t i);
			void Set(size_t i, size_t tsIndex, T val);
			std::vector<T*>* GetValues();
			size_t Size();
			ptime GetStartDate();
			void ResetSeries(const size_t& numSeries, const size_t& length, const ptime& startDate, const TimeStep& timeStep);
			void Clear();

		private:
			std::vector<TTimeSeries<T>*> * series;
			ptime startDate;
			TimeStep timeStep;
		};


		template<class T>
		using EnsembleForecastTimeSeries = TTimeSeries<MultiTimeSeries<T>>;

		template<class T>
		using ForecastTimeSeries = TTimeSeries<TTimeSeries<T>>;

		template <typename T>
		class DATATYPES_DLL_LIB TimeSeriesOperations
		{
		public:
			static TTimeSeries<T>* TrimTimeSeries(const TTimeSeries<T>& timeSeries, const ptime& startDate, const ptime& endDate);
			static TTimeSeries<T>* DailyToHourly(const TTimeSeries<T>& dailyTimeSeries);
			static TTimeSeries<T>* JoinTimeSeries(const TTimeSeries<T>& head, const TTimeSeries<T>& tail);
			static TTimeSeries<T>* Resample(const TTimeSeries<T>& timeSeries, const string& method);
			static bool AreTimeSeriesEqual(const TTimeSeries<T>& a, const TTimeSeries<T>& b);
		};

		/**
		 * \brief	An object that represents a time window, defining subset/trim operations on time series
		 *
		 * \tparam	T	Generic type parameter, element type of the time series
		 */
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
		******************/


		template <class T>
		void TTimeSeries<T>::UpdateEndDate()
		{
			this->endDate = EndForStart((this->startDate), GetLength());
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries()
		{
			SetDefaults();
			Reset(1, ptime(date(2000, 1, 1)));
			instances++;
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(T default_value, size_t length, const ptime& startDate, const TimeStep& timeStep)
		{
			SetDefaults();
			this->timeStep = timeStep;
			Reset(length, startDate);
			data = vector<T>(length, default_value);
			instances++;
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(T * values, size_t length, const ptime& startDate, const TimeStep& timeStep)
		{
			SetDefaults();
			this->timeStep = timeStep;
			Reset(length, startDate, values);
			instances++;
		}

		template <class T>
		TTimeSeries<T>::TTimeSeries(TTimeSeries<T>&& src) {   // Move constructor.
			*this = std::move(src);
			instances++;
		}

		template <class T>
		TTimeSeries<T>& TTimeSeries<T>::operator=(TTimeSeries<T>&& src){
			// Avoid self assignment
			if (&src == this){
				return *this;
			}
			//std::swap(naCode, src.naCode);
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
			instances++;
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
			//naCode = src.naCode;
			timeStep = src.timeStep;
			startDate = src.startDate;
			endDate = src.endDate;
			Tag = src.Tag;
			data = src.data;
		}

		template <class T>
		TTimeSeries<T>::~TTimeSeries()
		{
			instances--;
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
			auto uIndex = timeStep.GetUpperNumSteps(startDate, instant) - 1;
			datatypes::exceptions::ExceptionUtilities::CheckInRange<ptrdiff_t>(uIndex, 0, GetLength() - 1, "UpperIndexForTime");
			return (size_t)uIndex;
		}

		template <class T>
		size_t TTimeSeries<T>::LowerIndexForTime(const ptime& instant) const
		{
			auto uIndex = timeStep.GetNumSteps(startDate, instant) - 1;
			datatypes::exceptions::ExceptionUtilities::CheckInRange<ptrdiff_t>(uIndex, 0, GetLength() - 1, "LowerIndexForTime");
			return (size_t)uIndex;
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
			T a;
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
		void TTimeSeries<T>::CheckIntervalBounds(const size_t& from, size_t& to) const
		{
			size_t tsLen = this->GetLength();
			if (to == std::numeric_limits<size_t>::max())
				to = min(to, (tsLen - 1));
			datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(from, 0, tsLen - 1, "from");
			datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(to, 0, tsLen - 1, "to");
		}

		template <class T>
		T * TTimeSeries<T>::GetValues(size_t from, size_t to) const
		{
			CheckIntervalBounds(from, to);
			size_t len = (to - from) + 1;
			T * values = new T[len];
			this->UncheckedCopyTo(values, from, to);
			return values;
		}

		template <class T>
		std::vector<T> TTimeSeries<T>::GetValuesVector(size_t from, size_t to) const
		{
			CheckIntervalBounds(from, to);
			std::vector<T> values;
			this->CopyTo(values, from, to);
			return values;
		}

		template <class T>
		TTimeSeries<T> TTimeSeries<T>::Subset(size_t from, size_t to) const
		{
			CheckIntervalBounds(from, to);
			size_t len = (to - from) + 1;
			T * values = GetValues(from, to);
			auto res = TTimeSeries<T>(values, len, TimeForIndex(from), timeStep);
			delete[] values;
			return res;
		}

		template <class T>
		void TTimeSeries<T>::CopyTo(T * dest, size_t from, size_t to) const
		{
			CheckIntervalBounds(from, to);
			UncheckedCopyTo(dest, from, to);
		}

		template <class T>
		void TTimeSeries<T>::CopyTo(std::vector<T>& dest, size_t from, size_t to) const
		{
			CheckIntervalBounds(from, to);
			size_t len = (to - from) + 1;
			if (dest.size() != len)
			{
				dest.clear();
				dest.reserve(len);
			};
			std::copy(data.begin() + from, data.begin() + to + 1, dest.begin());
		}

		template <class T>
		void TTimeSeries<T>::UncheckedCopyTo(T * dest, const size_t& from, const size_t& to) const
		{
			// The following is neat and safe but not portable...
			//std::copy(data.begin() + from, data.begin() + to + 1, stdext::checked_array_iterator<T*>(dest, to - from + 1));
			// so, looking potentially unsafe:
			for (size_t i = from; i <= to; i++)
			{
				dest[i-from] = data[i];
			}
		}

		template <class T>
		void TTimeSeries<T>::SetValue(size_t index, T value)
		{
			data[index] = value;
		}

		//template <class T>
		//void TTimeSeries<T>::SetNA(T& naCode)
		//{
		//	this->naCode = naCode;
		//}

		template <class T>
		ptime TTimeSeries<T>::EndForStart(const ptime& start, const size_t& numSteps) const
		{
			return timeStep.AddSteps(start, numSteps - 1);
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

		template <class T>
		void TTimeSeries<T>::SetTimeStep(const TimeStep& timeStep)
		{
			this->timeStep = timeStep;
			UpdateEndDate();
		}

		template <class T>
		string TTimeSeries<T>::GetSummary() const
		{
			string result = "[" + to_iso_extended_string(GetStartDate()) + "-" + to_iso_extended_string(GetEndDate()) + "];" +
				std::to_string(GetLength()) + ";" + timeStep.GetName();
			return result;
		}

		// I need to do this HERE, not a cpp file, otherwise LNK2001
		template <typename T>
		std::atomic<int> TTimeSeries<T>::instances(0);


		typedef TTimeSeries < double > TimeSeries;
	}


	namespace exceptions
	{
		class TimeSeriesChecks
		{
		public:
			static void CheckOutOfRange(const string& msg, const datatypes::timeseries::TimeSeries& ts, const ptime& d)
			{
				if (d < ts.GetStartDate() || d > ts.GetEndDate())
					throw out_of_range(msg + ": " + to_iso_extended_string(d) + " out of range for " + ts.GetSummary());
			}
		};
	}
}
