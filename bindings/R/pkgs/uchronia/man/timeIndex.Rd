% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/uchronia-data-set.r
\name{timeIndex}
\alias{timeIndex}
\title{Get the time index of a time series}
\usage{
timeIndex(tsInfo)
}
\arguments{
\item{tsInfo}{A representation of a time series. Supported types are external pointers as data from uchronia C API, or an R list returned by some of the *_R functions.}
}
\value{
a vector of POSIXct
}
\description{
Get the time index of a time series
}
