# Building Debian packages

## Notes

```sh
sudo apt install dh-make
sudo apt install equivs
```

### Creating the libuchronia-dev pkg

```sh
src_pkgname=uchronia
src_pkgname_ver=${src_pkgname}-2.3
fn_ver=${src_pkgname}_2.3
SRC=~/src/csiro/stash/datatypes/datatypes
DEST_ROOT=~/tmp/uchronia/
DEST=${DEST_ROOT}/uchronia/${src_pkgname_ver}
FILES="CMakeLists.txt cmake_uninstall.cmake.in *.cpp datatypes.kdev4 datatypes.natvis datatypes.vcxproj datatypes.vcxproj.filters debian lib_paths.props.in tests uchronia.pc.in version.cmake"

INCLUDE_FILES="../include"

mkdir -p ${DEST_ROOT}
rm -rf ${DEST_ROOT}/*
mkdir -p ${DEST}
cd ${DEST}
cd ${SRC}
cp -Rf ${FILES} ${DEST}/
cp -Rf ${INCLUDE_FILES} ${DEST}/..
cd ${DEST}
# rm -rf ./obj-x86_64-linux-gnu
# rm -rf ./debian/libuchronia-dev  # whu not a tmp folder like other pkg?
ls -a
cd ${DEST}/..
tar -zcvf ${fn_ver}.orig.tar.gz ${src_pkgname_ver}
cd ${DEST}
debuild -us -uc 
```

Check:

```sh
cd ${DEST}/..
dpkg -c libuchronia_2.3-1_amd64.deb 
dpkg -c libuchronia-dev_2.3-1_amd64.deb 

sudo dpkg -i libuchronia_2.3-1_amd64.deb 
sudo dpkg -i libuchronia-dev_2.3-1_amd64.deb 
```


### Creating the r-uchronia pkg

Possibly `sudo apt install r-cran-generics r-cran-rcpp`. 

```sh
pkgname=r-uchronia
pkgname_ver=${pkgname}-2.3
fn_ver=${pkgname}_2.3
SRC=~/src/csiro/stash/datatypes/bindings/R/pkgs/uchronia
DEST=~/tmp/uchronia/${pkgname_ver}
FILES="./*"

mkdir -p ${DEST}
cd ${DEST}
rm -rf ${DEST}/*
cd ${SRC}
cp -Rf ${FILES} ${DEST}/
cd ${DEST}
ls -a
cd ${DEST}/..
tar -zcvf ${fn_ver}.orig.tar.gz ${pkgname_ver}
cd ${DEST}
debuild -us -uc 
```

Check:

```sh
cd ${DEST}/..
dpkg -c r-uchronia_2.3-1_amd64.deb 
sudo dpkg -i r-uchronia_2.3-1_amd64.deb 
```
