#pragma once

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <datatypes/exception_utilities.h>

using namespace boost::posix_time;
using namespace boost::gregorian;

namespace datatypes
{
	namespace timeseries
	{

		// NOTE: added DATATYPES_DLL_LIB to try to resolve https://jira.csiro.au/browse/WIRADA-415

		class DATATYPES_DLL_LIB TimeStepImplementation
		{
		public:
			virtual ~TimeStepImplementation();

			static void CheckIsDateTime(const ptime& instant);

			virtual TimeStepImplementation* Clone() = 0;

			virtual TimeStepImplementation* Multiply(int mult) const = 0;
			virtual TimeStepImplementation* Divide(int divisor) const = 0;
			virtual TimeStepImplementation* Multiply(double mult) const = 0;
			virtual bool Equals(TimeStepImplementation* tsImpl) const = 0;

			virtual const ptime AddIntSteps(const ptime& startTimeStep, int n) const = 0;
			virtual const ptime AddSteps(const ptime& startTimeStep, double mult) const = 0;

			virtual const time_duration GetTimeStepDuration(const ptime& startTimeStep) const = 0;
			virtual const void Increment(ptime* t) const = 0;

			virtual const ptrdiff_t GetUpperNumSteps(const ptime& start, const ptime& end) const;
			virtual const ptrdiff_t GetNumSteps(const ptime& start, const ptime& end) const;
			virtual const ptrdiff_t GetOffset(const ptime& start, const ptime& end) const;

			virtual bool IsRegular() const = 0;
			virtual time_duration GetRegularStepDuration() const = 0;
			virtual std::string GetName() const = 0;

		protected:
			virtual const double GetLinearIndexing(const ptime& start, const ptime& end) const = 0;
		};

		class DATATYPES_DLL_LIB RegularTimeStepImplementation :
			public TimeStepImplementation
		{
		public:
			static RegularTimeStepImplementation* GetHourly();
			static RegularTimeStepImplementation* GetDaily();
			
			RegularTimeStepImplementation(const time_duration& stepDuration);
			virtual ~RegularTimeStepImplementation();
			
			TimeStepImplementation* Clone();
			bool Equals(TimeStepImplementation* tsImpl) const;

			TimeStepImplementation* Multiply(int mult) const;
			TimeStepImplementation* Multiply(double mult) const;
			TimeStepImplementation* Divide(int divisor) const;

			const ptime AddIntSteps(const ptime& startTimeStep, int n) const;
			const ptime AddSteps(const ptime& startTimeStep, double mult) const;
			const time_duration GetTimeStepDuration(const ptime& startTimeStep) const;
			const void Increment(ptime* t) const;

			bool IsRegular() const;
			time_duration GetRegularStepDuration() const;
			std::string GetName() const;

		protected:
			const double GetLinearIndexing(const ptime& start, const ptime& end) const;

		private:
			static const time_duration hourlyTd;
			static const time_duration dailyTd;
			
			time_duration multiply(double mult) const;

			time_duration * regularStep = nullptr;

			// void CopyRegularImplementation(const RegularTimeStepImplementation& src);
			// const ptrdiff_t GetUpperNumInstants(const ptime& start, const ptime& end) const;
			// const ptrdiff_t GetNumInstants(const ptime& start, const ptime& end) const;
		};

		class DATATYPES_DLL_LIB IrregularTimeStepImplementation :
			public TimeStepImplementation
		{
		public:
			virtual ~IrregularTimeStepImplementation() {}

			virtual TimeStepImplementation* Clone() = 0;
			virtual bool Equals(TimeStepImplementation* tsImpl) const = 0;

			TimeStepImplementation* Multiply(int mult) const;
			TimeStepImplementation* Multiply(double mult) const;
			TimeStepImplementation* Divide(int divisor) const;

			virtual const ptime AddIntSteps(const ptime& startTimeStep, int n) const = 0;
			virtual const ptime AddSteps(const ptime& startTimeStep, double mult) const = 0;
			virtual const time_duration GetTimeStepDuration(const ptime& startTimeStep) const;
			virtual const void Increment(ptime* t) const = 0;

			bool IsRegular() const;
			time_duration GetRegularStepDuration() const;
			virtual std::string GetName() const = 0;

		protected:
			virtual const double GetLinearIndexing(const ptime& start, const ptime& end) const = 0;
		};

		class DATATYPES_DLL_LIB MonthlyQppTimeStepImplementation :
			public IrregularTimeStepImplementation
		{
		public:
			virtual ~MonthlyQppTimeStepImplementation() {}

			TimeStepImplementation* Clone() override;
			bool Equals(TimeStepImplementation* tsImpl) const override;

			const ptime AddIntSteps(const ptime& startTimeStep, int n) const override;
			const ptime AddSteps(const ptime& startTimeStep, double mult) const override;
			const time_duration GetTimeStepDuration(const ptime& startTimeStep) const override;
			const void Increment(ptime* t) const override;

			std::string GetName() const override;

		protected:
			const double GetLinearIndexing(const ptime& start, const ptime& end) const override;
		};
	}
}