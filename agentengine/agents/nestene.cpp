//--begin_license--
//
//Copyright 	2013 	Søren Vissing Jørgensen.
//			2014	Søren Vissing Jørgensen, Center for Biorobotics, Sydansk Universitet MMMI.  
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
#include <map>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "nestene.h"
#include "ID.h"
#include "master.h"
#include "output.h"

	Nestene::Nestene(double posX, double posY, double width, double height, Master* master)
:posX(posX), posY(posY),width(width),height(height),master(master), initAmount(0)
{	
	//Output::Inst()->kprintf("Nestene position %f , %f\n", posX , posY);
	//initialize the internal Events list:
	//iEvents = new std::list<EventQueue::iEvent*>;
}

Nestene::~Nestene(){
}


void Nestene::generateAuton(){
	//insertAuton(new Auton(generateAutonID(),0,0,0));
}

void Nestene::populate(int listenerSize, int screamerSize, int LUASize,std::string filename){
	//Output::Inst()->kprintf("Populating Nestenes\n");
	//first insert the listener autons:
	for(int i=0; i<listenerSize; i++){
		double xtmp = (double)rand()/ RAND_MAX * width + posX;
		double ytmp = (double)rand()/ RAND_MAX * height + posY;

		AutonListener auton = AutonListener(ID::generateAutonID(),xtmp,ytmp,1,this);
		itListeners = listeners.begin();
		listeners.insert(std::pair<int,AutonListener>(auton.getID(),auton));	
	}

	//the screamer autons:
	for(int i=0; i<screamerSize; i++){
		double xtmp = (double)rand()/ RAND_MAX * width + posX;
		double ytmp = (double)rand()/ RAND_MAX * height + posY;

		AutonScreamer auton = AutonScreamer(ID::generateAutonID(),xtmp,ytmp,1,this);
		itScreamers = screamers.begin();
		screamers.insert(std::pair<int,AutonScreamer>(auton.getID(),auton));
	}

	//the LUA autons:	
	for(int i=0; i<LUASize; i++){
		double xtmp = (double)rand()/ RAND_MAX * width + posX;
		double ytmp = (double)rand()/ RAND_MAX * height + posY;

		AutonLUA auton = AutonLUA(ID::generateAutonID(),xtmp,ytmp,1,this,filename);
		itLUAs = LUAs.begin();
		LUAs.insert(std::pair<int,AutonLUA>(auton.getID(),auton));
	}
}

/*
 * Populate squared with only LUA autons:
 *
 */
void Nestene::populateSquared(int LUASize,std::string filename){
	if(LUASize > 0){
		double chunkX = width/(double)LUASize;
		double chunkY = height/(double)LUASize;

		for(int i=0; i<LUASize; i++){
			for(int j = 0; j < LUASize; j++){
				AutonLUA auton(ID::generateAutonID(), (chunkX*j)+posX+chunkX/2, (chunkY*i)+posY+chunkY/2,1,this,filename);
				itLUAs = LUAs.begin();
				LUAs.insert(std::pair<int,AutonLUA>(auton.getID(),auton));
			}
		}
	}
}

/**
 * Populate squared aread with only Listerner autons: 
 */
void Nestene::populateSquaredListener(int listenerSize){
	if(listenerSize > 0){
		double chunkX = width/(double)listenerSize;
		double chunkY = height/(double)listenerSize;

		for(int i=0; i<listenerSize; i++){
			for(int j = 0; j < listenerSize; j++){
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

    for(itLUAs = LUAs.begin(); itLUAs !=LUAs.end(); itLUAs++){

        agentInfo info;

        info.id = itLUAs->second.getID();
        info.y = itLUAs->second.getPosY();
        info.x = itLUAs->second.getPosX();

        infolist.push_back(info);
	}
}


/**
 * Event initiation phase
 * Queries all autons on wether they are going create be an event or not, if
 * an event is initated it will be added the masters eventqueue.
 * @param macroResolution the resolution of the macrostep (microStepRes * macroFactor)
 */
void Nestene::initPhase(double macroResolution, unsigned long long tmu){
	//query it's population on whether there is going to be an event or not:
	for(itListeners = listeners.begin(); itListeners !=listeners.end(); itListeners++){
		EventQueue::eEvent* eevent = itListeners->second.initEvent(macroResolution,tmu);
		if(eevent != NULL){
			master->receiveInitEEventPtr(eevent);
		}
	}
	for(itScreamers = screamers.begin(); itScreamers !=screamers.end(); itScreamers++){
		EventQueue::eEvent* eevent = itScreamers->second.initEvent(macroResolution,tmu);
		if(eevent != NULL){
			master->receiveInitEEventPtr(eevent);
		}
	}
	for(itLUAs = LUAs.begin(); itLUAs !=LUAs.end(); itLUAs++){
		EventQueue::eEvent* eevent = itLUAs->second.initEvent();
		if(eevent != NULL){
			master->receiveInitEEventPtr(eevent);
		}
	}
}

/**
 * Event distribution phase.
 * Recieves an eventlist from the Master to distribute among local autons
 * @param event EventQueue ptr holding external events.
 */
void Nestene::distroPhase(EventQueue::eEvent* event){
	for(itListeners = listeners.begin(); itListeners != listeners.end(); ++itListeners){
		if(event->origin->getID() != itListeners->second.getID()){
			EventQueue::iEvent *ievent = itListeners->second.handleEvent(event);
			if(ievent != NULL)
				master->receiveIEventPtr(ievent);			
		}
	}
	for(itLUAs = LUAs.begin(); itLUAs != LUAs.end(); ++itLUAs){
		if(event->origin->getID() != itLUAs->second.getID()){
			EventQueue::iEvent *ievent = itLUAs->second.handleEvent(event);
			if(ievent != NULL)
				master->receiveIEventPtr(ievent);
		}
	}
}
/** 
 * End Phase
 * Check local eventQueue 'outbox' if any external events need to distributed.
 */
void Nestene::endPhase(){
	//distribute all Eevents that's in the Eevent outbox:
	if(!eEventsOutbox.empty()){
		for(iteEventsOutbox = eEventsOutbox.begin(); iteEventsOutbox != eEventsOutbox.end(); iteEventsOutbox++){
			EventQueue::eEvent *event = *iteEventsOutbox;
			//std::cout << "Auton responding on Event with external event " << event->id << std::endl;
			master->receiveEEventPtr(event);
		}
		eEventsOutbox.clear();
	}
}

void Nestene::simDone(){
	//query it's population on whether there is going to be an event or not:
	for(itListeners = listeners.begin(); itListeners !=listeners.end(); itListeners++){
		itListeners->second.simDone();
	}
	for(itScreamers = screamers.begin(); itScreamers !=screamers.end(); itScreamers++){
		itScreamers->second.simDone();
	}
	for(itLUAs = LUAs.begin(); itLUAs !=LUAs.end(); itLUAs++){
		itLUAs->second.simDone();
	}
}

void Nestene::queryPopulation(){

}

//perform an event for an auton in question:
void Nestene::performEvent(AutonListener *auton){

}

