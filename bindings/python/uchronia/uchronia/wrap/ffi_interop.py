"""
    Wrapper around the uchronia C API functions using CFFI.
"""
from functools import wraps
import sys
from cffi import FFI
import os

from refcount.interop import *
from refcount.putils import *

from cinterop.cffi.marshal import *

# TODO: consider refactoring to refcount?
def build_new_path_env (from_env='LIBRARY_PATH', to_env='PATH'):
    startup_msg = ''
    if(sys.platform == 'win32'):
        path_sep = ';'
        shared_lib_paths = os.environ.get(from_env)
        if(shared_lib_paths is not None):
            # startup_msg = appendstartup_msg(paste0('Found env var ', from_env, '=', shared_lib_paths), startup_msg)
            arch = os.environ["PROCESSOR_ARCHITECTURE"]
            if arch == 'AMD64':
                subfolder = '64'
            else:
                subfolder = '32'
            shared_lib_paths_vec = shared_lib_paths.split(path_sep)
            return prepend_path_env(shared_lib_paths_vec, subfolder, to_env=to_env)

def update_path_windows (from_env='LIBRARY_PATH', to_env='PATH'):
    if(sys.platform == 'win32'):
        os.environ[to_env] = build_new_path_env(from_env, to_env)

uchronia_ffi = FFI()
here = os.path.abspath(os.path.dirname(__file__))
uchronia_pkg_dir = os.path.join(here,'..')
cdefs_dir = os.path.join(uchronia_pkg_dir, 'data')
assert os.path.exists(cdefs_dir)

with open(os.path.join(cdefs_dir, 'structs_cdef.h')) as f_headers:
    uchronia_ffi.cdef(f_headers.read())

with open(os.path.join(cdefs_dir, 'funcs_cdef.h')) as f_headers:
    uchronia_ffi.cdef(f_headers.read())

update_path_windows()
short_fname = library_short_filename('datatypes')
# TODO is there a concrete use case to search custom paths and not let dlopen do its default??
# long_fname = find_first_full_path(short_fname, "uchronia")
long_fname = short_fname
uchronia_so = uchronia_ffi.dlopen(long_fname, 1) # Lazy loading

marshal = CffiMarshal(uchronia_ffi)
class UchroniaError(Exception):
    """ Exception when calling a uchronia function. """
    def __init__(self, message):
        super(UchroniaError, self).__init__(message)


# This will store the exception message raised by uchronia
_EXCEPTION_TXT_RAISED = None

@uchronia_ffi.callback("void(char *)")
def exception_callback(exception_string):
    """
        This function is called when uchronia raises an exception.
        It sets the global variable ``_EXCEPTION_TXT_RAISED``

        :param cdata exception_string: Exception string.
    """
    global _EXCEPTION_TXT_RAISED
    _EXCEPTION_TXT_RAISED = uchronia_ffi.string(exception_string)


def check_exceptions(func):
    """
        Returns a wrapper that raises a Python exception if a uchronia exception
        occured.
    """
    @wraps(func)
    def wrapper(*args, **kwargs):
        """
            This decorator will first call the function ``func``
            After that it will raise a Python UchroniaError exception if the
            global variable ``_EXCEPTION_TXT_RAISED`` is set.

            :param func func: Python function wrapping a uchronia function.
        """
        # log_func_call(func, *args, **kwargs)
        # Call the function
        return_value = func(*args, **kwargs)
        # Check if an exception was raised
        global _EXCEPTION_TXT_RAISED
        if _EXCEPTION_TXT_RAISED is not None:
            temp_exception = _EXCEPTION_TXT_RAISED
            _EXCEPTION_TXT_RAISED = None
            raise UchroniaError(temp_exception)
        return return_value
    return wrapper


uchronia_so.RegisterExceptionCallback(exception_callback)


@check_exceptions
def uchronia_dispose_multi_time_series_data(data):
    """
        :param ptr data: Pointer to a MultiTimeSeriesData.
    """
    uchronia_so.DisposeMultiTimeSeriesData(data)

