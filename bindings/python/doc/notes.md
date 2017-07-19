
# Overview

As of 2017-07-10 the following high level calls are possible to generate/update the low-level glue code for python (and R). The R codegen is working and stable; Python codegen is a work in progress.

## Dependencies

* [capigen](https://bitbucket.csiro.au/users/per202/repos/c-api-bindings/browse) is an ongoing refactor of the legacy package 'swiftdev',
* which loads and runs some [C# based code generation](https://github.com/jmp75/rcpp-wrapper-generation),
* re-using [rClr](https://github.com/jmp75/rClr) for this mix of R and .net.

## Generating code:

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

# Diving into more details

This section is a legacy capture of steps leading to the above.

## Preprocessing C API files to extract C code suitable for CFFI

```r
library(capigen)

uchronia_root <- "/home/per202/src/csiro/stash/per202/datatypes"
include_dirs <- file.path(uchronia_root, "include/datatypes")
api_importer_file <- file.path(uchronia_root, "bindings/codegen/api_importer.cpp")
uchronia_py_pkg_dir <- file.path(uchronia_root, "bindings/python/uchronia")
uchronia_cdefs_dir <- file.path(uchronia_root, "bindings/python/uchronia/data")
preprocessed_cpp_file <- "~/tmp/uchronia_c_api.cpp"

preprocess_result <- apply_c_preprocessor(include_dirs, api_importer_file, preprocessed_cpp_file)
create_cffi_cdefs(preprocessed_cpp_file, outdir=uchronia_cdefs_dir, pattern_start_structs="interop_struct.h")
```

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