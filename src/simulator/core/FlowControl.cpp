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
#include <time.h>

#include "FlowControl.h"
#include "src/simulator/api/Phys.h"
#include "src/simulator/api/GridMovement.h"
#include "src/simulator/api/Shared.h"
#include "src/simulator/Output.h"
#include "src/simulator/utility/ID.h"
#include "src/simulator/api/Scanning.h"
#include "src/simulator/Control.h"
#include "Interfacer.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::steady_clock;

FlowControl::FlowControl()
        : mapGenerated(false), masteragent(new Supervisor()), stop(false), fetchPositions(false),
          agentAmount(0), luaFilename(""), storePositions(true), flowDone(false),
          positionFilename("_positionData.pos")
{
    //Phys::seedMersenne();
    //file = std::ofstream(positionFilename.c_str(),std::ofstream::out| std::ofstream::trunc);
    //file.open(positionFilename.c_str(),std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
}

FlowControl::~FlowControl()
{
    //ID::resetSystem();
    Phys::setCTime(0);
    delete masteragent;
}

/**
 * Checks if an environment has been generated.
 * @return bool false if there is not environment, true if there is.
 * */
bool FlowControl::checkEnvPresence()
{
    return mapGenerated;
}

/**
 * Generates the environment.
 * Upon environment generation the sectors will be placed, autons will be
 * assigned to a sector and placed within it's parameters.
 */
void FlowControl::generateEnvironment(double width, double height, int threads,
                                      int agentAmount, double timeResolution, int macroFactor, std::string filename)
{

    //srand(time(0));
    //Phys::seedMersenne();
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
    Interfacer::initInterfacer(masteragent);
    //Scanning::edgeMask();

    masteragent->generateMap(width, height, threads, timeResolution, macroResolution);

    mapWidth = width;
    mapHeight = height;

    agentAmount = agentAmount;
    luaFilename = filename;

    masteragent->populateSystem(0, 0, agentAmount, filename);

    retrievePopPos();
    mapGenerated = true;
}

void FlowControl::populateSystem()
{
    //srand(time(0));
    //Phys::seedMersenne();
    masteragent->populateSystem(0, 0, agentAmount, luaFilename);
    retrievePopPos();
    mapGenerated = true;
    //Output::enableRunBotton(true);
}


/**
 * Retrieval of auton positions.
 * Will write the positions of all autons to the std::lists given as arguments.
 * @see Master::retrievePopPos()
 * @see Control::refreshPopPos()
 */
void FlowControl::retrievePopPos()
{

    std::list<agentInfo> agentPositions = masteragent->retrievePopPos();

    //control->refreshPopPos(agentPositions);

    if (Output::RunSimulation.load())
    {
        if (storePositions == true)
        {

            //std::ofstream file(positionFilename.c_str(),std::ofstream::out | std::ofstream::app);
            for (const auto &apos : agentPositions)
            {

                agentTmu agenttmu;
                agenttmu.x = apos.x;
                agenttmu.y = apos.y;
                agenttmu.id = apos.id;
                agenttmu.tmu = cMacroStep;
                //Output::Inst()->kprintf("id %d, posx %d, posY %d",agenttmu.info.id, agenttmu.info.x, agenttmu.info.y);

                file.write(reinterpret_cast<char *>(&agenttmu), sizeof(agentTmu));
            }
            //file.close();
        }
    }
}

void FlowControl::toggleLiveView(bool enable)
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
void FlowControl::runSimulation(unsigned long long time)
{


    std::string positionFilePath = Output::RanaDir;
    positionFilePath.append("/");
    positionFilePath.append(positionFilename.c_str());

    if (remove(positionFilePath.c_str()) != 0)
    {
        Output::kprintf("Position file does not exist");
    }

    file.open(positionFilePath.c_str(), std::ofstream::out | std::ofstream::app | std::ofstream::binary);

    stop = false;
    Output::kprintf("Running Simulation of: %i[s], with resolution of %f \n",
                    time, timeResolution);
    Output::RunSimulation = true;

    unsigned long long iterations = (double) time / timeResolution;

    auto start = steady_clock::now();
    auto start2 = steady_clock::now();
    auto end = steady_clock::now();

    //unsigned long long run_time = 0;
    cMacroStep = 0;
    cMicroStep = ULLONG_MAX;
    unsigned long long i = 0;//, j = 0;

    for (i = 0; i < iterations;)
    {
        if (Output::KillSimulation.load() == true)
            return;


        Phys::setCTime(i);

        if (i == cMicroStep && cMicroStep != ULLONG_MAX)
        {
            masteragent->microStep(i);
            //Output::Inst()->kprintf("i is now %lld", i);
        }

        if (i == cMacroStep)
        {
            masteragent->macroStep(i);
            cMacroStep += macroFactor;
            int delay = Output::DelayValue.load();
            if (delay != 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
            if (cMacroStep % (int) Phys::getMacroFactor() == 0)
            {
                retrievePopPos();
            }
        }

        i = cMacroStep;
        cMicroStep = masteragent->getNextMicroTmu();

        if (i > cMicroStep)
        {
            i = cMicroStep;
        }

        //		//Update the status and progress bar screens:
        end = steady_clock::now();
        if (duration_cast<milliseconds>(end - start).count() > 100)
        {
            masteragent->printStatus();
            Output::progressBar(cMacroStep, iterations);

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
        if (stop.load() == true || Output::RunSimulation == false)
        {
            Output::kprintf("Stopping simulator at microstep %llu \n", i);
            break;
        }
    }

    retrievePopPos();

    masteragent->printStatus();
    Output::progressBar(i, iterations);

    Output::RUNTIME = i;

    auto endsim = steady_clock::now();
    duration_cast<seconds>(start2 - endsim).count();
    Output::kprintf("Simulation run took:\t %llu[s] of computing time",
                    duration_cast<seconds>(endsim - start2).count()
    );
    file.close();
    masteragent->simDone();
    flowDone = true;
}

/**
 * Stop currently running simulation
 * Stops the active simulation run via setting an atomic boolean.
 */
void FlowControl::stopSimulation()
{
    stop.store(true);
}

/**
 * Save eEvent data to disk
 * @see EventQueue::saveEEventData
 */
void FlowControl::saveExternalEvents(std::string filename)
{
    masteragent->saveExternalEvents(filename);
}
