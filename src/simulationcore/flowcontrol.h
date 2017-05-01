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

#ifndef FLOWCONTROL_H
#define FLOWCONTROL_H

#include<iostream>
#include<mutex>
#include<atomic>
#include<sys/types.h>
#include<stdio.h>
#include<fstream>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "supervisor.h"
#include "src/simulationcore/interfacer.h"
#include "src/mainwindow.h"
#include "src/utility.h"
#include "src/control.h"

class Control;
class Supervisor;
class FlowControl
{
	public:
        FlowControl(Control *control);
        ~FlowControl();

        void generateEnvironment(double width, double height, int threads, int agentAmount,
                                 double timeResolution, int macroFactor, std::string filename);
        void retrievePopPos();
		void runSimulation(int time);

		bool checkEnvPresence();
		void stopSimulation();
		void saveExternalEvents(std::string filename);
		void updateStatus();

        void toggleLiveView(bool enable);
        void populateSystem();

        bool runAgain();

private:
        int numIt = 0;
        int simNumIt = 0;

        void setNewParameters();
        void resetSimulation();

        Control *control;
		bool mapGenerated;
        Interfacer doctor;
        Supervisor *masteragent;

        int threads;
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

        int agentAmount;
        std::string agentFilename;

		typedef std::list<agentInfo> onlineAgents;
		std::unordered_map<unsigned long long, onlineAgents> positionMap;
		bool storePositions;
        std::string positionFilename;

        std::ofstream file;

        lua_State* L;
};

#endif // FLOWCONTROL_H
