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

#ifndef NESTENE_H
#define NESTENE_H


#include <map>
#include <list>
#include <string>
#include <memory>
#include <utility>

#include "master.h"
#include "eventqueue.h"
#include "autonlistener.h"
#include "autonscreamer.h"
#include "autonLUA.h"

#include "utility.h"

class Master;
class AutonListener;
class AutonScreamer;
class AutonLUA;
class Nestene
{
	public:
		Nestene(double posX,double posY, double width, double height, Master* master, uint id);
		~Nestene();

		void generateAuton();
		void populate(int listenerSize, int screamerSize, int LUASize, std::string filename);
		void populateSquared(int LUASize,std::string filename);
		void populateSquaredListener(int listenerSize);

		void initPhase(double macroResolution, unsigned long long tmu);

        //function to receive events the master, and distribute them on all local nestene
        void distroPhase(const EventQueue::eEvent *event);

		std::list<EventQueue::iEvent> responsePhase();
		void endPhase();

        void retrievePopPos(std::list<agentInfo> &infolist);

		int initAmount;

		void simDone();

		uint getID(){ return id; }

		int addAuton(double x, double y, double z,
					  std::string filename, std::string type);

		bool removeAuton(int arg_id);
		int containsAuton(int arg_id);

private:
		//generates an event and puts it into the event map.
		EventQueue::eEvent* generateListenerEvent();
		EventQueue::eEvent* generateScreamerEvent();

		//purge events before current tmu.
		void purgeEvents();
		//Functions to perform during a microStep.
		//check whether or not there is to be initiated an event on one of its residents.
		void queryPopulation();
		//Act on event.
		void performEvent(AutonListener *auton);

		//initial calculation of whether or not an event will be initiated.
		void calculateInitEventChance();

		void registerIEvent(EventQueue::iEvent *event);
		void registerEEvent(EventQueue::eEvent *event);
		//EventQueue *ievents;
		//EventQueue *eevents;

		Master *master;

		//map of local autons with ID, and auton.
		std::map<int,AutonListener> listeners;
		std::map<int,AutonListener>::iterator itListeners;

		std::map<int,AutonScreamer> screamers;
		std::map<int,AutonScreamer>::iterator itScreamers;

        std::map<int,std::shared_ptr<AutonLUA>> LUAs;

		//list of ievents to be send back to the master:
		//std::list<EventQueue::iEvent*>* iEvents;

		//list of eeEvents that wants to send out,
		//Autons will register the external events they want to send out here:
		std::list<EventQueue::eEvent*> eEventsOutbox;
		std::list<EventQueue::eEvent*>::iterator iteEventsOutbox;

		std::list<int> removalIDs;

		friend class Auton;
		friend class AutonListener;
		friend class AutonScreamer;
		friend class AutonLUA;
		double posX;
		double posY;
		double width;
		double height;
		uint id;

		double listenerEventChance;
		double screamerEventChance;

};

#endif // NESTENE_H
