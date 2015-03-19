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

#include<eventqueue.h>
#include<iostream>
#include<fstream>
#include<climits>
#include<string.h>
#include<stdio.h>
#include<chrono>

#include"output.h"

#include"agentengine/agents/auton.h"
#include"ID.h"
#include"physics/phys.h"

	EventQueue::EventQueue()
:eSize(0), iSize(0)
{
	iMap = new std::unordered_map<unsigned long long, iEvents>();
	eMap = new std::unordered_map<unsigned long long, eEvents>();
}

/**
 * @brief EventQueues destructor.
 * @details When the event queue is destroyed it will visit each of the contained event pointers
 * and call a destructor on them, the price of storing pointers is paid here!
 */
EventQueue::~EventQueue(){

//	for(iMapIt = iMap->begin(); iMapIt != iMap->end(); ++iMapIt){
//		std::list<iEvent *> tmplist = iMapIt->second;
//		if(!tmplist.empty()){
//			std::list<iEvent *>::iterator tmplistItr;
//			for(tmplistItr = tmplist.begin();tmplistItr != tmplist.end();++tmplistItr){
//				delete *tmplistItr;
//			}
//		}
//	}
	for(eMapIt = eMap->begin(); eMapIt != eMap->end(); ++eMapIt){
		std::list<eEvent *> tmplist = eMapIt->second;
		if(!tmplist.empty()){
			std::list<eEvent *>::iterator tmplistItr;
			for(tmplistItr = tmplist.begin();tmplistItr != tmplist.end();++tmplistItr){
				delete *tmplistItr;
			}
		}
	}
	delete iMap;
	delete eMap;

	Output::Inst()->kprintf("EventQueue Cleared\n");
}

/* **********************************************************************
 * EXTERNAL EVENT HANDLING
 * ******************************************************************** */

/**
 * Insertion of a External Event.
 * External events are placed in the external events linked list at the defined tmu hashkey.
 * if a list doesn't exist at the given tmu a new one will be initialized. 
 * @param tmu when the event will be placed.
 * @param event pointer to the event which is to be indexed by the eventqueue.
 */
void EventQueue::insertEEvent(eEvent *event){
	eSize++;
	unsigned long long tmu = event->activationTime;
	//Output::Inst()->kprintf("eTMU inserted %lld \n", tmu);
	//if the tmu position is empty:
	if(eMap->find(tmu) == eMap->end()){
		eEvents tmp;
		tmp.push_back(event);
		eMap->insert(std::pair<unsigned long long,eEvents>(tmu,tmp));

	} else{
		eEvents *tmp = &eMap->find(tmu)->second;
		tmp->push_back(event);
	}
	if(tmuSet.find(tmu)==tmuSet.end()){
		tmuSet.insert(tmu);
		if(activeTmu.empty()){
			activeTmu.push_front(tmu);
		}else{	//do insertion sort:
			bool inserted = false;	
			std::list<unsigned long long>::iterator activeIt;

            for(activeIt = activeTmu.begin(); activeIt!=activeTmu.end(); activeIt++){
				if(*activeIt > tmu){
					//activeIt;
					activeTmu.insert(activeIt, tmu);
					inserted = true;
					break;
				}
			}
			if(!inserted){
				activeTmu.push_back(tmu);
			}
		}
	}
}

/**
 * Retrieves an external event ptr list.
 * @param tmu timestep for the event ptr list
 * @returns event list at current tmu index in the hashmap, or NULL if there isn't an eventlist 
 */
std::list<EventQueue::eEvent*> EventQueue::getEEventList(unsigned long long tmu){
	std::list<eEvent*> eeList = eMap->find(tmu)->second;
	return eeList;
}

/**
 * Checks if there is any external events.
 * @param tmu microstep too look for an event
 * @return false if there are no events, and true of there is
 */
bool EventQueue::eEventsAtTime(unsigned long long tmu){
	eMapIt = eMap->find(tmu);
	if(eMapIt == eMap->end() || eMapIt->second.empty()){
		return false;
	} else
		return true;
}




