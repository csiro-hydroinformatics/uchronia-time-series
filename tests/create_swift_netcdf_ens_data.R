# NOTE: may need to be run on R 32 bits only, due to an issue with ncdf4 depending on compiler used.

library(devtools)
document('f:/src/csiro/stash/netcdf-tools/R/pkgs/ncSwift')
install('f:/src/csiro/stash/netcdf-tools/R/pkgs/ncSwift')

# load_all('f:/src/csiro/stash/netcdf-tools/R/pkgs/ncSwift')


# library(ncSwift)

fname <- 'f:/STSF/tmp/testswift_2015.nc'
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

if(!file.exists(fname)) {
  snc <- sncCreate(fname, timeDimInfo, varDef, stationIds, ncAttributes=createVariableAttrDef(), leadLength=nLead, ensLength=nEns)
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

  for (i in 1:length(td)) {
    for (j in 1:length(stationIds)) {
      station <- stationIds[j]
      var1Values <- i + 0.1*j + m
      var2Values <- 2*var1Values
      dtime = td[i]
      snc$putEnsFcast(var1Values, varName = varNames[1], identifier = station, startTime = dtime)
      snc$putEnsFcast(var2Values, varName = varNames[2], identifier = station, startTime = dtime)
    }
  }

  timeSteps <- 1:length(td)
  for (j in 1:length(stationIds)) {
    var3Values <- timeSteps + 0.1*j
    var4Values <- var3Values + 0.01*timeSteps + 0.001*j

    station <- stationIds[j]
    snc$putSingleSeries(var3Values, varName = varNames[3], identifier = station)
    snc$putSingleSeries(var4Values, varName = varNames[4], identifier = station)
  }

  for (j in 1:length(stationIds)) {
  
    var5Xts <- matrix(rep(1:nEns, each=nTimeSteps) + timeSteps + 0.1*j, ncol=nEns)

    var5Values <- t(var5Xts) # [time,ens_member] to [ens_member,time], as expected by putEnsSeries
    var6Values <- 0.25 * var5Values
    
    station <- stationIds[j]
    snc$putEnsSeries(var5Values, varName = varNames[5], identifier = station)
    snc$putEnsSeries(var6Values, varName = varNames[6], identifier = station)
  }
} else { # if(!file.exists(fname)) {
  snc <- sncOpen(fname)
}


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

# ncfile <- snc$ncfile 
