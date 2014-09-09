//--begin_license--
//
//Copyright 	2013 	Søren Vissing Jørgensen.
//			2014	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.  
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
#include "runner.h"

#include "output.h"

Runner::Runner():
    agentDomain(NULL), runTime(0)
{

}

void Runner::run()
{

    if(agentDomain != NULL && runTime != 0)
    {
        agentDomain->runSimulation(runTime);
    } else
        Output::Inst()->kprintf("No Agentdomain defined, cannot run simulation");
    emit simulationDone();

}

void Runner::setParameters(AgentDomain *agentDomain, unsigned long long runTime)
{
	this->agentDomain = agentDomain;
    this->runTime = runTime;
}


//void Runner::startSimulation()
//{
//    Output::Inst()->kprintf("from within run thread");
//    //agentdomain->runSimulation(runTime);
//    emit simulationDone();
//}
