context("data")

library(lubridate)

mainTimeAxis <-function (deltaT, n, p){
  list(
  tsStartDate = lubridate::origin,
  tsEndDate = lubridate::origin + lubridate::seconds(deltaT*n),
  tsStartEns = lubridate::origin + lubridate::seconds(deltaT*p)
  )
}

buildTestEnsTs <- function(i) {
  deltaT <- 3600
  n <- 100
  p <- 80
  nLead <- 5
  nEns <- 4
  q <- n-p-1

  tt <- mainTimeAxis(deltaT, n, p)
  set.seed(465)
  x <- matrix(rnorm(n=nEns*nLead), ncol=nEns)
  mkHourlySeries(tt$tsStartDate+lubridate::hours(i), (x+i), is.na)
}

buildTestTsEnsTs <- function() {
  deltaT <- 3600
  n <- 100
  p <- 80
  nLead <- 5
  nEns <- 4
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
  ensFcTs <- buildTestTsEnsTs()
  deltaT <- 3600
  n <- 100
  p <- 80
  nLead <- 5
  nEns <- 4
  q <- n-p-1
  tt <- mainTimeAxis(deltaT, n, p)
  for (i in as.integer(1:q)) {
    multiTimeSeriesExpected <- mkHourlySeries(tt$tsStartDate+lubridate::hours(i), (x+i), is.na)
    multiTimeSeriesOut = uchronia::getItem(ensFcTs, i)
    expect_true(all(multiTimeSeriesOut == multiTimeSeriesExpected))
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

