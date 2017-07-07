"""
    Wrapper around uchronia C API functions using CFFI.
"""
from collections import OrderedDict
from ctypes.util import find_library as ctypes_find_library
import datetime
import sys
from cffi import FFI
import numpy as np
import pandas as pd
import os

def find_library(name):
    """Wrapper around ctypes.util.find_library that works around
       it not using python executable's RPATH value.

       (Ana)conda relies on RPATH in its binaries to ensure the linker
       finds libraries in its current environment.

       This may no longer be needed if this gets fixed:

         https://bugs.python.org/issue19317

       The cffi dlopen method used in this module relies on
       ctypes.util.find_library to find libraries.
    """
    libpath = None
    if name is not None:
        libname = 'lib{}.so'.format(name)
        prefixed_lib_pat = os.path.join(sys.prefix, 'lib*', libname)
        prefixed_libs = glob(prefixed_lib_pat)
        if prefixed_libs:
            libpath = prefixed_libs[0]
    if not libpath:
        libpath = ctypes_find_library(name)

    if libpath is None:
        # fall back on just passing the original library name and
        # letting cffi deal with it...
        libpath = name
    return libpath


uchronia_ffi = FFI()
uchronia_src_dir = '/home/per202/src/csiro/stash/per202/datatypes'
cdefs_dir = os.path.join(uchronia_src_dir, 'bindings/python/uchronia/data')
with open(os.path.join(cdefs_dir, 'structs_cdef.h')) as f_headers:
    uchronia_ffi.cdef(f_headers.read())

with open(os.path.join(cdefs_dir, 'funcs_cdef.h')) as f_headers:
    uchronia_ffi.cdef(f_headers.read())

uchronia_so = uchronia_ffi.dlopen(find_library('datatypes.so'), 1) # Lazy loading

class UchroniaError(Exception):
    """ Exception when calling a uchronia function. """
    def __init__(self, message):
        super(UchroniaError, self).__init__(message)


# This will store the exception message raised by uchronia
EXCEPTION_TXT_RAISED = None

@uchronia_ffi.callback("void(char *)")
def exception_callback(exception_string):
    """
        This function is called when uchronia raises an exception.
        It sets the global variable ``EXCEPTION_TXT_RAISED``

        :param cdata exception_string: Exception string.
    """
    global EXCEPTION_TXT_RAISED
    EXCEPTION_TXT_RAISED = uchronia_ffi.string(exception_string)


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
            global variable ``EXCEPTION_TXT_RAISED`` is set.

            :param func func: Python function wrapping a uchronia function.
        """
        log_func_call(func, *args, **kwargs)
        # Call the function
        return_value = func(*args, **kwargs)
        # Check if an exception was raised
        global EXCEPTION_TXT_RAISED
        if EXCEPTION_TXT_RAISED is not None:
            temp_exception = EXCEPTION_TXT_RAISED
            EXCEPTION_TXT_RAISED = None
            raise UchroniaError(temp_exception)
        return return_value
    return wrapper


uchronia_so.RegisterExceptionCallback(exception_callback)


def as_bytes(obj):
    """Convert obj to bytes if it is a string type."""
    if isinstance(obj, six.string_types) and not isinstance(obj, bytes):
        return obj.encode('utf-8')
    else:
        return obj


def as_string(obj):
    """Convert obj to string/unicode if it is a bytes object."""
    if isinstance(obj, bytes):
        return obj.decode('utf-8')
    return obj


def convert_strings(func):
    """Returns a wrapper that converts any str/unicode object arguments to
       bytes.
    """
    @wraps(func)
    def wrapper(*args, **kwargs):
        """Convert args.

           :param func func: Python function wrapping a uchronia function.
        """
        new_args = []
        for arg in args:
            new_args.append(as_bytes(arg))
        new_kwargs = {}
        for key in kwargs:
            new_kwargs[key] = as_bytes(kwargs[key])

        # Call the function
        return_value = func(*new_args, **new_kwargs)
        if isinstance(return_value, (list, tuple)):
            return [as_string(obj) for obj in return_value]
        else:
            return as_string(return_value)

    return wrapper


class CObjectPointer(object):
    """ This class maintains the C object pointer. """
    def __init__(self, ptr, python_object=False):
        self._ptr = ptr
        self.python_object = python_object

    @property
    def ptr(self):
        """ Return the pointer (cffi object) """
        return self._ptr

    @ptr.setter
    def ptr(self, value):
        """ Set the pointer (cffi object) """
        self._ptr = value

    @property
    def obj(self):
        """ Return the object pointed to (cffi object) """
        return self._ptr[0]

    def __del__(self):
        if not self.python_object:
            swift_dispose_shared_pointer(self.ptr)

