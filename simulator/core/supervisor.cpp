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

#include <string>
#include <sstream>
#include <set>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <future>

#include "supervisor.h"

#include "simulator/utility/utility.h"
#include "simulator/api/phys.h"

#include "simulator/output.h"

#define TASK_STEP    300
#define TASK_STOP    400

std::condition_variable Supervisor::CvStepStart;
std::condition_variable Supervisor::CvStepDone;
std::mutex Supervisor::mutexStep;
std::mutex Supervisor::mutexStepDone;
int Supervisor::task;

Supervisor::Supervisor()
        : sectorIndex(0), sectorAmount(0), eEventInitAmount(0), responseAmount(0),
          externalDistroAmount(0), tmu(0)
{
    Output::kprintf("Initiating master\n");
    eventQueue = new EventQueue;
    srand(static_cast<u_int >(time(nullptr)));
}

Supervisor::~Supervisor()
{
    delete eventQueue;

    for (const auto &s : sectors)
    {
        s->taskPromise.set_value(TASK_STOP);
    }
    for (const auto &t : threads)
    {
        t->join();
    }

    for (const auto &s : sectors)
    {
        delete s;
    }
}
/********************************************************
 * Simulation initialization functions.
 *
 ********************************************************/

/**
 * Generates the map
 * Places all sectors at positions to fit the width and height of the map,
 * set the time resolution and macroFactor of the map
 * @param width maps max X coordinate
 * @param height maps max Y coordinate
 * @param threads number of sector, i.e threads.
 * @param time resolution seconds between microsteps.
 * @param macroResolution factor to multiply with the time resolution for the macrostep intervals.
 */

void
Supervisor::generateMap(double width, double height, int sectorAmount, double timeResolution, double macroResolution)
{
    this->timeResolution = timeResolution;
    this->macroResolution = macroResolution;
    this->sectorAmount = sectorAmount;

    areaX = width;
    areaY = height;

    if (!sectors.empty())
    {
        sectors.clear();
    }

    for (int i = 0; i < sectorAmount; i++)
    {
        std::string id;
        std::stringstream ss;
        ss << i;
        id.append(ss.str());

        auto *nest = new Sector(0, 0, width, height, this, i);

        auto *t = new std::thread(Supervisor::runStepPhase, nest);
        threads.push_back(t);

        sectors.push_back(nest);
    }
}

/**
 * Get the X and Y positions of all actors within the masters domain
 * Retrieves the information from all actors, writes the information to the lists given.
 * @param sylist y positions of all Screamer autons
 * @param sxlist x positions of all Screamer autons
 * @param lylist y positions of all Listener autons
 * @param lxlist x positions of all Listener autons
 * @param aylist y positions of all Lua autons
 * @param axlist x positions of all Lua autons
 */

std::list<agentInfo> Supervisor::retrievePopPos()
{

    std::list<agentInfo> agentinfo;
    //Output::Inst()->kprintf("testing retrieve of master");
    for (const auto &s : sectors)
    {
        s->retrievePopPos(agentinfo);
    }

    return agentinfo;
}

/**
 * Populate the system.
 * Determines the number of Agents pr sector, assigns them to their respective sector and
 * orders the sector to populate itself with dem, give them positions within themselves.
 * @param listenerSize number of listeners total.
 * @param screamerSize number of screamers total.
 * @param LUASize number of LUAs total.
 * @param filename the lua file of the LUA autons definition.
 */

void Supervisor::populateSystem(int listenerSize, int screamerSize, int LUASize, std::string filename)
{

    std::vector<int> LUAVector;

    for (uint i = 0; i < sectors.size(); i++)
    {
        LUAVector.push_back(0);
    }

    autonAmount = LUASize;
    luaFilename = filename;

    uint j = 0;
    for (int i = 0; i < LUASize; i++, j++)
    {
        if (i % sectors.size() == 0) j = 0;
        LUAVector.at(j)++;
    }
    //Output::Inst()->kdebug("working here! %i, %i", LUAVector.size(), LUAVector.at(0));
    j = 0;
    for (auto itr = LUAVector.begin(); itr != LUAVector.end(); ++itr, j++)
    {
        Sector *sector = sectors.at(j);
        //Output::Inst()->kdebug("Working not here %i", *itr);
        sector->populate(*itr, filename);
    }

}

/********************************************************
 * Event processing functions: 							*
 * 														*
 ********************************************************/

void Supervisor::receiveEEventPtr(std::unique_ptr<EventQueue::eEvent> eEvent)
{
    //increase the initiated events counter:
    //eEventInitAmount++;
    //insert the event into the eventQueue:
    eventQueue->insertEEvent(std::move(eEvent));
}

