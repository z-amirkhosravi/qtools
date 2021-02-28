#include <vector>
#include <iostream>
#include <cmath>

#include "base.h"
#include "pricing.h"
/*
class IntrinsicCallValue: public virtual FunctionClass {
    public:
	IntrinsicCallValue(double strike_);
	double eval(double x)const;
    private:
	double strike, logstrike;
};

IntrinsicCallValue::IntrinsicCallValue(double strike_): strike(strike_) {
    logstrike = log(strike);
};

double IntrinsicCallValue::eval(double x) const {
    return x > logstrike ? (exp(x) - strike): 0;
}*/

int main(void) {
    double S0 = 120;
    double t = 3;
    double vol = 0.7;
    double rate = 0.02;
    double strike=90;
    double V0,V1;

    long n = 3000;
 
    V1 = PriceAmericanCall(S0,t,strike,rate,vol,n);
    std::cout << "Call Price:" << V1 << std::endl;

    V0 = PriceAmericanPut(S0,t,strike,rate,vol,n);
    std::cout << "Put Price:" << V0 << std::endl;
    return 0;
}

