from qtools import *
from pyqtools import *

from math import exp, log, sqrt

import time

days_in_a_year=356

spot = 105
strike = 100
tte = 30
vol = (40/100) * sqrt(1/days_in_a_year)
rate = exp(log(1.05)/days_in_a_year)-1
mcsteps = 200000
layers = 4000

def time_and_print(f, desc, *argv):
    t = time.time()
    x = f(*argv)
    t = time.time()-t
    print("{0:25} {1:.5f} ({2:.4f} seconds)".format(desc,x,t))

print("Parameters:")
print(f"Spot: {spot}, Strike: {strike}, Time to Expiry: {tte}, Volatility: {vol:.5f}, Interest Rate: {rate:.5f}")

print("")
print(f"Monte Carlo samples: {mcsteps}")
print(f"Binomial tree depth: {layers}")

print("")


american_calls = {  PriceAmericanCall: "Ad hoc", 
                    PriceAmericanCallTian: "Tian",
                    PriceAmericanCallCRR: "Cox-Ross-Rubinstein",
                    PriceAmericanCallTrig: "Trigeorgis",
                    PriceAmericanCallJR: "Jarrow-Rudd",
                    PriceAmericanCallJKY: "Jabbour-Kramin-Young",
                    }

american_puts = {   PriceAmericanPut: "Ad hoc",
                    PriceAmericanPutTian: "Tian",
                    PriceAmericanPutCRR: "Cox-Ross-Rubinstein",
                    PriceAmericanPutTrig: "Trigeorgis",
                    PriceAmericanPutJR: "Jarrow-Rudd",
                    PriceAmericanPutJKY: "Jabbour-Kramin-Young",
                    }
print("Model".ljust(27) + "Price".ljust(10) + "Time".ljust(10))
print("-----------------------------------------------------")
print("American Calls:")
print("-------------")
for call in american_calls:
    time_and_print(call, american_calls[call], spot, tte, strike, rate, vol, layers)

print("")

print("European Calls:")
print("-------------")
time_and_print(SimpleMCEuCall, "Monte Carlo (Python)", spot, tte, strike, rate, vol, mcsteps)
time_and_print(PriceVanillaEuCall, "Monte Carlo (C++)", spot, tte, strike, rate, vol, mcsteps)
time_and_print(BlackScholesEuCall, "Black-Scholes (Python)", spot, tte, strike, rate, vol)
time_and_print(PriceEuropeanCallLR, "Leimer-Reisen", spot, tte, strike, rate, vol, layers)

print("")

print("American Puts:")
print("-------------")
for put in american_puts:
    time_and_print(put, american_puts[put], spot, tte, strike, rate, vol, layers)

print("")

print("European Puts:")
print("-------------")
time_and_print(SimpleMCEuPut, "Monte Carlo (Python)", spot, tte, strike, rate, vol, mcsteps)
time_and_print(PriceVanillaEuPut, "Monte Carlo (C++)", spot, tte, strike, rate, vol, mcsteps)
time_and_print(BlackScholesEuPut, "Black-Scholes (Python)", spot, tte, strike, rate, vol)
time_and_print(PriceEuropeanPutLR, "Leimer-Reisen", spot, tte, strike, rate, vol, layers)