# library(roxygen2) ; roxygenize('F:/src/path/to/uchronia')

#' A package to handle multidimensional time series and ensemble thereof
#' 
#' \tabular{ll}{
#' Package: \tab uchronia \cr
#' Type: \tab Package\cr
#' Version: \tab -alpha1 \cr
#' Date: \tab 2020-01-27 \cr
#' Release Notes: \tab Surface support for monthly time series. Added to cater for the model GR2M. \cr
#' License: \tab BSD-3 \cr
#' }
#'
#' \tabular{lll}{
#' Version \tab Date \tab Notes \cr
#' 2.3.0 \tab 2020-01-10 \tab Cumulative maintenance release. \cr
#' 2.1.2 \tab 2020-01-25 \tab Cumulative maintenance release. \cr
#' 2.1.1 \tab 2019-09-16 \tab Cumulative maintenance release. \cr
#' 0.4.5 \tab 2019-02-24 \tab Improve performance retrieving time series from providers and models. \cr
#' 0.4.4 \tab 2019-01-10 \tab Maintenance release, with mostly functionally neutral structural changes. \cr
#' 0.4.3 \tab 2018-07-06 \tab Adjust uchronia R package after testing build on Windows 10 with RTools 3.5. \cr
#' 0.4.2 \tab 2018-04-06 \tab Minor improvements in messages and warnings. \cr
#' 0.4.1 \tab 2018-02-27 \tab Build against a version of the datatypes library with a changed API (no reference '&'). \cr
#' 0.4.0 \tab 2018-01-30 \tab Start to include visualisations using ggplot2: plotXtsQuantiles. \cr
#' 0.3.5 \tab 2017-12-22 \tab Fix: Workaround for netCDF files where the ANSI string for units are not null-terminated. \cr
#' 0.3.4 \tab 2017-12-01 \tab Fix cases where constructing time series spanning multiple decades. R integer mode is too limited so cast time step to numerics. Add api entry for time series retrieval from provider time series. \cr
#' 0.3.3 \tab 2017-11-07 \tab Add feature to transform each item in an ensemble forecast time series, with aggregation/disaggregation for now. Normal maintenance and tidy ups \cr
#' 0.3.2 \tab 2017-08-09 \tab Added features needed for RPP for interop from/to R. Enhanced some of the C API \cr
#' 0.3.1 \tab 2017-07-21 \tab Added features needed for RPP. Enhanced some of the C API. \cr
#' 0.2.2 \tab 2017-06-22 \tab Minor functions added. Note that the R package itself (i.e. not the C++ library) is still new and much documentation remains to be written/ported from other packages. \cr
#' 0.2.1 \tab 2017-05-26 \tab First version with port of all features from SWIFT, and succinct vignette \cr
#' }
#'
#' @import msvs
#' @import cinterop
#'
#' @name uchronia-package
#' @aliases uchronia
#' @docType package
#' @title A package to handle multidimensional time series and ensemble thereof
#' @author Jean-Michel Perraud \email{jean-michel.perraud_at_csiro.au}
#' @keywords datatypes
NULL
