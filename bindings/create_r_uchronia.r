
library(capigen)
load_wrapper_gen_lib('c:/src/github_jm/c-api-wrapper-generation')
uchroniaSrcPath <- 'c:/src/csiro/stash/datatypes'

# For R:
generate_uchronia_r_all_wrappers(uchroniaSrcPath)
roxy_uchronia()

# for Python
generate_uchronia_python_all_wrappers(uchroniaSrcPath)
