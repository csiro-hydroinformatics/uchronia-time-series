
library(capigen)
load_wrapper_gen_lib('c:/src/github_jm/rcpp-wrapper-generation')
uchroniaSrcPath <- 'c:/src/csiro/stash/datatypes'
# generate_uchronia_r_all_wrappers(uchroniaSrcPath)
# roxy_uchronia()

generate_uchronia_python_all_wrappers(uchroniaSrcPath)
