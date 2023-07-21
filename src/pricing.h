// author: Z. Amir-Khosravi
// 
// This header declares the classes used to represent the intrinsic value of different derivatives, as well as functions
// that use them to return the computed price.

#ifndef PRICING_H
#define PRICING_H

#include "base.h"

// This class represents the intrinsic value of a call option as a function of the log of the underlying.
class CallValueFromNormal: public virtual FunctionClass {
    public:
	double eval(double x) const;
	CallValueFromNormal(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_);
    private:
    	double spot, strike, time_to_expiry, rate, vol;
	double drift, threshold, base_price_factor, step_std;
};

// This class represents the intrinsic value of a put option as a function of the log of the underlying.
class PutValueFromLog: public virtual FunctionClass {
  public:
    double eval(double x) const;
    PutValueFromLog(double strike_);
  private:
    double logstrike, strike;
};


// This class represents the intrinsic value of a call option as a function of the log of the underlying.
class CallValueFromLog: public virtual FunctionClass {
  public:
    double eval(double x) const;
    CallValueFromLog(double strike_);
  private:
    double logstrike, strike;
};

// This class represents the intrinsic value of a put option as a function of a normal random variable.
class PutValueFromNormal: public virtual FunctionClass {
    public:
        double eval(double x) const;
	PutValueFromNormal(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_);
    private:
	double spot, strike, time_to_expiry, rate, vol;
	double drift, threshold, base_price_factor, step_std;
};

//This class represents the intrinsic value of a digital call option.
class DigitalCallValueFromNormal: public virtual FunctionClass {
    public:
	double eval(double x) const;
	DigitalCallValueFromNormal(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_);
    private:
	double spot, strike, time_to_expiry, rate, vol;
	double drift, threshold, base_price_factor, step_std;
};

// This class represents the intrinsic value of a digital put option.
class DigitalPutValueFromNormal: public virtual FunctionClass {
    public:
	double eval(double x) const;
	DigitalPutValueFromNormal(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_);
    private:
	double spot, strike, time_to_expiry, rate, vol;
	double drift, threshold, base_price_factor, step_std;
};


double PriceVanillaEuCall(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds);

double PriceVanillaEuPut(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds);

double PriceDigitalEuCall(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds);

double PriceDigitalEuPut(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds);

// This function is a naive computation of a call price. It's included only as an example, and should not be used.
double PriceAmericanCallNaive(double spot, double time_to_expiry, double strike, double rate, double vol, long tree_depth); 

double PriceAmericanCall(double spot, double time_to_expiry, double strike, double rate, double vol, long tree_depth); 

double PriceAmericanPut(double spot, double time_to_expiry, double strike, double rate, double vol, long tree_depth); 

double PriceAmericanCall_Tian(double spot, double time_to_expiry, double strike, double rate, double sigma, long N);
// Price an American call using the binomial model from the 1993 paper:
// "A Modified Lattice Approach to Option Pricing" by Yison Tian

double PriceAmericanPut_Tian(double spot, double time_to_expiry, double strike, double rate, double sigma, long N);
// Price an American put using Tian's binomial model, as above

double PriceAmericanCall_CRR(double spot, double time_to_expiry, double strike, double rate, double sigma, long N);
// Price an American call using the Cox-Ross-Rubinstein model

double PriceAmericanPut_CRR(double spot, double time_to_expiry, double strike, double rate, double sigma, long N);
// Price an American put using the Cox-Ross-Rubinstein model

double PriceAmericanCall_Trig(double spot, double time_to_expiry, double strike, double rate, double sigma, long N);
// Price an American call using the Trigeorgis binomial model

double PriceAmericanPut_Trig(double spot, double time_to_expiry, double strike, double rate, double sigma, long N);
// Price an American put using the Trigeorgis binomial model


#endif
