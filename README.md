# qtools: Quantitative Finance tools

This project implements some methods of option pricing from quantitative finance, using C++ and python. 

The methods are the binomial tree model, and simple Monte Carlo simulation. The code is mostly in C++, but wrapped as a Python package using Cython. 

To install as a Python package, clone the repository, enter the directory, and run:

```
rm -rf build
python3 setup.py build
```

This compiles the C++ code and creates a python package called `qtools`, which can be installed by:

```
pip install .
```

After that the package can be imported in python:

```
from qtools import *
```
