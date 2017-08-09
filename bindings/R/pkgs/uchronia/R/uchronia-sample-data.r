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

