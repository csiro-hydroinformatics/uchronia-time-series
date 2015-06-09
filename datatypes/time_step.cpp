#include "time_step.h"
#include "exception_utilities.h"

using namespace datatypes::exceptions;

namespace datatypes
{
	namespace timeseries
	{

		TimeStep::TimeStep(const time_duration& stepDuration)
		{
			regularStep = new time_duration(stepDuration);
		}

		TimeStep::TimeStep(const TimeStep& src)
		{
			CopyRegularStep(src);
		}

		TimeStep::TimeStep()
		{
			CopyRegularStep(TimeStep::GetHourly());
		}

		const time_duration TimeStep::dailyTd = time_duration(24, 0, 0, 0);
		const time_duration TimeStep::hourlyTd = time_duration(1, 0, 0, 0);

		void TimeStep::CopyRegularStep(const TimeStep& src)
		{
			if (regularStep != nullptr)
				delete regularStep;
			this->regularStep = new time_duration(*(src.regularStep));
		}

		TimeStep::~TimeStep()
		{
			if (regularStep != nullptr)
				delete regularStep;
		}

		TimeStep& TimeStep::operator=(const TimeStep &rhs)
		{
			CopyRegularStep(rhs);
			return *this;
		}

		TimeStep TimeStep::GetDaily() { return TimeStep(dailyTd); };

		TimeStep TimeStep::GetHourly() { return TimeStep(hourlyTd); };

		const time_duration TimeStep::GetTimeStepDuration(const ptime& startTimeStep) const
		{
			return time_duration(*regularStep);
		}

		std::string TimeStep::GetName() const
		{
			if (!IsRegular())
				ExceptionUtilities::ThrowNotImplemented("non-regular time steps not implemented");
			if (*(regularStep) == dailyTd)
				return std::string("daily");
			else if (*(regularStep) == hourlyTd)
				return std::string("hourly");
			else
				return to_iso_string(*regularStep);
				//ExceptionUtilities::ThrowInvalidArgument("time step is neither daily nor hourly; formatting of an ID is not yet implemented");
		}

		const ptime TimeStep::AddSteps(const ptime& startTimeStep, ptrdiff_t n) const
		{
			return startTimeStep + (*regularStep) * n;
		}

		TimeStep TimeStep::Parse(const std::string& name)
		{
			if (name == "daily")
				return GetDaily();
			else if (name == "hourly")
				return GetHourly();
			else
				ExceptionUtilities::ThrowInvalidArgument("time step " + name + " could not be parsed and recognized");
		}

		const void TimeStep::Increment(ptime* t) const
		{
			*t = *t + *regularStep;
		}

		const ptrdiff_t TimeStep::GetUpperNumInstants(const ptime& start, const ptime& end) const
		{
			return GetUpperNumSteps(start, end) + 1;
		}

		const ptrdiff_t TimeStep::GetNumInstants(const ptime& start, const ptime& end) const
		{
			return GetNumSteps(start, end) + 1;
		}

		const ptrdiff_t TimeStep::GetUpperNumSteps(const ptime& start, const ptime& end) const
		{
			auto delta = end - start;
			if (delta.ticks() < 0)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Number of steps calculation must be such that end >= start");
			return (ptrdiff_t)(ceil(GetLinearIndexing(start, end)) + 1);
		}

		const ptrdiff_t TimeStep::GetNumSteps(const ptime& start, const ptime& end) const
		{
			auto delta = end - start;
			if (delta.ticks() < 0)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Number of steps calculation must be such that end >= start");
			return (ptrdiff_t)(floor(GetLinearIndexing(start, end)) + 1);
		}

		const double TimeStep::GetLinearIndexing(const ptime& start, const ptime& end) const
		{
			auto delta = end - start;
			auto deltaStep = (*regularStep).ticks();
			if (deltaStep == 0) ExceptionUtilities::ThrowInvalidOperation("Time step is of length zero");
			auto integerPart = delta.ticks() / deltaStep;
			auto remainder = delta.ticks() % deltaStep;
			
			return integerPart + remainder / (double)deltaStep;
		}

		const ptrdiff_t TimeStep::GetOffset(const ptime& referenceTime, const ptime& testTime) const
		{
			return (ptrdiff_t)(floor(GetLinearIndexing(referenceTime, testTime)));
		}
	}
}