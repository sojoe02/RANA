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

#ifndef MASTER_H
#define MASTER_H

#include <vector>
#include <list>
#include <string>
#include <set>

#include "eventqueue.h"
#include "nestene.h"
#include "utility.h"


class Nestene;
class Master
{
public:
	/*
		  resolution is microseconds pr. turn
		*/
	Master();
	~Master();
	/*
		   height of map
		   width of map
		   resolution is is number of nestenes squared
		   */
	void generateMap(double width, double height, int resolution,
					 double timeResolution, double macroResolution);


	/*
		   Populate the system with a give population size
		   */
	void populateSystem(int listenerSize, int screamerSize,
						int LUASize, std::string filename);

	void populateSquareSystem(int LUASize, std::string filename);
	void populateSquareListenerSystem(int listenerSize);

	/*
		   Functions to excecute a microStep and macroStep
		   */
	//void microStep();
	void microStep(unsigned long long tmu);
	void macroStep(unsigned long long tmu);
	unsigned long long getNextMicroTmu();
	/*
		   Functions on what to do when receiving events:
		   */
    void receiveEEventPtr(std::unique_ptr<EventQueue::eEvent> eEvent);
    void receiveInitEEventPtr(std::unique_ptr<EventQueue::eEvent> eEvent);

    void decrementEEventCounter(unsigned long long id){eventQueue->decrementEeventCounter(id);}
    void incrementEEventCounter(unsigned long long id){eventQueue->incrementEeventCounter(id);}

	/*
		   Functions to add events to the masters eventQueue
		   */
    void receiveIEventPtr(std::unique_ptr<EventQueue::iEvent> ievent);
	void addExternalEventPtr(EventQueue::eEvent *eEvent);

	void printStatus();

	std::list<agentInfo> retrievePopPos();

	void saveExternalEvents(std::string filename);

	int addAuton(double x, double y, double z, std::string path, std::string filename, std::string type);
	bool removeAuton(int ID);

	void simDone();

    std::set<int> removedIDs;

private:

	std::vector<Nestene> nestenes;
	std::vector<Nestene>::iterator itNest;
	//nestene index to keep track of which nestene the next auton should be added at:
	int nesteneIndex;

	//functions for the different phases in a microstep:
	//list to hold events generated each step.
	std::list<EventQueue::eEvent*> stepEvents;

	//first the master will be queuering the eventQueue if
	//events will be excecuted here.
	void excecuteEvents();

	//then Nestenes will be queuried to check if an Auton will
	//initiate an event.
	void queryNestenes();
	double timeResolution;
	double macroResolution;

	int autonAmount;
	uint threads;
	std::string luaFilename;
	double areaX;
	double areaY;

	EventQueue *eventQueue;

	unsigned long long eEventInitAmount;
	unsigned long long responseAmount;
	unsigned long long externalDistroAmount;
	unsigned long long tmu;

};
#endif // MASTER_H
