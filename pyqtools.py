import numpy as np
import math

from scipy.stats import norm
from numpy import random

def hist_vol(data, window=30):
    """
    Returns historical volatility, defined as sqrt(252) times sample standard deviation of a rolling window.
    """
    return np.sqrt(252) * data.rolling(window = window).std()   #std is unbiased estimator (divides by w-1)

def park_vol(data, window = 30):
    """
    data: input dataframe with columns 'High' and 'Low'
    window: size of rolling window
    
    Returns Parkinson (High Low Range) volatility defined as rolling average of ln(High/Low)^2, multiplied by 252 / (2 * ln(2)) 
    """
    return np.sqrt( (252 / (4 * np.log(2))) * (np.log(data['High']/data['Low']) ** 2).rolling(window = window).mean())

def gk_vol(data, n=30, N=252):
    """
    data: input dataframe containing columns labelled: Open, Close, High, Low
    returns the Garman-Klass volatility as a series
    """
    hilo_mean = (np.log(data['High']/data['Low'])**2).rolling(window = n).mean()
    co_mean = (np.log(data['Close']/data['Open']) ** 2).rolling(window = n).mean()
    return np.sqrt( (N/2) * hilo_mean - N*(2 * np.log(2) - 1) * co_mean)

def stoch_vol_model(data, step =10, nu_lambda=0.1):
    """
    data: input series
    returns a pymc3 generalized student-t distribution for modeling input data with MCMC
    """
    with pm.Model() as model:
        step_size = pm.Exponential('step_size',step)
        vol = pm.GaussianRandomWalk('volatility', sigma=step_size, shape = len(data))
        nu = pm.Exponential('nu', nu_lambda)
        returns = pm.StudentT('returns', nu=nu, lam = np.exp(-2 *vol), observed=data)
    return model

def cBS(kappa, tau, r, sigma):
    """
    returns the ratio C / S between price C of a call option according to standard Black-Scholes and the price S of the underlying
    
    inputs:
    
    kappa: moneynesss, defined as K/S, where K is the strike price, S price of the underlying
    tau:   time to maturity
    r:     interest rate, assumed constant
    sigma: volatility, assumed constant
    """
    
    d1 = (-np.log(kappa) + (r + (sigma ** 2)/2 )*tau) / (sigma * np.sqrt(tau))
    d2 = d1 - sigma * np.sqrt(tau)
    return norm.cdf(d1) - kappa * math.exp(-r * tau) * norm.cdf(d2)

def pBS(kappa, tau, r, sigma):
    """
    returns the ratio P / S between price P of a put option according to standard Black-Scholes and the price S of the underlying
    
    inputs:
    
    kappa: moneynesss, defined as K/S, where K is the strike price, S price of the underlying
    tau:   time to maturity
    r:     interest rate, assumed constant
    sigma: volatility, assumed constant
    """
    
    d1 = (-np.log(kappa) + (r + (sigma ** 2)/2 )*tau) / (sigma * np.sqrt(tau))
    d2 = d1 - sigma * np.sqrt(tau)
    return -norm.cdf(-d1) + kappa * math.exp(-r * tau) * norm.cdf(-d2)

def BlackScholesEuCall(S, tau, K, r, sigma):
    """
    returns the price of a call option according to the standard Black-Scholes formula
    
    inputs:
        
    S:     price of the underlying
    tau:   time to maturity
    K:     strike price
    sigma: volatility, assumed constant
    r:     interest rate, assumed constant
    """
    return S * cBS(K/S, tau, r, sigma)

def BlackScholesEuPut(S, tau, K, r, sigma):
    """
    returns the price of a put option according to the standard Black-Scholes formula
    
    inputs:
        
    S:     price of the underlying
    tau:   time to maturity
    K:     strike price
    sigma: volatility, assumed constant
    r:     interest rate, assumed constant
    """
    return S * pBS(K/S, tau, r, sigma)


def SimpleMCEuCall(S0, tau, K, r, sigma, M):
    """
    Simple Monte Carlo method to compute a European call option, with constant volatility and constant continuously compounded interest rate
    
    S0: current price of security
    tau: time to expiry
    K: strike price
    r: interest rate
    sigma: volatility
    M: number of paths sampled
    """
    mu = r - (sigma ** 2) / 2                              # drift with Ito correction factor
    t0 = np.sqrt(tau) * sigma                              # standard deviation of approximating Gaussian
    # th = (np.log(K) - np.log(S0) - mu * tau) / t0          # threshold that log(K) has to pass for payoff
    R = S0 * np.exp( mu * tau)                               # non-stochastic factor of eventual price

    E = 0
    for _ in range(M):
        x = random.normal()
        P = R * np.exp(x * t0) - K                         # P = S - K 
        if P > 0:                                           
            E = E + P
            
        P = R * np.exp(-x * t0) - K                        # the antithetic path
        if P > 0:
            E = E + P
    return E * np.exp(- r * tau) / (2*M)                       # discounted average

def SimpleMCEuPut(S0, tau, K, r, sigma, M):
    """
    Simple Monte Carlo method to compute a European call option, with constant volatility and constant continuously compounded interest rate
    
    S0: current price of security
    tau: time to expiry
    K: strike price
    r: interest rate
    sigma: volatility
    M: number of paths sampled
    """
    mu = r - (sigma ** 2) / 2                              # drift with Ito correction factor
    t0 = np.sqrt(tau) * sigma                              # standard deviation of approximating Gaussian
    # th = (np.log(K) - np.log(S0) - mu * tau) / t0          # threshold that log(K) has to pass for payoff
    R = S0 * np.exp( mu * tau)                               # non-stochastic factor of eventual price

    E = 0
    for _ in range(M):
        x = random.normal()
        P = K - R * np.exp(x * t0)                          # P = S - K 
        if P > 0:                                           
            E = E + P
            
        P = K - R * np.exp(-x * t0)                         # the antithetic path
        if P > 0:
            E = E + P
    return E * np.exp(- r * tau) / (2*M)                       # discounted average

# def CallPayOff(S,K):
#     P = S - K
#     return P if P > 0 else 0

# def PutPayOff(S,K):
#     P = K - S
#     return P if P > 0 else 0

# def DoubleDigitalPayOff(S,K1,K2):
#     return 1 if ((K1 < S) & (S < K2)) else 0

# type_payoff_dict = { 'call': CallPayOff, 'put': PutPayOff, 'dd': DoubleDigitalPayOff}

# def EuOptionPrice(S0, tau, K, r, sigma, M, opt_type, K2=0):
#     if not opt_type in type_payoff_dict:
#         print("Option type not implemented.")
#         return
    
#     payoff = type_payoff_dict[opt_type]
    
#     mu = r - (sigma ** 2) / 2                              # drift with Ito correction factor
#     t0 = np.sqrt(tau) * sigma                              # standard deviation of approximating Gaussian
#     th = (np.log(K) - np.log(S0) - mu * tau) / t0          # threshold that log(K) has to pass for payoff
#     R = S0 * np.exp( mu * tau)                               # non-stochastic factor of eventual price

#     E = 0
#     for _ in range(M):
#         x = random.normal()
#         P = K - R * np.exp(x * t0)                          # P = S - K 
#         if P > 0:                                           
#             E = E + P
            
#         P = K - R * np.exp(-x * t0)                         # the antithetic path
#         if P > 0:
#             E = E + P
#     return E * np.exp(- r * tau) / (2*M)                       # discounted average
    
    
