# ---
# jupyter:
#   jupytext:
#     formats: ipynb,py:percent
#     text_representation:
#       extension: .py
#       format_name: percent
#       format_version: '1.3'
#       jupytext_version: 1.13.8
#   kernelspec:
#     display_name: HFC
#     language: python
#     name: hydrofc
# ---

# %%
import uchronia.uchronia_data_set as uds

# %%
import os

## -----------------------------------------------------------------------------
sample_data_dir = "/home/per202/data/documentation"
data_library_file = os.path.join(sample_data_dir, 'time_series_library.yaml')

# %%
## -----------------------------------------------------------------------------
data_library = uds.get_ensemble_dataset(data_library_file, '')
data_ids = uds.get_dataset_ids(data_library)
data_ids


# %%
single_obs = uds.get_dataset(data_library, "var1_obs")

# %%
single_obs

# %%
uds.as_xarray(single_obs)

# %%
single_obs

# %%
uds.as_xarray(single_obs)

# %%
multi_obs = uds.get_dataset(data_library, "var1_obs_collection")
multi_obs


# %%
import uchronia.uchronia_time_series as uts
station_ids = uts.sub_identifiers(data_library, "var1_obs_collection")
print(station_ids)

# %%
uds.as_xarray(multi_obs)
