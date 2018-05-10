#include "bopthook.h"

double bopthook::testFunction(unsigned int n, const double *x)
{
    double f = 10.;
    for (unsigned int i = 0; i < n; ++i){
        f += (x[i] - .53) * (x[i] - .53);
    }
    for (unsigned int i = 0; i < n; ++i){
        std::cout << x[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Returning f: " << f << std::endl;
}

double bopthook::evaluateSample( const vectord &Xi ){
    size_t i;
    double x[this->mDims];
    for (i = 0; i < Xi.size(); ++i)
    {
        x[i] = Xi(i);
    }

    std::cout << "Size V: " << i << std::endl;
    return testFunction(Xi.size(), x);
}
