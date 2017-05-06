##################
# 
# *** THIS FILE IS GENERATED ****
# DO NOT MODIFY IT MANUALLY, AS YOU ARE VERY LIKELY TO LOSE WORK
# 
##################

#' RegisterExceptionCallback_Pkg_R
#'
#' RegisterExceptionCallback_Pkg_R Wrapper function for RegisterExceptionCallback_Pkg
#'
#' @export
RegisterExceptionCallback_Pkg_R <- function() {
  RegisterExceptionCallback_Pkg()
}

#' GetTimeSeriesGeometry_Pkg_R
#'
#' GetTimeSeriesGeometry_Pkg_R Wrapper function for GetTimeSeriesGeometry_Pkg
#'
#' @param timeSeries R type equivalent for C++ type XPtr<opaque_pointer_handle>
#' @param variableIdentifier R type equivalent for C++ type CharacterVector
#' @export
GetTimeSeriesGeometry_Pkg_R <- function(timeSeries, variableIdentifier) {
  timeSeries <- cinterop::getExternalXptr(timeSeries)
  variableIdentifier <- cinterop::getExternalXptr(variableIdentifier)
  result <- GetTimeSeriesGeometry_Pkg(timeSeries, variableIdentifier)
  return(cinterop::mkExternalObjRef(result, 'Rcpp::S4'))
}

#' GetItemEnsembleForecastTimeSeries_Pkg_R
#'
#' GetItemEnsembleForecastTimeSeries_Pkg_R Wrapper function for GetItemEnsembleForecastTimeSeries_Pkg
#'
#' @param series R type equivalent for C++ type XPtr<opaque_pointer_handle>
#' @param i R type equivalent for C++ type IntegerVector
#' @export
GetItemEnsembleForecastTimeSeries_Pkg_R <- function(series, i) {
  series <- cinterop::getExternalXptr(series)
  i <- cinterop::getExternalXptr(i)
  result <- GetItemEnsembleForecastTimeSeries_Pkg(series, i)
  return(cinterop::mkExternalObjRef(result, 'Rcpp::S4'))
}

#' SetItemEnsembleForecastTimeSeries_Pkg_R
#'
#' SetItemEnsembleForecastTimeSeries_Pkg_R Wrapper function for SetItemEnsembleForecastTimeSeries_Pkg
#'
#' @param series R type equivalent for C++ type XPtr<opaque_pointer_handle>
#' @param i R type equivalent for C++ type IntegerVector
#' @param timeSeriesEnsemble R type equivalent for C++ type Rcpp::S4&
#' @export
SetItemEnsembleForecastTimeSeries_Pkg_R <- function(series, i, timeSeriesEnsemble) {
  series <- cinterop::getExternalXptr(series)
  i <- cinterop::getExternalXptr(i)
  timeSeriesEnsemble <- cinterop::getExternalXptr(timeSeriesEnsemble)
  SetItemEnsembleForecastTimeSeries_Pkg(series, i, timeSeriesEnsemble)
}

#' GetEnsembleTimeSeries_Pkg_R
#'
#' GetEnsembleTimeSeries_Pkg_R Wrapper function for GetEnsembleTimeSeries_Pkg
#'
#' @param series R type equivalent for C++ type XPtr<opaque_pointer_handle>
#' @export
GetEnsembleTimeSeries_Pkg_R <- function(series) {
  series <- cinterop::getExternalXptr(series)
  result <- GetEnsembleTimeSeries_Pkg(series)
  return(cinterop::mkExternalObjRef(result, 'Rcpp::S4'))
}

#' GetEnsembleForecastTimeSeriesGeometry_Pkg_R
#'
#' GetEnsembleForecastTimeSeriesGeometry_Pkg_R Wrapper function for GetEnsembleForecastTimeSeriesGeometry_Pkg
#'
#' @param timeSeries R type equivalent for C++ type XPtr<opaque_pointer_handle>
#' @export
GetEnsembleForecastTimeSeriesGeometry_Pkg_R <- function(timeSeries) {
  timeSeries <- cinterop::getExternalXptr(timeSeries)
  result <- GetEnsembleForecastTimeSeriesGeometry_Pkg(timeSeries)
  return(cinterop::mkExternalObjRef(result, 'Rcpp::S4'))
}

