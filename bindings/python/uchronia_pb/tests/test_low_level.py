
import os
import numpy as np
import cinteroppyb11 as c

p = os.environ['PATH']
os.environ['PATH'] = ';'.join([p,r'C:\src\csiro\stash\datatypes\Solutions\x64\Debug',r'c:\localdev\libs\64'])

import uchronia_pb as m

d = c.DateTimeToSecond()
d.year = 2000
d.month = 1
d.day = 1

mts = c.MultiRegularTimeSeries()
mts.clear_numeric_data()
mts.ensemble_size = 1


n = 365
geom = c.RegularTimeSeriesGeometry()
geom.length = n
geom.start = d
geom.time_step_seconds = 86400

mts.time_series_geometry = geom

x = np.random.normal(size=n)
mts.set_numeric_data([x])

xts = m.CreateSingleTimeSeriesDataFromStruct(mts)

assert m.__version__ == '0.0.1'
