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
#include<utility>
#include<mutex>

#include"output.h"

#include"agentengine/agents/auton.h"
#include"ID.h"
#include"physics/phys.h"

EventQueue::EventQueue()
    :eSize(0), iSize(0)
{
    //iMap = new std::unordered_map<unsigned long long, iEvents>();
    //eMap = new std::unordered_map<unsigned long long, eEvents>();
}

/**
 * @brief EventQueues destructor
 * @details When the event queue is destroyed it will visit each of the contained event pointers
 * and call a destructor on them, the price of storing pointers is paid here!
 */
EventQueue::~EventQueue()
{

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
void EventQueue::insertEEvent(std::unique_ptr<eEvent> eeventPtr)
{
    eSize++;

    unsigned long long tmu = eeventPtr->activationTime;

    if(eMap.find(tmu) == eMap.end())
    {
        eEvents tmp;
        tmp.push_back(std::move(eeventPtr));
        eMap.insert(std::make_pair(tmu, std::move(tmp)));

    } else
    {
        auto eeventItr = eMap.find(tmu);
        eeventItr->second.push_back(std::move(eeventPtr));
    }

    //Insert activation time into the trigger set:
    if(tmuSet.find(tmu)==tmuSet.end())
    {
        tmuSet.insert(tmu);
        if(activeTmu.empty())
        {
            activeTmu.push_front(tmu);
        }else
        {
            //do insertion sort:
            bool inserted = false;
            std::list<unsigned long long>::iterator activeIt;

            for(activeIt = activeTmu.begin(); activeIt!=activeTmu.end(); activeIt++)
            {
                if(*activeIt > tmu)
                {
                    //activeIt;
                    activeTmu.insert(activeIt, tmu);
                    inserted = true;
                    break;
                }
            }
            if(!inserted)
            {
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
std::list<std::unique_ptr<EventQueue::eEvent>> EventQueue::getEEventList(unsigned long long tmu)
{
    auto emapItr = eMap.find(tmu);
    std::list<std::unique_ptr<eEvent>> eList(std::move(emapItr->second)) ;
    return eList;
}

/**
 * Checks if there is any external events.
 * @param tmu microstep too look for an event
 * @return false if there are no events, and true of there is
 */
bool EventQueue::eEventsAtTime(unsigned long long tmu)
{
    auto eMapIt = eMap.find(tmu);

    if(eMapIt == eMap.end() || eMapIt->second.empty())
    {
        return false;
    } else
        return true;
}

const EventQueue::eEvent* EventQueue::addUsedEEvent(std::unique_ptr<eEvent> eEvent)
{
    int id = eEvent->id;
    //const EventQueue::eEvent* eEventPtr = eEvent.get();
    //legacyEvents.push_back(std::move(eEvent));
    usedEEvents.insert(std::make_pair(id, std::move(eEvent)));
    //return eEventPtr;
    return usedEEvents.find(id)->second.get();
}

void EventQueue::decrementEeventCounter(unsigned long long id)
{
    auto itr = usedEEvents.find(id);

    if(itr != usedEEvents.end())
    {
        itr->second->reference_count--;

        //Output::Inst()->kprintf("reference count for event with ID %i is %i", id, itr->second->reference_count.load());

        if (itr->second->reference_count.load() <= 0)
        {
            // Output::Inst()->kprintf("adding event with id %i to legacy map", id);
            legacyEvents.push_back(std::move(itr->second));
        }

    } else Output::Inst()->kprintf("event not found in Used Eevent hash map, something is wrong");
}

void EventQueue::incrementEeventCounter(unsigned long long id)
{
    auto itr = usedEEvents.find(id);

    if(itr != usedEEvents.end())
    {
        itr->second->reference_count++;
    } else Output::Inst()->kprintf("external event not found in Event hash map, cannot increment reference counter");
}

/* **********************************************************************
 * INTERNAL EVENT HANDLING have no interest in the stash tab. So for him to feel that way does not really affect me. Give it to seasons, not going to make me play seasons any more than I did before which is none.
 * ******************************************************************** */

/**
 * Insert internal event.
 * Adds a pointer to an internal event to the iMap hashmap, it will make a new
 * internal event list at its activation time if none exists.
 * It will also add the internal events activation time to a hashset and
 * event time list via insertion sort.
 * @param event, pointer to an internal event.
 */
void EventQueue::insertIEvent(std::unique_ptr<iEvent> ieventPtr)
{
    //put event in hashmap.
    //std::lock_guard<std::mutex> lk(iEventMutex);
    iSize++;
    unsigned long long tmu = ieventPtr->activationTime;

    if(iMap.find(tmu) == iMap.end())
    {
        iEvents tmp;
        tmp.push_back(std::move(ieventPtr));
        iMap.insert(std::make_pair(tmu,std::move(tmp)));

    } else
    {
        auto ieventItr = iMap.find(tmu);
        ieventItr->second.push_back(std::move(ieventPtr));
        //Output::Inst()->kprintf("size of list is %i ", iMap.find(tmu)->second.size());
    }

    //Insert the activation time into the trigger set:
    if(tmuSet.find(tmu)==tmuSet.end())
    {
        tmuSet.insert(tmu);
        if(activeTmu.empty())
        {
            activeTmu.push_front(tmu);
        }else
        {
            //std::list<unsigned long long>::iterator activeIt;
            bool inserted = false;
            //do insertion sort:
            for(auto activeIt = activeTmu.begin(); activeIt!=activeTmu.end(); activeIt++)
            {
                if(*activeIt > tmu)
                {
                    //Output::Inst()->kprintf("insertinginternal event %llu, %llu \n", *activeIt,tmu);
                    activeTmu.insert(activeIt, tmu);
                    inserted = true;
                    break;
                }
            }
            if(!inserted)
            {
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
std::list<std::unique_ptr<EventQueue::iEvent>> EventQueue::getIEventList(unsigned long long tmu)
{

    auto imapItr = iMap.find(tmu);
    //retrieve the list containing the events:
    std::list<std::unique_ptr<iEvent>> iList(std::move(imapItr->second));
    return iList;
}

/**
 * Returns the next relevant tmu.
 * @returns tmu
 */
unsigned long long EventQueue::getNextTmu()
{
    if(activeTmu.empty())
        return ULLONG_MAX;
    else
        return activeTmu.front();
}

/**
 * Check if there is an internal event.
 * @param tmu check whether there is an event at a given tmu
 * @return true if there is an event.
 */
bool EventQueue::iEventsAtTime(unsigned long long tmu)
{
    auto iMapIt = iMap.find(tmu);

    if(iMapIt == iMap.end() || iMapIt->second.empty())
    {
        return false;
    }
    else
        return true;
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
void EventQueue::saveEEventData(std::string path, std::string luaFileName,
                                int autonAmount, double areaY, double areaX){

    std::string filename = path;

    //Open the file and set the options:
    std::ofstream file(filename.c_str(), std::ofstream::binary | std::ofstream::trunc);
    //int bufferLimit = 100000;

    Output::Inst()->kprintf("Saving event data to file:  %s\n" , path.c_str());

    //first save the dataEvent to the file:
    simInfo dataInfo;
    strncpy(dataInfo.luaFileName, luaFileName.c_str(),1024);

    //Output::Inst()->kprintf("name %s", luaFileName.c_str());
    //Output::Inst()->kprintf("path saved is: %s", dataInfo.luaFileName);

    dataInfo.eventAmount = eSize;
    dataInfo.numberOfAutons = autonAmount;
    dataInfo.timeResolution = 1/Phys::getTimeRes();
    dataInfo.macroFactor = Phys::getMacroFactor();
    dataInfo.tmuAmount = Phys::getCTime();
    dataInfo.areaX = Phys::getEnvX();
    dataInfo.areaY = Phys::getEnvY();
    dataInfo.mapResolution = Phys::getScale();
    //Output::Inst()->kprintf("\nsize stuff %d \n", dataInfo.areaX);

    file.write(reinterpret_cast<char*>(&dataInfo),sizeof(dataInfo));

    //then save all the external events:
    //auto file_itr = agentFilenames.begin();

    for(auto event_itr = legacyEvents.begin();
        event_itr != legacyEvents.end(); ++event_itr)
    {
        dataEvent devent;

        devent.id = (*event_itr)->id;
        devent.activationTime = (*event_itr)->activationTime;
        devent.targetID = (*event_itr)->targetID;
        devent.originX = (*event_itr)->posX;
        devent.originY = (*event_itr)->posY;
        devent.originID = (*event_itr)->originID;
        devent.propagationSpeed = (*event_itr)->propagationSpeed;

        strncpy(devent.desc,(*event_itr)->desc.c_str(),150);
        strncpy(devent.table,(*event_itr)->luatable.c_str(),1024);

        //Add the filename if it exists:
        auto infoItr = agentFilenames.find((*event_itr)->originID);

        if(infoItr != agentFilenames.end())
        {
            strncpy(devent.filename, infoItr->second.c_str(), 256);
        }
        else
            strncpy(devent.filename, std::string("NULL").c_str(),256);

        //Output::Inst()->kprintf("data event des.%s ", devent.desc);

        file.write(reinterpret_cast<char*>(&devent),sizeof(dataEvent));

    }


    for(auto event_itr = usedEEvents.begin(); event_itr!=usedEEvents.end(); event_itr++)
    {
        dataEvent devent;

        if(event_itr->second != NULL)
        {

            devent.id = (*event_itr->second).id;
            devent.activationTime = (*event_itr->second).activationTime;
            devent.targetID = (*event_itr->second).targetID;
            devent.originX = (*event_itr->second).posX;
            devent.originY = (*event_itr->second).posY;
            devent.originID = (*event_itr->second).originID;
            devent.propagationSpeed = (*event_itr->second).propagationSpeed;

            strncpy(devent.desc,(*event_itr->second).desc.c_str(),150);
            strncpy(devent.table,(*event_itr->second).luatable.c_str(),1024);

            //Add the filename if it exists:
            auto infoItr = agentFilenames.find((*event_itr->second).originID);

            if(infoItr != agentFilenames.end())
            {
                strncpy(devent.filename, infoItr->second.c_str(), 256);
            }
            else
                strncpy(devent.filename, std::string("NULL").c_str(),256);

            //Output::Inst()->kprintf("data event des.%s ", devent.desc);

            file.write(reinterpret_cast<char*>(&devent),sizeof(dataEvent));
        }
    }

    file.close();

	std::string position_path = path;
	//Output::Inst()->kprintf(path_str());
    position_path.erase(position_path.end()-3,position_path.end());
    position_path.append("pos");
	std::remove(position_path.c_str());
	std::string posBufferPath = Output::Inst()->RanaDir;
	posBufferPath.append("/_positionData.pos");
	std::rename(posBufferPath.c_str(),position_path.c_str());

    Output::Inst()->kprintf("Saving position data to %s", position_path.c_str());
    Output::Inst()->kprintf("Saving data done\n");

}

/**
 * Prints all unique legacy tmus
 */
void EventQueue::printLTmus(){
    Output::Inst()->kprintf("----------------\n");

	for(legacyIt = legacyTmu.begin(); legacyIt!=legacyTmu.end(); ++legacyIt)
	{
        Output::Inst()->kprintf("%d\n", *legacyIt);
    }
    Output::Inst()->kprintf("----------------\n");
}

/**
 * Prints all unique active tmus
 */
void EventQueue::printATmus(){
    Output::Inst()->kprintf("----------------\n");

	for(activeIt = activeTmu.begin(); activeIt!=activeTmu.end(); activeIt++)
	{
        Output::Inst()->kprintf("%llu\n", *activeIt);
    }
    Output::Inst()->kprintf("----------------\n");
}

void EventQueue::addAutonInfo(int id, std::string filename)
{
    agentFilenames.insert(std::pair<int, std::string>(id, filename) );
}
