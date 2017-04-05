# An internal variable to buffer startup messages
startupMsg <- ''

# TODO: obvious opportunity to refactor to something more generic. Consider the repo rcpp-interop-commons

appendStartupMsg <- function(msg) {
  startupMsg <<- paste0(startupMsg, msg, '\n')
}

#' onLoad function for package 
#'
#' Function called when loading the package with the function library. 
#'
#' @name dotOnLoad
#' @rdname dotOnLoad
#' @param libname the path to the library from which the package is loaded
#' @param pkgname the name of the package.
.onLoad <- function(libname='~/R', pkgname='uchronia') {
  if(Sys.info()['sysname'] == 'Windows') {
    appendStartupMsg(cinterop:::updatePathWindows( libfilename='datatypes.dll'))
  } 
  # this package has a custom src/install.libs.R to use the name uchronia_r.so for the native dll. 
  tryCatch(library.dynam('uchronia_r', pkgname, libname),  
    error = function(e) { 
      if(Sys.info()['sysname'] == 'Windows') {
        cat("Error loading shared library.\nYou may need to set up an environment variable LIBRARY_PATH.\nSee SWIFT documentation at [link TODO]\n") ; 
      } ; 
      stop(e) 
    } 
  )

  # RcppExports.cpp functions have their own exception catching mechanism, 
  # we still do register a handler so that the exception is re-thrown from the uchronia_r dll, 
  # to foster binary compatibility in case we have different compilers/c++ runtimes at play
  RegisterExceptionCallback_Pkg()
  
  # SetTimeSeriesMissingValueValue_R(as.numeric(NA))
  
}

#' On Attach hook function
#' 
#' Print startup messages from package onLoad 
#' 
#' Print startup messages from package onLoad (prevents a 'NOTE' on package check)
#' 
#' @rdname dotOnAttach
#' @name dotOnAttach
#' @param libname the path to the library from which the package is loaded
#' @param pkgname the name of the package.
.onAttach <- function(libname='~/R', pkgname='uchronia') {
  if(startupMsg!='') {
    packageStartupMessage(startupMsg)
  }
}

# loadModule("uchroniaRObjects", TRUE, PACKAGE='uchronia_r')

# setGeneric( 'str' )
# methods::setMethod('str', 'RppRef', function(object, ...) {strRppRef(object, ...)})

