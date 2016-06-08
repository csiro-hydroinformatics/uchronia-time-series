#include "common.h"
#include "time_series.hpp"
#include "exception_utilities.h"

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
}
