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

#include "master.h"

#include "utility.h"
#include "../../physics/phys.h"

#include "output.h"

Master::Master()
        :nesteneIndex(0), threads(0) ,eEventInitAmount(0), responseAmount(0),
          externalDistroAmount(0), tmu(0)
{
    //Output::Inst()->kprintf("Initiating master\n");
    eventQueue = new EventQueue;
	srand(time(NULL));
}

Master::~Master()
{
    delete eventQueue;
}
/********************************************************
 * Simulation initialization functions.
 *
 ********************************************************/

/**
 * Generates the map
 * Places all Nestenes at positions to fit the width and height of the map,
 * set the time resolution and macroFactor of the map
 * @param width maps max X coordinate
 * @param height maps max Y coordinate
 * @param threads number of nestene, i.e threads.
 * @param time resolution seconds between microsteps.
 * @param macroResolution factor to multiply with the time resolution for the macrostep intervals.
 */

void Master::generateMap(double width, double height, int threads, double timeResolution, double macroResolution)
{
    this->timeResolution = timeResolution;
    this->macroResolution = macroResolution;
    this->threads = threads;

    areaX = width;
    areaY = height;

    if(!nestenes.empty())
    {
        nestenes.clear();
    }

    for(int i=0; i<threads; i++)
    {
        std::string id;
        std::stringstream ss;
        ss << i;
        id.append(ss.str());

        Nestene nest = Nestene(0,0,width,height,this,i);

        nestenes.push_back(nest);
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

std::list<agentInfo> Master::retrievePopPos()
{

    std::list<agentInfo> agentinfo;
	//Output::Inst()->kprintf("testing retrieve of master");
	for(auto itNest = nestenes.begin(); itNest !=nestenes.end(); itNest++)
	{
        itNest->retrievePopPos(agentinfo);
    }

    return agentinfo;
}

/**
 * Populate the system.
 * Determines the number of Autons pr Nestene, assigns them to their respective Nestene and
 * orders the Nestene to populate itself with dem, give them positions within themselves.
 * @param listenerSize number of listeners total.
 * @param screamerSize number of screamers total.
 * @param LUASize number of LUAs total.
 * @param filename the lua file of the LUA autons definition.
 */

void Master::populateSystem(int listenerSize,
                            int screamerSize, int LUASize, std::string filename)
{

    std::vector<int> LUAVector;

    for(uint i = 0; i < nestenes.size(); i++)
    {
        LUAVector.push_back(0);
    }

    autonAmount = LUASize;
    luaFilename = filename;

    uint j = 0;
    for(int i = 0; i<LUASize; i++, j++)
    {
        if(i % nestenes.size() == 0) j =0;
        LUAVector.at(j)++;
    }
    //Output::Inst()->kdebug("working here! %i, %i", LUAVector.size(), LUAVector.at(0));
    j = 0;
    for(auto itr = LUAVector.begin(); itr != LUAVector.end(); ++itr, j++)
    {
        Nestene *nestene = &nestenes.at(j);
        //Output::Inst()->kdebug("Working not here %i", *itr);
		nestene->populate(*itr, filename);
    }

}

/********************************************************
 * Event processing functions: 							*
 * 														*
 ********************************************************/

void Master::receiveEEventPtr(std::unique_ptr<EventQueue::eEvent> eEvent)
{
    //increase the initiated events counter:
    eEventInitAmount++;
    //insert the event into the eventQueue:
    eventQueue->insertEEvent(std::move(eEvent));
}

void Master::receiveIEventPtr(std::unique_ptr<EventQueue::iEvent> ievent)
{
    eventQueue->insertIEvent(std::move(ievent));
}


/**
 * Takes a microstep.
 * During a microstep the autons can take act on internal events
 * they will also receive external events.
 * @see Nestene::distroPhase()
 * @see Auton::actOnEvent()
 * @see Nestene::endPhase()
 */
void Master::microStep(unsigned long long tmu)
{

    //Output::Inst()->kprintf("Taking microstep at %d \n", tmu);
    if(eventQueue->eEventsAtTime(tmu))
    {
        auto elist = eventQueue->getEEventList(tmu);

        for(auto eListItr = elist.begin(); eListItr != elist.end(); ++eListItr)
        {
			const EventQueue::eEvent* eEventPtr =
					eventQueue->addUsedEEvent(std::move(*eListItr));

            for(itNest = nestenes.begin(); itNest != nestenes.end(); itNest++)
            {
                externalDistroAmount++;
                itNest->distroPhase(eEventPtr);

            }
        }
    }

    if(eventQueue->iEventsAtTime(tmu))
    {
        auto ilist = eventQueue->getIEventList(tmu);

        for(auto iListItr = ilist.begin(); iListItr != ilist.end(); ++iListItr)
        {
            //EventQueue::iEvent* event = *itlist;
            //Output::Inst()->kprintf("origin id is %i", event->originID);
            if (removedIDs.find((*iListItr)->originID) == removedIDs.end())
            {
                std::unique_ptr<EventQueue::iEvent> iEventPtr(std::move(*iListItr));

                AutonLUA *luaAgent = (AutonLUA*)iEventPtr->origin;
                eventQueue->decrementEeventCounter(iEventPtr->event->id);

				std::unique_ptr<EventQueue::eEvent> eEventPtr =
						luaAgent->actOnEvent(std::move(iEventPtr));


                if(eEventPtr != NULL)
                    eventQueue->insertEEvent(std::move(eEventPtr));

            }
        }
    }

    //then run the endPhase on the nestenes, this will handle the responses of the Autons:
    for(itNest =nestenes.begin(); itNest !=nestenes.end(); ++itNest)
    {
        itNest->endPhase();
    }

    eventQueue->legacyFront();
}

/**
 * Returns next viable tmu
 * @see EventQueue::getNextTmu()
 */
unsigned long long Master::getNextMicroTmu()
{
    //eventQueue->printATmus();
    return eventQueue->getNextTmu();
}

/**
 * Performs a macrostep
 * The macrostep, queries all autons on whether or not they will initiate and event.
 * @see Nestene::initPhase();
 */
void Master::macroStep(unsigned long long tmu)
{
    //Handle the initiation of events:
    for(itNest=nestenes.begin() ; itNest !=nestenes.end(); ++itNest)
    {
        itNest->initPhase(macroResolution, tmu+1);
    }
}


/**
 * Update Status Fields
 * Updates the status output field, on the running mode panel
 * @see Output::updateStatus()
 */
void Master::printStatus()
{
    Output::Inst()->updateStatus(Phys::getCTime(),eEventInitAmount,
                                 eventQueue->getISize(), eventQueue->getESize());
    //Output::Inst()->kprintf("%d\n", eventQueue->getISize());
    //	eventQueue->printATmus();
}

/**
 * Save eEvent data to disk
 * @see EventQueue::saveEEventData
 */
void Master::saveExternalEvents(std::string filename)
{
    eventQueue->saveEEventData(filename, luaFilename,autonAmount,areaY,areaX);
}

void Master::simDone()
{
    for(itNest=nestenes.begin() ; itNest !=nestenes.end(); ++itNest)
    {
        itNest->simDone();
    }
}

int Master::addAuton(double x, double y, double z, std::string path, 
					 std::string filename, std::string type = "Lua")
{
    std::vector<Nestene>::iterator nestItr = nestenes.begin();

    nesteneIndex++;

    if(nesteneIndex == (int)nestenes.size())
        nesteneIndex = 0 ;

    nestItr += nesteneIndex;

    int id = nestItr->addAuton(x, y, z, path+filename, type);

    eventQueue->addAutonInfo(id, filename);

    return id;

}

//The agent will be removed at the next macrostep:
bool Master::removeAuton(int arg_id)
{

    for(auto nestitr=nestenes.begin() ; nestitr !=nestenes.end(); ++nestitr)
    {
        //int i = itNest->containsAuton(arg_id);
        bool removed = nestitr->removeAuton(arg_id);

        if(removed)
        {
            removedIDs.insert(arg_id);
            return true;
        }
    }
    return false;
}
