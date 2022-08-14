# ---
# jupyter:
#   jupytext:
#     formats: py:percent,ipynb:percent
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

# %% [markdown]
#
# # Introduction
#
# `uchronia` is a package to handle time series of varying geometry and dimensionality, using a C++ library for efficient handling "under the hood", but with conversion to `xarray` time series in Python when suitable. The `uchronia` Python package also has siblings in R and MATLAB.
#
# # Creating multidimensional time series
#
#
#

# %%
from pathlib import Path
import uchronia.data_set as uds
from uchronia.sample_data import sample_data_dir
import os

# %%
## -----------------------------------------------------------------------------
s = sample_data_dir()
data_library_file = os.path.join(s, 'time_series_library.yaml')
assert Path(data_library_file).exists()

# %%
## -----------------------------------------------------------------------------
data_library = uds.get_ensemble_dataset(data_library_file, '')
data_ids = data_library.get_dataset_ids()
data_ids

# %%
single_obs = data_library.get_dataset("var1_obs")

# %%
single_obs

# %%
uds.as_xarray(single_obs)

# %%
single_obs

# %%
uds.as_xarray(single_obs)

# %%
multi_obs = data_library.get_dataset("var1_obs_collection")
multi_obs


# %%
station_ids = data_library.sub_identifiers("var1_obs_collection")
print(station_ids)

# %%
uds.as_xarray(multi_obs)






