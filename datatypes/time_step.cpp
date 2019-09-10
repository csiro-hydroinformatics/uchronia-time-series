#include <string>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string.hpp>    
#ifdef __GNUC__
// https ://jira.csiro.au/browse/WIRADA-350  GNU gcc regex bug; use boost instead
#if (__GNUC__ <= 4 && __GNUC_MINOR__ <= 9)
#include <boost/regex.hpp>
#else
#include <regex>
#endif
#else
#include <regex>
#endif // __GNUC__

#include "datatypes/time_step.h"
#include "datatypes/exception_utilities.h"

using namespace datatypes::exceptions;

namespace datatypes
{
	namespace timeseries
	{
		TimeStep::TimeStep(const time_duration& stepDuration)
		{
			tsImpl = new RegularTimeStepImplementation(stepDuration);
		}

		TimeStep::TimeStep(TimeStepImplementation* tsi)
		{
			tsImpl = tsi;
		}

		TimeStep::TimeStep(const TimeStep& src)
		{
			CopyTimeStepImplementation(src);
		}

		TimeStep::TimeStep()
		{
			CopyTimeStepImplementation(TimeStep::GetHourly());
		}

		void TimeStep::CopyTimeStepImplementation(const TimeStep& src)
		{
			if (tsImpl != nullptr)
				delete tsImpl;
			tsImpl = src.tsImpl->Clone();
		}

		TimeStep::~TimeStep()
		{
			if (tsImpl != nullptr)
				delete tsImpl;
		}

		TimeStep& TimeStep::operator=(const TimeStep &rhs)
		{
			CopyTimeStepImplementation(rhs);
			return *this;
		}

		TimeStep& TimeStep::operator=(const time_duration& stepDuration)
		{
			this->operator=(TimeStep(stepDuration));
			return *this;
		}

		TimeStep& TimeStep::operator=(const string& stepDuration)
		{
			this->operator=(TimeStep::Parse(stepDuration));
			return *this;
		}

		TimeStep TimeStep::FromSeconds(unsigned int seconds)
		{
			return TimeStep(new RegularTimeStepImplementation(time_duration(0, 0, seconds, 0)));
		}

		TimeStep TimeStep::GetDaily() { return TimeStep(RegularTimeStepImplementation::GetDaily()); };

		TimeStep TimeStep::GetHourly() { return TimeStep(RegularTimeStepImplementation::GetHourly()); };

		TimeStep TimeStep::GetMonthlyQpp() { return TimeStep(new MonthlyQppTimeStepImplementation()); };

		TimeStep TimeStep::GetUnknown() { return TimeStep(time_duration(not_a_date_time)); };

		bool TimeStep::IsUnknown() const { return this->GetRegularStepDuration().is_not_a_date_time(); };

		const time_duration TimeStep::GetTimeStepDuration(const ptime& startTimeStep) const
		{
			return tsImpl->GetTimeStepDuration(startTimeStep);
		}

		string TimeStep::GetName() const
		{
			return tsImpl->GetName();
		}

		const ptime TimeStep::AddSteps(const ptime& startTimeStep, size_t n) const
		{
			size_t max = 1000000000000 /*1e12*/ ;
			if (n > max)
				ExceptionUtilities::ThrowNotSupported("Time step arithmetic is limited to 1e12 steps");
			double x = (double)n;
			return AddSteps(startTimeStep, x);
		}

		const ptime TimeStep::AddSteps(const ptime& startTimeStep, int n) const
		{
			return tsImpl->AddSteps(startTimeStep, n);
		}

		const ptime TimeStep::AddSteps(const ptime& startTimeStep, double mult) const
		{
			return tsImpl->AddSteps(startTimeStep, mult);
		}

		vector<ptime> TimeStep::AddSteps(const vector<ptime>& times, double mult) const
		{
			vector<ptime> result(times.size());
			for (size_t i = 0; i < times.size(); i++)
				result[i] = AddSteps(times[i], mult);
			return result;
		}

		vector<ptime> TimeStep::AddSteps(const ptime& startTimeStep, const vector<double> timeSteps) const
		{
			vector<ptime> times(timeSteps.size());
			for (size_t i = 0; i < timeSteps.size(); i++)
				times[i] = tsImpl->AddSteps(startTimeStep, timeSteps[i]);
			return times;
		}

		ptime TimeStep::CreatePtime(int year, int month, int day, int hour, int minute, int second)
		{
			using namespace boost::gregorian;
			return ptime(date(year, month, day), hours(hour) + minutes(minute) + seconds(second));
		}

		ptime TimeStep::PtimeFromIsoString(const string& t)
		{
			string s(t);
			// from_iso_string cannot parse "2010-08-01T22:11:00", needs "20100801T221100"
			boost::algorithm::replace_all(s, "-", "");
			boost::algorithm::replace_all(s, ":", "");
			// (try to) cater for the case where date and time had a space rather than the T separator
			boost::trim_if(s, boost::is_any_of("\t "));
			boost::algorithm::replace_all(s, " ", "T");
			return from_iso_string(s);
		}

		string TimeStep::ToString(const ptime& dt, const string& format)
		{
			auto d = dt.date();
			auto pad = [&](int toSize, string& s)
			{
				while (s.size() < toSize) s = "0" + s;
				return s;
			};
			auto YYYY = std::to_string(d.year());
			YYYY = pad(4, YYYY);
			auto MM = std::to_string(d.month());
			MM = pad(2, MM);
			auto DD = std::to_string(d.day());
			DD = pad(2, DD);
			auto t = dt.time_of_day();
			auto hh = std::to_string(t.hours());
			hh = pad(2, hh);
			auto mm = std::to_string(t.minutes());
			mm = pad(2, mm);
			auto ss = std::to_string(t.seconds());
			ss = pad(2, ss);

			string s(format);
			boost::algorithm::replace_all(s, "YYYY", YYYY);
			boost::algorithm::replace_all(s, "MM", MM);
			boost::algorithm::replace_all(s, "DD", DD);
			boost::algorithm::replace_all(s, "hh", hh);
			boost::algorithm::replace_all(s, "mm", mm);
			boost::algorithm::replace_all(s, "ss", ss);
			return s;
		}

