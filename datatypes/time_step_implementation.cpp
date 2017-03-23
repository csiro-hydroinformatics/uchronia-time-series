#include "datatypes/time_step_implementation.h"

namespace datatypes
{
	namespace timeseries
	{
		TimeStepImplementation::~TimeStepImplementation() {}

		void TimeStepImplementation::CheckIsDateTime(const ptime& instant)
		{
			if (instant.is_not_a_date_time())
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Specified instant is not_a_date_time");
		}

		const ptrdiff_t TimeStepImplementation::GetUpperNumSteps(const ptime& start, const ptime& end) const
		{
			CheckIsDateTime(start);
			CheckIsDateTime(end);
			auto delta = end - start;
			if (delta.ticks() < 0)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Number of steps calculation must be such that end >= start");
			return (ptrdiff_t)(ceil(GetLinearIndexing(start, end)) + 1);
		}

		const ptrdiff_t TimeStepImplementation::GetNumSteps(const ptime& start, const ptime& end) const
		{
			CheckIsDateTime(start);
			CheckIsDateTime(end);
			auto delta = end - start;
			if (delta.ticks() < 0)
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("Number of steps calculation must be such that end >= start");
			return (ptrdiff_t)(floor(GetLinearIndexing(start, end)) + 1);
		}

		const ptrdiff_t TimeStepImplementation::GetOffset(const ptime& referenceTime, const ptime& testTime) const
		{
			return (ptrdiff_t)(floor(GetLinearIndexing(referenceTime, testTime)));
		}

		const time_duration RegularTimeStepImplementation::dailyTd = time_duration(24, 0, 0, 0);
		const time_duration RegularTimeStepImplementation::hourlyTd = time_duration(1, 0, 0, 0);

		RegularTimeStepImplementation* RegularTimeStepImplementation::GetHourly()
		{
			return new RegularTimeStepImplementation(hourlyTd);
		}

		RegularTimeStepImplementation* RegularTimeStepImplementation::GetDaily()
		{
			return new RegularTimeStepImplementation(dailyTd);
		}

		RegularTimeStepImplementation::RegularTimeStepImplementation(const time_duration& stepDuration)
		{
			regularStep = new time_duration(stepDuration);
		}

		RegularTimeStepImplementation::~RegularTimeStepImplementation()
		{
			if (regularStep != nullptr)
				delete regularStep;
		}

		TimeStepImplementation* RegularTimeStepImplementation::Clone()
		{
			return new RegularTimeStepImplementation(this->GetRegularStepDuration());
		}

		bool RegularTimeStepImplementation::Equals(TimeStepImplementation* tsImpl) const
		{
			RegularTimeStepImplementation* other = dynamic_cast<RegularTimeStepImplementation*>(tsImpl);
			return other != nullptr && other->GetRegularStepDuration() == GetRegularStepDuration();
		}

		TimeStepImplementation* RegularTimeStepImplementation::Divide(int divisor) const
		{
			using datatypes::exceptions::ExceptionUtilities;
			using tick_type = time_duration::tick_type;
			if (divisor == 0)
				ExceptionUtilities::ThrowInvalidArgument("divisor must not be zero");
			auto delta = *regularStep;
			tick_type v = delta.ticks();
			if ((v % (tick_type)divisor) != (tick_type)0)
				ExceptionUtilities::ThrowInvalidOperation(to_iso_string(delta) + " cannot be divided by " + std::to_string(divisor));
			return new RegularTimeStepImplementation(delta / divisor);
		}

		const ptime RegularTimeStepImplementation::AddIntSteps(const ptime& startTimeStep, int n) const
		{
			CheckIsDateTime(startTimeStep);
			return startTimeStep + (*regularStep) * n;
		}

		const ptime RegularTimeStepImplementation::AddSteps(const ptime& startTimeStep, double mult) const
		{
			CheckIsDateTime(startTimeStep);
			// TODO: keep a close eye on the following: check: it is probably wise to 
			// use AddIntSteps whenever possible.
			int whole = (int)std::floor(mult);
			if (whole == mult)
				return AddIntSteps(startTimeStep, whole);
			else
				return startTimeStep + multiply(mult);
		}

