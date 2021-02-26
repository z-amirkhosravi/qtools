#ifndef PRICING_H
#define PRICING_H

class CallPayOff: public virtual FunctionClass {
    public:
	double eval(double x) const;
	CallPayOff(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_);
    private:
    	double spot, strike, time_to_expiry, rate, vol;
	double drift, threshold, base_price_factor, step_std;
};

class PutPayOff: public virtual FunctionClass {
    public:
        double eval(double x) const;
	PutPayOff(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_);
    private:
	double spot, strike, time_to_expiry, rate, vol;
	double drift, threshold, base_price_factor, step_std;
};

class DigitalCallPayOff: public virtual FunctionClass {
    public:
	double eval(double x) const;
	DigitalCallPayOff(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_);
    private:
	double spot, strike, time_to_expiry, rate, vol;
	double drift, threshold, base_price_factor, step_std;
};

class DigitalPutPayOff: public virtual FunctionClass {
    public:
	double eval(double x) const;
	DigitalPutPayOff(double spot_, double time_to_expiry_, double strike_, double rate_, double vol_);
    private:
	double spot, strike, time_to_expiry, rate, vol;
	double drift, threshold, base_price_factor, step_std;
};


double PriceVanillaEuCall(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds);

double PriceVanillaEuPut(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds);

double PriceDigitalEuCall(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds);

double PriceDigitalEuPut(double spot, double time_to_expiry, double strike, double rate, double vol, long num_rounds);


#endif
