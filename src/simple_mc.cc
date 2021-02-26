#include <random>
#include "simple_mc.h"
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

double SimpleMC(const FunctionClass &f, long M) {
    std::mt19937 gen;
    std::normal_distribution<double> normdist(0,1);

    gen.seed(12317);

    double sum = 0;
    for (int i = 0; i < M; i++) {
	double x = normdist(gen);
        sum += f.eval(x);
	sum += f.eval(-x); // corresponding to  the 'antithetic' path
    }
    return sum / (2*M);
}
double SimpleMCUsingBoost(const FunctionClass &f, long M) {
    boost::mt19937 gen;
    boost::normal_distribution<> normdist(0,1);
    
    boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > varnorm(gen, normdist);

    double sum = 0;
    for (int i = 0; i < M; i++) {
	double x = varnorm();
        sum += f.eval(x);
	sum += f.eval(-x); // corresponding to  the 'antithetic' path
    }
    return sum / (2*M);
}

