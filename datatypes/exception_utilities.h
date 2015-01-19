#pragma once
#include <string> 
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
				if (value < min) ExceptionUtilities::ThrowOutOfRange(string("variable '") + variableName + "' is less than the minimum value");
				if (value > max) ExceptionUtilities::ThrowOutOfRange(string("variable '") + variableName + "' is more than the maximum value");
			}
		};
	}
}
