# draft exploratory/sample code for my MODSIM2017 paper

library(uchronia)
sampleDataDir <- uchronia::sampleDataDir()
dataLibraryFile <- file.path(sampleDataDir, 'time_series_library.yaml')
stopifnot(file.exists(dataLibraryFile))
dataLibraryFile




library(ncSwift)
wd <- '//clw-02-mel.it.csiro.au/OSM_MEL_CLW_HYDROFORECAST_common/Projects/Consulting/MD3416/Work/pearcey/SFF/ovens/ensemble'
library(ncSwift)
x <- sncOpen(file.path(wd, 'Streamflow_forecasts_201008012100.nc'))

x$getTimeDim()
x$getLeadTimeCount()
x$getValues('station_id')
blah = x$getEnsFcast('q_fcast_ens', identifier='15')
str(x)
class(blah)
str(blah)
swift::plotXtsQuantiles(blah)
which(is.na(blah))
swift::plotXtsQuantiles(blah[1:218,])



swiftr_dev() 
library(swift)

library(uchronia)
my_lib_file <- 'C:/src/csiro/stash/datatypes/doc/modsim_paper_demo.yaml'
data_dir <- '//clw-02-mel.it.csiro.au/OSM_MEL_CLW_HYDROFORECAST_common/Projects/Consulting/MD3416/Work/pearcey/SFF'
data_lib <- getEnsembleDataSet(my_lib_file, dataPath=data_dir)
getDataIdentifiers(data_lib)
# [1] "ovens_streamflow_forecasts_ens_13" "ovens_streamflow_forecasts_ens_15"
tsEnsTs <- getDataSet(data_lib, 'ovens_streamflow_forecasts_ens_15')
str(tsEnsTs)
# Formal class 'ExternalObjRef' [package "cinterop"] with 2 slots
#   ..@ obj :<externalptr> 
#   ..@ type: chr "ENSEMBLE_FORECAST_TIME_SERIES_PTR"
print(geometryOf(tsEnsTs))
# $temporal
# $temporal$start
# [1] "2010-08-01 21:00:00 UTC"
# $temporal$time_step
# [1] "86400s (~1 days)"
# etc.
fcast_times <- timeIndex(tsEnsTs)
fcast_ind <- 200

leadTimeCutoff <- 144 # just looks good enough - no other reason
fcast <- getItem(tsEnsTs, i=fcast_ind, convertToXts = TRUE)
geometryOf(fcast)

plotEnsFcast <- function(fcast_ind) {
  fcast <- getItem(tsEnsTs, i=fcast_ind, convertToXts = TRUE)
  # 2018-01 Ported to uchronia  
  plotXtsQuantiles(fcast[1:leadTimeCutoff,], probs = data.frame(low=c(.05, .1, .25), high=c(0.95, .9, 0.75)), 
   title=paste("Streamflow forecast issue time ", fcast_times[fcast_ind]), xlabel='Lead time', ylabel='m3/s') +
    ggplot2::scale_fill_brewer(palette="Blues")
}

plotEnsFcast(200)

# For the modsim presentation - illustrating what an ensemble forecast time series is:


GetEnsembleDatasetDataSummaries_R(data_lib)

library(uchronia)
my_lib_file <- '//server/path/to/ovens_library.yaml'
data_dir <- '//server/path/to/storage_root'
data_lib <- getEnsembleDataSet(my_lib_file, dataPath=data_dir)
getDataIdentifiers(data_lib)
# [1] "ovens_streamflow_forecasts_ens_13" "ovens_streamflow_forecasts_ens_15"
tsEnsTs <- getDataSet(data_lib, 'ovens_streamflow_forecasts_ens_15')
str(tsEnsTs)
# Formal class 'ExternalObjRef' [package "cinterop"] with 2 slots
#   ..@ obj :<externalptr> 
#   ..@ type: chr "ENSEMBLE_FORECAST_TIME_SERIES_PTR"
print(geometryOf(tsEnsTs))
# $temporal
# $temporal$start
# [1] "2010-08-01 21:00:00 UTC"
# $temporal$time_step
# [1] "86400s (~1 days)"
# etc.
fcast_times <- timeIndex(tsEnsTs)
fcast_ind <- 200
fcast <- getItem(tsEnsTs, i=fcast_ind, convertToXts = TRUE)
plotXtsQuantiles(fcast[1:144,], title=paste("Streamflow forecast issue time ", fcast_times[fcast_ind]), xlabel='Lead time', ylabel='m3/s')


