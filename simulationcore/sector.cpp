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
#include <api/phys.h>

#include "lua.hpp"
#include "lauxlib.h"
#include "lualib.h"

#include "sector.h"
#include "interfacer.h"

#include "ID.h"
#include "supervisor.h"
#include "output.h"
#include "../api/phys.h"

Sector::Sector(double posX, double posY, double width, double height, Supervisor* master, int id)
    :initAmount(0),master(master), posX(posX), posY(posY),width(width),height(height),id(id)
{

}

Sector::~Sector()
{
}

void Sector::populate(int LUASize ,std::string filename)
{
	for(int i=0; i<LUASize; i++)
	{

		if(Output::KillSimulation.load()) return;
        double xtmp = Phys::getMersenneFloat(0,width);
        double ytmp = Phys::getMersenneFloat(0,height);

        std::shared_ptr<AgentLuaInterface> luaPtr =
                std::make_shared<AgentLuaInterface>(ID::generateAgentID(), xtmp, ytmp,
										   1, this, filename);

        luaAgents.insert(std::make_pair(luaPtr->getID(), luaPtr));
        Interfacer::addLuaAgentPtr(luaPtr);

		luaPtr->InitializeAgent();
	}
}

/**
 * Get the X and Y positions of all actors
 * Retrieves the information from all actors, writes the information to the lists given.
 * @param agentinfo address of array that holds the info needed for graphic rendering.
 */
void Sector::retrievePopPos(std::list<agentInfo> &infolist){

    for(auto it = luaAgents.begin(); it !=luaAgents.end(); it++)
	{

		if(master->removedIDs.find(it->second->getID()) ==
				master->removedIDs.end())
		{
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
void Sector::takeStepPhase(unsigned long long tmu)
{

    for(auto itr = luaAgents.begin(); itr !=luaAgents.end(); itr++)
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
            auto itrLua = luaAgents.find(*itRemove);
            if(itrLua != luaAgents.end())
			{
                luaAgents.erase(itrLua);
			}
		}
		removalIDs.clear();
	}

    for(auto auton : newAgents)
    {
        luaAgents.insert(std::make_pair(auton->getID(),auton));
        auton->InitializeAgent();

    }
    newAgents.clear();

}

/**
 * Event distribution phase.
 * Recieves an eventlist from the Master to distribute among local autons
 * @param event EventQueue ptr holding external events.
 */
void Sector::distroPhase(const EventQueue::eEvent* event)
{
    for(auto itLUAs = luaAgents.begin(); itLUAs != luaAgents.end(); ++itLUAs)
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

void Sector::simDone()
{
    for(auto itlua = luaAgents.begin(); itlua !=luaAgents.end(); itlua++)
	{
		itlua->second->simDone();
	}
}

//perform an event for an auton in question:
void Sector::performEvent(std::unique_ptr<EventQueue::eEvent> event)
{
	master->receiveEEventPtr(std::move(event));
}


int Sector::addAgent(double x, double y, double z,
					  std::string filename, std::string type="Lua")
{
    int id = ID::generateAgentID();

	if(type.compare("Lua") == 0)
	{
        std::shared_ptr<AgentLuaInterface> luaPtr =
                std::make_shared<AgentLuaInterface>(id, x, y, 1, this, filename);

        if(Output::SimRunning)
        {
            newAgents.push_back(luaPtr);
        }
        else
        {
            luaAgents.insert(std::make_pair(luaPtr->getID(),luaPtr));
            luaPtr->InitializeAgent();
        }

	}

	return id;
}

bool Sector::removeAgent(int arg_id)
{
    auto luaItr = luaAgents.find(arg_id);
    if(luaItr != luaAgents.end())
	{
		luaItr->second->setRemoved();
		removalIDs.push_back(arg_id);
		return true;
	} else return false;

}