		bool TimeStep::FromGeneralStringPeriod(const string& name, TimeStep& tstep)
		{
#ifdef __GNUC__
			// https ://jira.csiro.au/browse/WIRADA-350  GNU gcc regex bug; use boost instead
#if (__GNUC__ <= 4 && __GNUC_MINOR__ <= 9)
			using boost::regex;
			using boost::regex_constants::icase;
			using boost::regex_search;
#else
			using std::regex;
			using std::regex_constants::icase;
			using std::regex_search;
#endif
#else
			using std::regex;
			using std::regex_constants::icase;
			using std::regex_search;
#endif // __GNUC__
			const string secondsOnlyPattern("^[0-9]+$");
			const string hmsPattern("^[0-9]+:[0-9]+:[0-9]+$");
			const regex rexhms(hmsPattern, icase);
			const regex rexs(secondsOnlyPattern, icase);
			string s = name;
			boost::trim_if(s, boost::is_any_of("\t "));
			
			if (regex_search(s, rexhms))
			{
				boost::posix_time::time_duration d = boost::posix_time::duration_from_string(s);
				tstep = TimeStep(d);
				return true;
			}
			else if (regex_search(s, rexs))
			{
				int durationSeconds = datatypes::utils::Parse<int>(s);
				tstep = TimeStep::FromSeconds(durationSeconds);
			}
			else
				return false;
		}

		TimeStep TimeStep::Parse(const string& name)
		{
			if (name == "daily")
				return GetDaily();
			else if (name == "hourly")
				return GetHourly();
			else if (name == "24:00:00")
				return GetDaily();
			else if (name == "01:00:00")
				return GetHourly();
			else if (name == "monthly_qpp")
				return GetMonthlyQpp();
			else
			{
				TimeStep tst;
				if (FromGeneralStringPeriod(name, tst))
					return tst;
				else
				{
					ExceptionUtilities::ThrowInvalidArgument("time step " + name + " could not be parsed and recognized");
					return GetUnknown(); // avoid compiler warning;
				}
			}
		}

		const void TimeStep::Increment(ptime* t) const
		{
			tsImpl->Increment(t);
		}

		const ptrdiff_t TimeStep::GetUpperNumSteps(const ptime& start, const ptime& end) const
		{
			return tsImpl->GetUpperNumSteps(start, end);
		}

		const ptrdiff_t TimeStep::GetNumSteps(const ptime& start, const ptime& end) const
		{
			return tsImpl->GetNumSteps(start, end);
		}

		const ptrdiff_t TimeStep::GetOffset(const ptime& referenceTime, const ptime& testTime) const
		{
			return tsImpl->GetOffset(referenceTime, testTime);
		}

		TimeStep TimeStep::operator*(int mult) const
		{
			return TimeStep(tsImpl->Multiply(mult));
		}

		TimeStep TimeStep::operator*(double mult) const
		{
			return TimeStep(tsImpl->Multiply(mult));
		}

		TimeStep TimeStep::operator/(int divisor) const
		{
			return TimeStep(tsImpl->Divide(divisor));
		}

		int TimeStep::operator/(const TimeStep& divisor) const
		{
			divisor.CheckIsRegular("division by time step");
			CheckIsRegular("division by time step");

			using tick_type = time_duration::tick_type;
			auto delta = this->GetRegularStepDuration();
			tick_type v = delta.ticks();
			if (v < 0) ExceptionUtilities::ThrowInvalidArgument("Negative time step not supported");
			auto otherdelta = divisor.GetRegularStepDuration();
			tick_type otherv = otherdelta.ticks();
			if (otherv < 0) ExceptionUtilities::ThrowInvalidArgument("Negative time step not supported");

			int div = (int)(v / otherv);
			return div;
		}

		time_duration TimeStep::operator%(const TimeStep& other) const
		{
			CheckIsRegular("modulo time step");
			using tick_type = time_duration::tick_type;
			auto delta = this->GetRegularStepDuration();
			tick_type v = delta.ticks();
			if (v < 0) ExceptionUtilities::ThrowInvalidArgument("Negative time step not supported");
			auto otherdelta = other.GetRegularStepDuration();
			tick_type otherv = otherdelta.ticks();
			if (otherv < 0) ExceptionUtilities::ThrowInvalidArgument("Negative time step not supported");

			tick_type remainder = v % otherv;

			long s = (long)(remainder / time_duration::ticks_per_second());
			return seconds(s);
		}

		bool TimeStep::IsRegular() const
		{
			return tsImpl->IsRegular();
		}

		void TimeStep::CheckIsRegular(const string& op) const
		{
			if(!tsImpl->IsRegular())
				datatypes::exceptions::ExceptionUtilities::ThrowInvalidOperation("Operation " + op + " requires a regular time step");
		}

		time_duration TimeStep::GetRegularStepDuration() const
		{
			return tsImpl->GetRegularStepDuration();
		}

		bool TimeStep::operator==(const TimeStep &rhs) const
		{
			return tsImpl->Equals(rhs.tsImpl);
		}

		bool TimeStep::operator!=(const TimeStep &rhs) const
		{
			return !tsImpl->Equals(rhs.tsImpl);
		}

		TimeSeriesInfoProvider::~TimeSeriesInfoProvider()
		{
		}
	}
}
