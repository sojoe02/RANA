//--begin_license--
//
//Copyright 	2013-2014 	Søren Vissing Jørgensen.
//			2014-	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.
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



#include "Control.h"
#include "Output.h"


Control::Control()
        : generated(false), stopped(true), generating(false), activeFlowId(0), activeSimulationFlow(nullptr)
{
}

Control::~Control()
{
}

bool Control::startSimulation(unsigned long long runTime)
{
    activeSimulationFlow->runSimulation(runTime);
    return true;
}

bool Control::stopActiveFlow()
{
    //NEEDS BETTER CONTROL! like a check on whether the simulation is still running or not!
    activeSimulationFlow->stopSimulation();
    return true;
}

int Control::generateFlow(int threads, double timeRes, int macroRes,
                          int agentAmount, std::string agentPath, int width, int height)
{
    if (!activeSimulationFlow->flowDone || activeSimulationFlow == nullptr)
    {
        activeFlowId++;

        activeSimulationFlow = std::make_unique<FlowControl>();
        activeSimulationFlow->generateEnvironment(width, height, threads,
                                                  agentAmount, timeRes, macroRes, std::move(agentPath));
        activeSimulationFlow->populateSystem();

        flowActive = true;

        return activeFlowId;
    }
    else
    {
        //flow is still running.
        return -1;
    }
}

bool Control::isActive(int flowId)
{
    if (activeSimulationFlow != nullptr)
        return activeSimulationFlow->flowDone;

    return false;
}

void Control::saveEvents(int flowId, std::string path)
{
    if (activeSimulationFlow != nullptr)
    {
        activeSimulationFlow->saveExternalEvents(std::move(path));

    }
}

//void Control::toggleLiveView(bool enable)
//{
//    if (flowControl != nullptr)
//        flowControl->toggleLiveView(enable);
//}


