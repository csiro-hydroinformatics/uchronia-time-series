#pragma once
#include <string> 
#include <stdexcept> 
#include <boost/filesystem.hpp>
#include "datatypes/io_helper.h"

using std::string;
using std::vector;
using std::map;
using std::pair;

namespace datatypes
{
	namespace exceptions
	{
		template <typename T>
		static void ThrowNotInRange(T value, T bound, const string& variableName, const string& condition, const string& boundType) {
			throw std::out_of_range(string("variable '") + variableName + "' (=" +
				datatypes::utils::ToString<T>(value) + ") is " + condition +
				" than its allowed " + boundType + " value " + 
				datatypes::utils::ToString<T>(bound));
		}

		template <typename T>
		struct RangeCheck {
			static void Check(T value, T min, T max, const string& variableName) {
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
		};

		template <>
		struct RangeCheck<size_t> {
			static size_t MaxIndexing()
			{
				return (std::numeric_limits<size_t>::max() - 2);
			}

			static void Check(size_t value, size_t min, size_t max, const string& variableName)
			{
				if ((min > MaxIndexing()) || (max > MaxIndexing()))
					throw std::logic_error("Range check is restricted to values less than <size_t>::max()-2");
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

			static void CheckTimeSeriesInterval(const size_t& from, size_t& to, const size_t& tsLength)
			{
				if (tsLength == 0)
					throw std::out_of_range("Cannot have a valid index specified for an empty time series");
				size_t maxIndex = tsLength - 1;
				if (to > MaxIndexing())
					to = std::min(to, maxIndex);
				Check(from, 0, maxIndex, "from");
				Check(to, 0, maxIndex, "to");
			}

			static void CheckTimeSeriesIndex(const size_t& index, const size_t& tsLength, const string& variableName="index")
			{
				if (tsLength == 0)
					throw std::out_of_range("Cannot have a valid index specified for an empty time series");
				size_t maxIndex = tsLength - 1;
				Check(index, 0, maxIndex, variableName);
			}

		};

		class ExceptionUtilities
		{
		public:
			static void ThrowInvalidArgument(const string& msg = "Invalid argument") { throw std::invalid_argument(msg); }
			static void ThrowInvalidOperation(const string& msg = "Invalid operation") { throw std::logic_error(msg); }
			static void ThrowInvalidArgumentModelVariableId(const string& variableId) { throw std::invalid_argument(string("Unknown model variable identifier: ") + variableId); }
			static void ThrowNotImplemented(const string& msg = "Not implemented") { throw std::logic_error(msg); }
			static void ThrowNotSupported(const string& typeName, const string& methodName) { throw std::logic_error("Type " + typeName + " does not support method " + methodName); }
			static void ThrowNotSupported(const string& msg) { throw std::logic_error(msg); }
			static void ThrowOutOfRange(const string& msg = "Operation led to a state out of range") { throw std::out_of_range(msg); }
			template <typename T>
			static void CheckInRange(T value, T min, T max, const string& variableName) {
				RangeCheck<T>::Check(value, min, max, variableName);
			}

			static void CheckFileExists(const boost::filesystem::path& p) {
				if (!boost::filesystem::exists(p))
					throw std::invalid_argument(string("Path does not exist: ") + p.generic_string());
				if (!boost::filesystem::is_regular_file(p))
					throw std::invalid_argument(string("Path exists but is not a regular file: ") + p.generic_string());
			}

			static void CheckFileDoesNotExist(const boost::filesystem::path& p) {
				if (boost::filesystem::exists(p))
					throw std::invalid_argument(string("Path already exists: ") + p.generic_string());
			}

			static void CheckExists(const boost::filesystem::path& p) {
				if (!boost::filesystem::exists(p))
					throw std::invalid_argument(string("Path does not exist: ") + p.generic_string());
			}
		};
	}
}