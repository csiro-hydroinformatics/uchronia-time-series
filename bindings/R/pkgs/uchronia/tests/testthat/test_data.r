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

test_that("Retrieval of data in time series libraries", {

  # Create a test data files for https://jira.csiro.au/browse/WIRADA-417
  testDataPath <- Sys.getenv('SWIFT_TEST_DIR')
  if(testDataPath=="") {
    Sys.setenv(SWIFT_TEST_DIR='//clw-02-mel.it.csiro.au/OSM_MEL_CLW_HYDROFORECAST_common/Staff/per202/sample_data/UnitTests')
  }
  testDataPath <- Sys.getenv('SWIFT_TEST_DIR')
  stopifnot(dir.exists(testDataPath))
  dataLibFile <- file.path(testDataPath, 'time_series_library_ut.yaml')
  stopifnot(file.exists(dataLibFile))
  dataLibrary <- getEnsembleDataSet(dataLibFile)
	var1_fcast_ens_id <- 'var1_fcast_ens'                    
	var2_fcast_ens_id	<- 'var2_fcast_ens'
	var1_ens_id		<- 'var1_ens'
	var1_obs_id		<- 'var1_obs'
	var1_obs_collection_id	<- 'var1_obs_collection'
	legacy_swift_1_id	<- 'legacy_swift_1'
	rain_fcast_ens_id	<- 'rain_fcast_ens'
	pet_fcast_ens_id<- 	'pet_fcast_ens'
  expectedIds <- c(
    var1_fcast_ens_id, 
    var2_fcast_ens_id, 
    var1_ens_id, 
    var1_obs_id, 
    var1_obs_collection_id, 
    legacy_swift_1_id, 
    rain_fcast_ens_id, 
    pet_fcast_ens_id)
  
  expect_true( setequal( getDataSetIds(dataLibrary), expectedIds) )

  expectedStationIds <- c('123','456')
  expect_true( setequal( GetEnsembleDatasetDataSubIdentifiers_R( dataLibrary, var1_obs_collection_id ), expectedStationIds) )
  expect_equal( setequal( GetEnsembleDatasetDataSubIdentifiers_R( dataLibrary, var1_fcast_ens_id ), '123') )
  expect_equal( setequal( GetEnsembleDatasetDataSubIdentifiers_R( dataLibrary, var2_fcast_ens_id ), '456') )

  expect_equal( setequal( GetEnsembleDatasetDataSubIdentifiers_R( dataLibrary, var1_ens_id ), '123') )
  expect_equal( setequal( GetEnsembleDatasetDataSubIdentifiers_R( dataLibrary, var1_obs_id ), '123') )

  GetEnsembleDatasetDataSubIdentifiers_R( dataLibrary, var1_fcast_ens_id )


  for (d in expectedIds) 
    print( GetEnsembleDatasetDataSubIdentifiers_R(dataLibrary, d) )

    })



