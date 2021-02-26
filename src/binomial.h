public class Binomial RV {
    public:
    
    int depth;
    double u,d;
    std::vector<std::vector<double>> tslice;

    BinomialRV(int n, double uprate, double S);

    void step_forward();
    
    void extend(int n);
    std::vector<double> slice(int t);
}
