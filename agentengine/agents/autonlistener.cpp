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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ID.h"
#include "autonlistener.h"
#include "../../physics/phys.h"
#include "output.h"

	AutonListener::AutonListener(int ID, double posX, double posY, double posZ, Nestene *nestene)
: Auton(ID, posX, posY, posZ, nestene), eventChance(0.0)
{
	desc = "Listener";
}

EventQueue::iEvent* AutonListener::handleEvent(EventQueue::eEvent *event){
	unsigned long long time = 
		Phys::speedOfSound(event->origin->getPosX(),event->origin->getPosY(), posX, posY);

	EventQueue::iEvent *ievent = new EventQueue::iEvent();

	ievent->origin = this;
	ievent->activationTime = time;
	ievent->event = event;
	return ievent;
}


EventQueue::eEvent* AutonListener::initEvent(double macroResolution, unsigned long long tmu){
	if(!eventInitiated){
		eventInitiated = true;
		EventQueue::eEvent *event = new EventQueue::eEvent();
		event->desc = "callEvent";
        event->targetID = 0;
		event->id = ID::generateEventID();
		event->activationTime = tmu+1;
		event->origin = this;
		return event;
	}

	return NULL;
}

EventQueue::eEvent* AutonListener::actOnEvent(EventQueue::iEvent *event){

	EventQueue::eEvent *sendEvent = new EventQueue::eEvent();
	sendEvent->desc = "callEvent";
    sendEvent->targetID = 0;
	sendEvent->id = ID::generateEventID();
	sendEvent->activationTime = event->activationTime+1;
	sendEvent->origin = this;

	distroEEvent(sendEvent);

	return NULL;
}

bool AutonListener::operator==(AutonListener &other) const{
	return (this->ID == other.getID());
}

bool AutonListener::operator!=(AutonListener &other) const{
	return !(*this == other);
}
