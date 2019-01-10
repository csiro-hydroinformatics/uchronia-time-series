import uchronia_pb as m
import cinteroppyb11 as c
import numpy as np
import datetime as dt
import pandas as pd
import pytz


#' mainTimeAxis
#'
#' Creates information on the time axis of a test time series of ensemble of time series: start date, end date, first date with an ensemble  
#'
#' @param deltaT time step in seconds.
#' @param n length of the time series
#' @param p index of the first ensemble in the time series.
#' @return a list of datetimes 
def mainTimeAxis(deltaT, n, p=n):
    state_date = dt.datetime(1970, 1, 1, 0, 0,
                                           tzinfo=pytz.utc)
    return dict(
        tsStartDate = state_date,
        tsEndDate = state_date + dt.timedelta(seconds=deltaT*n),
        tsStartEns = state_date + dt.timedelta(seconds=deltaT*p)
    )

def buildTestEnsTs(i, seed=None, deltaT=3600, n=100, nEns=4):
  tt = mainTimeAxis(deltaT, n)
  return buildTestHourlyXts(i, tt['tsStartDate'], seed=seed, n=n, nEns=nEns, deltaT=deltaT)

def to_daily_time_series(data_array, start_date):
    tseries_index = pd.date_range(start_date, periods=data_array.shape[0], freq='D')
    # return pd.Series(data_array, index=tseries_index)
    df = pd.DataFrame(data_array)
    df['date'] = tseries_index
    df = df.set_index('date')
    return df

def _is_negative(x): return x < 0

def mkSeriesRegularTstep (startDate, x, isMissingFunc = _is_negative, deltaTSec=3600):
    #   stopifnot(is(startDate, "POSIXct"))
    if not isMissingFunc is None:
        isMissing = isMissingFunc(x)
        y = x.copy()
        y[isMissing] = np.nan
    else:
        y = x
    return to_daily_time_series(y, startDate)

def buildTestHourlyXts(i, originDate, seed=None, n=5, nEns=4, deltaT=3600):
    if seed is None: seed = 456
    np.random.seed(seed)
    x = np.random.rand(n, nEns)
    return mkSeriesRegularTstep(originDate+dt.timedelta(hours=i), (x+i), None, deltaT)

def as_interop_dts(d):
    x = c.DateTimeToSecond()
    x.year = d.year
    x.month = d.month
    x.day = d.day
    x.hour = d.hour
    x.minute = d.minute
    x.second = d.second
    return x

def createEnsembleForecastTimeSeries(start, n, deltaT):
    s = as_interop_dts(start)
    # HACK
    return m.CreateEnsembleForecastTimeSeries(s, n, 'daily')

def asInteropRegularTimeSeries(pandas_df):
    # HACK
    hack_tstep = 86400
    n = pandas_df.shape[0]
    nEns = pandas_df.shape[1]
    mts = c.MultiRegularTimeSeries()
    mts.clear_numeric_data()
    mts.ensemble_size = nEns
    geom = c.RegularTimeSeriesGeometry()
    geom.length = n
    geom.start = as_interop_dts(pandas_df.index[0].to_pydatetime())
    geom.time_step_seconds = hack_tstep
    mts.time_series_geometry = geom
    mts.set_numeric_data(pandas_df.values)
    return mts

asInteropRegularTimeSeries(multiTimeSeriesIn)

def setItem(ensFcTs, i, value):
value = multiTimeSeriesIn
i = 0
mts = asInteropRegularTimeSeries(value)
m.SetItemEnsembleForecastTimeSeriesAsStructure(ensFcTs, i, mts)
m.SetItemEnsembleTimeSeriesAsStructure(ensFcTs, i, mts)


def uchronia.setItem(ensFcTs, i, multiTimeSeriesIn):

def buildTestTsEnsTs(deltaT=3600, n=100, p=80, deltaTLead=180, nLead=5, nEns=4):
    # if(p > n) 
    # stop("start of non-null ensemble is past the end of the time series")
    tt = mainTimeAxis(deltaT, n, p)
    ensFcTs = createEnsembleForecastTimeSeries(tt['tsStartDate'], n, deltaT)
    for i in range(p, n):
        multiTimeSeriesIn = buildTestEnsTs(i,n=nLead, deltaT=deltaTLead)
        uchronia.setItem(ensFcTs, i, multiTimeSeriesIn)
    return ensFcTs


deltaT = 3600*3
deltaTLead = 180
n = 20
p = 8
nLead = 5
nEns = 4
tt = mainTimeAxis(deltaT, n, p)
ensFcTs = createEnsembleForecastTimeSeries(tt['tsStartDate'], n, deltaT)
for i in range(p, n):
    multiTimeSeriesIn = buildTestEnsTs(i,n=nLead, deltaT=deltaTLead)
    uchronia.setItem(ensFcTs, i, multiTimeSeriesIn)
