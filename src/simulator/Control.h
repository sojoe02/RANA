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

#ifndef CONTROL_H
#define CONTROL_H

//STL libraries:
#include <string>
#include <list>

#include "src/simulator/core/FlowControl.h"
#include "src/simulator/utility/Utility.h"

class FlowControl;

class Control
{
public:

    Control();
    ~Control();

    bool startSimulation(unsigned long long runTime);

    bool stopActiveFlow();

    /**
     * @brief generateEnvironment
     * Generates a new environment, an environment is needed to
     * start a simulation run.
     * @param flowId
     * @param map pointer to the loaded image map
     * @param scale amount of m2 pr pixel
     * @param timeRes microstep resolution
     * @param macroRes macrostep resotution
     * @param agentAmount number of Lua agents
     * @param agentPath path to the agent
     */
    int generateFlow(int threads,
                     double timeRes, int macroRes,
                     int agentAmount, std::string agentPath,
                     int width, int height);

    bool isActive(int flowId);

    void saveEvents(int flowId, std::string path);

private:

    std::list<std::thread> runningFlows;
    bool generated;
    bool stopped;
    bool generating;
    int activeFlowId;
    std::unique_ptr<FlowControl> activeSimulationFlow;

    bool flowActive = false;
};

#endif // CONTROL_H