/* **********************************************************************
 * INTERNAL EVENT HANDLING
 * ******************************************************************** */

/**
 * Insert internal event.
 * Adds a pointer to an internal event to the iMap hashmap, it will make a new
 * internal event list at its activation time if none exists.
 * It will also add the internal events activation time to a hashset and 
 * event time list via insertion sort.
 * @param event, pointer to an internal event.
 */
void EventQueue::insertIEvent(iEvent *event){
	//put event in hashmap.
	iSize++;
	unsigned long long tmu = event->activationTime;

	if(iMap->find(tmu) == iMap->end()){
		iEvents tmp;
		tmp.push_back(event);
		iMap->insert(std::pair<unsigned long long,iEvents>(tmu,tmp));
	} else{
		iEvents *tmp = &iMap->find(tmu)->second;
		tmp->push_back(event);
	}
	if(tmuSet.find(tmu)==tmuSet.end()){
		tmuSet.insert(tmu);
		if(activeTmu.empty()){
			activeTmu.push_front(tmu);
		}else{
			std::list<unsigned long long>::iterator activeIt;
			bool inserted = false;			
			//do insertion sort:
			for(activeIt = activeTmu.begin(); activeIt!=activeTmu.end(); activeIt++){
				if(*activeIt > tmu){
					//Output::Inst()->kprintf("insertinginternal event %llu, %llu \n", *activeIt,tmu);

					//--activeIt;
					activeTmu.insert(activeIt, tmu);
					inserted = true;
					break;
				}
			}
			if(!inserted){
				//Output::Inst()->kprintf("not inserted event %llu, %llu \n", *activeIt,tmu);
				activeTmu.push_back(tmu);
			}
		}
	}
}

/* Retrieves an internal event ptr list.
 * @param tmu timestep for the event ptr list
 * @returns event list at current tmu index in the hashmap, or NULL if there isn't an eventlist 
 */
std::list<EventQueue::iEvent*> EventQueue::getIEventList(unsigned long long tmu){
	std::list<iEvent*> ieList = iMap->find(tmu)->second;
	return ieList;
}

/**
 * Returns the next relevant tmu.
 * @returns tmu
 */
unsigned long long EventQueue::getNextTmu(){
	if(activeTmu.empty())
		return ULLONG_MAX;
	else return activeTmu.front();
}

/**
 * Check if there is an internal event.
 * @param tmu check whether there is an event at a given tmu
 * @return true if there is an event.
 */
bool EventQueue::iEventsAtTime(unsigned long long tmu){
	iMapIt = iMap->find(tmu);
	if(iMapIt == iMap->end() || iMapIt->second.empty()){
		return false;
	} else return true;
}
/* **********************************************************************
 * UTILITY FUNCTIONS
 * ******************************************************************** */

/**
 * Legacy the front active event tmu.
 * Move the lowest active event tmu (active.front()) to the legacy tmu list. This
 * function is called whenever the simulator has completed the microstep
 * associated with the lowest active event tmu. 
 */
void EventQueue::legacyFront(){
	legacyTmu.push_back(activeTmu.front());
	activeTmu.pop_front();
}

/**
 * Get the total number of external events in the queue.
 * @return number of external events
 */
unsigned long long EventQueue::getESize(){
	return eSize;
}

/**
 * Get total number of internal events in queue.
 * @return number of internal events
 */
unsigned long long EventQueue::getISize(){
	return iSize;
}

/**
 * Save eEvent data to disk.
 * Saves eventQueue data to disk writes a binary kas file to 
 * the disk containing a data representation of the external 
 * events which contains their LUA distribution function this files
 * data will be visualized in the data tool.
 * It also saves an information struct with relevant information
 * on the simulation itself.
 * @see EventQueue::simInfo.
 * @param filename filename of the saved file '<filename>.kas',
 * @param luaFileName the filename of the lua auton.
 * @param autonAmount number of autons active.
 * @param areaY height of the area[m].
 * @param areaX width of the area[m].
 */
