#ifndef BOPTHOOK_H
#define BOPTHOOK_H

#include <iostream>

#include "bayesopt/bayesopt.hpp"
class bopthook: public bayesopt::ContinuousModel
{
    public:
        bopthook(size_t dim, bayesopt::Parameters param):ContinuousModel(dim, param)
        {
            // My constructor
        }

        double testFunction(unsigned int n, const double *x);
        double evaluateSample( const vectord &Xi );
        bool checkReachability( const vectord &query ){ return true; }
};

#endif // BOPTHOOK_H
