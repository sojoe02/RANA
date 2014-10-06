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
#include <string>
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
:eEventInitAmount(0), responseAmount(0), externalDistroAmount(0), tmu(0)
{
	//Output::Inst()->kprintf("Initiating master\n");
	eventQueue = new EventQueue;
	srand (time(NULL));
}

Master::~Master(){
	delete eventQueue;
}
/**
 * Generates the map
 * Places all Nestenes at positions to fit the width and height of the map, 
 * set the time resolution and macroFactor of the map
 * @param width maps max X coordinate
 * @param height maps max Y coordinate
 * @param resolution number of nestenes squared.
 * @param time resolution seconds between microsteps.
 * @param macroResolution factor to multiply with the time resolution for the macrostep intervals.
 */
void Master::generateMap(double width, double height, int resolution, double timeResolution, double macroResolution){

	this->timeResolution = timeResolution;
	this->macroResolution = macroResolution;
	areaX = width;
	areaY = height;

	if(!nestenes.empty()){
		//memoryleak here! not clearing out all autons!
		nestenes.clear();
	}

	for(int i=0; i<resolution; i++)
	{
		for(int j=0; j<resolution; j++)
		{
			std::string id;
			std::string tmp;

			double posY = height/resolution*j;
			double posX = width/resolution*i;

            char buffer[512];

            sprintf(buffer,"%f%f",posX,posY);

            id = buffer;

			Nestene nest = Nestene(posX,posY,width/resolution,height/resolution, this);

			nestenes.push_back(nest);

		}
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
std::list<agentInfo> Master::retrievePopPos(){

    std::list<agentInfo> agentinfo;

	for(itNest = nestenes.begin(); itNest !=nestenes.end(); itNest++){
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
		int screamerSize, int LUASize, std::string filename){

	std::vector<int> listenerVector;
	std::vector<int>::iterator itListerner;

	std::vector<int> ScreamerVector;
	std::vector<int>::iterator itScreamer;

	std::vector<int> LUAVector;
	std::vector<int>::iterator itLUA;

	for(uint i = 0; i < nestenes.size(); i++){
		listenerVector.push_back(0);
		ScreamerVector.push_back(0);
		LUAVector.push_back(0);
	}


	int tmpSize = 0;
	int tmpSize2 = 0;
	int nSize = 0;
	//first the listeners:
	while(tmpSize<listenerSize){
		for(uint i=0; i<listenerVector.size(); i++){
			nSize = rand()%2;
			tmpSize += nSize;
			if(tmpSize > listenerSize){
				nSize = listenerSize - tmpSize2;
			}
			tmpSize2 += nSize;
			listenerVector.at(i) += nSize;
		}
	}
	tmpSize = 0;
	tmpSize2 = 0;
	nSize = 0;
	while(tmpSize<screamerSize){
		for(uint i=0; i<ScreamerVector.size(); i++){
			nSize = rand()%2;
			tmpSize += nSize;
			if(tmpSize > screamerSize){
				nSize = screamerSize - tmpSize2;
			}
			tmpSize2 += nSize;
			ScreamerVector.at(i) += nSize;
		}
	}

	tmpSize = 0;
	tmpSize2 = 0;
	nSize = 0;
	while(tmpSize<LUASize){
		for(uint i=0; i<LUAVector.size(); i++){
			nSize = rand()%2;
			tmpSize += nSize;
			if(tmpSize > LUASize){
				nSize = LUASize - tmpSize2;
			}
			tmpSize2 += nSize;
			LUAVector.at(i) += nSize;
		}
	}

	autonAmount = LUASize;
	luaFilename = filename;

	//Output::Inst()->kprintf("lua size from master is : %d \n", LUASize);
	for(uint i= 0; i<listenerVector.size(); i++){
		//std::cout<< listenerVector.at(i) << std::endl;
		Nestene *nest = &nestenes.at(i);
		nest->populate(listenerVector.at(i), ScreamerVector.at(i),LUAVector.at(i), filename);
	}
}


void Master::populateSquareSystem(int LUASize, std::string filename){

	std::vector<int> LUAVector;

	for(uint i = 0; i < nestenes.size(); i++){
		LUAVector.push_back(LUASize);
	}

	autonAmount = LUASize;
	luaFilename = filename;

	Output::Inst()->kprintf("lua amount is : %d \n", LUASize);
	for(uint i= 0; i<LUAVector.size(); i++){
		Nestene *nest = &nestenes.at(i);
		nest->populateSquared(LUAVector.at(i), filename);
	}
}

void Master::populateSquareListenerSystem(int listenerSize){

	std::vector<int> listenerVector;

	for(uint i = 0; i < nestenes.size(); i++){
		listenerVector.push_back(listenerSize);
	}

	autonAmount = listenerSize;

	Output::Inst()->kprintf("listener amount is : %d \n", listenerSize);
	for(uint i= 0; i<listenerVector.size(); i++){
		Nestene *nest = &nestenes.at(i);
		nest->populateSquaredListener(listenerVector.at(i));
	}
}


void Master::receiveEEventPtr(EventQueue::eEvent *eEvent){
	eventQueue->insertEEvent(eEvent);
}

void Master::receiveInitEEventPtr(EventQueue::eEvent *eEvent){
	//increase the initiated events counter:
	eEventInitAmount++;
	//insert the event into the eventQueue:
	eventQueue->insertEEvent(eEvent);
}

void Master::receiveIEventPtr(EventQueue::iEvent *ievent){
	eventQueue->insertIEvent(ievent);
}


/**
 * Takes a microstep.
 * During a microstep the autons can take act on internal events
 * they will also receive external events.
 * @see Nestene::distroPhase()
 * @see Auton::actOnEvent()
 * @see Nestene::endPhase()
 */
void Master::microStep(unsigned long long tmu){

	//Output::Inst()->kprintf("Taking microstep at %d \n", tmu);
	if(eventQueue->eEventsAtTime(tmu)){
		std::list<EventQueue::eEvent*> list = eventQueue->getEEventList(tmu);
		std::list<EventQueue::eEvent*>::iterator itlist = list.begin();

		for(; itlist != list.end(); ++itlist){
			for(itNest = nestenes.begin(); itNest != nestenes.end(); itNest++){
				externalDistroAmount++;
				//EventQueue::eEvent* event = *itlist;
				itNest->distroPhase(*itlist);
			}
		}
	}

	if(eventQueue->iEventsAtTime(tmu)){
		std::list<EventQueue::iEvent*> list = eventQueue->getIEventList(tmu);		
		std::list<EventQueue::iEvent*>::iterator itlist = list.begin();

		for(; itlist != list.end(); ++itlist){
			EventQueue::iEvent* event = *itlist;
			event->origin->actOnEvent(*itlist);
		}
	}

	//then run the endPhase on the nestenes, this will handle the responses of the Autons:
	for(itNest =nestenes.begin(); itNest !=nestenes.end(); ++itNest){
		itNest->endPhase();
	}

	eventQueue->legacyFront();
}

/**
 * Returns next viable tmu
 * @see EventQueue::getNextTmu()
 */
unsigned long long Master::getNextMicroTmu(){
	//eventQueue->printATmus();
	return eventQueue->getNextTmu();
}

/**
 * Performs a macrostep
 * The macrostep, queries all autons on whether or not they will initiate and event. 
 * @see Nestene::initPhase();
 */
void Master::macroStep(unsigned long long tmu){
	//Handle the initiation of events:
	for(itNest=nestenes.begin() ; itNest !=nestenes.end(); ++itNest){
		itNest->initPhase(macroResolution, tmu+1);
	}
}


/**
 * Update Status Fields
 * Updates the status output field, on the running mode panel 
 * @see Output::updateStatus()
 */
void Master::printStatus(){
	Output::Inst()->updateStatus(Phys::getCTime(),eEventInitAmount,
			eventQueue->getISize(), eventQueue->getESize());
	//Output::Inst()->kprintf("%d\n", eventQueue->getISize());
	//	eventQueue->printATmus();
}

/**
 * Save eEvent data to disk
 * @see EventQueue::saveEEventData
 */
void Master::saveExternalEvents(std::string filename){
	eventQueue->saveEEventData(filename, luaFilename,autonAmount,areaY,areaX);
}

void Master::simDone(){
	for(itNest=nestenes.begin() ; itNest !=nestenes.end(); ++itNest){
		itNest->simDone();
	}
}
