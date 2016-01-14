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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utility>

#include "nestene.h"
#include "doctor.h"

#include "ID.h"
#include "master.h"
#include "output.h"

Nestene::Nestene(double posX, double posY, double width, double height, Master* master, uint id)
        :initAmount(0),master(master), posX(posX), posY(posY),width(width),height(height),id(id)
{	
    //Output::Inst()->kprintf("Nestene position %f , %f\n", posX , posY);
    //initialize the internal Events list:
    //iEvents = new std::list<EventQueue::iEvent*>;
}

Nestene::~Nestene()
{

}

void Nestene::generateAuton(){
    //insertAuton(new Auton(generateAutonID(),0,0,0));
}

void Nestene::populate(int LUASize ,std::string filename)
{

    //Output::Inst()->kdebug("nestene %i reporting %i autons", LUA);
	for(int i=0; i<LUASize; i++)
	{
        double xtmp = (double)rand()/ RAND_MAX * width + posX;
        double ytmp = (double)rand()/ RAND_MAX * height + posY;

		std::shared_ptr<AutonLUA> luaPtr =
				 std::make_shared<AutonLUA>(ID::generateAutonID(), xtmp, ytmp,
											1, this, filename);

		LUAs.insert(std::make_pair(luaPtr->getID(), luaPtr));
		Doctor::addLuaAutonPtr(luaPtr);

    }
}

/*
 * Populate squared with only LUA autons:
 *
 */
void Nestene::populateSquared(int LUASize,std::string filename)
{
	if(LUASize > 0)
	{
        double chunkX = width/(double)LUASize;
        double chunkY = height/(double)LUASize;

		for(int i=0; i<LUASize; i++)
		{
			for(int j = 0; j < LUASize; j++)
			{
                //AutonLUA auton(ID::generateAutonID(), (chunkX*j)+posX+chunkX/2, (chunkY*i)+posY+chunkY/2,1,this,filename);
                //itLUAs = LUAs.begin();
                //LUAs.insert(std::pair<int,AutonLUA*>(&auton.getID(),auton));
            }
        }
    }
}

/**
 * Populate squared aread with only Listerner autons:
 */
void Nestene::populateSquaredListener(int listenerSize)
{
	if(listenerSize > 0)
	{
        double chunkX = width/(double)listenerSize;
        double chunkY = height/(double)listenerSize;

		for(int i=0; i<listenerSize; i++)
		{
			for(int j = 0; j < listenerSize; j++)
			{
                AutonListener auton(ID::generateAutonID(), (chunkX*j)+posX+chunkX/2, (chunkY*i)+posY+chunkY/2,1,this);
                itListeners = listeners.begin();
                listeners.insert(std::pair<int,AutonListener>(auton.getID(),auton));
            }
        }
    }
}

/**
 * Get the X and Y positions of all actors
 * Retrieves the information from all actors, writes the information to the lists given.
 * @param agentinfo address of array that holds the info needed for graphic rendering.
 */
void Nestene::retrievePopPos(std::list<agentInfo> &infolist){

	for(auto it = LUAs.begin(); it !=LUAs.end(); it++)
	{

		if(master->removedIDs.find(it->second->getID()) ==
				master->removedIDs.end())
		{
			Output::Inst()->kprintf("id:%i, y:%i, x:%i", it->second->getID(),it->second->getPosY(), it->second->getPosX());

            agentInfo info;
			info.id = it->second->getID();
            info.y = it->second->getPosY();
            info.x = it->second->getPosX();
            infolist.push_back(info);
        }
    }
}


/**
 * Event initiation phase
 * Queries all autons on wether they are going create be an event or not, if
 * an event is initated it will be added the masters eventqueue.
 * @param macroResolution the resolution of the macrostep (microStepRes * macroFactor)
 */
void Nestene::initPhase(double macroResolution, unsigned long long tmu)
{

	for(auto itLUAs = LUAs.begin(); itLUAs !=LUAs.end(); itLUAs++)
	{

		std::unique_ptr<EventQueue::eEvent> eevent =
				itLUAs->second->initEvent();

		if(eevent != NULL)
		{
			master->receiveEEventPtr(std::move(eevent));
        }
    }

    if(!removalIDs.empty())
    {
        //remove all autons set for removal
		for(auto itRemove= removalIDs.begin(); itRemove!=
			removalIDs.end(); ++itRemove)
        {
            auto itrLua = LUAs.find(*itRemove);
            if(itrLua != LUAs.end())
            {
                LUAs.erase(itrLua);
            }
        }
        removalIDs.clear();
    }
}

/**
 * Event distribution phase.
 * Recieves an eventlist from the Master to distribute among local autons
 * @param event EventQueue ptr holding external events.
 */
void Nestene::distroPhase(const EventQueue::eEvent* event)
{
   for(auto itLUAs = LUAs.begin(); itLUAs != LUAs.end(); ++itLUAs)
   {
        if(event->origin->getID() != itLUAs->second->getID())
        {
			std::unique_ptr<EventQueue::iEvent> ieventPtr =
					itLUAs->second->handleEvent(event);

            if(ieventPtr != NULL)
            {
                master->incrementEEventCounter(event->id);
                master->receiveIEventPtr(std::move(ieventPtr));
            }
        }
    }
}

/** 
 * End Phase
 * Check local eventQueue 'outbox' if any external events need to distributed.
 */
void Nestene::endPhase()
{

}

void Nestene::simDone()
{
    //query it's population on whether there is going to be an event or not:
	for(itListeners = listeners.begin(); itListeners !=listeners.end(); itListeners++)
	{
        itListeners->second.simDone();
    }
	for(itScreamers = screamers.begin(); itScreamers !=screamers.end(); itScreamers++)
	{
        itScreamers->second.simDone();
    }
	for(auto itLUAs = LUAs.begin(); itLUAs !=LUAs.end(); itLUAs++)
	{
        itLUAs->second->simDone();
    }
}

void Nestene::queryPopulation(){

}

//perform an event for an auton in question:
void Nestene::performEvent(std::unique_ptr<EventQueue::eEvent> event)
{
	master->receiveEEventPtr(std::move(event));
}


int Nestene::addAuton(double x, double y, double z,
                      std::string filename, std::string type="Lua")
{
    int id = ID::generateAutonID();

    if(type.compare("Lua") == 0)
    {
		std::shared_ptr<AutonLUA> luaPtr =
				std::make_shared<AutonLUA>(id, x, y, 1, this, filename);

		LUAs.insert(std::make_pair(luaPtr->getID(),luaPtr));
		Doctor::addLuaAutonPtr(luaPtr);
    }

    return id;
}

bool Nestene::removeAuton(int arg_id)
{
    auto luaItr = LUAs.find(arg_id);
    if(luaItr != LUAs.end())
    {
        luaItr->second->setRemoved();
        removalIDs.push_back(arg_id);
        return true;
    } else return false;

}
