#include <string>
#include <vector>
#include "uchronia_struct_interop.h"

using std::string;
using std::vector;

void delete_ansi_string_array(char** values, int arrayLength)
{
	DeleteAnsiStringArray(values, arrayLength);
}

void delete_array(double* values)
{
	DeleteDoubleArray(values);
}

double checkedNumVecElement(const Rcpp::NumericVector& v, const std::string& name)
{
	std::vector<string> names = v.names();
	auto m = std::find(names.begin(), names.end(), name);
	if(m == names.end())
		throw std::invalid_argument(string("Name not found in the 'names' of the numeric vector: ") + name);
	return as<double>(v[name]);
}
