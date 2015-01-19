#include "common.h"
#include "time_series.h"
#include "exception_utilities.h"

using namespace boost::gregorian;

namespace datatypes
{
	namespace timeseries
	{
		template <class T>
		void TTimeSeries<T>::SetDefaults()
		{
			naCode = std::numeric_limits<T>::min();
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


		TimeSeries TimeSeries::operator*(double multiplicator) const
		{
			double a;
			TimeSeries ts(*this);
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
		void TTimeSeries<T>::Reset(int length, const ptime& startDate, T * value)
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

		template <typename T>
		MultiTimeSeries<T>::MultiTimeSeries(const std::vector<T*>& values, int length, const ptime& startDate)
		{
			series = new std::vector<TTimeSeries<T>*>();
			this->startDate = ptime(startDate);
			for (auto d : values)
			{
				series->push_back(new TTimeSeries<T>(d, length, startDate));
			}
		}

		template <typename T>
		MultiTimeSeries<T>::~MultiTimeSeries()
		{
			if (series != nullptr)
			{
				for (auto& d : *series)
				{
					if (d != nullptr) delete d;
				}
				delete series;
			}
		}

		template <typename T>
		TTimeSeries<T> * MultiTimeSeries<T>::Get(int i)
		{
			TTimeSeries<T>* a = series->at(i);
			return new TTimeSeries<T>(*a);
		}

		template <typename T>
		std::vector<T*>* MultiTimeSeries<T>::GetValues()
		{
			std::vector<T*>* result = new std::vector<T*>();
			for (auto& d : (*series))
			{
				result->push_back(d->GetValues());
			}
			return result;
		}

		template <typename T>
		void MultiTimeSeries<T>::Set(int i, int tsIndex, T val)
		{
			TTimeSeries<T>* a = series->at(i);
			(*a)[tsIndex] = val;
		}

		template <typename T>
		ptime MultiTimeSeries<T>::GetStart()
		{
			return ptime(startDate);
		}

		template <typename T>
		int MultiTimeSeries<T>::Size()
		{
			return this->series->size();
		}

		template <typename T>
		MultiTimeSeries<T>::MultiTimeSeries(const MultiTimeSeries<T>& src)
		{
			this->startDate = ptime(src.startDate);
			this->series = new vector<TTimeSeries<T>*>();
			
			for (int i = 0; i < src.series->size(); i++)
			{
				TTimeSeries<T>* copy = new TTimeSeries<T>(*src.series->at(i));
				this->series->push_back(copy);
			}
		}

		// explicit instantiations. Without these, code using this DL library would fail at link time.
		// see http://stackoverflow.com/a/495056/2752565

		template class TTimeSeries < double > ;
		template class TTimeSeries < float > ;

		template class MultiTimeSeries < double > ;
	}
}
