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

#ifndef SECTOR_H
#define SECTOR_H


#include <map>
#include <list>
#include <string>
#include <memory>
#include <utility>
#include <atomic>
#include <future>

#include "supervisor.h"
#include "eventqueue.h"
#include "agents/agentluainterface.h"

#include "utility.h"

class Supervisor;
class AgentListener;
class AgentScreamer;
class AgentLuaInterface;
class Sector
{
	public:
        Sector(double posX,double posY, double width, double height, Supervisor* master, int id);
        ~Sector();

		void generateAgent();
		void populate(int LUASize, std::string filename);
        void takeStepPhase(unsigned long long tmu);
		//function to receive events the master, and distribute them on all local sector
		void distroPhase(const EventQueue::eEvent *event);
		std::list<EventQueue::iEvent> responsePhase();
		void retrievePopPos(std::list<agentInfo> &infolist);
		int initAmount;
		void simDone();
		int getID(){ return id; }
		int addAgent(double x, double y, double z,
					  std::string filename, std::string type);

		bool removeAgent(int arg_id);
		int containsAgent(int arg_id);

        std::promise<int> taskPromise;
        std::promise<bool> taskDonePromise;

private:

		//purge events before current tmu.
		void purgeEvents();
		//Functions to perform during a microStep.
		//check whether or not there is to be initiated an event on one of its residents.
		void performEvent(std::unique_ptr<EventQueue::eEvent> event);
        Supervisor *master;

        std::map<int,std::shared_ptr<AgentLuaInterface>> luaAgents;
        std::list<std::shared_ptr<AgentLuaInterface>> newAgents;

		std::list<int> removalIDs;

		friend class Agent;
		friend class AgentLuaInterface;
		double posX;
		double posY;
		double width;
		double height;
		int id;

};

#endif // SECTOR_H
