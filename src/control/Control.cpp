#include <memory>

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
#include "src/observation/Output.h"
#include "src/observation/FlowVariables.h"

std::unique_ptr<FlowControl> Control::simulationFlow = nullptr;

Control::Control()
        : generated(false), stopped(true), generating(false)
{
}

Control::~Control()
{
}


bool Control::startSimulationThread(unsigned long long runTime)
{
    if (!FlowVariables::flowGenerating && FlowVariables::flowActive)
    {
        activeFlowThread = std::thread(startSimulation, runTime);
        return true;
    }
    return false;
}

void Control::startSimulation(unsigned long long runTime)
{
    simulationFlow->runSimulation(runTime);
}

bool Control::stopFlow()
{
    if (FlowVariables::flowRunning.load())
    {
        simulationFlow->stopSimulation();
        activeFlowThread.join();

        FlowVariables::flowActive.store(false);
        FlowVariables::flowRunning.store(false);

        return true;
    }

    else return false;
}

int Control::generateFlowThread(int threads, double timeRes, int macroRes,
                                int agentAmount, std::string flowInfo, int width, int height)
{
    if (!FlowVariables::flowRunning && !FlowVariables::flowGenerating)
    {
        activeFlowThread = std::thread(generateFlow, threads, timeRes, macroRes, agentAmount, flowInfo, width, height);
        return 0;
    }

    return -1;
}

void Control::generateFlow(int threads, double timeRes, int macroRes,
                           int agentAmount, std::string agentPath, int width, int height)
{
    if (!FlowVariables::flowRunning.load())
    {

//        //I am not sure that this block is needed!
//        FlowControl* flow = Control::simulationFlow.release();
//        if(flow != nullptr)
//            delete flow;
//        //
        FlowVariables::flowActive.store(true);

        Control::simulationFlow = std::make_unique<FlowControl>();

        simulationFlow->generateEnvironment(width, height, threads,
                                            agentAmount, timeRes, macroRes,
                                            std::move(agentPath));
        simulationFlow->populateSystem();

        FlowVariables::flowGenerating.store(false);
    }
}

void Control::saveEvents(int flowId, std::string path)
{
    if (simulationFlow != nullptr)
    {
        simulationFlow->saveExternalEvents(std::move(path));
    }
}

bool Control::isGenerated() const
{
    return generated;
}

bool Control::isStopped() const
{
    return stopped;
}

bool Control::isGenerating() const
{
    return generating;
}