return ensFcTs


def test_getting_data_from_efts(): 
deltaT = 3600*3
deltaTLead = 180
n = 20
p = 8
nLead = 5
nEns = 4
ensFcTs = buildTestTsEnsTs(deltaT=deltaT,deltaTLead=deltaTLead,n=n,p=p,nLead=nLead,nEns=nEns)
q = n-p-1
tt = mainTimeAxis(deltaT, n, p)
for i in range(n):
    ensTs = uchronia.getItem(ensFcTs, i)
    if i < p:
        assert np.isnan(ensTs)
    else:
        multiTimeSeriesExpected = buildTestHourlyXts(i, tt['tsStartDate'], n=nLead, nEns=nEns,deltaT=deltaTLead)
        expect_true(all(ensTs == multiTimeSeriesExpected))

# test_that("Splicing - round trip setting/getting data from an ensemble of time series", {
def test_splicing():
    deltaT = 3600*3
    n = 100
    # nLead = 5
    nEns = 4
    # q = n-p-1
    ensTsXts = buildTestEnsTs(1, deltaT=deltaT,n=n,nEns=nEns)
    ensTs = asUchroniaData(ensTsXts);
    for iEns in range(0, nEns):
        theTs = uchronia.getItem(ensTs, iEns)
        expected = ensTsXts[,iEns]
        expect_true(all(xts.index.xts(expected )== xts.index.xts(theTs)))
        expect_true(all(expected == theTs))
    # we create another set of data, to test the setting of each item series in the collection ensTs.
    newEnsTsXts = buildTestEnsTs(1, seed = 9876, deltaT=deltaT,n=n,nEns=nEns)
    for iEns in range(0, nEns):
        uchronia.setItem(ensTs, iEns, newEnsTsXts[,iEns])
    for iEns in range(0, nEns):
        theTs = uchronia.getItem(ensTs, iEns)
        expected = newEnsTsXts[,iEns]
        expect_true(all(xts.index.xts(expected ) == xts.index.xts(theTs)))
        expect_true(all(expected == theTs))

# The following test is still in swift land due to legacy txt ts reader.
# test_that("Retrieval of data in time series libraries", {

# test_that("Time indices are correctly retrieved", {
def test_time_indices_correctly_retrieved():
    deltaT = 3600 * 3
    deltaTLead = 3600 / 4
    n = 20
    p = 8
    nLead = 5
    nEns = 4
    ensFcTs = buildTestTsEnsTs(deltaT=deltaT,deltaTLead=deltaTLead,n=n,p=p,nLead=nLead,nEns=nEns)
    tt = timeIndex(ensFcTs)
    tst = mainTimeAxis(deltaT, n, p)
    expectedTaxis = tst['tsStartDate'] + (0:(n-1)) * deltaT
    expect_true(all(expectedTaxis == tt))


# test_that("data dimensions are correctly retrieved", {
def test_data_dimensions_correctly_retrieved():    
    deltaT = 180
    n = 10
    nEns = 4

    x = buildTestEnsTs(1,123,deltaT=deltaT, n=n, nEns=nEns)
    g = geometryOf(x)

    expect_equal(nEns, g$ensemble$size)
    expect_equal(n, g$temporal$size)
    expect_equal(as.integer(dt.timedelta(deltaT)), as.integer(g$temporal$time_step))
    expect_equal(lubridate::as.duration(dt.timedelta(deltaT)), g$temporal$time_step)

    ux = asUchroniaData(x)
    g = geometryOf(ux)

    expect_equal(nEns, g$ensemble$size)
    expect_equal(n, g$temporal$size)
    expect_equal(as.integer(dt.timedelta(deltaT)), as.integer(g$temporal$time_step))
    expect_equal(lubridate::as.duration(dt.timedelta(deltaT)), g$temporal$time_step)

    deltaTMain = 3*3600
    deltaTLead = 3600 / 4
    # we deliberately leave missing values in the first three items... Tricky!
    p = 3
    nLead = 8

    ensFcTs = buildTestTsEnsTs(deltaT=deltaTMain, deltaTLead=deltaTLead,n=n,p=p,nLead=nLead,nEns=nEns)
    g = geometryOf(ensFcTs)

    tst = mainTimeAxis(deltaT, n, p=p)
    expect_equal(n, g$temporal$size)
    expect_equal(tst['tsStartDate'], g$temporal$start)

    # given   p = 3   getting the geometry of the content is non-trivial! still TODO
    testthat::fail("getting the geometry of a TS of ensemble of TS is tricky if there are missing values!")
    expect_equal(as.integer(dt.timedelta(deltaTMain)), as.integer(g$temporal$time_step))
    expect_equal(lubridate::as.duration(dt.timedelta(deltaT)), g$temporal$time_step)


