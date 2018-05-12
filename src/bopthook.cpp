#include "bopthook.h"


bopthook::bopthook(Cli *_cli, size_t dim, bayesopt::Parameters _param):
    ContinuousModel(dim, _param)
{
    this->cli = _cli;
}

double bopthook::evaluateSample( const vectord &Xi ){
    size_t i;
    std::cout << "\tWe need to evaluate samples with the following:\n\t";
    for (i = 0; i < Xi.size(); ++i)
    {
        std::cout << Xi(i) << " ";
    }
    std::cout << "\n\tSize Vector/Dimensions: " << i << std::endl;

    return true;
}

std::string bopthook::getNextFile(){
    std::string tmp = "/home/theis/Dropbox/frog/v1/bopt/testFiles/8012_green_2.lua";
    return tmp;
}








