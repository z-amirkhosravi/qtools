#include <vector>
#include <iostream>

std::vector<double> cfunc(std::vector<double> &inp) {
    std::vector<double> outp;
    std::cout << "transforming\n";
    for (double x:inp)
        outp.push_back(x * x);
    return outp;
}


