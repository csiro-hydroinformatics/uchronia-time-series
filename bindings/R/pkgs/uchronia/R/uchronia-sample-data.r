#' Probe for the location of sample data directory (too big for package inclusion) 
#' 
#' Probe for the location of sample data directory (too big for package inclusion), defined by the SWIFT_SAMPLE_DATA_DIR environment variable.
#' 
#' @param doWarn issue a warning if SWIFT_SAMPLE_DATA_DIR env var is not set 
#' @return a string
#' @export
sampleDataDir <- function(doWarn=TRUE) {
  d <- Sys.getenv('SWIFT_SAMPLE_DATA_DIR')
  if(doWarn & !dir.exists(d)) {warning("Non-existent environment variable 'SWIFT_SAMPLE_DATA_DIR', or its value is not an existing directory")}
  return(d)
}

#' @export
hasSampleData <- function() {
  d <- sampleDataDir(doWarn=FALSE)
  dir.exists(d)
}

#' Get one of the sample time series libraries included in this package 
#' 
#' Get one of the sample time series libraries included in this package 
#' 
#' @param ident an identifier for the library. Choice (case insensitive): 'Upper Murray', 'Stanley'
#' @return a string
#' @export
sampleTimeSeriesLibrary <- function(ident) {
  ident <- tolower(ident)
  stopifnot(length(ident)==1)
  stopifnot(ident %in% c('upper murray', 'stanley'))
  docDataPath <- sampleDataDir()

  pkgDataDir <- system.file('data', package='uchronia')
  if(ident == 'upper murray') {
    dataLibraryFile <- file.path(pkgDataDir, 'time_series_library_upper_murray.yaml')
    dataPath <- file.path(docDataPath, 'UpperMurray')
    if(!dir.exists(dataPath)) stop(paste0('directory path expected but not found: ', dataPath))
    dataLibrary <- uchronia::getEnsembleDataSet(dataLibraryFile, dataPath)
  } else if(ident == 'stanley') {
    dataLibraryFile <- file.path(pkgDataDir, 'time_series_library_stanley.yaml')
    dataPath <- file.path(docDataPath, "RPP")
    if(!dir.exists(dataPath)) stop(paste0('directory path expected but not found: ', dataPath))
    dataLibrary <- uchronia::getEnsembleDataSet(dataLibraryFile, dataPath)
  }
  return(dataLibrary)
}


