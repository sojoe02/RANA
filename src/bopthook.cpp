#include "bopthook.h"

double bopthook::testFunction(const double *x)
{
    double f = 10.;
    x += 1;
    return f;
}

double bopthook::evaluateSample( const vectord &Xi ){
    size_t i;
    std::cout << "\tWe need to evaluate samples with the following:\n\t";
    for (i = 0; i < Xi.size(); ++i)
    {
        std::cout << Xi(i) << " ";
    }
    std::cout << "\n\tSize Vector/Dimensions: " << i << std::endl;

    std::cout << "hello 1234" << std::endl;

    return true;
}

bool bopthook::runMoreIterations(){

    if( this->getCurrentIter()+1 > this->getParameters()->n_iterations){
        return false;
    }

    return true;
}
