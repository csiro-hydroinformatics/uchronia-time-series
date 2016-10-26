# NOTE: may need to be run on R 32 bits only, due to an issue with ncdf4 depending on compiler used.

# library(devtools)
# document('f:/src/csiro/stash/netcdf-tools/R/pkgs/ncSwift')
# install('f:/src/csiro/stash/netcdf-tools/R/pkgs/ncSwift')

# load_all('f:/src/csiro/stash/netcdf-tools/R/pkgs/ncSwift')


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

ncAttributes=createVariableAttrDef()

createTestSwiftNcFile <- function(fname, timeDimInfo, varDef, stationIds, nLead, nEns) {
  if(!file.exists(fname)) {
    varNames <- varDef$name
    nTimeSteps <- length(timeDimInfo$values)
    snc <- sncCreate(fname, timeDimInfo, varDef, stationIds, ncAttributes=ncAttributes, leadLength=nLead, ensLength=nEns)
    td <- snc$getTimeDim()
    
    m <- matrix(ncol=nEns, nrow=nLead)
    for (rnum in 1:nLead) {
      for (cnum in 1:nEns) {
        m[rnum,cnum] = rnum*0.01 + cnum*0.1
      }
    }

    #      [,1] [,2] [,3]
    # [1,] 0.11 0.21 0.31
    # [2,] 0.12 0.22 0.32
    # [3,] 0.13 0.23 0.33
    # [4,] 0.14 0.24 0.34

    v4 <- varNames[which(varDef$dimensions=='4')]
    if(length(v4)>0) {
    for (i in 1:length(td)) {
      for (j in 1:length(stationIds)) {
        station <- stationIds[j]
        var1Values <- i + 0.1*j + m
        dtime = td[i]
        for (k in 1:length(v4)) {
          snc$putEnsFcast(k*var1Values, varName = v4[k], identifier = station, startTime = dtime)
        }
      }
    }
    }

    v2 <- varNames[which(varDef$dimensions=='2')]
    timeSteps <- 1:length(td)
    if(length(v2)>0) {
    for (k in 1:length(v2)) {
      for (j in 1:length(stationIds)) {
        dummy <- k %% 2 # KLUDGE just for backward compat reasons - existing unit tests.
        varValues <- (timeSteps + 0.1*j)
        varValues <- varValues + (1-dummy) * (0.01*timeSteps + 0.001*j)

        station <- stationIds[j]
        snc$putSingleSeries(varValues, varName = v2[k], identifier = station)
      }
    }
    }

    v3 <- varNames[which(varDef$dimensions=='3')]
    if(length(v3)>0) {
    for (k in 1:length(v3)) {
      for (j in 1:length(stationIds)) {
        dummy <- k %% 2 # KLUDGE just for backward compat reasons - existing unit tests.
        var5Xts <- matrix(rep(1:nEns, each=nTimeSteps) + timeSteps + 0.1*j, ncol=nEns)

        varValues <- t(var5Xts) # [time,ens_member] to [ens_member,time], as expected by putEnsSeries
        if(dummy==0) {
          varValues <- 0.25 * varValues
        }
        station <- stationIds[j]
        snc$putEnsSeries(varValues, varName = v3[k], identifier = station)
      }
    }
    }
  } else { # if(!file.exists(fname)) {
    snc <- sncOpen(fname)
  }
  return(snc)
}

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

library(ncSwift)
outputDataDir <- 'c:/STSF/tmp'

fname <- file.path(outputDataDir, 'testswift_w407.nc')
file.remove(fname)

stationIds <- c(123,456)
nTimeSteps <- 48

ncAttributes <- createVariableAttrDef()

nEns <- 1
nLead <- 1
timeAxisStart <- ISOdate(year=2010, month=08, day=01, hour = 14, min = 0, sec = 0, tz = "UTC")
timeDimInfo <- createTimeInfo(from=timeAxisStart, n=nTimeSteps, tStep='hours since', tStepDelta=1L)
varNames <- c('var1_obs_4d','var2_obs_4d', 'var1_obs_3d', 'var2_obs_3d')
varDef <- createVariableDefDataframe(varNames=varNames, longNames = paste(varNames, 'synthetic data'))
varDef$dimensions <- c('4','4','3','3')
# options(error=recover)
snc <- createTestSwiftNcFile(fname, timeDimInfo, varDef, stationIds, nLead, nEns)

snc$close()
rm(snc)


# 2016-06 
# creating files for testing ensemble retrospective forecasts time series on multiple netCDF files
datetxtformat <- "%Y%m%d%H"

fileStampDates <- ISOdate(year=2010, month=08, day=01, hour = 00, min = 0, sec = 0, tz = "UTC")
fileStampDates <- fileStampDates + days(0:13)

# adapted from joki::makeTextDate
makeTextDate <- function (instant, tz = "UTC", txtformat = datetxtformat) 
{
    instant <- as.POSIXct(instant)
    format(instant, format = txtformat, tz = tz, usetz = FALSE)
}

# PET data.
varNames <- c('pet_fcast')
varDef <- createVariableDefDataframe(varNames=varNames, longNames = paste(varNames, 'synthetic data'))
varDef$dimensions <- c('4')
for (i in 1:length(fileStampDates)) {
  xt <- fileStampDates[i]
  fname <- file.path(outputDataDir, paste0("testpet", makeTextDate(xt, txtformat=datetxtformat),".nc"))
  if(file.exists(fname)) { file.remove(fname) }
  timeDimInfo <- createTimeInfo(from=xt, n=1, tStep='hours since', tStepDelta=1L)
  snc <- createTestSwiftNcFile(fname, timeDimInfo, varDef, stationIds, nLead, nEns)
  snc$close()
}

# Rainfall data.
# We deliberately generate dates in the file names that are different from the real time stamp, 
# to test that time series retrieval schemes are doing the right thing in SWIFT.
varNames <- c('rainfall_fcast')
varDef <- createVariableDefDataframe(varNames=varNames, longNames = paste(varNames, 'synthetic data'))
varDef$dimensions <- c('4')
for (i in 1:length(fileStampDates)) {
  xt <- fileStampDates[i]
  fname <- file.path(outputDataDir, paste0("testrainfall", makeTextDate(xt, txtformat=datetxtformat),".nc"))
  if(file.exists(fname)) { file.remove(fname) }
  startTime <- xt + hours(9) # 9AM to 9AM rainfall thing, why not.
  timeDimInfo <- createTimeInfo(from=startTime, n=1, tStep='hours since', tStepDelta=1L)
  snc <- createTestSwiftNcFile(fname, timeDimInfo, varDef, stationIds, nLead, nEns)
  snc$close()
}
# ncfile <- snc$ncfile 