		TimeStepImplementation* RegularTimeStepImplementation::Multiply(int mult) const
		{
			return new RegularTimeStepImplementation(*regularStep * mult);
		}

		TimeStepImplementation* RegularTimeStepImplementation::Multiply(double mult) const
		{
			return new RegularTimeStepImplementation(multiply(mult));
		}

		time_duration RegularTimeStepImplementation::multiply(double mult) const
		{
			if (std::abs(mult) > std::numeric_limits<int>::max())
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidArgument("time step multiplication is limited by integer maximum value");
			int n = (int)std::floor(mult);
			auto delta = this->GetRegularStepDuration();
			auto nDuration = delta * n;

			double frac = mult - n;
			double dfDuration = delta.total_seconds() * frac;
			long fSeconds = (long)std::floor(dfDuration);

			return nDuration + seconds(fSeconds);
		}

		const time_duration RegularTimeStepImplementation::GetTimeStepDuration(const ptime& startTimeStep) const
		{
			return time_duration(*regularStep);
		}

		const void RegularTimeStepImplementation::Increment(ptime* t) const
		{
			*t = *t + *regularStep;
		}

		bool RegularTimeStepImplementation::IsRegular() const
		{
			return true;
		}

		time_duration RegularTimeStepImplementation::GetRegularStepDuration() const
		{
			return *regularStep;
		}

		std::string	RegularTimeStepImplementation::GetName() const
		{
			if (*(regularStep) == dailyTd)
				return std::string("daily");
			else if (*(regularStep) == hourlyTd)
				return std::string("hourly");
			else
				return to_iso_string(*regularStep);
		}

		void RegularTimeStepImplementation::CopyRegularImplementation(const RegularTimeStepImplementation& src)
		{
			if (regularStep != nullptr)
				delete regularStep;
			this->regularStep = new time_duration(*(src.regularStep));
		}

		const ptrdiff_t RegularTimeStepImplementation::GetUpperNumInstants(const ptime& start, const ptime& end) const
		{
			CheckIsDateTime(start);
			CheckIsDateTime(end);
			return GetUpperNumSteps(start, end) + 1;
		}

		const ptrdiff_t RegularTimeStepImplementation::GetNumInstants(const ptime& start, const ptime& end) const
		{
			CheckIsDateTime(start);
			CheckIsDateTime(end);
			return GetNumSteps(start, end) + 1;
		}

		const double RegularTimeStepImplementation::GetLinearIndexing(const ptime& start, const ptime& end) const
		{
			CheckIsDateTime(start);
			CheckIsDateTime(end);
			auto delta = end - start;
			auto deltaStep = (*regularStep).ticks();
			if (deltaStep == 0) datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Time step is of length zero");
			auto integerPart = delta.ticks() / deltaStep;
			auto remainder = delta.ticks() % deltaStep;

			return integerPart + remainder / (double)deltaStep;
		}

		TimeStepImplementation* IrregularTimeStepImplementation::Multiply(int mult) const
		{
			datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Cannot multiply irregular TimeSteps");
			return nullptr;
		}

		TimeStepImplementation* IrregularTimeStepImplementation::Multiply(double mult) const
		{
			datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Cannot multiply irregular TimeSteps");
			return nullptr;
		}

		TimeStepImplementation* IrregularTimeStepImplementation::Divide(int divisor) const
		{
			datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Cannot divide irregular TimeSteps");
			return nullptr;
		}

		const time_duration IrregularTimeStepImplementation::GetTimeStepDuration(const ptime& startTimeStep) const
		{
			datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Irregular TimeSteps have no fixed duration");
			return time_duration();
		}

		bool IrregularTimeStepImplementation::IsRegular() const
		{
			return false;
		}

		time_duration IrregularTimeStepImplementation::GetRegularStepDuration() const
		{
			datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Irregular TimeSteps have no fixed duration");
			return time_duration();
		}

