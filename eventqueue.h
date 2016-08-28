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
#include <memory>
#include <atomic>
#include <tuple>
#include <mutex>

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
		struct eEvent
		{
			unsigned long long id;
			double propagationSpeed;
			double posX;
			double posY;
            std::string luatable;
			std::string desc;
			unsigned long long activationTime;
			int targetID;
			int targetGroup;
			int originID;
			std::atomic_uint reference_count;
            eEvent() : targetGroup(0), reference_count(0){}
		};



		//define the internal Event:
		struct iEvent
		{
            Auton *origin;
			const eEvent *event;
			unsigned long long activationTime;
			unsigned long long id;
			std::string desc;
			int originID;
		};

		//define the data event, precisely the same as events:
		struct dataEvent
		{
			unsigned long long id;
            int targetID;
			unsigned long long activationTime;
			double propagationSpeed;
			double originX;
			double originY;
			int originID;
			char desc[150];
			char table[1024];
			char filename[256];
		};

		struct autonInfo
		{
			int ID;
			int eventAmount;
			char info[1000];
		};


		//handling of external Events:
        void insertEEvent(std::unique_ptr<eEvent> eeventPtr);
        std::list<std::unique_ptr<eEvent> > getEEventList(unsigned long long tmu);
		bool eEventsAtTime(unsigned long long tmu);
		unsigned long long getNextTmu();
        void legacyFront();

        const EventQueue::eEvent* addUsedEEvent(std::unique_ptr<eEvent> eEvent);

        //handle the reference mechanism of the external event pointers:
        void decrementEeventCounter(unsigned long long id);
        void incrementEeventCounter(unsigned long long id);

		//handling of internal Events:
        void insertIEvent(std::unique_ptr<EventQueue::iEvent> ieventPtr);
        std::list<std::unique_ptr<iEvent> > getIEventList(unsigned long long tmu);
		bool iEventsAtTime(unsigned long long tmu);
		unsigned long long getNextItmu();

		void printLTmus();
		void printATmus();

		//saving events to a binary file:
		void saveEEventData(std::string path, std::string luaFileName,
				int autonAmount, double areaY, double areaX);

		//check the size of the eventQueue:
		unsigned long long getESize();
		unsigned long long getISize();

		void addAutonInfo(int id, std::string filename);

	private:

		void printTest();
        typedef std::list<std::unique_ptr<eEvent>> eEvents;
        typedef std::list<std::unique_ptr<iEvent>> iEvents;

        std::unordered_map<unsigned long long, iEvents> iMap;
        std::unordered_map<unsigned long long, eEvents> eMap;
        std::unordered_map<unsigned long long, std::unique_ptr<eEvent>> usedEEvents;

        std::list<std::unique_ptr<eEvent>> legacyEvents;

		//time keeper lists:
		std::list<unsigned long long> activeTmu;
		std::list<unsigned long long> legacyTmu;
		std::list<unsigned long long>::iterator legacyIt;
		std::list<unsigned long long>::iterator activeIt;

		//time keeper hash:
		std::unordered_set<unsigned long long> tmuSet;

		//agent info map:
		std::unordered_map<int, std::string> agentFilenames;

		//size of the eventqueue:
		unsigned long long eSize;
        unsigned long long iSize;

        std::mutex eEventMutex;
        std::mutex iEventMutex;
};

#endif // EVENTQUEUE_H
