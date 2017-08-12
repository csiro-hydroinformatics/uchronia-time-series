# context("data")

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
mainTimeAxis <-function (deltaT, n, p=n) {
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
buildTestEnsTs <- function(i, seed=NA, deltaT=3600, n=100, nEns=4) {
  tt <- mainTimeAxis(deltaT, n)
  buildTestHourlyXts(i, tt$tsStartDate, seed=seed, n=n, nEns=nEns, deltaT=deltaT)
}

buildTestHourlyXts <- function(i, originDate, seed=NA, n=5, nEns=4, deltaT) {
  if(is.na(seed)) seed <- 456
  set.seed(seed)
  x <- matrix(rnorm(n=nEns*n), ncol=nEns)
  mkSeriesRegularTstep(originDate+lubridate::hours(i), (x+i), is.na, deltaT)
}

buildTestTsEnsTs <- function(deltaT=3600, n=100, p=80, deltaTLead, nLead=5, nEns=4) {
  if(p > n) stop("start of non-null ensemble is past the end of the time series")
  tt <- mainTimeAxis(deltaT, n, p)
  ensFcTs <- createEnsembleForecastTimeSeries(tt$tsStartDate, n, deltaT)
  for (i in as.integer(p:n)) {
    multiTimeSeriesIn <- buildTestEnsTs(i,n=nLead, deltaT=deltaTLead)
    uchronia::setItem(ensFcTs, i, multiTimeSeriesIn)
  }
  return(ensFcTs)
}

test_that("Getting data from an ensemble forecast time series", {
  deltaT <- 3600*3
  deltaTLead <- 180
  n <- 20
  p <- 8
  nLead <- 5
  nEns <- 4
  ensFcTs <- buildTestTsEnsTs(deltaT=deltaT,deltaTLead=deltaTLead,n=n,p=p,nLead=nLead,nEns=nEns)
  q <- n-p-1
  tt <- mainTimeAxis(deltaT, n, p)
  for (i in as.integer(1:n)) {
    ensTs = uchronia::getItem(ensFcTs, i)
    if(i < p) {
      expect_true(is.na(ensTs))
    } else {
      multiTimeSeriesExpected <- buildTestHourlyXts(i, tt$tsStartDate, n=nLead, nEns=nEns,deltaT=deltaTLead)
      expect_true(all(ensTs == multiTimeSeriesExpected))
    }
  }
})

test_that("Splicing - round trip setting/getting data from an ensemble of time series", {
  deltaT <- 3600*3
  n <- 100
  # nLead <- 5
  nEns <- 4
  # q <- n-p-1
  ensTsXts <- buildTestEnsTs(1, deltaT=deltaT,n=n,nEns=nEns)
  ensTs <- asUchroniaData(ensTsXts);
  for (iEns in as.integer(1:nEns)) {
    theTs <- uchronia::getItem(ensTs, iEns)
    expected <- ensTsXts[,iEns]
    expect_true(all(xts:::index.xts(expected )== xts:::index.xts(theTs)))
    expect_true(all(expected == theTs))
  }

  # we create another set of data, to test the setting of each item series in the collection ensTs.
  newEnsTsXts <- buildTestEnsTs(1, seed = 9876, deltaT=deltaT,n=n,nEns=nEns)

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
  deltaT <- 3600 * 3
  deltaTLead <- 3600 / 4
  n <- 20
  p <- 8
  nLead <- 5
  nEns <- 4

  ensFcTs <- buildTestTsEnsTs(deltaT=deltaT,deltaTLead=deltaTLead,n=n,p=p,nLead=nLead,nEns=nEns)
  tt <- timeIndex(ensFcTs)
  tst <- mainTimeAxis(deltaT, n, p)
  expectedTaxis <- tst$tsStartDate + (0:(n-1)) * deltaT
  expect_true(all(expectedTaxis == tt))
})


test_that("data dimensions are correctly retrieved", {
  deltaT <- 180
  n <- 10
  nEns <- 4

  x <- buildTestEnsTs(1,123,deltaT=deltaT, n=n, nEns=nEns)
  g <- geometryOf(x)

  expect_equal(nEns, g$ensemble$size)
  expect_equal(n, g$temporal$size)
  expect_equal(as.integer(lubridate::seconds(deltaT)), as.integer(g$temporal$time_step))
  expect_equal(lubridate::as.duration(lubridate::seconds(deltaT)), g$temporal$time_step)

  ux <- asUchroniaData(x)
  g <- geometryOf(ux)

  expect_equal(nEns, g$ensemble$size)
  expect_equal(n, g$temporal$size)
  expect_equal(as.integer(lubridate::seconds(deltaT)), as.integer(g$temporal$time_step))
  expect_equal(lubridate::as.duration(lubridate::seconds(deltaT)), g$temporal$time_step)

  deltaTMain <- 3*3600
  deltaTLead <- 3600 / 4
  # we deliberately leave missing values in the first three items... Tricky!
  p <- 3
  nLead <- 8

  ensFcTs <- buildTestTsEnsTs(deltaT=deltaTMain, deltaTLead=deltaTLead,n=n,p=p,nLead=nLead,nEns=nEns)
  g <- geometryOf(ensFcTs)

  tst <- mainTimeAxis(deltaT, n, p=p)
  expect_equal(n, g$temporal$size)
  expect_equal(tst$tsStartDate, g$temporal$start)

  # given   p <- 3   getting the geometry of the content is non-trivial! still TODO
  testthat::fail("getting the geometry of a TS of ensemble of TS is tricky if there are missing values!")
  expect_equal(as.integer(lubridate::seconds(deltaTMain)), as.integer(g$temporal$time_step))
  expect_equal(lubridate::as.duration(lubridate::seconds(deltaT)), g$temporal$time_step)
  
})


