//--begin_license--
//
//Copyright 	2013 	Søren Vissing Jørgensen.
//				2014-2016	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.
//
//This file is part of RANA.
//
//RANA is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//RANA is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with RANA.  If not, see <http://www.gnu.org/licenses/>.
//
//--end_license--

#include "src/runner.h"
#include "src/output.h"

Runner::Runner(){
    std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
    std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
    std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
    std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
    std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
}

void Runner::doWork(FlowControl *agentDomain, unsigned long long runTime)
{
    std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
    if( agentDomain != nullptr ){
        if( runTime != 0 ){
            std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
            agentDomain->runSimulation(runTime);
            std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
        } else{
            Output::Inst()->kprintf("No Runtime defined");
        }
    } else{
        Output::Inst()->kprintf("No Agentdomain defined");
    }
    std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << "\t - emit simulationDone" << std::endl;
    emit simulationDone();
    std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << "\t - post simulationDone" << std::endl;
}

