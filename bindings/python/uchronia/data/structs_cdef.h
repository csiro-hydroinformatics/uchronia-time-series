// namespace std
// {
// typedef long unsigned int size_t;
// typedef long int ptrdiff_t;
// typedef decltype(nullptr) nullptr_t;
// }
// namespace std
// {
// inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
// }
// namespace __gnu_cxx
// {
// inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
// }
// typedef long int ptrdiff_t;
// typedef long unsigned int size_t;
// typedef struct {
// long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
// long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
// } max_align_t;
// typedef decltype(nullptr) nullptr_t;
// namespace std
// {
// using ::max_align_t;
// }
typedef struct _date_time_to_second
{
int year;
int month;
int day;
int hour;
int minute;
int second;
} date_time_to_second;
typedef struct _named_values_vector
{
size_t size;
double* values;
char** names;
} named_values_vector;
typedef struct _regular_time_series_geometry
{
date_time_to_second start;
int time_step_seconds;
int length;
} regular_time_series_geometry;
typedef struct _multi_regular_time_series_data
{
regular_time_series_geometry time_series_geometry;
int ensemble_size;
double** numeric_data;
} multi_regular_time_series_data;
typedef struct _time_series_dimension_description
{
char* dimension_type;
size_t size;
} time_series_dimension_description;
typedef struct _time_series_dimensions_description
{
time_series_dimension_description* dimensions;
int num_dimensions;
} time_series_dimensions_description;
