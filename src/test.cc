#include "simple_mc.h"
#include <random>
#include "pricing.h"
#include <iostream>

int main(void) {
    PayOff call_payoff;

    call_payoff.call_initializer(120,3,100,0.02, 0.7);

    call_payoff.debug();

    std::default_random_engine generator;
    std::normal_distribution<double> normdist(0,1);
    for(int i=0;i<10;i++)
	std::cout << call_payoff.eval(normdist(generator)) << std::endl;
    return 0;
}
