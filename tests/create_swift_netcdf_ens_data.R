# NOTE: may need to be run on R 32 bits only, due to an issue with ncdf4 depending on compiler used.

# library(devtools)
# document('f:/src/csiro/stash/netcdf-tools/R/pkgs/ncSwift')
# install('f:/src/csiro/stash/netcdf-tools/R/pkgs/ncSwift')

# load_all('f:/src/csiro/stash/netcdf-tools/R/pkgs/ncSwift')


library(datadev)
library(ncSwift)

outputDataDir <- 'c:/STSF/tmp'
if (!(dir.exists(outputDataDir))) { dir.create(outputDataDir, recursive=TRUE) }

fname <- file.path(outputDataDir, 'testswift_2015.nc')
file.remove(fname)

stationIds <- c(123,456)
nEns <- 3
nLead <- 4
nTimeSteps <- 48

timeAxisStart <- ISOdate(year=2010, month=08, day=01, hour = 14, min = 0, sec = 0, tz = "UTC")
timeDimInfo <- createTimeInfo(from=timeAxisStart, n=nTimeSteps, tStep='hours since', tStepDelta=1L)
varNames <- c('var1_fcast_ens','var2_fcast_ens', 'var1_obs', 'var2_obs', 'var1_ens','var2_ens')
varDef <- createVariableDefDataframe(varNames=varNames, longNames = paste(varNames, 'synthetic data'))
varDef$dimensions <- c('4','4','2','2','3','3')

snc <- createTestSwiftNcFile(fname, timeDimInfo, varDef, stationIds, nLead, nEns)

td <- snc$getTimeDim()
snc$getEnsFcast('var1_fcast_ens', 123, startTime=td[23])
snc$getEnsFcast('var2_fcast_ens', 123, startTime=td[23])
snc$getEnsFcast('var1_fcast_ens', 456, startTime=td[23])
snc$getEnsFcast('var2_fcast_ens', 456, startTime=td[23])

head(snc$getSingleSeries('var1_obs', 123))
head(snc$getSingleSeries('var2_obs', 123))
head(snc$getSingleSeries('var1_obs', 456))
head(snc$getSingleSeries('var2_obs', 456))

head(snc$getEnsSeries('var1_ens', 123))
head(snc$getEnsSeries('var2_ens', 123))
head(snc$getEnsSeries('var1_ens', 456))
head(snc$getEnsSeries('var2_ens', 456))
tail(snc$getEnsSeries('var2_ens', 456))


snc$close()
rm(snc)


#################################################
# Create a test data file for https://jira.csiro.au/browse/WIRADA-407

outputDataDir <- 'c:/STSF/tmp'

fname <- file.path(outputDataDir, 'testswift_w407.nc')
file.remove(fname)

stationIds <- c(123,456)
nTimeSteps <- 48
nEns <- 1
nLead <- 1
timeAxisStart <- ISOdate(year=2010, month=08, day=01, hour = 14, min = 0, sec = 0, tz = "UTC")
timeDimInfo <- createTimeInfo(from=timeAxisStart, n=nTimeSteps, tStep='hours since', tStepDelta=1L)
varNames <- c('var1_obs_4d','var2_obs_4d', 'var1_obs_3d', 'var2_obs_3d')
varDef <- createVariableDefDataframe(varNames=varNames, longNames = paste(varNames, 'synthetic data'))
varDef$dimensions <- c('4','4','3','3')

snc <- createTestSwiftNcFile(fname, timeDimInfo, varDef, stationIds, nLead, nEns)

snc$close()
rm(snc)

# 2016-06 
# creating files for testing ensemble retrospective forecasts time series on multiple netCDF files
datetxtformat <- "%Y%m%d%H"

fileStampDates <- ISOdate(year=2010, month=08, day=01, hour = 00, min = 0, sec = 0, tz = "UTC")
fileStampDates <- fileStampDates + days(0:13)

# PET data.
mkMultiNcFileEnsembleTs(varNames='pet_fcast', fileStampDates=fileStampDates, offsetFromFileStamp=hours(0), nLead=nLead, nEns=nEns, outputDataDir=outputDataDir, rootFileName="testpet")
# Rainfall data.
# We deliberately generate dates in the file names that are different from the real time stamp, 
# to test that time series retrieval schemes are doing the right thing in SWIFT.
mkMultiNcFileEnsembleTs(varNames='rainfall_fcast', fileStampDates=fileStampDates, offsetFromFileStamp=hours(9), nLead=nLead, nEns=nEns, outputDataDir=outputDataDir, rootFileName="testrainfall")


#################################################
# Create a test data files for https://jira.csiro.au/browse/WIRADA-417

library(datadev)
library(ncSwift)
options(error=recover)
outputDataDir <- 'c:/STSF/tmp'

nTimeSteps <- 48
timeAxisStart <- ISOdate(year=2010, month=08, day=01, hour = 14, min = 0, sec = 0, tz = "UTC")
timeDimInfo <- createTimeInfo(from=timeAxisStart, n=nTimeSteps, tStep='hours since', tStepDelta=1L)

fname <- file.path(outputDataDir, 'testswift_w417_1.nc')
file.remove(fname)

stationIds <- 1
nEns <- 1
nLead <- 1
varNames <- c('var_single')
varDef <- createVariableDefDataframe(varNames=varNames, longNames = paste(varNames, 'synthetic data'))
varDef$dimensions <- c('2')
snc <- createTestSwiftNcFile(fname, timeDimInfo, varDef, stationIds, nLead, nEns)
snc$close()
rm(snc)

fname <- file.path(outputDataDir, 'testswift_w417_2.nc')
file.remove(fname)
stationIds <- c(123,456)
nEns <- 1
nLead <- 1
varNames <- c('var_multi_stations')
varDef <- createVariableDefDataframe(varNames=varNames, longNames = paste(varNames, 'synthetic data'))
varDef$dimensions <- c('2')
snc <- createTestSwiftNcFile(fname, timeDimInfo, varDef, stationIds, nLead, nEns)
snc$close()
rm(snc)
