"""
    Wrapper around the uchronia C API functions using CFFI.
"""
from functools import wraps
from cffi import FFI
import os

from typing import List, Dict, Any
from refcount.putils import library_short_filename, update_path_windows
import pandas as pd

from refcount.interop import OwningCffiNativeHandle, CffiNativeHandle
from cinterop.cffi.marshal import CffiMarshal


uchronia_ffi = FFI()
here = os.path.abspath(os.path.dirname(__file__))
uchronia_pkg_dir = os.path.join(here,'..')
cdefs_dir = os.path.join(uchronia_pkg_dir, 'data')
assert os.path.exists(cdefs_dir)

with open(os.path.join(cdefs_dir, 'structs_cdef.h')) as f_headers:
    uchronia_ffi.cdef(f_headers.read())

with open(os.path.join(cdefs_dir, 'funcs_cdef.h')) as f_headers:
    uchronia_ffi.cdef(f_headers.read())

short_fname = library_short_filename('datatypes')
update_path_windows(from_env='LIBRARY_PATH', to_env='PATH')
# TODO is there a concrete use case to search custom paths and not let dlopen do its default??
# long_fname = find_first_full_path(short_fname, "uchronia")
long_fname = short_fname
uchronia_so = uchronia_ffi.dlopen(long_fname, uchronia_ffi.RTLD_LAZY) # Lazy loading

marshal = CffiMarshal(uchronia_ffi)
class UchroniaError(Exception):
    """ Exception when calling a uchronia function. """
    def __init__(self, message):
        super(UchroniaError, self).__init__(message)


# This will store the exception message raised by uchronia
_exception_txt_raised_uchronia = None

class NativeException(Exception):
    """ Raised when a call to a native library raised an error via a python callback function """
    def __init__(self, message):
        super(NativeException, self).__init__(message)

@uchronia_ffi.callback("void(char *)")
def _exception_callback_uchronia(exception_string):
    """
        This function is called when uchronia raises an exception.
        It sets the global variable ``_exception_txt_raised_uchronia``

        :param cdata exception_string: Exception string.
    """
    global _exception_txt_raised_uchronia
    _exception_txt_raised_uchronia = uchronia_ffi.string(exception_string)


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
            global variable ``_exception_txt_raised_uchronia`` is set.

            :param func func: Python function wrapping a uchronia function.
        """
        # log_func_call(func, *args, **kwargs)
        # Call the function
        return_value = func(*args, **kwargs)
        # Check if an exception was raised
        global _exception_txt_raised_uchronia
        if _exception_txt_raised_uchronia is not None:
            temp_exception = _exception_txt_raised_uchronia
            _exception_txt_raised_uchronia = None
            raise UchroniaError(temp_exception)
        return return_value
    return wrapper


uchronia_so.RegisterExceptionCallback(_exception_callback_uchronia)


@check_exceptions
def uchronia_dispose_multi_time_series_data(data):
    """
        :param ptr data: Pointer to a MultiTimeSeriesData.
    """
    uchronia_so.DisposeMultiTimeSeriesData(data)

