#pragma once
#include <stdio.h>
#include <atomic>
#include <string> 
#include <vector>
#include <map> 
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>

#ifdef _WIN32
  #ifdef USING_DATATYPES
    #define DATATYPES_DLL_LIB __declspec(dllimport)
	#define TEMPLATE_SPECIALIZATION_EXTERN extern
  #else
    #define DATATYPES_DLL_LIB __declspec(dllexport)
	#define TEMPLATE_SPECIALIZATION_EXTERN
  #endif
#else
  #define DATATYPES_DLL_LIB // nothing
  #define TEMPLATE_SPECIALIZATION_EXTERN
#endif

#define DEFAULT_MISSING_DATA_VALUE -9999.0
#define DEFAULT_STATION_IDENTIFIER { "0" }

// You can define the following to activate the Visual Leak Detector tool for visual C++
// https://vld.codeplex.com
//#define DATATYPES_USE_VLD

#ifdef DATATYPES_USE_VLD
  #include<vld.h>
#endif

#ifdef _WIN32
  #define STRDUP _strdup
#else
  #define STRDUP strdup
#endif

#define STLSTR_TO_ANSISTR(x) STRDUP(x.c_str())

using std::string;
using std::vector;
using std::map;
using std::pair;

#define DATATYPES_DOUBLE_PRECISION_ID "double"
#define DATATYPES_SINGLE_PRECISION_ID "single"

#define DATATYPES_FOUR_DIMENSIONS_DATA "4"
#define DATATYPES_THREE_DIMENSIONS_DATA "3"
#define DATATYPES_TWO_DIMENSIONS_DATA "2"

#define SWIFT_TIME_UNIT_MINUTE string("minute")
#define SWIFT_TIME_UNIT_HOUR string("hour")
#define SWIFT_TIME_UNIT_DAY string("day")

namespace datatypes
{
	namespace utils
	{
		class STLHelper
		{
		private:
			STLHelper();

			template <typename U>
			class Comparer
			{
				std::function<bool(const U&, const U&)>& valueCompare;
			public:
				Comparer(std::function<bool(const U&, const U&)>& valueCompare) : valueCompare(valueCompare)
				{
				}
				typedef typename vector<U>::const_iterator const_iterator;
				bool operator()(
					const std::pair<size_t, const_iterator>& a,
					const std::pair<size_t, const_iterator>& b)
				{
					return valueCompare(*(a.second), *(b.second));
				}
			};


		public:
			template<typename K = string, typename V = string>
			static bool HasKey(const map<K, V>& dict, const string& key)
			{
				return(dict.find(key) != dict.end());
			}

			template<typename K = string, typename V = string>
			static vector<K> GetKeys(const map<K, V>& dict)
			{
				vector<K> res;
				res.reserve(dict.size());
				for (auto& kvpair : dict)
					res.push_back(kvpair.first);
				return res;
			}

			template<typename K = string, typename V = string>
			static map<K, V> Remap(const map<K, V>& dict, const map<K, K>& newKeys)
			{
				map<K, V> res;
				for (const auto& kvpair : dict)
				{
					K newKey = newKeys.at(kvpair.first);
					res[newKey] = kvpair.second;
				}
				return res;
			}

			template<typename K = string, typename V = string>
			static vector<K> GetValues(const map<K, V>& dict)
			{
				vector<V> res;
				res.reserve(dict.size());
				for (auto& kvpair : dict)
					res.push_back(kvpair.second);
				return res;
			}

			template<typename K = string, typename V = string>
			static vector<V> GetValues(const map<K, V>& dict, const vector<K>& keys)
			{
				vector<V> res;
				res.reserve(keys.size());
				for (auto& k : keys)
					res.push_back(dict.at(k));
				return res;
			}

			template <typename U>
			static bool LessThan(const U& first, const U& second)
			{
				return first < second;
			}

			template <typename U>
			static bool MoreThan(const U& first, const U& second)
			{
				return first > second;
			}

			template <typename K, typename V>
			static vector<V> SortValues(
				const std::map<K,V>& in,
				const vector<K>& order)
			{
				vector<V> result;
				for(auto& k : order)
				{ 
					if (!HasKey(in, k))
						throw std::logic_error("No entry in std::map for key " + k);
					result.push_back(in.at(k));
				}
				return result;
			}

			template <typename T>
			static vector<T> Serialize(const vector<vector<T>>& series)
			{
				vector<T> result;
				for (size_t i = 0; i < series.size(); i++)
				{
					auto& v = series[i];
					auto n = result.size();
					auto m = v.size();
					result.resize(n + m);
					std::copy(v.begin(), v.end(), result.begin() + n);
				}
				return result;
			}

