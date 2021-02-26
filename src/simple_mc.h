#ifndef SIMPLE_MC_H
#define SIMPLE_MC_H

class FunctionClass {
    public:
    virtual double eval(double x) const = 0; 
};

double SimpleMC(const FunctionClass &f, long M);

double SimpleMCUsingBoost(const FunctionClass &f, long M);


#endif
