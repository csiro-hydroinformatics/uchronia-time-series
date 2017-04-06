
library(swiftdev)
load_wrapper_gen_lib('c:/src/github_jm/rcpp-wrapper-generation')
uchroniaSrcPath <- 'c:/src/csiro/stash/per202/datatypes'
generate_uchronia_r_all_wrappers(uchroniaSrcPath)
roxy_uchronia()

# devmode() ; prepend_rlibpath() ; library(uchronia)
