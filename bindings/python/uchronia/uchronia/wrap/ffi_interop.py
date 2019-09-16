"""
    Wrapper around uchronia C API functions using CFFI.
"""
from collections import OrderedDict
from functools import wraps
from ctypes.util import find_library as ctypes_find_library
import datetime
import sys
from cffi import FFI
import numpy as np
import pandas as pd
import os

from refcount.interop import *
from refcount.putils import library_short_filename

def prepend_path_env (added_paths, to_env='PATH'):
    path_sep = ';'
    prior_path_env = os.environ.get(to_env)
    prior_paths = prior_path_env.split(path_sep)
    # prior_paths = tolower(prior_paths)
    added_paths = [os.path.join(x, subfolder) for x in added_paths]
    added_paths = [x for x in added_paths if os.path.exists(x)]
    # TODO: check for duplicate folders BUT if using 'set' for set difference we'd mess up order which is wrong in principle.
    new_paths = prior_paths + added_paths
    new_env_val = path_sep.join(new_paths)
    return new_env_val
    # if(file.exists(sharedLibPath)):
    #     if(file.exists(fullpath)):
    #     if(!(tolower(fullpath) %in% prior_paths)):
    #         startup_msg = appendstartup_msg(paste('Appending to the PATH env var:', fullpath), startup_msg)
    #         new_paths = c(new_paths, fullpath)
    #         else 
    #         startup_msg = appendstartup_msg(paste('Path', fullpath, 'already found in PATH environment variable'), startup_msg)

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
            return prepend_path_env(shared_lib_paths_vec, to_env=to_env)

def update_path_windows (from_env='LIBRARY_PATH', to_env='PATH'):
    if(sys.platform == 'win32'):
        os.environ[to_env] = build_new_path_env(from_env, to_env)

def none_or_empty(x):
    return (x is None or x == '')

def find_first_full_path(native_lib_file_name, readable_lib_name = "native library", env_var_name = ""):
    if os.path.isabs(native_lib_file_name):
        if (not os.path.exists(native_lib_file_name)):
            raise FileNotFoundError("Could not find specified file {0} to load for {1}".format(native_lib_file_name, readable_lib_name))
        return native_lib_file_name
    if (native_lib_file_name is None or native_lib_file_name == ''):
        raise FileNotFoundError("Invalid empty file name to load for {0}".format(readable_lib_name))
    native_lib_file_name = _find_first_full_path(native_lib_file_name, env_var_name)
    return native_lib_file_name

def _find_first_full_path(short_file_name, env_var_name = ""):
    if (none_or_empty(short_file_name)):
        raise Exception("short_file_name")
    lib_search_path_env_var = env_var_name
    if (none_or_empty(lib_search_path_env_var)):
        if(sys.platform == 'win32'):
            lib_search_path_env_var = "PATH"
        else:
            lib_search_path_env_var =  "LD_LIBRARY_PATH"
    candidates = find_full_path_env_var(short_file_name, lib_search_path_env_var)
    if ((len(candidates) == 0) and (sys.platform == 'win32')):
        if (os.path.exists(short_file_name)):
            candidates = [short_file_name]
    if (len(candidates) == 0):
        raise FileNotFoundError("Could not find native library named '{0}' within the directories specified in the '{1}' environment variable".format(short_file_name, lib_search_path_env_var))
    else:
        return candidates[0]

def find_full_path_env_var(dll_short_name, env_var_name="PATH"):
    x = os.environ.get(env_var_name)
    if x is not None:
        search_paths = x.split(os.pathsep)
    else:
        search_paths = [""]
    return find_full_paths(dll_short_name, search_paths)

def find_full_paths(dll_short_name, directories = []):
    full_paths = [os.path.join(d, dll_short_name) for d in directories]
    return [x for x in full_paths if os.path.exists(x)]



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
long_fname = find_first_full_path(short_fname, "uchronia")
uchronia_so = uchronia_ffi.dlopen(long_fname, 1) # Lazy loading

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

@check_exceptions
def swift_dispose_multi_time_series_data(data):
    """
        :param ptr data: Pointer to a MultiTimeSeriesData.
    """
    uchronia_so.DisposeMultiTimeSeriesData(data)

