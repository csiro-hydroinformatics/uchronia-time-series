#pragma once

#include "boost/date_time/posix_time/posix_time.hpp"
#include "common.h"
#include "exception_utilities.h"

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
			bool operator==(const TimeStep &rhs) const;

			bool operator!=(const TimeStep &rhs) const;

			TimeStep operator*(int mult) const;
			bool IsRegular() const;

			time_duration GetRegularStepDuration() const;

			const ptime AddSteps(const ptime& startTimeStep, ptrdiff_t n) const;

			const time_duration GetTimeStepDuration(const ptime& startTimeStep) const;
			const ptrdiff_t GetUpperNumSteps(const ptime& start, const ptime& end) const;
			const ptrdiff_t GetNumSteps(const ptime& start, const ptime& end) const;
			const ptrdiff_t GetOffset(const ptime& start, const ptime& end) const;
			const void Increment(ptime* t) const;
			static TimeStep Parse(const string& name);
			static TimeStep FromSeconds(unsigned int seconds);

			static TimeStep GetDaily();
			static TimeStep GetHourly();

			string GetName() const;

		private:
			// May not need to have thiis comcept of instant.
			const ptrdiff_t GetUpperNumInstants(const ptime& start, const ptime& end) const;
			const ptrdiff_t GetNumInstants(const ptime& start, const ptime& end) const;

			time_duration * regularStep = nullptr;
			void CopyRegularStep(const TimeStep& src);

			static const time_duration hourlyTd;
			static const time_duration dailyTd;

			const double GetLinearIndexing(const ptime& start, const ptime& end) const;

		};
	}
}
