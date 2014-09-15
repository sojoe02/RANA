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

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

//STL includes:
#include <unordered_map>
#include <list>
#include <vector>
#include <string>
#include <unordered_set>

class Auton;
class EventQueue
{
	public:
		EventQueue(); 
		~EventQueue();

		struct simInfo{
			char luaFileName[1024]; /*!< Filename of the lua auton thats active in this simulation*/
			unsigned long long eventAmount; /*!< Number of external events active in the simulation*/
			int numberOfAutons; /*!< Total number of autons*/
			double timeResolution; /*!< Resolution of the simulation number of microsteps possible pr second*/
			int macroFactor; /*!< Macro Factor */
			unsigned long long tmuAmount; /*!< number of total timeunist of  Timeresolution performed  */
			double areaY; /*!< Areas Y size[m] (std computer coordinate system with reversed Y axis)*/
			double areaX; /*!< Areas X size[m] */
			double mapResolution;
		};

		//define the external Event:
		struct eEvent {
			unsigned long long id;
			double propagationSpeed;
			Auton *origin;
			double posX;
			double posY;
			std::string table;
			std::string desc;
			unsigned long long activationTime;
            int targetID;
			//double funcArray[11];
		};

		//define the internal Event:
		struct iEvent {
			Auton *origin;
			eEvent *event;
			unsigned long long activationTime;
			unsigned long long id;
			std::string desc;
		};

		//define the data event, precisely the same as events:
		struct dataEvent{
			unsigned long long id;
            int targetID;
			unsigned long long activationTime;
			double propagationSpeed;
			double originX;
			double originY;
			int originID;
			char desc[150];
			char table[1024];
		};

		struct autonInfo{
			int ID;
			int eventAmount;
			char info[1000];
		};


		//handling of external Events:
		void insertEEvent(eEvent *event);
		eEvent* popBackEEvent(unsigned long long tmu);
		std::list<eEvent*> getEEventList(unsigned long long tmu);
		bool eEventsAtTime(unsigned long long tmu);
		unsigned long long getNextTmu();
		void legacyFront();

		//handling of internal Events:
		void insertIEvent(iEvent *event);
		iEvent* popBackIEvent(unsigned long long tmu);
		std::list<iEvent*> getIEventList(unsigned long long tmu);
		bool iEventsAtTime(unsigned long long tmu);
		unsigned long long getNextItmu();
		void printLTmus();
		void printATmus();

		//saving events to a binary file:
		void saveEEventData(std::string filename, std::string luaFileName, 
				int autonAmount, double areaY, double areaX);

		//check the size of the eventQueue:
		unsigned long long getESize();
		unsigned long long getISize();

	private:
		void printTest();
		//the eventmaps, (event):
		typedef std::list<eEvent *> eEvents;
		typedef std::list<iEvent *> iEvents;
		//the eventqueues, (tmu, eventmap):
		std::unordered_map<unsigned long long,iEvents> *iMap;
		std::unordered_map<unsigned long long,eEvents> *eMap;
		//iterators:
		std::unordered_map<unsigned long long,iEvents>::iterator iMapIt;
		std::unordered_map<unsigned long long,eEvents>::iterator eMapIt;
		//time keeper lists:
		std::list<unsigned long long> activeTmu;
		std::list<unsigned long long> legacyTmu;
		std::list<unsigned long long>::iterator legacyIt;
		std::list<unsigned long long>::iterator activeIt;

		//time keeper hash:
		std::unordered_set<unsigned long long> tmuSet;

		//size of the eventqueue:
		unsigned long long eSize;
		unsigned long long iSize;

};

#endif // EVENTQUEUE_H
