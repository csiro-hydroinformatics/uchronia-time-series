# library(devtools) ; load_all('F:/src/csiro/stash/datatypes/tests/R/datadev')
# document('F:/src/csiro/stash/datatypes/tests/R/datadev')
# build('F:/src/csiro/stash/datatypes/tests/R/datadev')
# install('F:/src/csiro/stash/datatypes/tests/R/datadev')

# library(devtools) ; load_all('C:/src/csiro/stash/per202/datatypes/tests/R/datadev')
# document('C:/src/csiro/stash/per202/datatypes/tests/R/datadev')
# build('C:/src/csiro/stash/per202/datatypes/tests/R/datadev')
# install('C:/src/csiro/stash/per202/datatypes/tests/R/datadev')

#' TODO
#' 
#' default test station integer identifiers.
#' 
#' @export
defaultStationIds <- function() { return(c(123,456)) }

#' TODO
#' 
#' Create a test data file 
#' 
#' @export
createTestSwiftNcFile <- function(fname, timeDimInfo, varDef, stationIds=defaultStationIds(),nLead, nEns, ncAttributes=ncSwift::createVariableAttrDef(), overwrite=FALSE) {

  if(overwrite) {
    if(file.exists(fname)) {
      file.remove(fname)
    }
  }

  if(!file.exists(fname)) {
    varNames <- varDef$name
    nTimeSteps <- length(timeDimInfo$values)
    snc <- ncSwift::sncCreate(fname, timeDimInfo, varDef, stationIds=defaultStationIds(),ncAttributes=ncAttributes, leadLength=nLead, ensLength=nEns)
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
    snc <- ncSwift::sncOpen(fname)
  }
  return(snc)
}

#' TODO
#' 
#' Create a test data file for https://jira.csiro.au/browse/WIRADA-407
#' 
#' @export
createFileWithDegenerateDimensions <- function(outputDataDir, stationIds=defaultStationIds(), nTimeSteps=48, nEns=1, nLead=1, 
  timeAxisStart = ISOdate(year=2010, month=08, day=01, hour = 14, min = 0, sec = 0, tz = "UTC"),
  overwrite=TRUE) {

  fname <- file.path(outputDataDir, 'testswift_w407.nc')
  if(file.exists(fname)) {
    if(overwrite) {
      file.remove(fname)
    } else {
      stop(paste0('File already exists: ', fname))
    }
  }

  ncAttributes <- ncSwift::createVariableAttrDef()
  timeDimInfo <- ncSwift::createTimeInfo(from=timeAxisStart, n=nTimeSteps, tStep='hours since', tStepDelta=1L)
  varNames <- c('var1_obs_4d','var2_obs_4d', 'var1_obs_3d', 'var2_obs_3d')
  varDef <- ncSwift::createVariableDefDataframe(varNames=varNames, longNames = paste(varNames, 'synthetic data'))
  varDef$dimensions <- c('4','4','3','3')
  # options(error=recover)
  snc <- createTestSwiftNcFile(fname, timeDimInfo, varDef, stationIds, nLead, nEns)
  snc$close()
  rm(snc)
}


# 2016-06 
# creating files for testing ensemble retrospective forecasts time series on multiple netCDF files

datetxtformat <- function() {"%Y%m%d%H"}

# adapted from joki::makeTextDate

#' @export
makeTextDate <- function (instant, tz = "UTC", txtformat = datetxtformat()) 
{
    instant <- as.POSIXct(instant)
    format(instant, format = txtformat, tz = tz, usetz = FALSE)
}

#' TODO
#' 
#' Create a test data file
#' 
#' @export
mkMultiNcFileEnsembleTs <- function(varNames='rainfall_fcast', stationIds=defaultStationIds(), nLead, nEns, fileStampDates, dimensions='4', offsetFromFileStamp=lubridate::hours(0), outputDataDir, rootFileName="testrainfall", tStep='hours since', tStepDelta=1L) {
  varDef <- ncSwift::createVariableDefDataframe(varNames=varNames, longNames = paste(varNames, 'synthetic data'))
  varDef$dimensions <- dimensions
  for (i in 1:length(fileStampDates)) {
    xt <- fileStampDates[i]
    fname <- file.path(outputDataDir, paste0(rootFileName, makeTextDate(xt, txtformat=datetxtformat()),".nc"))
    if(file.exists(fname)) { file.remove(fname) }
    startTime <- xt + offsetFromFileStamp
    timeDimInfo <- ncSwift::createTimeInfo(from=startTime, n=1, tStep, tStepDelta)
    snc <- createTestSwiftNcFile(fname, timeDimInfo, varDef, stationIds, nLead, nEns)
    snc$close()
  }
  invisible(return(NULL))
}