		TimeStepImplementation* MonthlyQppTimeStepImplementation::Clone()
		{
			return new MonthlyQppTimeStepImplementation();
		}

		bool MonthlyQppTimeStepImplementation::Equals(TimeStepImplementation* tsImpl) const
		{
			MonthlyQppTimeStepImplementation* other = dynamic_cast<MonthlyQppTimeStepImplementation*>(tsImpl);
			return other != nullptr && other->GetName() == this->GetName();
		}

		const ptime MonthlyQppTimeStepImplementation::AddSteps(const ptime& startTimeStep, double mult) const
		{
			CheckIsDateTime(startTimeStep);
			// TODO: keep a close eye on the following: check: it is probably wise to 
			// use AddIntSteps whenever possible.
			int whole = (int)std::floor(mult);
			if (whole == mult)
				return AddIntSteps(startTimeStep, whole);
			else
				datatypes::exceptions::ExceptionUtilities::ThrowNotSupported("Cannot add non-integer values of MonthlyQppTimeStepImplementation");
		}

		const ptime MonthlyQppTimeStepImplementation::AddIntSteps(const ptime& startTimeStep, int n) const
		{
			int year = startTimeStep.date().year();
			int month = startTimeStep.date().month();
			int day = startTimeStep.date().day();
			int new_month = month;
			int new_year = year;
			int new_day = day;
			if (n >= 0)
			{
				for (ptrdiff_t i = 0; i < n; i++)
				{
					new_month++;
					if (new_month > 12)
					{
						new_month = 1;
						new_year++;
					}
				}
			}
			else
			{
				for (ptrdiff_t i = 0; i < -n; i++)
				{
					new_month--;
					if (new_month < 1)
					{
						new_month = 12;
						new_year--;
					}
				}
			}
			if (new_day >= 24)
			{
				int days_in_month = boost::gregorian::gregorian_calendar::end_of_month_day(year, month);
				int offset = days_in_month - day;
				int new_days_in_month = boost::gregorian::gregorian_calendar::end_of_month_day(new_year, new_month);
				new_day = new_days_in_month - offset;
			}
			boost::gregorian::date date = boost::gregorian::date(new_year, new_month, new_day);
			time_duration time = startTimeStep.time_of_day();
			ptime result = ptime(date, time);
			return result;
		}

		const void MonthlyQppTimeStepImplementation::Increment(ptime* t) const
		{
			*t = AddSteps(*t, 1);
		}

		std::string MonthlyQppTimeStepImplementation::GetName() const
		{
			return "monthly_qpp";
		}

		const double MonthlyQppTimeStepImplementation::GetLinearIndexing(const ptime& start, const ptime& end) const
		{
			CheckIsDateTime(start);
			CheckIsDateTime(end);
			auto delta = end - start;

			int s_year = start.date().year();
			int s_month = start.date().month();
			int s_day = start.date().day();

			int e_year = end.date().year();
			int e_month = end.date().month();
			int e_day = end.date().day();

			int y_diff = e_year - s_year;
			int m_diff = e_month - s_month;

			int target_end_day = s_day;
			if (s_day >= 24)
			{
				int days_in_month = boost::gregorian::gregorian_calendar::end_of_month_day(s_year, s_month);
				int offset = days_in_month - s_day;
				int new_days_in_month = boost::gregorian::gregorian_calendar::end_of_month_day(e_year, e_month);
				target_end_day = new_days_in_month - offset;
			}
			int d_diff_whole = e_day - target_end_day;

			time_duration time_diff = end.time_of_day() - start.time_of_day();
			double d_diff_fraction = time_diff.ticks() / time_diff.ticks_per_second() / 86400.0;

			double d_diff = d_diff_whole + d_diff_fraction;

			double average_days_per_month = 365.25 / 12.0;

			double diff = (y_diff * 12.0) + m_diff + (d_diff / average_days_per_month);
			return diff;
		}

	}
}