void Supervisor::receiveIEventPtr(std::unique_ptr<EventQueue::iEvent> ievent)
{
    eventQueue->insertIEvent(std::move(ievent));
}


/**
 * Takes a microstep.
 * During a microstep the autons can take act on internal events
 * they will also receive external events.
 * @see sector::distroPhase()
 * @see Agent::actOnEvent()
 * @see sector::endPhase()
 */
void Supervisor::microStep(unsigned long long tmu)
{

    //Output::Inst()->kprintf("Taking microstep at %d \n", tmu);
    if (eventQueue->eEventsAtTime(tmu))
    {
        auto elist = eventQueue->getEEventList(tmu);

        for (auto &e : elist) //; eListItr != elist.end(); ++eListItr)
        {
            const EventQueue::eEvent *eEventPtr =
                    eventQueue->addUsedEEvent(std::move(e));

            for (const auto &s : sectors)
            {
                s->distroPhase(eEventPtr);
            }
        }
    }

    if (eventQueue->iEventsAtTime(tmu))
    {
        auto ilist = eventQueue->getIEventList(tmu);

        for (auto &e : ilist)
        {
            //Output::Inst()->kprintf("origin id is %i", event->originID);
            if (removedIDs.find(e->originID) == removedIDs.end())
            {
                std::unique_ptr<EventQueue::iEvent> iEventPtr(std::move(e));

                auto *luaAgent = (AgentLuaInterface *) iEventPtr->origin;
                eventQueue->decrementEeventCounter(iEventPtr->event->id);

                std::unique_ptr<EventQueue::eEvent> eEventPtr =
                        luaAgent->handleEvent(std::move(iEventPtr));


                if (eEventPtr != nullptr)
                    eventQueue->insertEEvent(std::move(eEventPtr));

            }
        }
    }

    eventQueue->legacyFront();
}

/**
 * Returns next viable tmu
 * @see EventQueue::getNextTmu()
 */
unsigned long long Supervisor::getNextMicroTmu()
{
    //eventQueue->printATmus();
    return eventQueue->getNextTmu();
}

/**
 * Performs a macrostep
 * The macrostep, queries all autons on whether or not they will initiate and event.
 * @see sector::initPhase();
 */
void Supervisor::macroStep(unsigned long long tmu)
{

    for (const auto &s : sectors)
    {
        s->taskPromise.set_value(TASK_STEP);
    }

    for (const auto &s : sectors)
    {
        std::future<bool> stepDoneFuture = s->taskDonePromise.get_future();
        stepDoneFuture.wait();
        s->taskDonePromise = std::promise<bool>();
    }
}

/**
 * @brief Master::runStepPhase is static method designed to run the takeStep phase of each
 * preconfigured thread.
 * @param sector
 * @param macroResolution
 * @param time
 */
void Supervisor::runStepPhase(Sector *sector)
{
    while (true)
    {
        std::future<int> stepFuture = sector->taskPromise.get_future();
        stepFuture.wait();
        int task = stepFuture.get();
        sector->taskPromise = std::promise<int>();

        if (task == TASK_STOP)
        {
            break;
        }
        else if (task == TASK_STEP)
        {
            sector->takeStepPhase(Phys::getCTime());
        }

        sector->taskDonePromise.set_value(true);
    }
}

/**
 * Update Status Fields
 * Updates the status output field, on the running mode panel
 * @see Output::updateStatus()
 */
void Supervisor::printStatus()
{
    Output::updateStatus(eventQueue->getISize(), eventQueue->getESize());
}

/**
 * Save eEvent data to disk
 * @see EventQueue::saveEEventData
 */
void Supervisor::saveExternalEvents(std::string filename)
{
    eventQueue->saveEEventData(std::move(filename), luaFilename, autonAmount, areaY, areaX);
}

void Supervisor::simDone()
{
    for (const auto &s : sectors)
    {
        s->simDone();
    }
}

int Supervisor::addAgent(double x, double y, double z, std::string path,
                         std::string filename, std::string type = "Lua")
{
    auto nestItr = sectors.begin();

    sectorIndex++;

    if (sectorIndex == (int) sectors.size())
        sectorIndex = 0;

    nestItr += sectorIndex;

    int id = (*nestItr)->addAgent(x, y, z, path + filename, type);

    eventQueue->addAgentInfo(id, filename);

    return id;

}

//The agent will be removed at the next macrostep:
bool Supervisor::removeAgent(int arg_id)
{
    for (const auto &s : sectors)
    {
        bool removed = s->removeAgent(arg_id);

        if (removed)
        {
            removedIDs.insert(arg_id);
            return true;
        }
    }
    return false;
}
