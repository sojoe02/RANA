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

#include <chrono>
#include <climits>
#include <thread>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "agentdomain.h"
#include "../physics/phys.h"
#include "../physics/gridmovement.h"
#include "../physics/shared.h"
#include "output.h"
#include "ID.h"
#include "agents/doctor.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::steady_clock;

AgentDomain::AgentDomain(Control *control)
    :control(control), mapGenerated(false), stop(false), fetchPositions(false),
      LuaAgentAmount(0),luaFilename(""),storePositions(true),
      positionFilename("_positionData.pos")
{
    Phys::seedMersenne();
    masteragent = new Master();
}

AgentDomain::~AgentDomain(){
    //ID::resetSystem();
    Phys::setCTime(0);
    delete masteragent;
}
/**
 * Checks if an environment has been generated.
 * @return bool false if there is not environment, true if there is.
 * */
bool AgentDomain::checkEnvPresence()
{
    return mapGenerated;
}

/**
 * Generates the environment.
 * Upon environment generation the nestenes will be placed, autons will be
 * assigned to a nestene and placed within it's parameters.
 */
void AgentDomain::generateEnvironment(double width, double height, int resolution,
                                      int listenerSize, int screamerSize, int LUASize,
                                      double timeResolution, int macroFactor, std::string filename)
{

    this->timeResolution = timeResolution;
    this->macroFactor = macroFactor;

    macroResolution = macroFactor * timeResolution;

    Output::KillSimulation = false;

    ID::resetSystem();
    Phys::setTimeRes(timeResolution);
    Phys::setCTime(0);
    Phys::setMacroFactor(macroFactor);
    Phys::setEnvironment(width, height);
    Shared::initShared();
    Doctor::InitDoctor(masteragent);

    masteragent->generateMap(width,height,resolution,timeResolution, macroResolution);

    mapWidth = width;
    mapHeight = height;

    LuaAgentAmount = LUASize;
    luaFilename = filename;

    //    masteragent->populateSystem(listenerSize, screamerSize, LUASize, filename);
    //retrievePopPos();
    //mapGenerated = true;
}

void AgentDomain::populateSystem()
{
    masteragent->populateSystem(0, 0, LuaAgentAmount, luaFilename);
    retrievePopPos();
    mapGenerated = true;
}


/**
 * Retrieval of auton positions.
 * Will write the positions of all autons to the std::lists given as arguments.
 * @see Master::retrievePopPos()
 * @see Control::refreshPopPos()
 */
void AgentDomain::retrievePopPos()
{

    std::list<agentInfo> agentPositions = masteragent->retrievePopPos();

    control->refreshPopPos(agentPositions);

    if(Output::RunSimulation.load())
    {
        if(storePositions == true )
        {
            std::ofstream file(positionFilename.c_str(),std::ofstream::out | std::ofstream::app);

            for(auto itr = agentPositions.begin(); itr != agentPositions.end(); ++itr)
            {

                agentTmu agenttmu;
                agenttmu.info = *(itr);
                agenttmu.tmu = cMacroStep;

                file.write(reinterpret_cast<char*>(&agenttmu), sizeof(agentTmu));
            }
            file.close();
        }
    }
}

void AgentDomain::toggleLiveView(bool enable)
{
    fetchPositions.store(enable);
}

/**
 * Runs the simulation.
 * Start a simulation run, this will run a simulation, width the defined macro and micro
 * precision, the simulation can be cancelled via the atomic boolean stop. The run will
 * update the progress bar and status window in the running panel.
 * @param time the amount of seconds the simulation will simulate.
 */
void AgentDomain::runSimulation(int time)
{
    if(remove(positionFilename.c_str()) != 0)
    {
        Output::Inst()->kprintf("Position file does not exist");
    }

    stop = false;
    Output::Inst()->kprintf("Running Simulation of: %i[s], with resolution of %f \n",
                            time, timeResolution);
    Output::RunSimulation = true;

    unsigned long long iterations = (double)time/timeResolution;

    auto start = steady_clock::now();
    auto start2 = steady_clock::now();
    auto end = steady_clock::now();

    //unsigned long long run_time = 0;
    cMacroStep = 0;
    cMicroStep = ULLONG_MAX;
    unsigned long long i = 0;//, j = 0;

    for(i = 0; i < iterations;)
    {
        if(Output::KillSimulation.load() == true)
            return;


        Phys::setCTime(i);
        if(i == cMicroStep && cMicroStep != ULLONG_MAX)
        {
            masteragent->microStep(i);
            //Output::Inst()->kprintf("i is now %lld", i);
        }
        if(i == cMacroStep)
        {
            masteragent->macroStep(i);
            cMacroStep +=macroFactor;
            int delay = Output::DelayValue.load();
            if(delay != 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
            if(cMacroSter % 1000 == 0)
                retrievePopPos();
        }
        i = cMacroStep;
        cMicroStep = masteragent->getNextMicroTmu();

        if( i > cMicroStep)
        {
            i = cMicroStep;
        }

        //		//Update the status and progress bar screens:
        end = steady_clock::now();
        if(duration_cast<milliseconds>(end-start).count() > 50)
        {
            masteragent->printStatus();
            Output::Inst()->progressBar(cMacroStep,iterations);
            //int delay = Output::DelayValue.load();
            //std::this_thread::sleep_for(std::chrono::milliseconds(5));
            //if(delay != 0)
            //fetchPositions.store(true);
            //if(fetchPositions.load())
            //{
            //   retrievePopPos();
            //}

            start = end;
        }
        if(stop.load() == true || Output::RunSimulation==false)
        {
            Output::Inst()->kprintf("Stopping simulator at microstep %llu \n", i);
            break;
        }
    }

    retrievePopPos();
    masteragent->simDone();
    masteragent->printStatus();
    Output::Inst()->progressBar(i,iterations);

    Output::RUNTIME = i;

    auto endsim = steady_clock::now();
    duration_cast<seconds>(start2-endsim).count();
    Output::Inst()->kprintf("Simulation run took:\t %llu[s] of computing time"
                            , duration_cast<seconds>(endsim - start2).count()
                            );
}

/**
 * Stop currently running simulation
 * Stops the active simulation run via setting an atomic boolean.
 */
void AgentDomain::stopSimulation()
{
    stop.store(true);
}

/**
 * Save eEvent data to disk
 * @see EventQueue::saveEEventData
 */
void AgentDomain::saveExternalEvents(std::string filename)
{
    masteragent->saveExternalEvents(filename);
}
