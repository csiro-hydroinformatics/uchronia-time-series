Overarching questions:
* Should the creation of time series all go through a time series library?
* Should time series be accessed via R6 objects


```r
library(uchronia)
```

## Create an EFTS, populate and write out to a single file.

```R
efts <- create_efts(start_date, time_step, length)
# fill in values
# Add metadata (how/what?)
save_to(efts, filename)
```

## Create an EFTS, multi-file back end with windowed access

```R
```

## Open an nc file with one or more EFTS. metadata and data are retrievable.

```R
data_lib <- open_library(filename)
summarise(data_lib)
efts <- get_data(data_lib, some_identifier)
# read metadata (how/what?)
first_item <- efts[1]
first_item_xts <- as_xts(first_item)
```

## Open and overwrite data: what is happening to the history attribute

```R
```


