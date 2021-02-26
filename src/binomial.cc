#include <vector>
#include "binomial.h"
    
BinomimalRV::BinomialRV(int n, double delta, double s) {
    tslice = {{s}};
    maxdepth = n;
    d = delta;
}

void BinomialRV::step_forward() {
    tslice.push_back({0}); // add a new slice

    for(i = 0; i <= depth ; i++) {
        tslice[depth][i] = tslice[depth-1][i] + d;
        tslice[depth].push_back(tslice[depth-1][i] - d);
    }
    depth++;
}

void BinomialRV::extend(int n) {
    while ( n++ < depth )
        step_forward();
    return;
}

std::vector<double> BinomialRV::slice(int t) {
    return tslice[t];
}


