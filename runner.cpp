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
