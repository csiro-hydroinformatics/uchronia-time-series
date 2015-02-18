#pragma once

#include "common.h"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost::posix_time;

namespace datatypes
{
	namespace timeseries
	{
		class DATATYPES_DLL_LIB TimeStep
		{
		public:
			TimeStep(const time_duration& stepDuration);
			TimeStep(const TimeStep& src);
			TimeStep();
			~TimeStep();

			TimeStep& operator=(const TimeStep &rhs);

			const ptime AddSteps(const ptime& startTimeStep, int n) const;

			const time_duration GetTimeStepDuration(const ptime& startTimeStep) const;
			const int GetUpperNumSteps(const ptime& start, const ptime& end) const;
			const int GetNumSteps(const ptime& start, const ptime& end) const;
			const int GetOffset(const ptime& start, const ptime& end) const;
			const void Increment(ptime* t) const;
			static TimeStep Parse(const std::string& name);

			static TimeStep GetDaily();
			static TimeStep GetHourly();

			std::string GetName();

		private:
			time_duration * regularStep = nullptr;
			void CopyRegularStep(const TimeStep& src);

			static const time_duration hourlyTd;
			static const time_duration dailyTd;

			const double GetLinearIndexing(const ptime& start, const ptime& end) const;

		};
	}
}
