#pragma once
#include <string> 
#include <boost/lexical_cast.hpp>
#include <stdexcept> 
#include "common.h"

using namespace std;

namespace datatypes
{
	namespace exceptions
	{
		class ExceptionUtilities
		{
		public:
			static void ThrowInvalidArgument(const string& msg) { throw invalid_argument(msg); }
			static void ThrowInvalidOperation(const string& msg) { throw logic_error(msg); }
			static void ThrowInvalidArgumentModelVariableId(const string& variableId) { throw invalid_argument(string("Unknown model variable identifier: ") + variableId); }
			static void ThrowNotImplemented(const string& msg) { throw logic_error(msg); }
			static void ThrowOutOfRange(const string& msg) { throw out_of_range(msg); }
			template <typename T>
			static void CheckInRange(T value, T min, T max, const string& variableName) {
				if (value >= min && value <= max)
					return;
				else
				{
					if (value < min)
						ThrowNotInRange(value, min, variableName, "less", "minimum");
					if (value > max)
						ThrowNotInRange(value, max, variableName, "more", "maximum");
				}
			}
		private:
			template <typename T>
			static void ThrowNotInRange(T value, T bound, const string& variableName, const string& condition, const string& boundType) {
				string valStr = boost::lexical_cast<string,T>(value);
				ExceptionUtilities::ThrowOutOfRange(string("variable '") + variableName + "' (=" + valStr + ") is " + condition +
					" than its allowed " + boundType + " value " + boost::lexical_cast<string,T>(bound));
			}
		};
	}
}
