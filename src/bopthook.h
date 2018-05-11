#ifndef BOPTHOOK_H
#define BOPTHOOK_H

#include <iostream>
#include "bayesopt/bayesopt.hpp"
#include "src/cli.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class Cli;
class bopthook: public bayesopt::ContinuousModel
{
    public:
        bopthook(Cli *_cli, size_t dim, bayesopt::Parameters _param):ContinuousModel(dim, _param)
        {
            this->cli = _cli;
        }

        double testFunction(const double *x);
        double evaluateSample( const vectord &Xi );

        bool runMoreIterations();
        bool checkReachability( const vectord &query ){ return true; }

    private:
        Cli *cli = nullptr;
};

#endif // BOPTHOOK_H
