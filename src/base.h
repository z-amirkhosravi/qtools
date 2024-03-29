// author: Z. Amir-Khosravi
//
// Declares the base structures for numerical approximation methods

#ifndef BASE_H
#define BASE_H

#include <vector>
// An abstract function class that will be passed to the numerical methods 
// We can't use a function pointer, because some of these functions will be non-constant methods of instantiated classes

class FunctionClass {
    public:
    virtual double eval(double x) const = 0; 
};

// The class Lattice implements what's called the "binary tree model" for derivative pricing in quant finance..
// But the structure in those models is not actually a tree, rather a truncated lattice, hence the name of this class.
// 
// Given size n, simulation data is stored in n+1 vectors of size 1,...,n. 
// The values in the kth vector will correspond to lattice points (a,b) with a+b=k and a,b>=0.

// This corresponds to a binomial model with "n steps".
//
// The values are conceptually linked in a directed graph as follows :
//
//                 (3,3) 
//                ^
//               /
//            (1,2) ---> (3,2) 
//           ^          ^    
//          /          /       
//       (1,1) ---> (2,1) ---> (3,1) 
//       ^          ^          ^   
//      /          /          /       
// (0,0) ---> (1,0) --> (2,0) ---> (3,0) 
//
// (size 3 example)
//
//  The horizontal edges (a,b) --> (a+1,b) are the "down" directions of binomial models in the literature.
//  The diagonal edges (a,b) -- > (a+1,b+1) correspond to the "up" directionss.
//
//  The choice of direction names is meant to conform with the literature..
//
class Lattice {
  public:
    std::vector<std::vector<double>> points = {{}};
    
    int size(void);

    // the set_size method will also allocate the required memory, which is (n+1)*(n+2)/2 doubles
    void set_size(int n);  

    void set_initial(double initial_value);
    void set_terminal(std::vector<double> terminal_values);
    std::vector<double> get_terminal(void);

    Lattice(void);
    Lattice(int n);

    void AdditiveForwardPass(double initial_value, double delta_up, double delta_down);
    // This function firt sets the node at (0,0) to the given initial value, then progressively sets all the other values.
    // 
    // If a node values V(a,b) are calculated according to the rules:
    // V(a+1,b) = V(a,b) + delta_up
    // V(a,b+1) = V(a,b) + delta_down
    //
    // In the literature, this process is describe multiplicatively, with delta_up and delta_down corresponding to log(u), log(d)
    //
    // 1) Addition is faster than multiplication.
    // 2) Without taking logs, the node values further out will grow extremely large, and possibly overflow.
    //


    void AdditiveForwardPass(double seed, double logu);
    // Same as above, except assumes delta_down = -delta_up, e.g when they are log(u) and log(d) with ud=1



   void MultiplicativeRollback(Lattice &ref_lattice, double p, double q, FunctionClass &f);
    // This method implements the core process of pricing an American derivative using a binomial model.
    //
    // The parameter L should contain the log values of the underlying, typically generated by AdditiveForwardPass.
    //
    // The parameters p and q will be the weights used to calculate the expected value of the derivative at each node.
    //
    // The function f should return the intrinsic value of the option given the log of underlying. 
    //
    // NOTE 1: p and q are _not_ assumed to add to 1, because they may incorporate discounting and dividend factors.
    // NOTE 2: f should take log values as input, but must return actual (exponentiated) values.
    //

    void MultiplicativeRollbackEU(Lattice &ref_lattice, double p, double q, FunctionClass &f);

    // Same as MultiplicativeRollback(), except without comparing with intrinsic value.
};

// This function implements a simple Monte Carlo average. Given a function f and integer M, it returns the average
// value of f over M draws from normal distribution. The values are generated using the standard library.
 
double SimpleMC(const FunctionClass &f, long M);

// This function implements a simple Monte Carlo average. Given a functino f and integer M, it returns the average
// value of f over M draws from a normal distribution. The values are generated using the boost library.
//
double SimpleMCUsingBoost(const FunctionClass &f, long M);


#endif
