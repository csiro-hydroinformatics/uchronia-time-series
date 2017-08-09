context("data")

library(lubridate)
library(uchronia)
library(testthat)

#' mainTimeAxis
#'
#' Creates information on the time axis of a test time series of ensemble of time series: start date, end date, first date with an ensemble  
#'
#' @param deltaT time step in seconds.
#' @param n length of the time series
#' @param p index of the first ensemble in the time series.
#' @return a list of datetimes 
mainTimeAxis <-function (deltaT, n, p) {
  list(
  tsStartDate = lubridate::origin,
  tsEndDate = lubridate::origin + lubridate::seconds(deltaT*n),
  tsStartEns = lubridate::origin + lubridate::seconds(deltaT*p)
  )
}

#' buildTestEnsTs
#'
#' buildTestEnsTs
#'
#' @param i index of the test ensemble of time series
#' @return an xts
buildTestEnsTs <- function(i, seed=NA, deltaT=3600, n=100, p=80, nLead=5, nEns=4) {
  tt <- mainTimeAxis(deltaT, n, p)
  buildTestHourlyXts(i, tt$tsStartDate, seed=seed, nLead=nLead, nEns=nEns)
}

buildTestHourlyXts <- function(i, originDate, seed=NA, nLead=5, nEns=4) {
  if(is.na(seed)) seed <- 456
  set.seed(seed)
  x <- matrix(rnorm(n=nEns*nLead), ncol=nEns)
  mkHourlySeries(originDate+lubridate::hours(i), (x+i), is.na)
}

buildTestTsEnsTs <- function(deltaT=3600, n=100, p=80, nLead=5, nEns=4) {
  q <- n-p-1
  tt <- mainTimeAxis(deltaT, n, p)
  ensFcTs <- createEnsembleForecastTimeSeries(tt$tsStartEns, q, deltaT)

  set.seed(465)
  x <- matrix(rnorm(n=nEns*nLead), ncol=nEns)

  for (i in as.integer(1:q)) {
    multiTimeSeriesIn <- buildTestEnsTs(i)
    uchronia::setItem(ensFcTs, i, multiTimeSeriesIn)
  }
  return(ensFcTs)
}

test_that("Round trip setting/getting data from an ensemble forecast time series", {
  deltaT <- 3600
  n <- 100
  p <- 80
  nLead <- 5
  nEns <- 4
  ensFcTs <- buildTestTsEnsTs(deltaT=deltaT,n=n,p=p,nLead=nLead,nEns=nEns)
  q <- n-p-1
  tt <- mainTimeAxis(deltaT, n, p)
  for (i in as.integer(1:q)) {
    multiTimeSeriesExpected <- buildTestHourlyXts(i, tt$tsStartDate, nLead=nLead, nEns=nEns)
    multiTimeSeriesOut = uchronia::getItem(ensFcTs, i)
    expect_true(all(multiTimeSeriesOut == multiTimeSeriesExpected))
  }
})

test_that("Splicing - round trip setting/getting data from an ensemble of time series", {
  deltaT <- 3600
  n <- 100
  p <- 80
  nLead <- 5
  nEns <- 4
  q <- n-p-1
  ensTsXts <- buildTestEnsTs(1, deltaT=deltaT,n=n,p=p,nLead=nLead,nEns=nEns)
  ensTs <- asUchroniaData(ensTsXts);
  for (iEns in as.integer(1:nEns)) {
    theTs <- uchronia::getItem(ensTs, iEns)
    expected <- ensTsXts[,iEns]
    expect_true(all(xts:::index.xts(expected )== xts:::index.xts(theTs)))
    expect_true(all(expected == theTs))
  }

  # we create another set of data, to test the setting of each item series in the collection ensTs.
  newEnsTsXts <- buildTestEnsTs(1, seed = 9876, deltaT=deltaT,n=n,p=p,nLead=nLead,nEns=nEns)

  for (iEns in as.integer(1:nEns)) {
    uchronia::setItem(ensTs, iEns, newEnsTsXts[,iEns])
  }

  for (iEns in as.integer(1:nEns)) {
    theTs <- uchronia::getItem(ensTs, iEns)
    expected <- newEnsTsXts[,iEns]
    expect_true(all(xts:::index.xts(expected )== xts:::index.xts(theTs)))
    expect_true(all(expected == theTs))
  }
})


# The following test is still in swift land due to legacy txt ts reader.
# test_that("Retrieval of data in time series libraries", {

test_that("Time indices are correctly retrieved", {
  ensFcTs <- buildTestTsEnsTs()
  deltaT <- 3600
  n <- 100
  p <- 80
  nLead <- 5
  nEns <- 4
  q <- n-p-1
  tt <- timeIndex(ensFcTs)
  tst <- mainTimeAxis(deltaT, n, p)
  expectedTaxis <- tst$tsStartEns + (0:(q-1)) * deltaT
  expect_true(all(expectedTaxis == tt))

  # TODO test retrieving the time axis of TIME_SERIES_PTR, but less essential.
  # uchronia::GetTimeSeriesGeometry_Pkg_R
})

test_that("Round-trip between univariate xts and external time series", {
  # TODO 
})

