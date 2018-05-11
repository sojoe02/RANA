#include "bopthook.h"


bopthook::bopthook(Cli *_cli, size_t dim, bayesopt::Parameters _param):
    ContinuousModel(dim, _param)
{
    this->cli = _cli;

    connect(this, &bopthook::runBoptSimSignal, cli, &Cli::runBoptController);
    //connect(this, SIGNAL(runBoptSimSignal(std::string)), this, SLOT(runSimulation(std::string)), Qt::BlockingQueuedConnection);

}

double bopthook::testFunction(const double *x)
{
    double f = 10.;
    x += 1;
    return f;
}

void bopthook::runSimulation(){
    cli->runBoptController();
    std::cout << "Done in runSimulation" << std::endl;
}

double bopthook::evaluateSample( const vectord &Xi ){
    size_t i;
    std::cout << "\tWe need to evaluate samples with the following:\n\t";
    for (i = 0; i < Xi.size(); ++i)
    {
        std::cout << Xi(i) << " ";
    }
    std::cout << "\n\tSize Vector/Dimensions: " << i << std::endl;

    emit runBoptSimSignal();

    //while(true);

    std::cout << "TEST TEST TEST TEST FUCK FUCK FUCK" << std::endl;
    std::cout << "TEST TEST TEST TEST FUCK FUCK FUCK" << std::endl;
    std::cout << "TEST TEST TEST TEST FUCK FUCK FUCK" << std::endl;
    std::cout << "TEST TEST TEST TEST FUCK FUCK FUCK" << std::endl;
    std::cout << "TEST TEST TEST TEST FUCK FUCK FUCK" << std::endl;



    return true;
}

std::string bopthook::getNextFile(){
    std::string tmp = "/home/theis/Dropbox/frog/v1/bopt/testFiles/8012_green_2.lua";
    return tmp;
}








