#pragma once

#include "boost/date_time/posix_time/posix_time.hpp"
#include "datatypes/common.h"
#include "datatypes/exception_utilities.h"
#include "datatypes/time_step_implementation.h"

using namespace boost::posix_time;
using sec_type = boost::posix_time::time_duration::sec_type;

namespace datatypes
{
	namespace timeseries
	{
		/**
		 * \class	TimeStep
		 *
		 * \brief	Time step handling for time series
		 *
		 *			A TimeStep is responsible for the definition of time steps in time series 
		 *			and the associated calculations for determining time instants and durations
		 */

		class DATATYPES_DLL_LIB TimeStep
		{
		public:

			/**
			 * \fn	TimeStep::TimeStep(const time_duration& stepDuration);
			 *
			 * \brief	Define a time step where every step is a fixed time duration
			 *
			 * \param	stepDuration	Duration of the step.
			 */
			TimeStep(const time_duration& stepDuration);
			TimeStep(TimeStepImplementation* tsi);

			/**
			 * \fn	TimeStep::TimeStep(const TimeStep& src);
			 *
			 * \brief	Copy constructor.
			 *
			 * \param	src	TimeStep to copy.
			 */

			TimeStep(const TimeStep& src);
			TimeStep();
			~TimeStep();

			TimeStep& operator=(const TimeStep &rhs);

			/**
			 * \fn	TimeStep& TimeStep::operator=(const time_duration& stepDuration);
			 *
			 * \brief	Assignment operator.
			 *
			 * \param	stepDuration	Duration of the step.
			 *
			 * \return	A reference to this object.
			 */

			TimeStep& operator=(const time_duration& stepDuration);

			TimeStep& operator=(const string& stepDuration);

			bool operator==(const TimeStep &rhs) const;

			bool operator!=(const TimeStep &rhs) const;

			/**
			 * \fn	TimeStep TimeStep::operator*(int mult) const;
			 *
			 * \brief	Multiplication operator, using the time_duration multiplication operator.
			 *
			 * \param	mult	The multiplier.
			 *
			 * \return	The result of the operation.
			 */

			TimeStep operator*(int mult) const;

			/**
			* \fn	TimeStep TimeStep::operator*(int mult) const;
			*
			* \brief	Multiplication operator, using the time_duration multiplication operator if the argument is an integer.
			*
			* \param	mult	The multiplier.
			*
			* \return	The result of the operation.
			*/
			TimeStep operator*(double mult) const;

			int operator/(const TimeStep& divisor) const;

			TimeStep operator/(int divisor) const;

			time_duration operator%(const TimeStep& other) const;

			/**
			 * \fn	bool TimeStep::IsRegular() const;
			 *
			 * \brief	Query if this time step is defined by a time duration in the true sense (day, week). Monthly time step would return false;
			 *
			 * \return	true if regular, false if not.
			 */

			bool IsRegular() const;

			/**
			 * \fn	time_duration TimeStep::GetRegularStepDuration() const;
			 *
			 * \brief	Gets the underlying time_duration for this time step. Exception thrown if not a regular time step.
			 *
			 * \return	The regular step duration.
			 */

			time_duration GetRegularStepDuration() const;

			const ptime AddSteps(const ptime& startTimeStep, sec_type n) const;

			const ptime AddSteps(const ptime& startTimeStep, size_t n) const;

			const ptime AddSteps(const ptime& startTimeStep, int n) const;

			/**
			 * \fn	const ptime TimeStep::AddSteps(const ptime& startTimeStep, double mult) const;
			 *
			 * \brief	Adds a number of steps to an instant, which can be non-integer for regular time steps. Behavior TBC for irregular time steps.
			 *
			 * \param	startTimeStep	The start time step.
			 * \param	mult		 	The number of steps added.
			 *
			 * \return	A ptime.
			 */

			const ptime AddSteps(const ptime& startTimeStep, double mult) const;

