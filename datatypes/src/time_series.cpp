#include "datatypes/common.h"
#include "datatypes/time_series.hpp"
#include "datatypes/exception_utilities.h"

using namespace boost::gregorian;

namespace datatypes
{
	namespace timeseries
	{
		// explicit instantiations. Without these, code using this DL library would fail at link time.
		// see http://stackoverflow.com/a/495056/2752565

		template class TTimeSeries < double > ;
		template class TTimeSeries < float >;

		//template class TTimeSeries < vector<double> >;
		//template class MultiTimeSeries < double >;

		template class TimeSeriesOperations < TimeSeries >;

		template class TimeWindow < TimeSeries >;

	}

	namespace exceptions
	{
		void TimeSeriesChecks::CheckOutOfRange(const string& msg, const datatypes::timeseries::TimeSeries& ts, const ptime& d)
		{
			if (d < ts.GetStartDate() || d > ts.GetEndDate())
				throw std::out_of_range(msg + ": " + to_iso_extended_string(d) + " out of range for " + ts.GetSummary());
		}
	}

}