void EventQueue::saveEEventData(std::string name, std::string luaFileName, 
		int autonAmount, double areaY, double areaX){ 

	std::string filename = name;

	//Open the file and set the options:
	std::ofstream file (filename.c_str(), std::ofstream::binary | std::ofstream::trunc);
	//int bufferLimit = 100000;

	Output::Inst()->kprintf("Saving event data to file:  %s\n" , luaFileName.c_str());

	//first save the dataEvent to the file:
	simInfo dataInfo;
	strncpy(dataInfo.luaFileName, luaFileName.c_str(),1024);

	Output::Inst()->kprintf("name %s", luaFileName.c_str());
	Output::Inst()->kprintf("path saved is: %s", dataInfo.luaFileName);

	dataInfo.eventAmount = eSize;
	dataInfo.numberOfAutons = autonAmount;
	dataInfo.timeResolution = 1/Phys::getTimeRes();
	dataInfo.macroFactor = Phys::getMacroFactor();
	dataInfo.tmuAmount = Phys::getCTime();
	dataInfo.areaX = Phys::getEnvX();
	dataInfo.areaY = Phys::getEnvY();
	dataInfo.mapResolution = Phys::getScale();
	//


	//Output::Inst()->kprintf("\nsize stuff %d \n", dataInfo.areaX);

	file.write(reinterpret_cast<char*>(&dataInfo),sizeof(dataInfo));

	//then save all the external events:

	auto infoItr = agentFilenames.begin();

	for(eMapIt = eMap->begin(); eMapIt != eMap->end(); ++eMapIt)
	{
		std::list<eEvent *> tmplist = eMapIt->second;
		if(!tmplist.empty())
		{
			std::list<eEvent *>::iterator tmplistItr;
			for(tmplistItr = tmplist.begin();tmplistItr != tmplist.end();++tmplistItr)
			{
				eEvent* tmp = *tmplistItr;
				dataEvent devent;
				devent.id = tmp->id;
				devent.activationTime = tmp->activationTime;
                devent.targetID = tmp->targetID;
				devent.originX = tmp->posX;
				devent.originY = tmp->posY;
				devent.originID = tmp->origin->getID();
				devent.propagationSpeed = tmp->propagationSpeed;
				strncpy(devent.desc,tmp->desc.c_str(),150);
				strncpy(devent.table,tmp->table.c_str(),1024);
				//Add the filename if it exists:
				infoItr = agentFilenames.find(tmp->id);
				if(infoItr != agentFilenames.end())
				{
					strncpy(devent.filename, infoItr->second.c_str(), 256);
				}else strncpy(devent.filename, std::string("NULL").c_str(),256);

				//Output::Inst()->kprintf("Event id %i \n", tmp->id);
				file.write(reinterpret_cast<char*>(&devent),sizeof(devent));
			}	
		} else{
		}
	}
	Output::Inst()->kprintf("Saving data done\n");

}

/**
 * Prints all unique legacy tmus
 */
void EventQueue::printLTmus(){
	Output::Inst()->kprintf("----------------\n");

	for(legacyIt = legacyTmu.begin(); legacyIt!=legacyTmu.end(); ++legacyIt){
		Output::Inst()->kprintf("%d\n", *legacyIt);							
	}
	Output::Inst()->kprintf("----------------\n");
}

/**
 * Prints all unique active tmus
 */
void EventQueue::printATmus(){
	Output::Inst()->kprintf("----------------\n");

	for(activeIt = activeTmu.begin(); activeIt!=activeTmu.end(); activeIt++){
		Output::Inst()->kprintf("%llu\n", *activeIt);							
	}
	Output::Inst()->kprintf("----------------\n");
}

void EventQueue::addAutonInfo(int id, std::string filename)
{
	agentFilenames.insert(std::pair<int, std::string>(id, filename) );
}
