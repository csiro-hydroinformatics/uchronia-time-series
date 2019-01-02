# Uchronia data representation in python

## Purpose

This document contains the (my) thinking as of Jan 2019, regarding which 'pythonic' structures could be used to handle ensemble of time series and EFTS series. This is on the back of needing to get pySwift working since the maintenance by the BoM is unclear due to staff changes and departure.

## Background and related work

Previously, `pandas` had panels for handling multi-dimensional series (data frames). This has since been dropped. This is second hand information so I could be wrong. What seems to have picked momentum though is xarray and pangeo. We want to take that into account if we are to evolve python packages.

* [pyLatte package for climate forecast verification](https://research.csiro.au/dfp/wp-content/uploads/sites/148/2018/05/Hobart_workshop-dougie.pdf)
* [Example 3: Dealing with ensembles of forecasts](http://meteo.unican.es/work/xarray_seminar/xArray_seminar.html#example3)

Another initiative to handle multidimensional data is [xtensor python bindings](https://github.com/QuantStack/xtensor-python). The fact that xtensor is primarily about c++ rather than python-centric is appealing for interop, but interplay with xarray is unclear (and also there may be naming clashes). It has a large numpy compatibility.

## Design considerations

* Could `uchronia` be a backend to `xarray`, the way netCDF is ?

