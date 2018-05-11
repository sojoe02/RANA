#ifndef BOPTHOOK_H
#define BOPTHOOK_H

#include <iostream>
#include "bayesopt/bayesopt.hpp"
#include "src/cli.h"

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


public slots:
    void runSimulation();

private:
    Cli *cli = nullptr;

signals:
    void runBoptSimSignal();
};

#endif // BOPTHOOK_H
