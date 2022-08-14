# Uchronia - time series handling for ensembles simulations and forecasts in C++

## Overview

Uchronia is a C++ library designed to handle multidimensional time series and ensemble thereof. [Uchronia](https://en.wikipedia.org/wiki/Uchronia) is a literary neologism for a hypothetical or fictional time-period of our world. This seems a suitable name for a library primarily designed notably to handle retrospective ensemble forecast time series.

Some key capabilities are:

* Off the shelf support for point time series and time series of ensemble forecast
* Extensibility to various data types (numeric or not) through C++ templates, e.g. to a "time series of spatial grids"
* High level language bindings for R, Python, Matlab, and others via a consistent "C" API.

## Design goals

* Consistent handling of multidimensional time series across programming languages, from prototyping to operations
* Prevent wasting resource in I/O code duplication and idiosyncratic data structure across (matlab/R/Python/etc.) implementations
* Scalable from interactive exploratory data analysis to high-performance cluster computing
* Promote high level data identification for provenance and data integrity

## License

MIT-derived CSIRO license (see [License.txt](./LICENSE.txt))

Please contact David Robertson (david.robertson@csiro.au) if you want to reuse this library, to facilitate the maintenance of IP registers.

## Getting started

"Getting started" with Uchronia may mean:

* Using existing capabilities
* Using Uchronia C++ templates in your C++ code to create new types of time series suited to your context

### Using off-the-shelf

_uchronia_ is largely header-only but can be installed as a shared library, for off the shelf use.

On Linux:

```sh
ROOT_DIR=~/src/uchronia-time-series

INSTALL_PREFIX=/usr/local

CXX_COMP=-DCMAKE_CXX_COMPILER=g++
C_COMP=-DCMAKE_C_COMPILER=gcc
CM_PREF=-DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
CM="cmake ${CXX_COMP} ${C_COMP} ${CM_PREF} -DBUILD_SHARED_LIBS=ON .."

# placeholder: depending on Linux machine getting yaml-cpp, moirai, netCDF, cinterop, and Boost packages pre-installed
cd ${ROOT_DIR}/datatypes
mkdir -p build ; cd build
$CM
make 
sudo make install
```

### R bindings

Building the package from source can be done with:

```sh
cd ${ROOT_DIR}/bindings/R/pkgs
R CMD build uchronia
R CMD INSTALL uchronia_x.y.tar.gz
```

The package includes examples in the form of built-in vignettes:

```r
library(uchronia)
browseVignettes("uchronia")
```

### Example in Python

```python
import uchronia
# etc.
```

## As a developer

### using existing types of time series:

```c++
#include "datatypes/time_series.hpp"
// <snip> other includes skipped
using namespace datatypes::timeseries;
using TS = TTimeSeries<double>;

void simplest() {
    vector<double> d = { 1,2,3,4,5,6,7 };
    auto start = ptime(date(2017, 1, 1));
    TS oneWeek(d, start, TimeStep::GetDaily());
    double val = oneWeek[start + days(2)];
    TS other = oneWeek + 3.0;
}

void ensembleForecasts() {
    size_t tsEtsLength = 31 * 6;
    size_t ensSize = 5;
    size_t tsLength = 24;
    ptime start(date(2008, 3, 4));
    auto hourly = TimeStep::GetHourly();
    TimeStep daily = TimeStep::GetDaily();
    TimeStep etsTstep = hourly * 6;
    TimeStep fcastTstep = hourly / 4;

    // shorthand type aliases
    using DTH = datatypes::tests::DataTestHelper<double>;
    using EFTS = EnsembleForecastTimeSeries<>;
    using MTSPTR = EFTS::ElementType;
    using MTS = std::remove_pointer<MTSPTR>::type;

    // a time series of ensembles of time series:
    EFTS ensTs = DTH::CreateTsEnsembleTs(
        tsEtsLength,
        ensSize,
        tsLength,
        start,
        etsTstep,
        fcastTstep);
    MTSPTR multiTs = ensTs[0];
    TS* forecast = multiTs.Get(3);
} 
```

### Extending, customizing 

uchronia is largely template based, you can create time series for other item types such as spatial grids. Thanks to the static typing of C++ and its template metaprogramming features, errors are often caught at compile time, and several operations such as arithmetic ones will "just work":

```c++ 

class SpatialGrid
{
private:
    std::vector<double> data;
public:
    double easting, northing;
    SpatialGrid operator+(const SpatialGrid& rhs) const {/*TODO*/ return SpatialGrid(); }
    SpatialGrid operator+(double rhs) const {/*TODO*/ return SpatialGrid(); }
};

TEST_CASE("Extensibility of templated time series")
{
    using SpatialTs = TTimeSeries<SpatialGrid>;

    auto start = ptime(date(2017, 1, 1));
    SpatialTs oneWeek(7, start, TimeStep::GetDaily());
    SpatialGrid val = oneWeek[start + days(2)];
    SpatialTs other = oneWeek + val;

}
```

## Background

Ensemble prediction techniques have been shown to produce more accurate predictions as well as formally quantify prediction uncertainty in a range of scientific applications. Statistically meaningful quantification of uncertainty can require very large ensembles, with associated increases in computation and data storage, particularly for predictions in the form of time series. In addition, the verification of statistical properties of an ensemble, such as the reliability of the ensemble spread, requires very long retrospective verification periods. This presents logistical and conceptual challenges for researchers and practitioners still transitioning from simulations based on deterministic, single instance realisations. A software system for handling such ensemble time series has to address several needs, notably: 

* retrospective ensemble predictions can quickly require several hundred gigabytes of data, and be accessible from workstations or on a high performance parallel compute cluster; 
* users should still have an interactive, responsive experience when processing that data, with little concern for on-disk logistics - a well-known principle that remains unsatisfactory in many implementations; and 
* the data infrastructure must promote strong data identity and versioning to help sustain a reproducible simulation outcome.   

In "Uchronia" the core entities of time series and ensembles are implemented in portable C++, using template metaprogramming. This permits a unified but versatile implementation for time series of various dimensional complexity. Time series elements can thus range from an atomic value, typically numeric, all the way up to a time series of ensemble forecasts. Object-oriented design patterns are used to allow for RAM caching of large data. A “C“ API permits convenient data manipulation from a variety of interactive higher-level technical computing languages such as Python and R. Time series can be accessed via a time series library. Time series creation and retrieval relies on string identifiers and metadata rather than the still prevalent paths to data files. Libraries can bring otherwise disparate data into a consistent data set for the simulation or analytical purpose. We envisage the time series library facilities are a solid basis for use in conjunction with federated data provenance infrastructure. 

## Acknowledgements

Contributions to this library were made to support some of the streamflow forecasting R&D under the [Water Information Research And Development Alliance](https://www.csiro.au/en/Research/LWF/Areas/Water-resources/Assessing-water-resources/WIRADA/About) between the CSIRO and the Australian Bureau of Meteorology. 
