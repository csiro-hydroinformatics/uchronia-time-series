# ---
# jupyter:
#   jupytext:
#     formats: ipynb,py:percent
#     text_representation:
#       extension: .py
#       format_name: percent
#       format_version: '1.3'
#       jupytext_version: 1.14.0
#   kernelspec:
#     display_name: HFC
#     language: python
#     name: hydrofc
# ---

# %%

# %%
from pathlib import Path
import uchronia.data_set as uds
from uchronia.sample_data import sample_data_dir
import os

## -----------------------------------------------------------------------------
s = sample_data_dir()
data_library_file = os.path.join(s, 'time_series_library.yaml')
assert Path(data_library_file).exists()

# %%
## -----------------------------------------------------------------------------
data_library = uds.get_ensemble_dataset(data_library_file, '')
data_ids = data_library.get_dataset_ids()
data_ids

## -----------------------------------------------------------------------------
single_obs = data_library.get_dataset("var1_obs")
single_obs
asXts = uds.as_xarray
asXts(single_obs).plot()

import uchronia.time_series as uts
## -----------------------------------------------------------------------------
stationIds =  data_library.sub_identifiers("var1_obs_collection")
print(stationIds)
multiObs = data_library.get_dataset("var1_obs_collection")

multiObs

multiObs = asXts(multiObs)
multiObs.head()
multiObs.plot()

## -----------------------------------------------------------------------------
uds.datasets_summaries(data_library)

## -----------------------------------------------------------------------------
fcasts = data_library.get_dataset("var2_fcast_ens")
fcasts
# TODO: currently conversion ot xts is the default. Review:
firstFcast = fcasts.get_item(1)
firstFcast.head()
firstFcast.plot()

# -


# %% [markdown]
# # Ensemble SWIFT model runs

# %% [markdown]
# ## About this document
#
# ## Elaboration

# %%
import pandas as pd
import numpy as np
import xarray as xr

# %%
## -----------------------------------------------------------------------------
import matplotlib.pyplot as plt

# %%
# %matplotlib inline

# %% [markdown]
# the package `uchronia` includes facilities to access time series from a "library", akin to what you would do to manage books.

# %%
import uchronia.sample_data as usd

# %%
doc_data_path = usd.sample_data_dir()
data_path = os.path.join(doc_data_path, 'UpperMurray')
data_path

# %%

# %%
data_library = usd.sample_time_series_library('upper murray')

# %%
data_library

# %%
data_ids = data_library.get_dataset_ids()

# %%
data_ids

# %%
data_library.datasets_summaries()
# 'rain_obs'
# 'pet_obs'

# %%
x = data_library.get_dataset('rain_obs')

# %%
b = uts.as_xarray(x);


# %%
def _is_positive(x): return x > 0
# b[_is_negative(b)]


# %%
b.where(lambda arr: _is_positive(arr), np.nan).plot()

# %%
from cinterop.cffi.marshal import as_timestamp

# %% [markdown]
# First, before demonstrating ensemble forecasting simulations, let's demonstrate how we can get a snapshot of the model states at a point in time and restore it later on, hot-starting further simulation.

# %%
ens_rain = data_library.get_dataset('rain_fcast_ens')

# %%
one_forecast = ens_rain.get_item(1)

# %%
one_forecast

# %%
q = one_forecast.quantile([0.05, .25, .5, .75, 0.95], 'ensemble')

# %%
fig, ax = plt.subplots(figsize=(16,9))
ax.fill_between(q.time.values, q.sel(quantile=0.05), q.sel(quantile=0.95), alpha=0.3, label='Perc. 50-95')
ax.fill_between(q.time.values, q.sel(quantile=0.25), q.sel(quantile=.75), alpha=0.5, label='Perc. 25-75')
ax._get_lines.get_next_color()  # Hack to get different line
ax.plot(q.time.values, q.sel(quantile=.5), linewidth=2, label='Median')
ax.legend()
plt.show()

# %%

# %%





