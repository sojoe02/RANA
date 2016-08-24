//--begin_license--
//
//Copyright 	2013 	Søren Vissing Jørgensen.
//				2014-2016	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.
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
#include <physics/phys.h>

#include "lua.hpp"
#include "lauxlib.h"
#include "lualib.h"

#include "nestene.h"
#include "doctor.h"

#include "ID.h"
#include "master.h"
#include "output.h"
#include "../../physics/phys.h"

Nestene::Nestene(double posX, double posY, double width, double height, Master* master, int id)
    :initAmount(0),takingStep(false),master(master), posX(posX), posY(posY),width(width),height(height),id(id)
{

}

Nestene::~Nestene()
{

    Output::Inst()->kprintf("DELETING NESTENE");

}

void Nestene::populate(int LUASize ,std::string filename)
{
	for(int i=0; i<LUASize; i++)
	{

		if(Output::KillSimulation.load()) return;
        double xtmp = Phys::getMersenneFloat(0,width);
        double ytmp = Phys::getMersenneFloat(0,height);
        //double xtmp = (double)rand()/ RAND_MAX * width + posX;
        //double ytmp = (double)rand()/ RAND_MAX * height + posY;

		std::shared_ptr<AutonLUA> luaPtr =
				std::make_shared<AutonLUA>(ID::generateAutonID(), xtmp, ytmp,
										   1, this, filename);

		luaAutons.insert(std::make_pair(luaPtr->getID(), luaPtr));
		Doctor::addLuaAutonPtr(luaPtr);

		luaPtr->InitializeAgent();
	}
}

/**
 * Get the X and Y positions of all actors
 * Retrieves the information from all actors, writes the information to the lists given.
 * @param agentinfo address of array that holds the info needed for graphic rendering.
 */
void Nestene::retrievePopPos(std::list<agentInfo> &infolist){

	for(auto it = luaAutons.begin(); it !=luaAutons.end(); it++)
	{

		if(master->removedIDs.find(it->second->getID()) ==
				master->removedIDs.end())
		{
			//Output::Inst()->kprintf("id:%i, y:%i, x:%i", it->second->getID(),it->second->getPosY(), it->second->getPosX());

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
void Nestene::takeStepPhase(unsigned long long tmu)
{
    takingStep.store(true);

	for(auto itr = luaAutons.begin(); itr !=luaAutons.end(); itr++)
	{
		int macroFactorMultipler = itr->second->getMacroFactorMultipler();

		if(macroFactorMultipler > 0 &&
				(tmu-1)%macroFactorMultipler*Phys::getMacroFactor() == 0 )
		{
			std::unique_ptr<EventQueue::eEvent> eevent =
                    itr->second->takeStep();


			if(eevent != NULL)
			{
				master->receiveEEventPtr(std::move(eevent));
			}
		}
	}

	if(!removalIDs.empty())
	{
		//remove all autons set for removal
		for(auto itRemove= removalIDs.begin(); itRemove!=
			removalIDs.end(); ++itRemove)
		{
			auto itrLua = luaAutons.find(*itRemove);
			if(itrLua != luaAutons.end())
			{
				luaAutons.erase(itrLua);
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
	for(auto itLUAs = luaAutons.begin(); itLUAs != luaAutons.end(); ++itLUAs)
	{
		if(event->originID != itLUAs->second->getID() &&
				(event->targetGroup == 0 ||
				 itLUAs->second->checkGroup(event->targetGroup) == true))
		{
			std::unique_ptr<EventQueue::iEvent> ieventPtr =
                    itLUAs->second->processEvent(event);

			if(ieventPtr != NULL)
			{
				master->incrementEEventCounter(event->id);
				master->receiveIEventPtr(std::move(ieventPtr));
			}
		}
	}
}

void Nestene::simDone()
{
	for(auto itlua = luaAutons.begin(); itlua !=luaAutons.end(); itlua++)
	{
		itlua->second->simDone();
	}
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

		luaAutons.insert(std::make_pair(luaPtr->getID(),luaPtr));
		Doctor::addLuaAutonPtr(luaPtr);
		luaPtr->InitializeAgent();
	}

	return id;
}

bool Nestene::removeAuton(int arg_id)
{
	auto luaItr = luaAutons.find(arg_id);
	if(luaItr != luaAutons.end())
	{
		luaItr->second->setRemoved();
		removalIDs.push_back(arg_id);
		return true;
	} else return false;

}
