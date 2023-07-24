// author: Z. Amir-Khosravi
//
// This file implements the various instances of FunctionClass that compute intrinsic values of different options, as
  // well as the actual functions that return the price. The latter may be directly called by the interface.

#include <functional>
#include <cmath>
#include <iostream>
#include "pricing.h"
#include "base.h"

CallValueFromLog::CallValueFromLog(double strike_): strike(strike_) {
  logstrike = log(strike);
}

double CallValueFromLog::eval(double logvalue) const {
  return logvalue > logstrike? exp(logvalue) - strike : 0;
}

PutValueFromLog::PutValueFromLog(double strike_): strike(strike_) {
  logstrike = log(strike);
}

double PutValueFromLog::eval(double logvalue) const {
  return logvalue < logstrike? strike - exp(logvalue) : 0;
}


CallValueFromNormal::CallValueFromNormal(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_):
     spot(spot_),
     strike(strike_),
     time_to_expiry(time_to_expiry_),
     rate(rate_),
     vol(vol_) 
{
     drift = rate - (vol * vol) / 2;
     step_std = sqrt(time_to_expiry) * vol;
     // threshold = (log(strike) - log(spot) - drift * time_to_expiry) / step_std;
     base_price_factor = spot * exp( drift * time_to_expiry);
 }

double CallValueFromNormal::eval(double x) const { 
     double payoff = base_price_factor * exp(x * step_std) - strike;
     return payoff>0? payoff: 0;
 }

PutValueFromNormal::PutValueFromNormal(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_):
     spot(spot_),
     strike(strike_),
     time_to_expiry(time_to_expiry_),
     rate(rate_),
     vol(vol_) 
{
     drift = rate - (vol * vol) / 2;
     step_std = sqrt(time_to_expiry) * vol;
     // threshold = (log(strike) - log(spot) - drift * time_to_expiry) / step_std;
     base_price_factor = spot * exp( drift * time_to_expiry);
 }

double PutValueFromNormal::eval(double x) const {            
     double payoff = strike - base_price_factor * exp(x * step_std);
     return payoff > 0 ?payoff: 0;
 }


DigitalPutValueFromNormal::DigitalPutValueFromNormal(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_):
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

double DigitalPutValueFromNormal::eval(double x) const {     
     double payoff = strike - base_price_factor * exp(x * step_std);
     return payoff > 0 ?1: 0;
 }

DigitalCallValueFromNormal::DigitalCallValueFromNormal(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_):
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

double DigitalCallValueFromNormal::eval(double x) const {                        
     double payoff = base_price_factor * exp(x * step_std) - strike;
     return payoff>0? 1: 0;
 }

double PriceVanillaEuCall(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds) {
    CallValueFromNormal payoff(spot, time_to_expiry, strike, rate, vol);
 
    return SimpleMC(payoff, num_rounds) * exp(-rate * time_to_expiry);    
}

double PriceVanillaEuPut(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds) {
    PutValueFromNormal payoff(spot, time_to_expiry, strike, rate, vol);

    return SimpleMC(payoff, num_rounds) * exp(-rate * time_to_expiry);
}

double PriceDigitalEuCall(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds) {
    DigitalCallValueFromNormal payoff(spot, time_to_expiry, strike, rate, vol);
 
    return SimpleMC(payoff, num_rounds) * exp(-rate * time_to_expiry);   
}

double PriceDigitalEuPut(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds) {
    DigitalPutValueFromNormal payoff(spot, time_to_expiry, strike, rate, vol);

    return SimpleMC(payoff, num_rounds) * exp(-rate * time_to_expiry);
}

double PriceAmericanCallNaive(double spot, double time_to_expiry, double strike, double rate, double vol, long tree_depth) {

  double time_delta = time_to_expiry/tree_depth;
  double vol_delta = vol * sqrt(time_delta);
  

  double adjusted_drift = rate - vol * vol / 2;
  double drift_delta = adjusted_drift * time_delta;

  double u = exp(drift_delta + vol_delta);
  double d = exp(drift_delta - vol_delta);
 

  Lattice L(tree_depth+1), V(tree_depth+1);
  
  L.AdditiveForwardPass(log(spot), drift_delta, vol_delta);
 
  CallValueFromLog call_value(strike);
  double p = exp(-rate* time_delta) * (1 - d) / (u - d);
  double q = exp(-rate* time_delta) * (u - 1) / (u - d);
  V.MultiplicativeRollback(L, p, q, call_value);

  return V.points[0][0];
}

