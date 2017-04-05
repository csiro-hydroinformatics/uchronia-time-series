
## This package needs to rename swift.so to swift_r.so. This is not easy to do so at compilation time, as the name of the package seems hard-wired to be used for the shared library. A custom Makefile would overcome this (?) but this is a pain. instead, we use the possibility to use a custom install.libs.r file. 
## shlib_install <- function(instdir, arch) in file install.R in the standard R package tools of the R distribution.
## See also 1.1.5 Package subdirectories in the manual "Writing R Extensions"

## We largely use the default behavior of shlib_install

if(Sys.getenv('COMPILE_WITH_VCPP') != '') {
	files <- Sys.glob(paste0("*", SHLIB_EXT))
	msvs:::custom_install_shlib(files, srclibname='uchronia_r', shlib_ext=SHLIB_EXT, r_arch=R_ARCH, r_package_dir=R_PACKAGE_DIR, windows=WINDOWS, group.writable=FALSE)
}
