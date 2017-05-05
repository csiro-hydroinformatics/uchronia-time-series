context("data")

library(lubridate)

test_that("Round trip setting/getting data from an ensemble forecast time series", {
  deltaT <- 3600
  n <- 100
  p <- 80
  nLead <- 5
  nEns <- 4
  
  tsStartDate <- lubridate::origin
  tsEndDate <- tsStartDate + lubridate::seconds(deltaT*n)
  tsStartEns <- tsStartDate + lubridate::seconds(deltaT*p)

  q <- n-p-1
  ensFcTs <- createEnsembleForecastTimeSeries(tsStartEns, q, deltaT)

  set.seed(465)
  x <- matrix(rnorm(n=nEns*nLead), ncol=nEns)

  for (i in as.integer(0:(q-1))) {
    multiTimeSeriesIn <- mkHourlySeries(tsStartDate+lubridate::hours(i), (x+i), is.na)
    uchronia::setItem(ensFcTs, i, multiTimeSeriesIn)
    multiTimeSeriesOut = uchronia::getItem(ensFcTs, i)
    expect_true(all(multiTimeSeriesOut == multiTimeSeriesIn))
  }
})

# The following test is still in swift land due to lagacy txt ts reader.
# test_that("Retrieval of data in time series libraries", {
