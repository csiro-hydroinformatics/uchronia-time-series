#include "common.h"
#include "time_series.h"
#include "exception_utilities.h"

using namespace boost::gregorian;

namespace datatypes
{
	namespace timeseries
	{
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

		template <typename T>
		TTimeSeries<T>* TimeSeriesOperations<T>::TrimTimeSeries(TTimeSeries<T>* timeSeries, const ptime& startDate, const ptime& endDate)
		{
			ptime sd = timeSeries->GetStartDate();
			ptime ed = timeSeries->GetEndDate();

			int sIndex = timeSeries->UpperIndexForTime(sd);
			int eIndex = timeSeries->LowerIndexForTime(ed);

			//auto timeStepSpan = seconds(timeSeries->GetTimeStepSeconds());

			//int offset = 0;
			//for (ptime dt = sd; dt < startDate; dt += timeStepSpan)
			//	offset++;
			//int startOffset = offset;

			//for (ptime dt = startDate; dt < endDate; dt += timeStepSpan)
			//	offset++;
			//int endOffset = offset;

			int n = eIndex - sIndex + 1;
			double* data = new double[n];
			timeSeries->CopyTo(data, sIndex, eIndex);

			TTimeSeries<T>* result = new TTimeSeries<T>(data, n, startDate);

			delete[] data;

			return result;
		}

		template <typename T>
		TTimeSeries<T>* TimeSeriesOperations<T>::DailyToHourly(TTimeSeries<T>* dailyTimeSeries)
		{
			int length = dailyTimeSeries->GetLength();

			double* data = new double[length * 24];

			for (int i = 0; i < length; i++)
				for (int j = 0; j < 24; j++)
					data[(i * 24) + j] = dailyTimeSeries->GetValue(i) / 24;

			TTimeSeries<T>* result = new TTimeSeries<T>(data, length * 24, dailyTimeSeries->GetStartDate());

			delete[] data;

			return result;
		}

		template <typename T>
		TTimeSeries<T>* TimeSeriesOperations<T>::JoinTimeSeries(TTimeSeries<T>* head, TTimeSeries<T>* tail)
		{
			ptime startDate = head->GetStartDate();

			int headLength = head->GetLength();
			int tailLength = tail->GetLength();

			int length = headLength + tailLength;

			double* data = new double[length];

			for (int i = 0; i < length; i++)
			{
				if (i < headLength)
					data[i] = head->GetValue(i);
				else
					data[i] = tail->GetValue(i - headLength);
			}

			TTimeSeries<T>* result = new TTimeSeries<T>(data, length, startDate);

			delete[] data;

			return result;
		}

		template <typename T>
		bool TimeSeriesOperations<T>::AreTimeSeriesEqual(TTimeSeries<T>* a, TTimeSeries<T>* b)
		{
			ptime startA = a->GetStartDate();
			ptime startB = b->GetStartDate();

			if (startA != startB)
				return false;

			int lengthA = a->GetLength();
			int lengthB = b->GetLength();

			if (lengthA != lengthB)
				return false;

			for (int i = 0; i < lengthA; i++)
			{
				auto valA = a->GetValue(i);
				auto valB = b->GetValue(i);

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
		TTimeSeries<T>* TimeWindow<T>::Trim(TTimeSeries<T>* timeSeries) const
		{
			return TimeSeriesOperations<T>::TrimTimeSeries(timeSeries, startDate, endDate);
		}

		template class TimeWindow < double >;

	}
}
