// author: Z. Amir-Khosravi
//
// This file defines the numerical approximation methods




#include <vector>
#include <iostream>
#include <cmath>
#include <random>

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

#include "base.h"


// This function runs a simple Monte Carlo method by averaging a given function over M random numbers
// drawn from a normal distribution as provided by the standard library

double SimpleMC(const FunctionClass &f, long M) {
    std::mt19937 gen;
    std::normal_distribution<double> normdist(0,1);

    gen.seed(12317);

    double sum = 0;
    for (int i = 0; i < M; i++) {
	double x = normdist(gen);
        sum += f.eval(x);
	sum += f.eval(-x);  // corresponding to  the 'antithetic' path, for better performance
    }
    return sum / (2*M);
}

// This function runs a simple Monte Carlo method by averaging a given function over M random numbers
// drawn from a normal distribution as provided by the boost library

double SimpleMCUsingBoost(const FunctionClass &f, long M) {
    boost::mt19937 gen;
    boost::normal_distribution<> normdist(0,1);
    
    boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > varnorm(gen, normdist);

    double sum = 0;
    for (int i = 0; i < M; i++) {
	double x = varnorm();
        sum += f.eval(x);
	sum += f.eval(-x); 
    }
    return sum / (2*M);
}

   
Lattice::Lattice(int n) {  
  set_size(n);
}

Lattice::Lattice() {
set_size(0);
}

int Lattice::size(void) {
  return points.size();
}

// This function sets the size of the lattice, and allocates enough memory.

void Lattice::set_size(int n) {
  points.resize(n);
  while (n-- > 0) 
    points[n].resize(n+1);
}

void Lattice::set_initial(double initial_value) {
  points[0][0] = initial_value;
}

// This function sets the last column of the lattice to the given vector of doubles.

void Lattice::set_terminal(std::vector<double> terminal_values) {
  long unsigned int n = points.size();
  if (terminal_values.size() != n)
    return;

  for (long unsigned int i = 0 ; i < n ; i++) 
    points[n-1][i] = terminal_values[i];       

}

std::vector<double> Lattice::get_terminal(void) {
  return points.back();
}

// void Lattice::AdditiveForwardPass(double seed, double drift_delta, double stoch_delta) {
//     int n = points.size();
//     points[0][0] = seed;
//     for (int k = 0; k < n - 1 ; k++) { 
// 	for (int i = 0; i < k + 1; i++) 
// 	    points[k+1][i] = points[k][i] + drift_delta - stoch_delta;
//         points[k+1][k+1] = points[k][k] + drift_delta + stoch_delta;
//     }
// }

void Lattice::AdditiveForwardPass(double seed, double logu, double logd) {
    int n = points.size();
    points[0][0] = seed;
    for (int k = 0; k < n - 1 ; k++) { 
	for (int i = 0; i < k + 1; i++) 
	    points[k+1][i] = points[k][i] + logd;
        points[k+1][k+1] = points[k][k] + logu;
    }
}

void Lattice::AdditiveForwardPass(double seed, double logu) {
    int n = points.size();
    points[0][0] = seed;
    for (int k = 0; k < n - 1 ; k++) { 
	for (int i = 0; i < k + 1; i++) 
	    points[k+1][i] = points[k][i] - logu;
        points[k+1][k+1] = points[k][k] + logu;
    }
}

// The values V(a,b) of the node (a,b) is set to the maximum of the following two quantities:
//
// 1:  p * V(a+1,b+1) + q * V(a,b+1)
// 2:  f(L(a,b))
//
void Lattice::MultiplicativeRollback(Lattice &ref_lattice, double p, double q, FunctionClass &f) {
    long unsigned int n = ref_lattice.size(); 
    if (n!=points.size() || n==0)
	return;

    auto it = points.rbegin();
    auto ref_it = ref_lattice.points.rbegin();

    for (long unsigned int i = 0; i < n; i++)                         // sets the terminal nodes to the intrinsic value
      (*it)[i] = f.eval((*ref_it)[i]);

    while (++it != points.rend()) {
	++ref_it;
	for (long unsigned int i = 0; i < (*it).size(); i++) {
		double t1 = q * (*(it-1))[i] + p * (*(it-1))[i+1] ;     //  computes the evalue of the derivative
		double t2 = f.eval((*ref_it)[i]);	                //  computes the the intrinsic value 
		(*it)[i] = t1 > t2 ? t1 : t2;                           //  sets value to the maximum of the two
	}
    }
}


