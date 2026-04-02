"""uchronia package.

Python interface to a C++ library designed to handle multidimensional time series and ensemble thereof
"""

from __future__ import annotations

# Trigger the loading of dependency package first
# May guard against obscure bugs. Bug not observed in uchronia or swift, but definitely for fogss package.
# upcast of opaque external pointers (e.g. passing a COMPOSITE_PARAMETERIZER_PTR to an API
# function that accepts HYPERCUBE_PTR)
# Candidate fix for https://jira.csiro.au/projects/WIRADA/issues/WIRADA-640 is thus:
import cinterop as __cinterop  # noqa: F401

# Trigger the initialisation of the custom wrapper generation function.
import uchronia.classes as __uclasses  # noqa: F401

# Trigger the loading of the native library.
import uchronia.wrap.ffi_interop as __interop  # noqa: F401
from uchronia._internal.cli import get_parser, main

__all__: list[str] = ["get_parser", "main"]
