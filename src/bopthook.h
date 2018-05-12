#ifndef BOPTHOOK_H
#define BOPTHOOK_H

#include <bayesopt/bayesopt.hpp>
#include "src/cli.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class Cli;
class bopthook: public QObject, public bayesopt::ContinuousModel
{
    Q_OBJECT
public:
    bopthook(Cli *_cli, size_t dim, bayesopt::Parameters _param);

    double testFunction(const double *x);
    double evaluateSample( const vectord &Xi );

    bool checkReachability( const vectord &query ){ return true; }
    std::string getNextFile();

private:
    Cli *cli = nullptr;
};

#endif // BOPTHOOK_H
