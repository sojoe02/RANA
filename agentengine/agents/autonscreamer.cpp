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
#include "autonscreamer.h"
#include "output.h"

AutonScreamer::AutonScreamer(int ID, double posX, double posY, double posZ, Nestene *nestene)
    : Auton(ID, posX, posY, posZ, nestene){

    desc = "screamer";
    //Output::Inst()->kprintf("Screamer Auton: %i, posX: %f, posY: %f \n", ID,posX,posY);
}


std::shared_ptr<EventQueue::iEvent> AutonScreamer::handleEvent(EventQueue::eEvent *event){
    return NULL;
}


EventQueue::eEvent *AutonScreamer::initEvent(double macroResolution, unsigned long long tmu ){
	double random = (double)rand() /  double(RAND_MAX);
	
	if( random < (0.1 * macroResolution) ){
		EventQueue::eEvent *event = new EventQueue::eEvent();
		event->desc = "callEvent";
        event->targetID = 0;
		event->id = ID::generateEventID();
		event->activationTime = tmu+1;
		event->origin = this;
		//Output::Inst()->kprintf("auton starts event at time %lld \n",tmu);
		return event;
	} else
		return NULL;
}


