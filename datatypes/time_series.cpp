#include "common.h"
#include "time_series.h"
#include "exception_utilities.h"

using namespace boost::gregorian;

namespace datatypes
{
	namespace timeseries
	{
		template <typename T>
		MultiTimeSeries<T>::MultiTimeSeries(const std::vector<T*>& values, size_t length, const ptime& startDate, const TimeStep& timeStep)
		{
			series = new std::vector<TTimeSeries<T>*>();
			this->startDate = ptime(startDate);
			this->timeStep = TimeStep(timeStep);
			for (auto d : values)
			{
				series->push_back(new TTimeSeries<T>(d, length, startDate, timeStep));
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
		TTimeSeries<T> * MultiTimeSeries<T>::Get(size_t i)
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
		void MultiTimeSeries<T>::Set(size_t i, size_t tsIndex, T val)
		{
			TTimeSeries<T>* a = series->at(i);
			(*a)[tsIndex] = val;
		}

		template <typename T>
		ptime MultiTimeSeries<T>::GetStartDate()
		{
			return ptime(startDate);
		}

		template <typename T>
		size_t MultiTimeSeries<T>::Size()
		{
			return this->series->size();
		}

		template <typename T>
		MultiTimeSeries<T>::MultiTimeSeries(const MultiTimeSeries<T>& src)
		{
			this->startDate = ptime(src.startDate);
			this->series = new vector<TTimeSeries<T>*>();
			this->timeStep = TimeStep(src.timeStep);
			
			for (size_t i = 0; i < src.series->size(); i++)
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

		template <typename T>
		TTimeSeries<T>* TimeSeriesOperations<T>::TrimTimeSeries(const TTimeSeries<T>& timeSeries, const ptime& startDate, const ptime& endDate)
		{
			ptime sd = timeSeries.GetStartDate();
			ptime ed = timeSeries.GetEndDate();

			size_t sIndex = timeSeries.UpperIndexForTime(startDate);
			size_t eIndex = timeSeries.LowerIndexForTime(endDate);

			return new TTimeSeries<T>(timeSeries.Subset(sIndex, eIndex));
		}

		template <typename T>
		TTimeSeries<T>* TimeSeriesOperations<T>::DailyToHourly(const TTimeSeries<T>& dailyTimeSeries)
		{
			size_t length = dailyTimeSeries.GetLength();

			T * data = new T[length * 24];

			for (size_t i = 0; i < length; i++)
				for (size_t j = 0; j < 24; j++)
					data[(i * 24) + j] = (dailyTimeSeries[i] / 24);

			TTimeSeries<T>* result = new TTimeSeries<T>(data, length * 24, dailyTimeSeries.GetStartDate());

			delete[] data;

			return result;
		}

		template <typename T>
		TTimeSeries<T>* TimeSeriesOperations<T>::JoinTimeSeries(const TTimeSeries<T>& head, const TTimeSeries<T>& tail)
		{
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

			TTimeSeries<T>* result = new TTimeSeries<T>(data, length, startDate);

			delete[] data;

			return result;
		}

		template <typename T>
		bool TimeSeriesOperations<T>::AreTimeSeriesEqual(const TTimeSeries<T>& a, const TTimeSeries<T>& b)
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

		template <typename T>
		TimeWindow<T>::TimeWindow(const ptime& startDate, const ptime& endDate)
		{
			this->startDate = startDate;
			this->endDate = endDate;
		}

		template class TimeSeriesOperations < double >;

		template <typename T>
		TTimeSeries<T>* TimeWindow<T>::Trim(const TTimeSeries<T>& timeSeries) const
		{
			return TimeSeriesOperations<T>::TrimTimeSeries(timeSeries, startDate, endDate);
		}

		template class TimeWindow < double >;

	}
}
