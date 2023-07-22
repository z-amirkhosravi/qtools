# qtools: Quantitative Finance tools

This project implements some methods of derivative pricing from quantitative finance, using C++ and python. It contains several different binomial tree models, and one method using Monte Carlo integration. The code is mostly in C++, but wrapped as a Python package using Cython. 

This was chiefly a two-fold exercise: to get some hands-on experience computing stuff in quant finance, and to figure out how to integrate C++ code into Python.

To install the Python package, clone the repository, enter the directory, and run:

```
rm -rf build
python3 setup.py build
```

This compiles the C++ code and creates a python package called `qtools`. The `Makefile` is configured for `g++`, so it may need to be edited first. After the package is built, it can be installed with:

```
pip install .
```

Then it can be imported in Python using:

```
from qtools import *
```

#  Binomial Tree Models

Here's a very brief description of what binomial tree models are:

In quantitative finance the price of some financial derivatives can be described using stochastic differential equations, and expressed in terms of Brownian motion. These can be regarded as continuous limits of discrete random walks, which are modeled by binomial trees. This allows approximating the price of derivatives by finite computations.

The basic idea is as follows. There are $n$ i.i.d Bernoulli processes $X_1,X_2,\cdots, X_n$, which are each $1$ or $0$ with probability $p$ and $q$, respectively. Together with _up_ and _down_ factors $u$ and $d$, they determine the
price $S_t$ of a stock at a discrete time $t$, with given $S_0$, by the rule:

$$S_t = \left\\{ \begin{array}{cc} u S_{t-1} & \text{ if }X_t = 1,\\\\ d S_{t-1} & \text{ if } X_t = 0.\end{array}\right.$$

The goal is to determine the price $V_t$ of a financial derivative of that stock, at time $t=0$. This quantity depends on the entire distribution of $S_t$ for $0 \leq t \leq T$, where $T$ is some fixed expiration date. Typically, there is an _intrinsic value_ function $X_t$, a deterministic function of $S_t$, which provides the boundary condition $V_T = X_T$. Then $V_t$ for $t < T$ depends both on the intrinsic value $X_t$, along with the distribution of $V_{t+1}$.

All this is to say that there are two steps to computing $V_0$: first, one has to compute all the values $S_t$ by starting from $S_0$ and multiplying by $u$ and $d$ at each branch. This is a _forward pass_ through the tree of possibilities. Then in the second step, one computes $V_0$ by starting from $V_T = X_T$, and making a _backward pass_ through the tree, at each step computing the distribution of $V_t$ for $t$ smaller, until arriving at $t=0$. 

The implementation of the binomial tree model here abstracts this entire process, so that different models from the literature can be computed easily by specifiying the parameters: the intrinsic function $X_t$, the values $p$, $q$, $u$, $d$, plus a discounting factor that depends on interest rates. The models so far implemented are: Cox-Ross-Rubinstein, Jarrow-Rud, Tian and Trigeorgis. I have followed the descriptions given in <a href=https://quant.opengamma.io/Tree-Option-Pricing-Model.pdf>this paper</a> by Yukinori Iwashita.

# Monte Carlo Integration

For some instruments, such as European options, the value $V_t$ at time $t$ only depends on the distribution of the intrinsic value function $X_t$ at time $t=T$. In this case $V_0$ is just the (discounted) expected value of $V_T = X_T$ at time $t=0$.
Since $X_T$ is a function of $S_T$, and $\log (S_T)$ is asssumed normally distributed, $V_0$ can be expressed as an integral against the normal probability distribution function. When the integral can be computed explicitly we get exact answers such as the famous Black-Scholes formula.

When the integral needs to be approximated, aside from the binomial tree model, one can also use Monte Carlo integration, where integration is replaced with averaging over random numbers pulled from a normal distribution.

This is simple enough to implement, and I've included both a C++ version in `qtools` and a pure Python version in `pyqtools.py`. It was surprising to me how fast the C++ implementation turned out to be. The Python version was by far the slowest method, but the same algorithm implemented in C++ was faster than all the binomial tree models, and second only to direct computation by the Black-Scholes formula.
