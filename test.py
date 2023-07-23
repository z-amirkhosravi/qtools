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
mcsteps = 100000
layers = 4000

print(f"Spot: {spot}, Strike: {strike}, Time to Expiry: {tte}, Volatility: {vol}, Interest Rate: {rate}")

print(f"Using {mcsteps} steps in MC, and depth {layers} in binomial tree")

print("C++:")
t = time.time()
price = PriceAmericanCall(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanCall (BIN): ", price, f" ({t} seconds)")

t = time.time()
price = PriceAmericanPut(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanPut (BIN): ", price, f" ({t} seconds)")

t = time.time()
price = PriceAmericanCallTian(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanCall (Tian): ", price, f" ({t} seconds)")

t = time.time()
price = PriceAmericanPutTian(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanPut (Tian): ", price, f" ({t} seconds)")

t = time.time()
price = PriceAmericanCallCRR(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanCall (CRR): ", price, f" ({t} seconds)")

t = time.time()
price = PriceAmericanPutCRR(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanPut (CRR): ", price, f" ({t} seconds)")

t = time.time()
price = PriceAmericanCallTrig(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanCall (Trig): ", price, f" ({t} seconds)")

t = time.time()
price = PriceAmericanPutTrig(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanPut (Trig): ", price, f" ({t} seconds)")

t = time.time()
price = PriceAmericanCallJKY(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanCall (JKY): ", price, f" ({t} seconds)")

t = time.time()
price = PriceAmericanPutJKY(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanPut (JKY): ", price, f" ({t} seconds)")

t = time.time()
price = PriceVanillaEuCall(spot, tte, strike, rate, vol, mcsteps)
t = time.time() - t
print("PriceVanillaEuCall (MC): ", price, f" ({t} seconds)")

t = time.time()
price = PriceVanillaEuPut(spot, tte, strike, rate, vol, mcsteps)
t = time.time() - t
print("PriceVanillaEuPut (MC): ", price, f" ({t} seconds)")

print("")
print("Python:")
t = time.time()
price = SimpleMCEuCall(spot, tte, strike, rate, vol, mcsteps)
t = time.time() - t
print("SimpleMCEuCall: ", price, f" ({t} seconds)")

t = time.time()
price = SimpleMCEuPut(spot, tte, strike, rate, vol, mcsteps)
t = time.time() - t
print("SimpleMCEuPut: ", price, f" ({t} seconds)")

t = time.time()
price = BlackScholesEuCall(spot, tte, strike, rate, vol)
t = time.time() -  t
print("BlackScholesEuCall: ", price, f" ({t} seconds)")

t = time.time()
price = BlackScholesEuPut(spot, tte, strike, rate, vol)
t = time.time() -  t
print("BlackScholesEuPut: ", price, f" ({t} seconds)")

t = time.time()
price = PriceEuropeanCallLR(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceEuropeanCall (LR): ", price, f" ({t} seconds)")

t = time.time()
price = PriceEuropeanPutLR(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceEuropeanPut (LR): ", price, f" ({t} seconds)")