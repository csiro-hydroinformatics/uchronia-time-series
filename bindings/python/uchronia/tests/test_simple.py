# the inclusion of the tests module is not meant to offer best practices for
# testing in general, but rather to support the `find_packages` example in
# setup.py that excludes installing the "tests" package

import os
import pandas as pd
import numpy as np
import datetime as dt
import sys
from datetime import datetime, timedelta
from cinterop.timeseries import as_pydatetime

_origin = as_pydatetime("1970-01-01")

def mainTimeAxis(deltaT, n, p=None):
    if p is None: p = n
    return {
        "tsStartDate": _origin,
        "tsEndDate": _origin + timedelta(seconds=deltaT*n),
        "tsStartEns": _origin + timedelta(seconds=deltaT*p)
        }

#' buildTestEnsTs
#'
#' buildTestEnsTs
#'
#' @param i index of the test ensemble of time series
#' @return an xts
def buildTestEnsTs(i, seed=None, deltaT=3600, n=100, nEns=4):
    tt = mainTimeAxis(deltaT, n)
    return buildTestHourlyXts(i, tt["tsStartDate"], seed=seed, n=n, nEns=nEns, deltaT=deltaT)


def buildTestHourlyXts(i, originDate, seed=None, n=5, nEns=4, deltaT=1):
    from cinterop.timeseries import mk_even_step_xarray_series, ENSEMBLE_DIMNAME
    if seed is None: seed = 456
    np.random.seed(seed)
    x = np.random.normal(size=nEns*n).reshape((n, nEns))
    return mk_even_step_xarray_series(
        data = x+i,
        start_date= originDate+timedelta(hours=i),
        time_step_seconds=deltaT,
        dim_name=ENSEMBLE_DIMNAME,
        colnames=[str(j) for j in range(nEns)]
        )

from uchronia.classes import EnsembleForecastTimeSeries

def buildTestTsEnsTs(deltaT=3600, n=100, p=80, deltaTLead=3600, nLead=5, nEns=4):
    from cinterop.timeseries import create_ensemble_forecasts_series, create_even_time_index
    if p > n:
        raise ValueError("start of non-null ensemble is past the end of the time series")
    tt = mainTimeAxis(deltaT, n, p)
    ensFcTs = EnsembleForecastTimeSeries.new(start=_origin, length=n, time_step=timedelta(seconds=deltaT))
    # ensFcTs = create_ensemble_forecasts_series(
    #     None,
    #     ens_index=np.arange(nEns),
    #     lead_time_index=np.arange(nLead) + 1,
    #     time_index=create_even_time_index(
    #         start=_origin, time_step_seconds=deltaT, n=n)
    # )
    
    for i in range(p, n):
        multiTimeSeriesIn = buildTestEnsTs(i,n=nLead, deltaT=deltaTLead, nEns=nEns)
        ensFcTs[i] = multiTimeSeriesIn
    return ensFcTs

def test_get_data_from_efts():
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
        ensTs = ensFcTs[i]
        if i < p:
            assert ensTs is None # np.all(np.isnan(ensTs))
        else:
            multiTimeSeriesExpected = buildTestHourlyXts(i, tt["tsStartDate"], n=nLead, nEns=nEns,deltaT=deltaTLead)
            assert np.all(ensTs == multiTimeSeriesExpected)


def test_round_trip_ensts():
    import uchronia.data_set as uds
    from cinterop.timeseries import ENSEMBLE_DIMNAME, TIME_DIMNAME
    deltaT = 3600*3
    n = 100
    # nLead = 5
    nEns = 4
    # q = n-p-1
    ensTsXts = buildTestEnsTs(1, deltaT=deltaT,n=n,nEns=nEns)
    ensTs = uds.as_uchronia_data(ensTsXts)
    for iEns in range(nEns):
        theTs = ensTs[iEns].to_xarray()
        expected = ensTsXts.sel({ENSEMBLE_DIMNAME: str(iEns)})
        assert np.all(expected.coords[TIME_DIMNAME] == theTs.coords[TIME_DIMNAME] )
        assert np.all(expected.values == theTs.values)
    # # we create another set of data, to test the setting of each item series in the collection ensTs.
    newEnsTsXts = buildTestEnsTs(1, seed = 9876, deltaT=deltaT,n=n,nEns=nEns)
    for iEns in range(nEns):
        ensTs[iEns] = newEnsTsXts.sel({ENSEMBLE_DIMNAME: str(iEns)})
    for iEns in range(nEns):
        theTs = ensTs[iEns].to_xarray()
        expected = newEnsTsXts.sel({ENSEMBLE_DIMNAME: str(iEns)})
        assert np.all(expected.coords[TIME_DIMNAME] == theTs.coords[TIME_DIMNAME] )
        assert np.all(expected.values == theTs.values)

def test_time_indices_retrieved():
    deltaT = 3600 * 3
    deltaTLead = 3600 // 4
    n = 20
    p = 8
    nLead = 5
    nEns = 4

    ensFcTs = buildTestTsEnsTs(deltaT=deltaT,deltaTLead=deltaTLead,n=n,p=p,nLead=nLead,nEns=nEns)
    tt = ensFcTs.time_index()
    tst = mainTimeAxis(deltaT, n, p)
    expectedTaxis = [tst['tsStartDate'] + timedelta(seconds = int(i)) for i in (np.arange(n) * deltaT)]
    assert np.all(np.array(expectedTaxis) == tt.array)

def test_data_dimensions_detection():
    import uchronia.data_set as uds
    deltaT = 180
    n = 10
    nEns = 4
  
    x = buildTestEnsTs(1,123,deltaT=deltaT, n=n, nEns=nEns)
    x = uds.as_uchronia_data(x)

    # TODO?
    # g = x.geometry()
    # 
    # expect_equal(nEns, g$ensemble$size)
    # expect_equal(n, g$temporal$size)
    # expect_equal(as.integer(lubridate::seconds(deltaT)), as.integer(g$temporal$time_step))
    # expect_equal(lubridate::as.duration(lubridate::seconds(deltaT)), g$temporal$time_step)
    # 
    # ux = asUchroniaData(x)
    # g = geometryOf(ux)
    # 
    # expect_equal(nEns, g$ensemble$size)
    # expect_equal(n, g$temporal$size)
    # expect_equal(as.integer(lubridate::seconds(deltaT)), as.integer(g$temporal$time_step))
    # expect_equal(lubridate::as.duration(lubridate::seconds(deltaT)), g$temporal$time_step)


if __name__ == "__main__":
    blah = test_time_indices_retrieved()
