# uchronia_pb

Testbed for python package uchronia. Started using [pybind11 getting started example](https://github.com/pybind/python_example) as a template. Refer to it for getting back to initial steps taken.

## Test build

```bat
conda activate uchronia
cd c:\src\csiro\stash\datatypes\bindings\python\uchronia_pb
REM after adding include and libraries path to setup.py
python setup.py develop
```

## Test call

```python
from uchronia_pb import *
```

```
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ImportError: DLL load failed: The specified module could not be found.
```

things like pyswift were loading the dll explicitely; in the present case, not sure how we can do this. Probably a matter of updateing the PATH before loading the module, unfortunately.

`set PATH=%PATH%;c:\local\libs\64` then

```python
from uchronia_pb import *
CreateSingleTimeSeriesDataFromStruct()
```

Buyah!