			template <typename T, typename U>
			static vector<T> SortFromRef(
				const vector<T>& in,
				const vector<U>& reference,
				std::function<bool(const U&, const U&)> comparer = STLHelper::LessThan<U>)
			{
				// Attribution: derived from http://stackoverflow.com/a/236199/2752565, Konrad Rudolph

				typedef typename vector<U>::const_iterator myiter;
				vector<std::pair<size_t, myiter>> order(reference.size());

				size_t n = 0;
				for (myiter it = reference.begin(); it != reference.end(); ++it, ++n)
					order[n] = std::make_pair(n, it);


				Comparer<U> ordering(comparer);

				sort(order.begin(), order.end(), ordering);

				vector<T> ret(in.size());

				size_t const size = in.size();
				for (size_t i = 0; i < size; ++i)
					ret[i] = in[order[i].first];

				return ret;
			}
		};

		template<typename T = double>
		vector<T>  SeqVec(T from, T by, int num)
		{
			vector<T> data;
			data.resize(num);
			for (size_t i = 0; i < num; i++)
				data[i] = from + i*by;
			return data;
		}

		// Could not find an easy if_then_else in the STL or Boost. IfThenElse will probably be replaced.
		// primary template: yield second or third argument depending on first argument
		template<bool C, typename Ta, typename Tb>
		class IfThenElse;
		// Credits:
		// C++ templates : the complete guide / David Vandevoorde, Nicolai M. Josuttis.
		//				ISBN 0 - 201 - 73484 - 2 (alk.paper)

		// partial specialization: true yields second argument
		template<typename Ta, typename Tb>
		class IfThenElse < true, Ta, Tb > {
		public:
			typedef Ta ResultT;
		};

		// partial specialization: false yields third argument
		template<typename Ta, typename Tb>
		class IfThenElse < false, Ta, Tb > {
		public:
			typedef Tb ResultT;
		};

		template <typename T>
		class ValueTypeVectorDispose
		{
		public:
			static void Dispose(vector<T>& a) { a.clear(); }
		};

		template <typename T>
		class PointerTypeVectorDispose
		{
		public:
			static void Dispose(vector<T>& a)
			{
				for (size_t i = 0; i < a.size(); i++)
				{
					auto p = a[i];
					if (p != nullptr)
						delete p;
				}
				a.clear();
			}
		};

		template <typename T>
		struct DisposeVectorTypeFactory {
			typedef typename IfThenElse<
				std::is_pointer<T>::value,
				ValueTypeVectorDispose<T>,
				PointerTypeVectorDispose<T>
			>::ResultT type;
		};


		template <typename T>
		void DisposeVector(vector<T>& v)
		{
			using U = typename datatypes::utils::DisposeVectorTypeFactory<T>::type;
			U::Dispose(v);
		}

		class DATATYPES_DLL_LIB StringProcessing
		{
		public:
			static vector<string> Split(const string& s, const string& separators);
			static string TrimAny(const string& s, const string& charactersToTrim);
			static vector<string> SplitOnSpaces(const string& s, bool removeEmptyEntries);
			static vector<string> RemoveEmpty(const vector<string>& s);
			//			static bool Contains(const string& toTest, const vector<string>& toMatch, bool caseSensitive = true);
			static bool Contains(const string& toTest, const string& toMatch, bool caseSensitive = true);
			static bool StartsWith(const string& toTest, const string& toMatch, bool caseSensitive = true);
			static bool Equals(const string& toTest, const string& toMatch, bool caseSensitive = true);
			static bool EqualsAny(const string& toTest, const vector<string>& toMatch, bool caseSensitive = true);
			static bool StringPredicate(const string& toTest, const string& toMatch, boost::function<bool(const string&, const string&)> predicate, bool caseSensitive = true);
			static string Paste(const vector<string>& items, const string& sep = "");
			static vector<string> Paste(const string& prefix, const vector<string>& postfixes);
			static bool ContainsAll(const vector<string>& toTest, const vector<string>& toMatch, bool caseSensitive = true);
			static bool SetEquals(const vector<string>& toTest, const vector<string>& toMatch, bool caseSensitive = true);
			static vector<string> SetDiff(const vector<string>& toTest, const vector<string>& toRemove, bool caseSensitive = true);
			static vector<string> Unique(const vector<string>& set);

			template <typename Target>
			static Target Parse(const string& strId)
			{
				return boost::lexical_cast<Target>(strId);
			}

			template <typename Target>
			static string ToString(const Target& value)
			{
				return boost::lexical_cast<string>(value);
			}

		private:
			static bool funContains(const string& toTest, const string& toMatch);
			static bool funStartsWith(const string& toTest, const string& toMatch);
			static bool funEquals(const string& toTest, const string& toMatch);
		};

	}
}