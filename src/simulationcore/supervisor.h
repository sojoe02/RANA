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

#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <vector>
#include <list>
#include <string>
#include <set>
#include <thread>
#include <condition_variable>
#include <future>

#include "src/simulationcore/eventqueue.h"
#include "src/simulationcore/sector.h"
#include "src/utility.h"

class Sector;
class Supervisor
{
public:
    Supervisor();
    ~Supervisor();

	void generateMap(double width, double height, int resolution,
					 double timeResolution, double macroResolution);


	void populateSystem(int listenerSize, int screamerSize,
						int LUASize, std::string filename);


	void microStep(unsigned long long tmu);
	void macroStep(unsigned long long tmu);
	unsigned long long getNextMicroTmu();

	void receiveEEventPtr(std::unique_ptr<EventQueue::eEvent> eEvent);

    void decrementEEventCounter(unsigned long long id){eventQueue->decrementEeventCounter(id);}
    void incrementEEventCounter(unsigned long long id){eventQueue->incrementEeventCounter(id);}

    void receiveIEventPtr(std::unique_ptr<EventQueue::iEvent> ievent);
	void addExternalEventPtr(EventQueue::eEvent *eEvent);

	void printStatus();

	std::list<agentInfo> retrievePopPos();

	void saveExternalEvents(std::string filename);

    int addAgent(double x, double y, double z, std::string path, std::string filename, std::string type);
	bool removeAgent(int ID);

	void simDone();

    std::set<int> removedIDs;

    void setSimulationType(int numberOfAgents);

private:

    std::vector<Sector*> sectors;
    std::vector<std::thread*> threads;

    std::vector<Sector*>::iterator itNest;
	//sector index to keep track of which sector the next auton should be added at:
	int sectorIndex;

	//functions for the different phases in a microstep:
	//list to hold events generated each step.
	std::list<EventQueue::eEvent*> stepEvents;

	void excecuteEvents();
	void querysectors();
	double timeResolution;
	double macroResolution;

	int autonAmount;
    int sectorAmount;
    std::string agentFilename;
	double areaX;
	double areaY;

	EventQueue *eventQueue;

	unsigned long long eEventInitAmount;
	unsigned long long responseAmount;
	unsigned long long externalDistroAmount;
	unsigned long long tmu;

    static void runStepPhase(Sector *sector);
    static std::condition_variable CvStepStart;
    static std::condition_variable CvStepDone;
    static std::mutex mutexStep;
    static std::mutex mutexStepDone;
    static int task;

    int simulationType;
};
#endif // SUPERVISOR_H
