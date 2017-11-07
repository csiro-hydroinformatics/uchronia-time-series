# library(roxygen2) ; roxygenize('F:/src/path/to/uchronia')

#' A package to handle multidimensional time series and ensemble thereof
#' 
#' \tabular{ll}{
#' Package: \tab uchronia \cr
#' Type: \tab Package\cr
#' Version: \tab 0.3.3 \cr
#' Date: \tab 2017-11-07 \cr
#' Release Notes: \tab Add feature to transform each item in an ensemble forecast time series, with aggregation/disaggregation for now. Normal maintenance and tidy ups \cr
#' License: \tab LGPL-3 \cr
#' }
#'
#' \tabular{lll}{
#' Version \tab Date \tab Notes \cr
#' 0.3.2 \tab 2017-08-09 \tab Added features needed for RPP for interop from/to R. Enhanced some of the C API \cr
#' 0.3.1 \tab 2017-07-21 \tab Added features needed for RPP. Enhanced some of the C API. \cr
#' 0.2.2 \tab 2017-06-22 \tab Minor functions added. Note that the R package itself (i.e. not the C++ library) is still new and much documentation remains to be written/ported from other packages. \cr
#' 0.2.1 \tab 2017-05-26 \tab First version with port of all features from SWIFT, and succinct vignette \cr
#' }
#'
#' @import xts
#' @import lubridate
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
