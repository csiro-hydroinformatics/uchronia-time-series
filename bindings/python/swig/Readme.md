
This folder has material (re)assessing SWIG as an option to generate binbings for some of the higher level languages.

CFFI has been the means to handle interop with python for things like SWIFT, and for R packages I have my custom glue generator (I considered swig but judged the bagage was not worth trying to reuse... I should have documented that outcome better mind you).

Given the nascent uchronia in its own right, it is time to reassess options and which is best overall.

# Prior work

* swift python bindings with swift exploration :  SWIFT repo \swift\bindings\python\Readme.md

# walkthrough / log

as of 2017-09:



```r
library(capigen)
load_wrapper_gen_lib('c:/src/github_jm/rcpp-wrapper-generation')
uchroniaSrcPath <- 'c:/src/csiro/stash/datatypes'
generate_uchronia_python_all_wrappers(uchroniaSrcPath)
```
