# Trigger the loading of dependency package first
# May guard against obscure bugs. Bug not observed in uchronia or swift, but definitely for fogss package. 
# upcast of opaque external pointers (e.g. passing a COMPOSITE_PARAMETERIZER_PTR to an API
# function that accepts HYPERCUBE_PTR)
# Candidate fix for https://jira.csiro.au/projects/WIRADA/issues/WIRADA-640 is thus:
import cinterop

# Trigger the loading of the native library.
import uchronia.wrap.ffi_interop as _interop

# Trigger the initialisation of the custom wrapper generation function.
import uchronia.classes as _s

from ._version import __version__

