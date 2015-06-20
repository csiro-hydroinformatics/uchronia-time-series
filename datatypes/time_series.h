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


		template <typename T = double>
		class DefaultMissingFloatingPointPolicy{
		private:
			const T missingValue = (T)(-9999.0);
		public:
			inline bool IsMissingValue(T a) const { return (a == missingValue); };
			inline T GetMissingValue() const { return missingValue; };
		};

		// Use something like at http://en.cppreference.com/w/cpp/types/enable_if, maybe.
		template <typename T>
		class NullPointerIsMissingPolicy{
		public:
			inline bool IsMissingValue(T a) const { return (a == nullptr); };
			inline T GetMissingValue() const { return nullptr; };
		};


		template <typename T = double>
		class NegativeIsMissingFloadingPointPolicy{
		private:
			const T missingValue = (T)(-9999.0);
		public:
			inline bool IsMissingValue(T a) const { return (a < T()); };
			inline T GetMissingValue() const { return missingValue; };
		};

		template <typename T = double>
		class DefaultStorageFloatingPointPolicy{
		public:
			typedef typename std::vector<T> StorageType;
			void Allocate(StorageType& data, size_t length, T value)
			{
				data.clear();
				data.reserve(length);
				data.assign(length, value);
			}
			void AllocateValues(StorageType& data, size_t length, T* values)
			{
				data.clear();
				data.reserve(length);
				data.assign(values, values + length);
			}
		};

		/**
		 * \brief	A template for univariate, single realisasion time series
		 *
		 * \tparam	T	Element type, typically double or float, but possibly a more complicated type such as another TTimeSeries.
		 */
		template <typename T = double,
			template <typename> class StP = DefaultStorageFloatingPointPolicy,
			template <typename> class MvP = DefaultMissingFloatingPointPolicy
		>
		class TTimeSeries
		{

		protected:

			typedef typename StP<T> StoragePolicy;
			typedef typename MvP<T> MissingValuePolicy;
			typedef typename StoragePolicy::StorageType StorageType;

			StoragePolicy stp;
			MissingValuePolicy mvp;

			StorageType data;

			inline T GetNACode() const { return this->mvp.GetMissingValue(); }

		public:

			inline bool IsMissingValue(T value) const { return this->mvp.IsMissingValue(value); }

			inline T GetMissingValue() const { return this->mvp.GetMissingValue(); }

			typedef typename T ElementType;

			TTimeSeries()
			{
				SetDefaults();
				Reset(1, ptime(date(2000, 1, 1)));
				instances++;
			}

			/**
			* \brief	Time Series Constructor.
			*
			* \param	default_value	The default value.
			* \param	length		 	The length.
			* \param	startDate	 	The start date.
			* \param	timeStep	 	The time step of the time series.
			*/
			TTimeSeries(T default_value, size_t length, const ptime& startDate, const TimeStep& timeStep = TimeStep::GetHourly())
			{
				SetDefaults();
				this->timeStep = timeStep;
				Reset(length, startDate);
				data = vector<T>(length, default_value);
				instances++;
			}

			TTimeSeries(size_t length, const ptime& startDate, const TimeStep& timeStep = TimeStep::GetHourly())
			{
				SetDefaults();
				this->timeStep = timeStep;
				Reset(length, startDate);
				data = vector<T>(length, mvp.GetMissingValue());
				instances++;
			}

			TTimeSeries(T * values, size_t length, const ptime& startDate, const TimeStep& timeStep = TimeStep::GetHourly())
			{
				SetDefaults();
				this->timeStep = timeStep;
				Reset(length, startDate, values);
				instances++;
			}

			TTimeSeries(const TTimeSeries<T, StP, MvP>& src) {   // (Deep) Copy constructor.
				*this = src;
				instances++;
			}

			TTimeSeries(const TTimeSeries<T, StP, MvP>* src) {   // (Deep) Copy constructor.
				*this = *src;
				instances++;
			}

			/**
			* \brief	Constructor using the move semantics
			*
			* \param [in,out]	src	time series from which to move data.
			* \remarks 		C++ for the Impatient
			* 					Appendix A. A Painless Introduction to Rvalue References (C++11)
			* 					See also http://stackoverflow.com/a/3109981/2752565 for information on move semantic
			*/
			TTimeSeries(TTimeSeries<T, StP, MvP>&& src) {   // Move constructor.
				*this = std::move(src);
				instances++;
			}

			TTimeSeries<T, StP, MvP>& operator=(TTimeSeries<T, StP, MvP>&& src){
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

			TTimeSeries<T, StP, MvP>& operator=(const TTimeSeries<T, StP, MvP>& src)
			{
				if (&src == this){
					return *this;
				}
				DeepCopyFrom(src);
				return *this;
			}

			virtual /*TODO revisit whether virtual is necessary, if any downside.*/ ~TTimeSeries()
			{
				instances--;
			}

			T GetValue(size_t index)
			{
				if (index < 0 || index >= GetLength())
					datatypes::exceptions::ExceptionUtilities::ThrowOutOfRange("Trying to access a time series value with an index outside of its bounds");
				return data[index];
			}

			void CopyTo(T * dest, size_t from = 0, size_t to = -1) const
			{
				CheckIntervalBounds(from, to);
				UncheckedCopyTo(dest, from, to);
			}

			void CopyTo(std::vector<T>& dest, size_t from = 0, size_t to = -1) const
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

			T * GetValues(size_t from = 0, size_t to = std::numeric_limits<size_t>::max()) const
			{
				CheckIntervalBounds(from, to);
				size_t len = (to - from) + 1;
				T * values = new T[len];
				this->UncheckedCopyTo(values, from, to);
				return values;
			}

			std::vector<T> GetValuesVector(size_t from = 0, size_t to = std::numeric_limits<size_t>::max()) const
			{
				CheckIntervalBounds(from, to);
				std::vector<T> values;
				this->CopyTo(values, from, to);
				return values;
			}

			TTimeSeries<T, StP, MvP> Subset(size_t from = 0, size_t to = std::numeric_limits<size_t>::max()) const
			{
				CheckIntervalBounds(from, to);
				size_t len = (to - from) + 1;
				T * values = GetValues(from, to);
				auto res = TTimeSeries<T, StP, MvP>(values, len, TimeForIndex(from), timeStep);
				delete[] values;
				return res;
			}
			void SetValue(size_t index, T value)
			{
				data[index] = value;
			}

			void Reset(size_t length, const ptime& startDate, T * values = nullptr)
			{
				if (values == nullptr)
					this->stp.Allocate(data, length, mvp.GetMissingValue());
				else
					this->stp.AllocateValues(data, length, values);
				this->startDate = startDate;
				UpdateEndDate();
			}

			size_t GetLength() const
			{
				return data.size();
			}

			ptime GetStartDate() const
			{
				return ptime(startDate);
			}

			ptime GetEndDate() const
			{
				return ptime(endDate);
			}

			TimeStep GetTimeStep() const
			{
				return this->timeStep;
			}

			void SetTimeStep(const TimeStep& timeStep)
			{
				this->timeStep = timeStep;
				UpdateEndDate();
			}

			ptime TimeForIndex(size_t timeIndex) const
			{
				return timeStep.AddSteps(this->startDate, timeIndex);
			}

			size_t IndexForTime(const ptime& instant) const {
				return LowerIndexForTime(instant);
			}

			size_t UpperIndexForTime(const ptime& instant) const
			{
				auto uIndex = timeStep.GetUpperNumSteps(startDate, instant) - 1;
				datatypes::exceptions::ExceptionUtilities::CheckInRange<ptrdiff_t>(uIndex, 0, GetLength() - 1, "UpperIndexForTime");
				return (size_t)uIndex;
			}


			size_t LowerIndexForTime(const ptime& instant) const
			{
				auto uIndex = timeStep.GetNumSteps(startDate, instant) - 1;
				datatypes::exceptions::ExceptionUtilities::CheckInRange<ptrdiff_t>(uIndex, 0, GetLength() - 1, "LowerIndexForTime");
				return (size_t)uIndex;
			}

			string GetSummary() const
			{
				string result = "[" + to_iso_extended_string(GetStartDate()) + "-" + to_iso_extended_string(GetEndDate()) + "];" +
					std::to_string(GetLength()) + ";" + timeStep.GetName();
				return result;
			}

			T& operator[](const ptime& instant) {
				return data[IndexForTime(instant)];
			}


			T& operator[](const size_t i) {
				T& vRef = data[i];
				return vRef;
			}


			const T& operator[](const ptime& instant) const {
				return this->operator[](IndexForTime(instant));
			}


			const T& operator[](const size_t i) const {
				return data[i];
			}


			TTimeSeries<T, StP, MvP> operator+(const TTimeSeries<T, StP, MvP>& rhs) const
			{
				// TODO check geometry compatibility
				T a, b;
				T na = this->mvp.GetMissingValue();
				size_t length = rhs.GetLength();
				TTimeSeries<T, StP, MvP> ts(na, length, startDate, timeStep);
				//ts.naCode = this->naCode;
				for (size_t i = 0; i < length; i++)
				{
					a = this->data[i];
					b = rhs.data[i];
					if (IsMissingValue(a) || rhs.IsMissingValue(b))
						ts.data[i] = ts.GetNACode();
					else
						ts.data[i] = a + b;
				}
				return ts;
			}

			template<typename M>
			TTimeSeries<T, StP, MvP> operator*(M multiplicator) const
			{
				T a;
				TTimeSeries<T, StP, MvP> ts(*this);
				auto length = GetLength();
				for (size_t i = 0; i < length; i++)
				{
					a = this->data[i];
					if (IsMissingValue(a))
						ts.data[i] = ts.GetNACode();
					else
						ts.data[i] = a * multiplicator;
				}
				return ts;
			}

			//std::enable_if_t<std::is_arithmetic<T>::value, void>
			//	SetNA(T& naCode)
			//{
			//	this->naCode = naCode;
			//}
			//std::enable_if_t<std::is_arithmetic<T>::value, T>
			//	GetNA() const { return naCode; }

			string Tag;

		private:
			ptime EndForStart(const ptime& start, const size_t& numSteps) const
			{
				return timeStep.AddSteps(start, numSteps - 1);
			}

			void CheckIntervalBounds(const size_t& from, size_t& to) const
			{
				size_t tsLen = this->GetLength();
				if (to == std::numeric_limits<size_t>::max())
					to = min(to, (tsLen - 1));
				datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(from, 0, tsLen - 1, "from");
				datatypes::exceptions::ExceptionUtilities::CheckInRange<size_t>(to, 0, tsLen - 1, "to");
			}

			void UncheckedCopyTo(T * dest, const size_t& from, const size_t& to) const
			{
				// The following is neat and safe but not portable...
				//std::copy(data.begin() + from, data.begin() + to + 1, stdext::checked_array_iterator<T*>(dest, to - from + 1));
				// so, looking potentially unsafe:
				for (size_t i = from; i <= to; i++)
				{
					dest[i - from] = data[i];
				}
			}

			void DeepCopyFrom(const TTimeSeries<T, StP, MvP>& src) {
				//naCode = src.naCode;
				timeStep = src.timeStep;
				startDate = src.startDate;
				endDate = src.endDate;
				Tag = src.Tag;
				data = src.data;
			}

			ptime startDate;
			ptime endDate;
			TimeStep timeStep;
			static std::atomic<int> instances;

			void SetDefaults()
			{
				//naCode = std::numeric_limits<T>::min();
				timeStep = TimeStep::GetHourly();
			}

			void UpdateEndDate()
			{
				this->endDate = EndForStart((this->startDate), GetLength());
			}

		public:
			static int NumInstances() { return instances; };
		};

		template <typename T, template <typename> class StP, template <typename> class MvP>
		std::atomic<int> TTimeSeries<T, StP, MvP>::instances(0);


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

		typedef TTimeSeries < double > TimeSeries;

		/**
		 * \brief	Template for time series with multiple values at time point; e.g. to hold multiple realizations of time series in an ensemble
		 *
		 * \tparam	T	Elemental type, typically double or float.
		 */
		template <typename TsType = TimeSeries>
		class MultiTimeSeries // This may become an abstract class with specializations for lazy loading time series from the data store.
		{
		public:

			typedef typename std::remove_pointer<TsType>::type Type;
			typedef typename std::add_pointer<Type>::type PtrType;
			typedef typename Type::ElementType ElementType;

			MultiTimeSeries(const std::vector<ElementType*>& values, size_t length, const ptime& startDate, const TimeStep& timeStep)
			{
				Clear();
				this->startDate = startDate;
				this->timeStep = timeStep;
				for (auto& d : values)
				{
					series.push_back(new Type(d, length, startDate, timeStep));
				}
			}

			MultiTimeSeries(const MultiTimeSeries<TsType>& src)
			{
				this->startDate = ptime(src.startDate);
				this->timeStep = TimeStep(src.timeStep);

				for (size_t i = 0; i < src.series.size(); i++)
				{
					Type* copy = new Type(*src.series.at(i));
					this->series.push_back(copy);
				}
			}

			MultiTimeSeries()
			{
				this->startDate = ptime(not_a_date_time);
				this->timeStep = TimeStep::GetHourly();
			}

			~MultiTimeSeries()
			{
				Clear();
			}

			void ResetSeries(const size_t& numSeries, const size_t& lengthSeries, const ptime& startDate, const TimeStep& timeStep)
			{
				Clear();
				for (size_t i = 0; i < numSeries; i++)
				{
					series.push_back(new Type(lengthSeries, startDate, timeStep));
				}
				this->startDate = startDate;
				this->timeStep = timeStep;
			}

			TsType Get(size_t i)
			{
				return TsType(series.at(i));
			}

			ElementType Get(size_t i, size_t tsIndex)
			{
				return series[i][tsIndex];
			}

			void Set(size_t i, size_t tsIndex, ElementType val)
			{
				PtrType a = series.at(i);
				(*a)[tsIndex] = val;
			}

			std::vector<ElementType*>* GetValues()
			{
				std::vector<ElementType*>* result = new std::vector<ElementType*>();
				for (auto& d : series)
				{
					result->push_back(d->GetValues());
				}
				return result;
			}

			size_t Size()
			{
				return this->series.size();
			}

			ptime GetStartDate()

			{
				return ptime(startDate);
			}

			void Clear()
			{
				for (auto& d : series)
				{
					if (d != nullptr) delete d;
				}
				series.clear();
			}

		private:

			std::vector<PtrType> series;
			ptime startDate;
			TimeStep timeStep;
		};


		template <typename ItemType>
		using PointerTypeTimeSeries = TTimeSeries < ItemType*, DefaultStorageFloatingPointPolicy, NullPointerIsMissingPolicy >;

		template <typename Tts = TimeSeries>
		using MultiTimeSeriesPtr = MultiTimeSeries < Tts* > ;

		template <typename Tts = TimeSeries>
		using ForecastTimeSeries = PointerTypeTimeSeries < Tts > ;

		template <typename Tts = TimeSeries>
		using EnsembleForecastTimeSeries = PointerTypeTimeSeries < MultiTimeSeriesPtr<Tts> > ;

		template <typename Tts = TimeSeries>
		class TimeSeriesOperations
		{
		public:
			static Tts* TrimTimeSeries(const Tts& timeSeries, const ptime& startDate, const ptime& endDate)
			{
				ptime sd = timeSeries.GetStartDate();
				ptime ed = timeSeries.GetEndDate();

				size_t sIndex = timeSeries.UpperIndexForTime(startDate);
				size_t eIndex = timeSeries.LowerIndexForTime(endDate);

				return new Tts(timeSeries.Subset(sIndex, eIndex));
			}

			static Tts* Resample(const Tts& timeSeries, const string& method)
			{
				using namespace boost::algorithm;
				string m = method;
				to_lower(m);
				if (m == "")
					return new TimeSeries(timeSeries);
				else if (m == "daily_to_hourly")
					return DailyToHourly(timeSeries);
				else
					datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Time series resampling method not known: " + method);
			}

			static Tts* DailyToHourly(const Tts& dailyTimeSeries)
			{
				using T = Tts::ElementType;
				size_t length = dailyTimeSeries.GetLength();

				T * data = new T[length * 24];

				for (size_t i = 0; i < length; i++)
					for (size_t j = 0; j < 24; j++)
						data[(i * 24) + j] = (dailyTimeSeries[i] / 24);

				Tts* result = new Tts(data, length * 24, dailyTimeSeries.GetStartDate());

				delete[] data;

				return result;
			}

			static Tts* JoinTimeSeries(const Tts& head, const Tts& tail)
			{
				using T = Tts::ElementType;
				ptime startDate = head.GetStartDate();

				size_t headLength = head.GetLength();
				size_t tailLength = tail.GetLength();

				size_t length = headLength + tailLength;

				T * data = new T[length];

				for (size_t i = 0; i < length; i++)
				{
					if (i < headLength)
						data[i] = head[i];
					else
						data[i] = tail[i - headLength];
				}

				Tts* result = new Tts(data, length, startDate);

				delete[] data;

				return result;
			}

			static bool AreTimeSeriesEqual(const Tts& a, const Tts& b)
			{
				ptime startA = a.GetStartDate();
				ptime startB = b.GetStartDate();

				if (startA != startB)
					return false;

				size_t lengthA = a.GetLength();
				size_t lengthB = b.GetLength();

				if (lengthA != lengthB)
					return false;

				for (size_t i = 0; i < lengthA; i++)
				{
					auto valA = a[i];
					auto valB = b[i];

					if (std::abs(valA - valB) > 1.0e-12)
						return false;
				}

				return true;
			}
		};

		/**
		 * \brief	An object that represents a time window, defining subset/trim operations on time series
		 *
		 * \tparam	T	Generic type parameter, element type of the time series
		 */
		template <typename Tts = TimeSeries>
		class TimeWindow
		{
		public:
			TimeWindow<Tts>::TimeWindow(const ptime& startDate, const ptime& endDate)
			{
				this->startDate = startDate;
				this->endDate = endDate;
			}

			Tts* Trim(const Tts& timeSeries) const
			{
				return TimeSeriesOperations<Tts>::TrimTimeSeries(timeSeries, startDate, endDate);
			}

		private:
			ptime startDate;
			ptime endDate;

		};

		/*******************
		Below are implementations of the template code; they would normally be found in a .cpp file, but as
		templates putting them here makes it more reusable from other programs.
		******************/

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
