library(devtools)

root_src <- 'c:/src/csiro/stash'
root_os_src <- 'c:/src/github_jm'

load_all(file.path(root_src, 'c-api-bindings/codegen/capimatlabgen'))
load_all(file.path(root_src, 'c-api-bindings/codegen/capigen'))
wgenDir = 'c:/src/github_jm/c-api-wrapper-generation'
list.files(wgenDir)
load_wrapper_gen_lib(wgenDir = wgenDir)

# Adapting from capigen:

cppGen <- rClr::clrNew('ApiWrapperGenerator.CppApiWrapperGenerator')
# rClr::clrSet (cppGen, 'OpaquePointers', TRUE)
rClr::clrSet (cppGen, 'DeclarationOnly', FALSE)
rClr::clrSet (cppGen, 'FunctionNamePostfix', '_cpp')
rClr::clrSet (cppGen, 'OpaquePointerClassName', "opaque_pointer_handle")


## Duplicates from capigen::create_rcpp_generator_base
# Converters for datatypes (uchronia)
# What python equivalent could we set up for const Rcpp::S4& ?
# Note https://github.com/tdegeus/pyxtensor
# Also start by giving a go with conda install -c conda-forge xtensor-python   in the conda uchronia environment


  set_wrapper_type_map(cppGen, 'regular_time_series_geometry*', 'regular_time_series_geometry&')
  set_wrapper_type_converter(cppGen, 'regular_time_series_geometry*', '_tsgeom', 
    'regular_time_series_geometry* C_ARGNAME = &RCPP_ARGNAME;', 
    '// no delection for C_ARGNAME;')

  set_wrapper_type_map(cppGen, 'TS_GEOMETRY_PTR', 'regular_time_series_geometry&')
  set_wrapper_type_converter(cppGen, 'TS_GEOMETRY_PTR', '_tsgeom', 
    'regular_time_series_geometry* C_ARGNAME = &RCPP_ARGNAME;', 
    '// no delection for C_ARGNAME;')

  set_wrapper_type_map(cppGen, 'const multi_regular_time_series_data*', 'multi_regular_time_series_data&')
  set_wrapper_type_converter(cppGen, 'const multi_regular_time_series_data*', '_tsd_ptr', 
    'multi_regular_time_series_data* C_ARGNAME = &RCPP_ARGNAME;', 
    '// no delection for C_ARGNAME;')



prepend_impl<- '
// This file was GENERATED
//Do NOT modify it manually, as you are very likely to lose work

#include "../include/uchronia_pb_exports.h"
#include "../include/cpp_bindings_generated.h"

'

rClr::clrSet (cppGen, 'PrependOutputFile', prepend_impl)

cppGen <- configure_cpp_typemap(cppGen)

api_filter <- create_uchronia_api_filter()
#clrSet(api_filter, 'ContainsNone', character(0))
(blacklist <- clrGet(api_filter, 'ContainsNone'))

blacklist <- c(
"DeleteAnsiStringArray",
"DeleteAnsiString",
"DisposeMultiTimeSeriesData",
"GetDataDimensionsDescription",
"DisposeDataDimensionsDescriptions",
'DeleteDoubleArray',
'GetTimeSeriesValues', 
'GetProviderTimeSeriesValues')

clrSet(api_filter, 'ContainsNone', blacklist)


gen <- create_wrapper_generator(cppGen, api_filter)

# swiftr_include_dir <- file.path( pkg_path_swiftr(swiftSrcPath), 'inst/include')
# if(!dir.exists(swiftr_include_dir)) stop(paste0(swiftr_include_dir, ' not found'))

out_root_dir <- 'C:/src/csiro/stash/datatypes/bindings/python/uchronia_pb/src'

outfile_cpp <- file.path(out_root_dir, 'cpp_bindings_generated.cpp')
outfile_h <- file.path(out_root_dir, '..', 'include', 'cpp_bindings_generated.h')

infile <- extern_c_api_header_file_uchronia_r()
if(!file.exists(infile)) stop(paste0(infile, ' not found'))

generate_wrapper_code(gen, infile, outfile_cpp)

rClr::clrSet (cppGen, 'DeclarationOnly', TRUE)

prepend_header<- '
// This file was GENERATED
//Do NOT modify it manually, as you are very likely to lose work

#include <string>
#include <vector>
#include "moirai/opaque_pointers.hpp"
using moirai::opaque_pointer_handle;
#include "cinterop/c_cpp_interop.hpp"
#include "cinterop/timeseries_c_interop.h"
'

rClr::clrSet (cppGen, 'PrependOutputFile', prepend_header)
generate_wrapper_code(gen, infile, outfile_h)

#####################################################
# Adapting C:\src\csiro\stash\swift\bindings\matlab\create_simple_header_files.r
#####################################################

cinterop_dir <- file.path(root_os_src, 'rcpp-interop-commons/include/cinterop')
wila_dir <- file.path(root_os_src, 'wila/include/wila')

struct_ptrs_patterns <- c(
  'OptimizerLogData\\*', 
  'NodeInfo\\*', 
  'LinkInfo\\*', 
  'SubareaInfo\\*', 
  'NodeLinkConnection\\*', 
  'SubareaLinkConnection\\*', 
  'SceParameters\\*', 
  'CatchmentStructure\\*'
)


datatypes_fcts <- get_api_functions(
  file.path(root_src, 'datatypes/include/datatypes/extern_c_api.h'),
  api_marker='DATATYPES_API')
datatypes_fcts <- to_opaque_pointers( api_fcts=datatypes_fcts, non_opaque_ptrs_patterns=struct_ptrs_patterns)

x <- stringr::str_split(datatypes_fcts, ' |\\(')
get_second <- function(s) {s[2]}
datatypes_func_names <- sapply(x, get_second)

f <- datatypes_func_names

# todo: what was the package at user2018? glue?
pybind_datatypes_func_names <- paste0(
  '    m.def("', f, '", &', f, '_cpp, R"pbdoc( TODO doc for ', f, ')pbdoc");'
)

out_dir <- 'c:/tmp/'

write_simple(pybind_datatypes_func_names, 'pybind_def_file.cpp', directory=out_dir)




