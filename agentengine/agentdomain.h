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

#ifndef AGENTDOMAIN_H
#define AGENTDOMAIN_H

#include<iostream>
#include<mutex>
#include<atomic>
#include<sys/types.h>
#include<stdio.h>
#include<fstream>

#include "agents/master.h"
#include "../agentengine/agents/doctor.h"
#include "mainwindow.h"
#include "utility.h"
#include "control.h"

class Control;
class Master;
class AgentDomain
{
	public:
        AgentDomain(Control *control);
		~AgentDomain();

		void generateEnvironment(double width, double height, int resolution,
				int listenerSize, int screamerSize, int LUASize,
				double timeResolution, int macroFactor, std::string filename);

        void retrievePopPos();

		void runSimulation(int time);

		bool checkEnvPresence();
		void stopSimulation();
		void saveExternalEvents(std::string filename);
		void updateStatus();

        void toggleLiveView(bool enable);
        void populateSystem();
private:

        Control *control;
		bool mapGenerated;
        Doctor doctor;
        Master *masteragent;

		double timeResolution;
		double macroResolution;

		unsigned long long cMacroStep;
		unsigned long long cMicroStep;

		int macroFactor;
		int mapWidth, mapHeight;
		unsigned long long iterations;
		unsigned long long i;

		//Atomic thread controllers:
		std::atomic_bool stop;
		std::mutex stopMutex;
        std::atomic_bool fetchPositions;

        int LuaAgentAmount;
        std::string luaFilename;

		typedef std::list<agentInfo> onlineAgents;
		std::unordered_map<unsigned long long, onlineAgents> positionMap;
		bool storePositions;
        std::string positionFilename;

        std::ofstream file;
};

#endif // AGENTDOMAIN_H
