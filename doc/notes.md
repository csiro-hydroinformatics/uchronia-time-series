# Notes

## Working notes

### Aug 2012

Sprucing up documentation across several repositories. Mostly focusing on getting the Python interop packages refcount and cinterop.

So, uchronia... I had used doxygen for the swift codebase (which likely also docunented dependency codebased I think)

* doxygen and mkdocs intersect?
https://github.com/JakubAndrysek/mkdoxy which points to https://matusnovak.github.io/doxybook, not the latest
https://github.com/pieterdavid/mkdocs-doxygen-plugin cool but looks stale still

[doxybook2](https://github.com/matusnovak/doxybook2/)

* doxygen and sphinx intersect?
MS blog: [Clear, Functional C++ Documentation with Sphinx + Breathe + Doxygen + CMake](https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/)


#### Trial

```sh
git clone --depth=1 https://github.com/matusnovak/doxybook2.git
ls
cd doxybook2/
```

```sh
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh
```

bit of adjustment for compiling doxygen2:

```sh
cmake -B ./build -G "Unix Makefiles"     -DCMAKE_BUILD_TYPE=MinSizeRel     -DCMAKE_TOOLCHAIN_FILE=/home/per202/src/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build ./build --config MinSizeRel  # could do with a -j4 perhaps
/home/per202/src/doxybook2/build/src/DoxybookCli/doxybook2 --help
```

```sh
DX2=/home/per202/src/doxybook2/build/src/DoxybookCli/doxybook2
cd ~/src/datatypes/doc/api
```

MkDocs + Material theme - example/mkdocs-material/.doxybook/config.json adapted.

cpp relative url because we aim to host not only cpp. Try, at least.

```sh
mkdir -p docs/cpp
$DX2 --input ./doxyoutput/xml --output ./docs/cpp --config config.json
```

```sh
. ~/config/baseconda
conda activate poetry
mkdocs build --clean --site-dir _build/html --config-file mkdocs.yml
```


http://127.0.0.1:8000/cppClasses/classdatatypes_1_1timeseries_1_1MultiTimeSeries/
