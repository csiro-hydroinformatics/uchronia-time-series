"""Python package bindings for the data handling features in uchronia

See:
https://bitbucket.csiro.au/projects/SF/repos/datatypes/browse
"""

# Always prefer setuptools over distutils
from setuptools import setup, find_packages
# To use a consistent encoding
from codecs import open
import os
import re

here = os.path.abspath(os.path.dirname(__file__))

verstr = 'unknown'
VERSIONFILE = "uchronia/_version.py"
with open(VERSIONFILE, "r")as f:
    verstrline = f.read().strip()
    pattern = re.compile(r"__version__ = ['\"](.*)['\"]")
    mo = pattern.search(verstrline)
if mo:
    verstr = mo.group(1)
else:
    raise RuntimeError("Unable to find version string in %s." % (VERSIONFILE,))

# Get the long description from the README file
# with open(path.join(here, 'README.rst'), encoding='utf-8') as f:
#     long_description = f.read()

with open(os.path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()
    long_description_content_type='text/markdown'

REQUIREMENTS = ['refcount', 'cffi', 'cinterop']

TEST_REQUIREMENTS = ['pytest',
                    #  'coveralls',
                    #  'pytest-cov',
                    #  'pytest-mpl'
                     ]

CLASSIFIERS = ['Development Status :: 3 - Alpha',
                'Intended Audience :: Developers',
                'Topic :: Software Development ',
                # 'License :: OSI Approved :: MIT License',
                'Operating System :: OS Independent',
                'Programming Language :: Python'
                ]

setup(
    name='uchronia',
    version=verstr,
    description='Uchronia - time series handling for ensembles simulations and forecasts in C++', 
    long_description=long_description,
    long_description_content_type=long_description_content_type,
    url='https://github.com/csiro-hydroinformatics/uchronia-time-series',
    author='Jean-Michel Perraud',
    author_email='per202@csiro.au',
    classifiers=CLASSIFIERS,
    keywords='ensemble forecast time series',
    packages=['uchronia', 'uchronia.wrap'],
    install_requires=REQUIREMENTS,
    project_urls={  # Optional
        'Bug Reports': 'https://github.com/csiro-hydroinformatics/uchronia-time-series/issues',
        # 'Funding': 'https://donate.pypi.org',
        # 'Say Thanks!': 'http://saythanks.io/to/example',
        'Source': 'https://github.com/csiro-hydroinformatics/uchronia-time-series',
    },
    include_package_data=True, # C definitions for CFFI 
    package_data={'': ['data/*.h']},
)
