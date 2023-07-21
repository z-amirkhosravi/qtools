from qtools import *
from pyqtools import *

import time

spot = 120
strike = 90
tte = 3
vol = 0.7 
rate = 5
mcsteps = 100000
layers = 3000

print(f"Spot: {spot}, Strike: {strike}, Time to Expiry: {tte}, Volatility: {vol}, Interest Rate: {rate}")

print(f"Using {mcsteps} steps in MC, and depth {layers} in binomial tree")

print("C++:")
t = time.time()
price = PriceAmericanCall(spot, tte, strike, rate, vol, layers)
t =  time.time() - t
print("PriceAmericanCall: ", price, f" ({t} seconds)")

t = time.time()
price = PriceVanillaEuCall(spot, tte, strike, rate, vol, mcsteps)
t = time.time() - t
print("PriceVanillaEuCall(MC): ", price, f" ({t} seconds)")

print("")
print("Python:")
t = time.time()
price = SimpleMCEuCall(spot, tte, strike, rate, vol, mcsteps)
t = time.time() - t
print("SimpleMCEuCall: ", price, f" ({t} seconds)")

t = time.time()
price = BlackScholesEuCall(spot, tte, strike, rate, vol)
t = time.time() -  t
print("BlackScholesEuCall: ", price, f" ({t} seconds)")