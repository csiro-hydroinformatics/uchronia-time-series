
# Python bindings using codegen

## Working notes

Jan 2019

Moving to use [refcount](https://pypi.org/project/refcount/) for managing c++ objects. Using bindings\python\uchronia\tests\test_low_level.py as a start. Got to the point where I can set the lowest level. Now time to get the equivalent of R's `cinterop.mkExternalObjRef(result, 'ENSEMBLE_DATA_SET_PTR')` working - for now there is no cinterop package. Also:
    * `values_charpp = to_c_char_ptrptr(values)` which is more equivalent to c++. 

Should I have a cinterop package? probably,  because some data marshalling has nothing to do with refcount. However, the wrapping/unwrapping using refcount maybe should not be in a cinterop package, but refcount itself. New release, too...

### pybind11 testbed

Code generation pointers for pybind11:  [pybind11 issue 252](https://github.com/pybind/pybind11/issues/252)  [pybind11 issue 553](https://github.com/pybind/pybind11/issues/553)  . Also [https://github.com/ranr01/pybind11-code-generator](https://github.com/ranr01/pybind11-code-generator)

[binder](https://pybind11.readthedocs.io/en/master/compiling.html#binder) is mentioned by the pybind11 doc. Worth a look.

## Dependencies

* [capigen](https://bitbucket.csiro.au/users/per202/repos/c-api-bindings/browse) is an ongoing refactor of the legacy package 'swiftdev',
* which loads and runs some [C# based code generation](https://github.com/jmp75/rcpp-wrapper-generation),
* re-using [rClr](https://github.com/jmp75/rClr) for this mix of R and .net.

## Generating code:

As of 2017-07-10 the following high level calls are possible to generate/update the low-level glue code for python (and R). The R codegen is working and stable; Python codegen is a work in progress.

### Preprocessing C API files to extract C code suitable for CFFI

The R package `capigen` has some facilities to help extract C functions and struct definitions for Pytnon interop via CFFI. For now this is not fully streamlined but a lot of tedium is already hidden in functions. For now:

```r
library(capigen)


if(Sys.info()['sysname'] == 'Windows') {
    uchronia_root <- "c:/src/csiro/stash/datatypes"
    preprocessed_cpp_file <- "c:/tmp/uchronia_c_api.cpp"
} else {
    uchronia_root <- "/home/per202/src/csiro/stash/per202/datatypes"
    preprocessed_cpp_file <- "~/tmp/uchronia_c_api.cpp"
}

include_dirs <- file.path(uchronia_root, "include/datatypes")
if(Sys.info()['sysname'] == 'Windows') {
    include_dirs <- c(include_dirs, 'c:/local/include')
    libp <- Sys.getenv("PATH")
    p <- 'C:/Rtools/mingw_64/bin/'
    libp <- paste(p, libp, sep = ";")
    Sys.setenv(PATH = libp)
}
Sys.which('gcc')

api_importer_file <- file.path(uchronia_root, "bindings/codegen/api_importer.cpp")
uchronia_py_pkgs_dir <- file.path(uchronia_root, "bindings/python/uchronia")
uchronia_cdefs_dir <- file.path(uchronia_py_pkgs_dir, "uchronia/data")

preprocess_result <- apply_c_preprocessor(include_dirs, api_importer_file, preprocessed_cpp_file)
create_cffi_cdefs(preprocessed_cpp_file, outdir=uchronia_cdefs_dir, pattern_start_structs="typedef struct _date_time_to_second", extern_c_start_match='char.+GetLastStdExceptionMessage.*' , extern_c_end_match='^\\}')
```

Then we can generate bindings including the python wrappers.

```r
library(capigen)
load_wrapper_gen_lib('c:/src/github_jm/rcpp-wrapper-generation')
uchroniaSrcPath <- 'c:/src/csiro/stash/datatypes'

# For R:
generate_uchronia_r_all_wrappers(uchroniaSrcPath)
roxy_uchronia()

# for Python
generate_uchronia_python_all_wrappers(uchroniaSrcPath)
```

# Installing the packge

```bat
python setup.py build_ext --inplace
python setup.py develop --install-dir c:\local\python-pkgs
```

# Getting started

```py

```

# Diving into more details

This section is a legacy capture of steps leading to the above.

## A minimal, low level test

```py
import sys
from cffi import FFI
uchronia_ffi = FFI()
import os
uchronia_src_dir = '/home/per202/src/csiro/stash/per202/datatypes'
cdefs_dir = os.path.join(uchronia_src_dir, 'bindings/python/uchronia/data')
with open(os.path.join(cdefs_dir, 'structs_cdef.h')) as f_headers:
    uchronia_ffi.cdef(f_headers.read()) 

with open(os.path.join(cdefs_dir, 'funcs_cdef.h')) as f_headers:
    uchronia_ffi.cdef(f_headers.read()) 

uchronia_so = uchronia_ffi.dlopen( 'libdatatypes.so', 1)
```