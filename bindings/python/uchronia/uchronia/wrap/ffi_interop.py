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
            prior_path_env = os.environ.get(to_env)
            prior_paths = prior_path_env.split(path_sep)
            # prior_paths = tolower(prior_paths)
            added_paths = [os.path.join(x, subfolder) for x in shared_lib_paths_vec]
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

def update_path_windows (from_env='LIBRARY_PATH', to_env='PATH'):
    if(sys.platform == 'win32'):
        os.environ[to_env] = build_new_path_env(from_env, to_env)
        # if(file.exists(sharedLibPath)):
        #     if(file.exists(fullpath)):
        #     if(!(tolower(fullpath) %in% prior_paths)):
        #         startup_msg = appendstartup_msg(paste('Appending to the PATH env var:', fullpath), startup_msg)
        #         new_paths = c(new_paths, fullpath)
        #         else 
        #         startup_msg = appendstartup_msg(paste('Path', fullpath, 'already found in PATH environment variable'), startup_msg)




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

def to_swift_freq(frequency):
    """
        Get SWIFT frequency.

        :param str frequency: Pandas frequency.
        :returns: A tuple:
            - SWIFT frequency ('hourly', 'daily')
            - Timestep in seconds
    """
    if frequency == b'D':
        freq = b'daily'
        timestep_seconds = 86400
    elif frequency == b'H':
        freq = b'hourly'
        timestep_seconds = 3600
    else:
        raise ValueError(b'Frequency not supported: ' + frequency)
    return freq, timestep_seconds

    
def to_frequency(timestep_seconds):
    """
        Get Pandas frequency from ``timestep_seconds``.

        :param int timestep_seconds: Timestep in seconds.
        :returns: A str ('H', 'D')
    """
    if timestep_seconds == 86400:
        return b'D'
    elif timestep_seconds == 3600:
        return b'H'
    else:
        raise ValueError('Timestep not supported: ' + timestep_seconds)


class MarshaledDateTime(CffiNativeHandle):
    """
        :param int year: ...
        :param int month: ...
        :param int day: ...
        :param int hour: ...
        :param int minute: ...
        :param int second: ...
    """
    def __init__(self, year, month, day, hour=0, minute=0, second=0):
        ptr = uchronia_ffi.new("date_time_to_second*")
        super(MarshaledDateTime, self).__init__(ptr, type_id='date_time_to_second*', prior_ref_count = 0)
        if year is not None and month is not None and day is not None:
            self.get_handle().year = year
            self.get_handle().month = month
            self.get_handle().day = day
            self.get_handle().hour = hour
            self.get_handle().minute = minute
            self.get_handle().second = second

    @property
    def year(self):
        """ Get year """
        return self.get_handle().year

    @property
    def month(self):
        """ Get month """
        return self.get_handle().month

    @property
    def day(self):
        """ Get day """
        return self.get_handle().day

    @property
    def hour(self):
        """ Get hour """
        return self.get_handle().hour

    @property
    def minute(self):
        """ Get minute """
        return self.get_handle().minute

    @property
    def second(self):
        """ Get second """
        return self.get_handle().second

    def to_datetime(self):
        """ Get a datetime representation of this object """
        return datetime.datetime(self.year, self.month, self.day, self.hour,
                                 self.minute, self.second)

    @staticmethod
    def from_datetime(date_time):
        """ Create a MarshaledDateTime from a datetime. """
        return MarshaledDateTime(date_time.year, date_time.month,
                                 date_time.day, date_time.hour,
                                 date_time.minute, date_time.second)

    @staticmethod
    def from_struct(marshaled_date_time_struct):
        """
            Create a MarshaledDateTime from a struct.

            :param cffiobject marshaled_date_time_struct: CFFI struct.
        """
        year = marshaled_date_time_struct.year
        month = marshaled_date_time_struct.month
        day = marshaled_date_time_struct.day
        hour = marshaled_date_time_struct.hour
        minute = marshaled_date_time_struct.minute
        sec = marshaled_date_time_struct.second
        return MarshaledDateTime(year, month, day, hour, minute, sec)

    @staticmethod
    def empty_marshaled_date_time():
        """
            Create an empty MarshaledDateTime.
        """
        return MarshaledDateTime(None, None, None)

    def __str__(self):
        return '{y}-{m}-{d} {h}:{mm}:{s}'.format(y=self.get_handle().year,
                                                 m=self.get_handle().month,
                                                 d=self.get_handle().day,
                                                 h=self.get_handle().hour,
                                                 mm=self.get_handle().minute,
                                                 s=self.get_handle().second)

    def __repr__(self):
        return str(self)

    def _release_handle(self):
        pass # No entry point for that. Can only be owned by the cffi wrapper.

