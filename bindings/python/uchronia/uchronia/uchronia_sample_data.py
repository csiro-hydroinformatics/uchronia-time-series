import os

from uchronia.uchronia_data_set import getEnsembleDataSet

#' Probe for the location of sample data directory (too big for package inclusion) 
#' 
#' Probe for the location of sample data directory (too big for package inclusion), defined by the SWIFT_SAMPLE_DATA_DIR environment variable.
#' 
#' @param doWarn issue a warning if SWIFT_SAMPLE_DATA_DIR env var is not set 
#' @return a string
#' @export
def sampleDataDir(doWarn=True):
    d = os.environ['SWIFT_SAMPLE_DATA_DIR']
    if doWarn:
        if not os.path.exists(d):
            raise Exception("Non-existent environment variable 'SWIFT_SAMPLE_DATA_DIR', or its value is not an existing directory")
    return d


#' @export
def hasSampleData():
    d = sampleDataDir(doWarn=False)
    return os.path.exists(d)

#' Get one of the sample time series libraries included in this package 
#' 
#' Get one of the sample time series libraries included in this package 
#' 
#' @param ident an identifier for the library. Choice (case insensitive): 'Upper Murray', 'Stanley'
#' @return a string
#' @export
def sampleTimeSeriesLibrary(ident:str):
    ident = ident.lower()
    assert ident in ['upper murray', 'stanley']
    docDataPath = sampleDataDir()

    here = os.path.abspath(os.path.dirname(__file__))
    pkg_dir = os.path.join(here,'.')
    pkgDataDir = os.path.join(pkg_dir, 'data')
    if ident == 'upper murray':
        dataLibraryFile = os.path.join(pkgDataDir, 'time_series_library_upper_murray.yaml')
        dataPath = os.path.join(docDataPath, 'UpperMurray')
    elif ident == 'stanley':
        dataLibraryFile = os.path.join(pkgDataDir, 'time_series_library_stanley.yaml')
        dataPath = os.path.join(docDataPath, "RPP")
    if not os.path.exists(dataPath): raise Exception('directory path expected but not found: ' + dataPath)
    dataLibrary = getEnsembleDataSet(dataLibraryFile, dataPath)
    return dataLibrary


