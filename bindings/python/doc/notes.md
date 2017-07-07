

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