double PriceAmericanCall(double spot, double time_to_expiry, double strike, double rate, double vol, long tree_depth) {

  double step_size = time_to_expiry/tree_depth;
  
  double A = (exp(-rate * step_size) + exp((rate + vol*vol)*step_size))/2;
  
  double u = A + sqrt(A*A - 1);
  double d = A - sqrt(A*A - 1);

  double p = (exp(rate * step_size) - d) / (u-d);
  double q = 1-p;

  Lattice L(tree_depth+1), V(tree_depth+1);
  L.AdditiveForwardPass(log(spot), log(u));
   
  CallValueFromLog call_value(strike);
  V.MultiplicativeRollback(L, exp(-rate * step_size) * p, exp(-rate * step_size) * q, call_value);
  
  return V.points[0][0];
}

double PriceAmericanCall_Tian(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {

  double h = time_to_expiry/N;
  double M = exp(rate * h);
  double V = exp(sigma * sigma * h);

  double u = M*V*(V + 1 + sqrt(V*V + 2*V - 3))/2;
  double d = M*V*(V + 1 - sqrt(V*V + 2*V - 3))/2;

  double p = (M-d)/(u-d);
  double q = 1 - p;

  Lattice L(N+1), C(N+1);
  L.AdditiveForwardPass(log(spot), log(u), log(d));
   
  CallValueFromLog call_value(strike);
  C.MultiplicativeRollback(L, p/M, q/M, call_value);
  
  return C.points[0][0];
}

double PriceAmericanPut_Tian(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {

  double h = time_to_expiry/N;
  double M = exp(rate * h);
  double V = exp(sigma * sigma * h);

  double u = M*V*(V + 1 + sqrt(V*V + 2*V - 3))/2;
  double d = M*V*(V + 1 - sqrt(V*V + 2*V - 3))/2;

  double p = (M-d)/(u-d);
  double q = 1 - p;

  Lattice L(N+1), P(N+1);
  L.AdditiveForwardPass(log(spot), log(u), log(d));
   
  PutValueFromLog put_value(strike);
  P.MultiplicativeRollback(L, p/M, q/M, put_value);
  
  return P.points[0][0];
}

double PriceAmericanCall_CRR(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {
  double h = time_to_expiry/N;

  double nu = rate - sigma*sigma/2;
  double u = exp(sigma * sqrt(h));

  double p = 0.5 + nu * sqrt(h) / (2*sigma);
  double q = 1-p;

  Lattice L(N+1), V(N+1);
  L.AdditiveForwardPass(log(spot), log(u));
   
  CallValueFromLog call_value(strike);
  V.MultiplicativeRollback(L, exp(-rate * h) * p, exp(-rate * h) * q, call_value);
  
  return V.points[0][0];

}

double PriceAmericanPut_CRR(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {
  double h = time_to_expiry/N;

  double nu = rate - sigma*sigma/2;
  double u = exp(sigma * sqrt(h));

  double p = 0.5 + nu * sqrt(h) / (2*sigma);
  double q = 1-p;

  Lattice L(N+1), V(N+1);
  L.AdditiveForwardPass(log(spot), log(u));
   
  PutValueFromLog put_value(strike);
  V.MultiplicativeRollback(L, exp(-rate * h) * p, exp(-rate * h) * q, put_value);
  
  return V.points[0][0];

}

double PriceAmericanPut(double spot, double time_to_expiry, double strike, double rate, double vol, long tree_depth) {

 double step_size = time_to_expiry/tree_depth;
  
 double A = (exp(-rate * step_size) + exp((rate + vol*vol)*step_size))/2;
  
 double u = A + sqrt(A*A - 1);
 double d = A - sqrt(A*A - 1);

 double p = (exp(rate * step_size) - d) / (u-d);
 double q = 1-p;

 Lattice L(tree_depth+1), V(tree_depth+1);
 L.AdditiveForwardPass(log(spot), log(u));
   
 PutValueFromLog put_value(strike);
 V.MultiplicativeRollback(L, exp(-rate * step_size) * p, exp(-rate * step_size) * q, put_value);
  
 return V.points[0][0];
}

double PriceAmericanCall_Trig(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {
  double h = time_to_expiry/N;

  double nu = rate - sigma*sigma/2;
  double logu = sqrt(sigma*sigma*h + nu*nu*h*h);

  double p = 0.5 + nu * h / (2*logu);
  double q = 1 - p;

  Lattice L(N+1), V(N+1);
  L.AdditiveForwardPass(log(spot), logu);
   
  CallValueFromLog call_value(strike);
  V.MultiplicativeRollback(L, exp(-rate * h) * p, exp(-rate * h) * q, call_value);
  
  return V.points[0][0];

}

double PriceAmericanCall_JR(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {
  double h = time_to_expiry/N;

  double nu = rate - sigma*sigma/2;
  double logu = nu * h + sigma * sqrt(h);
  double logd = nu * h - sigma * sqrt(h);
  double p = 0.5;

  Lattice L(N+1), V(N+1);
  L.AdditiveForwardPass(log(spot), logu, logd);
   
  CallValueFromLog call_value(strike);
  V.MultiplicativeRollback(L, exp(-rate * h) * p, exp(-rate * h) * (1-p), call_value);
  
  return V.points[0][0];

}

double PriceAmericanPut_JR(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {
  double h = time_to_expiry/N;

  double nu = rate - sigma*sigma/2;
  double logu = nu * h + sigma * sqrt(h);
  double logd = nu * h - sigma * sqrt(h);
  double p = 0.5;

  Lattice L(N+1), V(N+1);
  L.AdditiveForwardPass(log(spot), logu, logd);
   
  PutValueFromLog put_value(strike);
  V.MultiplicativeRollback(L, exp(-rate * h) * p, exp(-rate * h) * (1-p), put_value);
  
  return V.points[0][0];

}

double PriceAmericanPut_Trig(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {
  double h = time_to_expiry/N;

  double nu = rate - sigma*sigma/2;
  double logu = sqrt(sigma*sigma*h + nu*nu*h*h);

  double p = 0.5 + nu * h / (2*logu);
  double q = 1 - p;

  Lattice L(N+1), V(N+1);
  L.AdditiveForwardPass(log(spot), logu);
   
  PutValueFromLog put_value(strike);
  V.MultiplicativeRollback(L, exp(-rate * h) * p, exp(-rate * h) * q, put_value);
  
  return V.points[0][0];

}

double PriceAmericanCall_JKY(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {
  double h = time_to_expiry/N;

  double nu = rate - sigma*sigma/2;
  double p = 0.5 + sigma * sqrt(h) / (2*sqrt(4+sigma*sigma*h));
  double logu = nu * h + (1-p)*sigma*sqrt(h)/sqrt(p*(1-p));
  double logd = nu * h - p*sigma*sqrt(h)/sqrt(p*(1-p));

  Lattice L(N+1), V(N+1);
  L.AdditiveForwardPass(log(spot), logu, logd);
   
  CallValueFromLog call_value(strike);
  V.MultiplicativeRollback(L, exp(-rate * h) * p, exp(-rate * h) * (1-p), call_value);
  
  return V.points[0][0];

}

double PriceAmericanPut_JKY(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {
  double h = time_to_expiry/N;

  double nu = rate - sigma*sigma/2;
  double p = 0.5 + sigma * sqrt(h) / (2*sqrt(4+sigma*sigma*h));
  double logu = nu * h + (1-p)*sigma*sqrt(h)/sqrt(p*(1-p));
  double logd = nu * h - p*sigma*sqrt(h)/sqrt(p*(1-p));

  Lattice L(N+1), V(N+1);
  L.AdditiveForwardPass(log(spot), logu, logd);
   
  PutValueFromLog put_value(strike);
  V.MultiplicativeRollback(L, exp(-rate * h) * p, exp(-rate * h) * (1-p), put_value);
  
  return V.points[0][0];

}

double PriceEuropeanCall_LR(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {
  double h = time_to_expiry/N;

  double nu = rate - sigma*sigma/2;

  double d1 = (log(spot/strike) + nu*time_to_expiry) / (sigma * sqrt(time_to_expiry));
  double d2 = d1 - sigma*sqrt(time_to_expiry);

  double rr = exp(rate * h);
  
  auto f = [N](double z) {return 0.5 + (z>=0?0.5:-0.5)*pow(1 - exp( -pow( z / (N + 1.0/3 + 0.1 / (N+1)),2 ) *(N+1.0/6)  ),0.5 );};

  double p = f(d2);
  double pp = f(d1);
  double u = rr*pp/p;
  double d = (rr - p*u)/(1-p);

  Lattice L(N+1), V(N+1);
  L.AdditiveForwardPass(log(spot), log(u), log(d));
   
  CallValueFromLog call_value(strike);
  V.MultiplicativeRollbackEU(L, exp(-rate * h) * p, exp(-rate * h) * (1-p), call_value);
  
  return V.points[0][0];

}

double PriceEuropeanPut_LR(double spot, double time_to_expiry, double strike, double rate, double sigma, long N) {
  double h = time_to_expiry/N;

  double nu = rate - sigma*sigma/2;
  double rr = exp(rate * h);

  double d1 = (log(spot/strike) + nu*time_to_expiry) / (sigma * sqrt(time_to_expiry));
  double d2 = d1 - sigma*sqrt(time_to_expiry);
  
  auto f = [N](double z) {return 0.5 + (z>=0?0.5:-0.5)*pow(1 - exp( - pow( z / (N + 1.0/3 + (0.1 / (N+1))), 2) *(N+(1.0/6))  ),0.5 );};

  double p = f(d2);
  double pp = f(d1);
  double u = rr*pp/p;
  double d = (rr - p*u)/(1-p);

  Lattice L(N+1), V(N+1);
  L.AdditiveForwardPass(log(spot), log(u), log(d));
   
  PutValueFromLog put_value(strike);
  V.MultiplicativeRollbackEU(L, exp(-rate * h) * p, exp(-rate * h) * (1-p), put_value);
  
  return V.points[0][0];

}