class MarshaledTsGeometry(CffiNativeHandle):
    """
        :param MarshaledDateTime start: ...
        :param str freq: 'H' or 'D'
        :param int length: ...
    """
    def __init__(self, start, freq, length):
        if freq is not None:
            _, timestep_seconds = to_swift_freq(freq)

        ptr = uchronia_ffi.new("regular_time_series_geometry *")
        super(MarshaledTsGeometry, self).__init__(ptr, type_id='regular_time_series_geometry*', prior_ref_count = 0)
        if start is not None and freq is not None \
        and length is not None:
            self.get_handle().start = start.get_handle()[0]
            self.get_handle().time_step_seconds = timestep_seconds
            self.get_handle().length = length

    @property
    def start(self):
        """
            Get start.
            :returns: MarshaledDateTime
        """
        return MarshaledDateTime.from_struct(self.get_handle().start)

    @property
    def timestep_seconds(self):
        """ Get timestep_seconds. """
        return self.get_handle().time_step_seconds

    @property
    def length(self):
        """ Get length. """
        return self.get_handle().length

    @property
    def freq(self):
        """ Get frequency ('D' or 'H') """
        if self.get_handle().time_step_seconds == 86400:
            return 'D'
        elif self.get_handle().time_step_seconds == 3600:
            return 'H'

    @staticmethod
    def empty_marshaled_ts_geometry():
        """ Returns an empty MarshaledTsGeometry. """
        start = MarshaledDateTime.empty_marshaled_date_time()
        return MarshaledTsGeometry(start, None, None)

    @staticmethod
    def from_struct(marshaled_ts_geom_struct):
        """
            Create a MarshaledTsGeometry from a struct.

            :param cffiobject marshaled_ts_geom_struct: CFFI struct.
        """
        start = MarshaledDateTime.from_struct(marshaled_ts_geom_struct.start)
        freq = to_frequency(marshaled_ts_geom_struct.time_step_seconds)
        length = marshaled_ts_geom_struct.length
        return MarshaledTsGeometry(start, freq, length)

    def __str__(self):
        geom_str = 'Geometry: start: {s}, time step seconds: {t}, length: {l}'
        try:
            return geom_str.format(s=self.start,
                                   t=self.timestep_seconds,
                                   l=self.length)
        except AttributeError:
            return "Geometry: empty"

    def __repr__(self):
        return str(self)

    def _release_handle(self):
        pass # No API entry point for that. Can only be owned by the cffi wrapper.

class MultiTimeSeriesData(CffiNativeHandle):
    """
        :param MarshaledTsGeometry ts_geom: ...
        :param int ensemble_size: ...
        :param ptr numeric_data: ...
    """
    def __init__(self, ts_geom, ensemble_size, numeric_data,
                 python_object=True):
        ptr = uchronia_ffi.new("multi_regular_time_series_data*")
        super(MultiTimeSeriesData, self).__init__(ptr, type_id='regular_time_series_geometry*', prior_ref_count = 1)
        if ts_geom is not None and ensemble_size is not None \
        and numeric_data is not None:
            self.get_handle().time_series_geometry = ts_geom.get_handle()[0]
            self.get_handle().ensemble_size = ensemble_size
            self.get_handle().numeric_data = numeric_data

    @property
    def geom(self):
        """
            Get Geometry.
            :returns: MarshaledTsGeometry
        """
        return MarshaledTsGeometry.from_struct(self.get_handle().time_series_geometry)

    @property
    def ensemble_size(self):
        """ Get ensemble_size. """
        return self.get_handle().ensemble_size

    @property
    def numeric_data(self):
        """ Get numeric_data. """
        ndata = np.zeros((self.ensemble_size, self.geom.length), dtype='f')

        for ens_idx in range(self.ensemble_size):
            ndata[ens_idx] = np.frombuffer(
                uchronia_ffi.buffer(
                    uchronia_ffi.cast('double[%d]' % (self.geom.length),
                              self.get_handle().numeric_data[ens_idx])
                )
            )

        return ndata

    @staticmethod
    def from_struct(multi_time_series_data_struct, python_object=False):
        """
            Create a MarshaledTsGeometry from a struct.

            :param cffiobject multi_time_series_data_struct: CFFI struct.
        """
        geom_ptr = multi_time_series_data_struct.time_series_geometry
        ts_geom = MarshaledTsGeometry.from_struct(geom_ptr)
        ens_size = multi_time_series_data_struct.ensemble_size
        numeric_data = multi_time_series_data_struct.numeric_data
        return MultiTimeSeriesData(ts_geom, ens_size, numeric_data,
                                   python_object=python_object)

    def to_dataframe(self):
        """ Get DataFrame representation """
        return pd.DataFrame(self.numeric_data,
                            columns=range(1, self.geom.length + 1))

    def __str__(self):
        mtsd_str = 'MultiTimeSeriesData:\n- geom: {g}\n- ens size: {es}'
        try:
            return mtsd_str.format(g=self.geom, es=self.ensemble_size)
        except AttributeError:
            return "MultiTimeSeriesData: empty"

    def __repr__(self):
        return str(self)

    def _release_handle(self):
        swift_dispose_multi_time_series_data(self.get_handle())

@check_exceptions
def swift_dispose_multi_time_series_data(data):
    """
        :param ptr data: Pointer to a MultiTimeSeriesData.
    """
    uchronia_so.DisposeMultiTimeSeriesData(data)

