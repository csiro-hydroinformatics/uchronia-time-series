
```R
library(capigen) # or load_all
sessionInfo()
load_wrapper_gen_lib('c:/src/github_jm/rcpp-wrapper-generation')
uchroniaSrcPath <- 'c:/src/csiro/stash/datatypes'

conv <- create_py_cffi_wrap_generator()
p <- clrGetProperties(conv)
pp <- lapply(p, function(x) {clrGet(conv, x)})
names(pp) <- p
pp 


prepend_r_xptr_header=default_py_cffi_wrapper_prepend()

generate_uchronia_python_all_wrappers(uchroniaSrcPath)

clrGet(conv, 'CreateXptrObjRefFunction')

clrGetLoadedAssemblies()
clrGetLoadedAssemblies
clrGetLoadedAssemblies(TRUE, TRUE)
```

```sh
source ~/anaconda3/bin/activate

my_env_name=pyuchronia
conda create --name ${my_env_name} python=3.7
conda activate  ${my_env_name}

conda install pytest cffi
conda install numpy pandas
pip install refcount

cd ~/src/github_jm/rcpp-interop-commons/bindings/python/cinterop
python setup.py develop

cd /home/per202/src/csiro/stash/datatypes/bindings/python/uchronia

# Cannot get the VSCode debugger to work on linux drives me nuts.
export LD_LIBRARY_PATH=/usr/local/lib # this needs a fix, in uchronia. Should be unnecessary.
python /home/per202/src/csiro/stash/datatypes/bindings/python/uchronia/tests/test_low_level.py


```
