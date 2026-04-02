"""uchronia package.

Python interface to a C++ library designed to handle multidimensional time series and ensemble thereof
"""

from __future__ import annotations

from uchronia._internal.cli import get_parser, main

__all__: list[str] = ["get_parser", "main"]