			/**
			 * \fn	vector<ptime> TimeStep::AddSteps(const ptime& startTimeStep, const vector<double> timeSteps) const;
			 *
			 * \brief	Vectorized version of shifting time steps. 
			 * 			Adds a number of steps to an instant, which can be non-integer 
			 * 			for regular time steps. Behavior TBC for irregular time steps.
			 *
			 * \param	startTimeStep	The start time step.
			 * \param	timeSteps	 	The time steps.
			 *
			 * \return	A vector&lt;ptime&gt;
			 */

			vector<ptime> AddSteps(const ptime& startTimeStep, const vector<double> timeSteps) const;

			vector<ptime> AddSteps(const vector<ptime>& times, double mult) const;

			/**
			 * \fn	const time_duration TimeStep::GetTimeStepDuration(const ptime& startTimeStep) const;
			 *
			 * \brief	Given an instant, what is the next time instant according to this present Time
			 * 			step
			 *
			 * \param	startTimeStep	The start time step.
			 *
			 * \return	The time step duration.
			 */

			const time_duration GetTimeStepDuration(const ptime& startTimeStep) const;

			/**
			 * \fn	const ptrdiff_t TimeStep::GetUpperNumSteps(const ptime& start, const ptime& end) const;
			 *
			 * \brief	Gets the minimum number of time steps covering a time interval
			 *
			 * \param	start	The start of the time interval
			 * \param	end  	The end of the time interval
			 *
			 * \return	the number of steps needed to get beyond 'end'
			 */

			const ptrdiff_t GetUpperNumSteps(const ptime& start, const ptime& end) const;

			/**
			 * \fn	const ptrdiff_t TimeStep::GetNumSteps(const ptime& start, const ptime& end) const;
			 *
			 * \brief	Gets the maximum number of time steps from a starting instant to not get beyond an instant in time, 'end'
			 *
			 * \param	start	The start of the time interval
			 * \param	end  	The end of the time interval
			 *
			 * \return	the number of steps needed to not get beyond 'end'
			 */

			const ptrdiff_t GetNumSteps(const ptime& start, const ptime& end) const;
			const ptrdiff_t GetOffset(const ptime& start, const ptime& end) const;

			/**
			 * \fn	const void TimeStep::Increment(ptime* t) const;
			 *
			 * \brief	Increments an instant by one time step
			 *
			 * \param [in,out]	t	If non-null, the ptime to process.
			 */

			const void Increment(ptime* t) const;
			static TimeStep Parse(const string& name);
			static TimeStep FromSeconds(unsigned int seconds);

			static TimeStep GetDaily();
			static TimeStep GetHourly();
			static TimeStep GetMonthlyQpp();

			/**
			 * \fn	static TimeStep TimeStep::GetUnknown();
			 *
			 * \brief	Gets an instance of time step that is unknown. 
			 * 			This value is indented for use in methods as a default parameter value.
			 *
			 * \return	The "unknown" value of the time step
			 */

			static TimeStep GetUnknown();

			/**
			 * \fn	bool TimeStep::IsUnknown() const;
			 *
			 * \brief	Query if this object is the time step value "unknown".
			 *
			 * \return	true if unknown, false if not.
			 */

			bool IsUnknown() const;

			static ptime CreatePtime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0);
			static ptime PtimeFromIsoString(const string& t);

			static string ToString(const ptime& dt, const string& format = "YYYYMMDDThhmmss");

			string GetName() const;

		private:
			TimeStepImplementation* tsImpl = nullptr;

			void CopyTimeStepImplementation(const TimeStep& src);
			void CheckIsRegular(const string& op) const;

			static const time_duration hourlyTd;
			static const time_duration dailyTd;
			static bool FromGeneralStringPeriod(const string& name, TimeStep& tstep);

		};

		/**
		 * \class	TimeSeriesInfoProvider
		 *
		 * \brief	An interface definition for classes that can 
		 * 			provide essential time series temporal characteristics.
		 */
		class DATATYPES_DLL_LIB TimeSeriesInfoProvider
		{
		public:
			virtual ~TimeSeriesInfoProvider();
			virtual size_t GetLength() const = 0;
			virtual TimeStep GetTimeStep() const = 0;
			virtual ptime GetStart() const = 0;
		};
	}
}
