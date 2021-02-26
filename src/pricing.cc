#include "simple_mc.h"
#include <functional>
#include <cmath>
#include "pricing.h"
#include <iostream>

CallPayOff::CallPayOff(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_):
     spot(spot_),
     strike(strike_),
     time_to_expiry(time_to_expiry_),
     rate(rate_),
     vol(vol_) 
{
     drift = rate - (vol * vol) / 2;
     step_std = sqrt(time_to_expiry) * vol;
     threshold = (log(strike) - log(spot) - drift * time_to_expiry) / step_std;
     base_price_factor = spot * exp( drift * time_to_expiry);
 }

double CallPayOff::eval(double x) const {                            // given a double x, computes payoff of a call option when underlying price has moved according to x
     double payoff = base_price_factor * exp(x * step_std) - strike;
     return payoff>0? payoff: 0;
 }

PutPayOff::PutPayOff(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_):
     spot(spot_),
     strike(strike_),
     time_to_expiry(time_to_expiry_),
     rate(rate_),
     vol(vol_) 
{
     drift = rate - (vol * vol) / 2;
     step_std = sqrt(time_to_expiry) * vol;
     threshold = (log(strike) - log(spot) - drift * time_to_expiry) / step_std;
     base_price_factor = spot * exp( drift * time_to_expiry);
 }

double PutPayOff::eval(double x) const {                            // given a double x, computes payoff of a call option when underlying price has moved according to x
     double payoff = strike - base_price_factor * exp(x * step_std);
     return payoff > 0 ?payoff: 0;
 }


DigitalPutPayOff::DigitalPutPayOff(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_):
     spot(spot_),
     strike(strike_),
     time_to_expiry(time_to_expiry_),
     rate(rate_),
     vol(vol_) 
{
     drift = rate - (vol * vol) / 2;
     step_std = sqrt(time_to_expiry) * vol;
     threshold = (log(strike) - log(spot) - drift * time_to_expiry) / step_std;
     base_price_factor = spot * exp( drift * time_to_expiry);
 }

double DigitalPutPayOff::eval(double x) const {                            // given a double x, computes payoff of a call option when underlying price has moved according to x
     double payoff = strike - base_price_factor * exp(x * step_std);
     return payoff > 0 ?1: 0;
 }


DigitalCallPayOff::DigitalCallPayOff(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_):
     spot(spot_),
     strike(strike_),
     time_to_expiry(time_to_expiry_),
     rate(rate_),
     vol(vol_) 
{
     drift = rate - (vol * vol) / 2;
     step_std = sqrt(time_to_expiry) * vol;
     threshold = (log(strike) - log(spot) - drift * time_to_expiry) / step_std;
     base_price_factor = spot * exp( drift * time_to_expiry);
 }

double DigitalCallPayOff::eval(double x) const {                            // given a double x, computes payoff of a call option when underlying price has moved according to x
     double payoff = base_price_factor * exp(x * step_std) - strike;
     return payoff>0? 1: 0;
 }


/*
void PayOff::debug(void) {
    std::cout << "spot=" << spot_ << std::endl;
    std::cout << "time_to_expiry=" << time_to_expiry_ << std::endl;
    std::cout << "strike=" << strike_ << std::endl;
    std::cout << "rate=" << rate_ << std::endl;
    std::cout << "vol=" << vol_ << std::endl;
    std::cout << "drift=" << drift_ << std::endl;
    std::cout << "step_std=" << step_std_ << std::endl;
    std::cout << "threshold=" << threshold_ << std::endl;
    std::cout << "base_price_factor=" << base_price_factor_ << std::endl;
}*/


double PriceVanillaEuCall(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds) {
    CallPayOff payoff(spot, time_to_expiry, strike, rate, vol);
 
    return SimpleMC(payoff, num_rounds) * exp(-rate * time_to_expiry);   // discount factor
}

double PriceVanillaEuPut(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds) {
    PutPayOff payoff(spot, time_to_expiry, strike, rate, vol);

    return SimpleMC(payoff, num_rounds) * exp(-rate * time_to_expiry);
}

double PriceDigitalEuCall(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds) {
    DigitalCallPayOff payoff(spot, time_to_expiry, strike, rate, vol);
 
    return SimpleMC(payoff, num_rounds) * exp(-rate * time_to_expiry);   
}

double PriceDigitalEuPut(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds) {
    DigitalPutPayOff payoff(spot, time_to_expiry, strike, rate, vol);

    return SimpleMC(payoff, num_rounds) * exp(-rate * time_to_expiry);
}
