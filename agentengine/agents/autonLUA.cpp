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
#include <cstring>
#include <string>
#include <random>
#include <chrono>
#include <iostream>
#include <exception>
#include <climits>

#include "lua.hpp"
#include "lauxlib.h"
#include "lualib.h"

#include "output.h"

#include "ID.h"
#include "autonLUA.h"
#include "doctor.h"
#include "../../physics/phys.h"
#include "../../physics/gridmovement.h"
#include "../../physics/maphandler.h"
#include "../../physics/shared.h"

AutonLUA::AutonLUA(int ID, double posX, double posY, double posZ, Nestene *nestene, std::string filename)
	: Auton(ID, posX, posY, posZ, nestene), filename(filename),
	  nofile(false),removed(false)
{
	desc = "LUA";
	//Output::Inst()->kprintf("%f,%f", posX, posY);
	/*
	 * Setup up the LUA stack:
	 */
	L = luaL_newstate();
	luaL_openlibs(L);
	/*
	 * Register all the API functions:
	 */
	lua_register(L, "l_speedOfSound", l_speedOfSound);
	lua_register(L, "l_distance", l_distance);
	lua_register(L, "l_currentTime",l_currentTime);
	lua_register(L, "l_debug", l_debug);
	lua_register(L, "l_print", l_print);
	lua_register(L, "l_generateEventID", l_generateEventID);
	lua_register(L, "l_getMacroFactor", l_getMacroFactor);
	lua_register(L, "l_getTimeResolution", l_getTimeResolution);
	lua_register(L, "l_getMersenneFloat", l_getMersenneFloat);
	lua_register(L, "l_getRandomFloat", l_getMersenneFloat);
	lua_register(L, "l_getRandomInteger", l_getMersenneInteger);
	lua_register(L, "l_getMersenneInteger", l_getMersenneInteger);
	lua_register(L, "l_getEnvironmentSize", l_getEnvironmentSize);
	lua_register(L, "l_modifyMap", l_modifyMap);
	lua_register(L, "l_checkMap", l_checkMap);
	lua_register(L, "l_checkPosition", l_checkPosition);
	lua_register(L, "l_updatePosition", l_updatePosition);
	lua_register(L, "l_checkCollision", l_checkCollision);
	lua_register(L, "l_gridMove", l_gridMove);
	lua_register(L, "l_stopSimulation", l_stopSimulation);
	lua_register(L, "l_getSharedNumber", l_getSharedNumber);
	lua_register(L, "l_addSharedNumber",l_addSharedNumber);
	lua_register(L, "l_getSharedString", l_getSharedString);
	lua_register(L, "l_addSharedString", l_addSharedString);
	lua_register(L, "l_getAgentPath", l_getAgentPath);
	lua_register(L, "l_getAutonPath", l_getAgentPath);
	lua_register(L, "l_addAuton", l_addAuton);
	lua_register(L, "l_removeAuton", l_removeAuton);
	lua_register(L, "l_removeAgent", l_removeAuton);
	lua_register(L, "l_addAgent", l_addAuton);

	if(luaL_loadfile(L, filename.c_str() ) || lua_pcall(L,0,0,0)){
		Output::Inst()->kprintf("error : %s \n", lua_tostring(L, -1));
		nofile = true;
		Output::Inst()->kprintf("Lua Auton disabled\n");
	}

	//lua_atpanic (L, AutonLUA::luapanic);

	//init the LUA frog:
	lua_getglobal(L,"initAuton");
	lua_pushnumber(L,posX);
	lua_pushnumber(L,posY);
	lua_pushnumber(L,ID);
	int mf = Phys::getMacroFactor();
	double tr = Phys::getTimeRes();
	lua_pushnumber(L,mf);
	lua_pushnumber(L,tr);

	if(nestene != NULL)
	{
		Output::Inst()->kdebug("I belong to Nestene %i", nestene->getID());
	}

	try{
		//Call the initAuton function (3 arguments, 0 results):
		if(lua_pcall(L,5,0,0)!=LUA_OK)
		{
			Output::Inst()->kprintf("<b><font color=\"brown\">error on init autonLUA: %s\n</font></b></>",	lua_tostring(L,-1));
			nofile = true;
			Output::Inst()->kprintf("Lua Auton disabled\n");
		}


		//lua_settop(L,0);
		//sync positions:
		lua_getglobal(L,"getSyncData");
		if(lua_pcall(L,0,2,0)!=LUA_OK){
			Output::Inst()->kprintf("<b><font color=\"brown\">error on initiateEvent:getSyncData:\t %s\n</font></b></>",lua_tostring(L,-1));
		}else{

			Auton::posX = lua_tonumber(L,-2);
			Auton::posY = lua_tonumber(L,-1);
		}
	}catch(std::exception& e){
		Output::Inst()->kprintf("<b><font color=\"red\">Error on Agent Initiation..%s, %s</font></b></>" , e.what());
		Output::RunSimulation = false;
	}

	lua_settop(L,0);
	getSyncData();
}

AutonLUA::~AutonLUA(){
	//delete L;
	//lua_close(L);
	lua_settop(L,0);
	lua_close(L);
}


/**
 * Handler for external events.
 * Will send all relevant event data to the LUA script which will then
 * process the event and either return a null string, or arguments
 * to initiate an internal event.
 * @param event pointer to the external event.
 * @return internal event.
 */
EventQueue::iEvent* AutonLUA::handleEvent(EventQueue::eEvent *event)
{
	if (removed) return NULL;

	if (event->targetID == 0 || event->targetID == ID)
	{
		EventQueue::iEvent *ievent = new EventQueue::iEvent();

		ievent->origin = this;
		ievent->event = event;
		if (event->propagationSpeed == 0 )
		{
			ievent->activationTime = Phys::getCTime() + 1;

		} else
		{
			ievent->activationTime =
					Phys::speedOfSound(event->origin->getPosX(),
									   event->origin->getPosY(),
									   posX, posY, event->propagationSpeed) + 1;
		}

		ievent->id = ID::generateEventID();
		ievent->desc = "";
		ievent->originID = ID;

		return ievent;
	} else
		return NULL;
}

/**
 * Query if the auton will initiate an event.
 * This will call up the LUA autons initEvent function which will
 * either return a 'null' string or data to build an
 * external event.
 * @return EventQueue::eEvent pointer to an external event or
 * a null pointer in which case nothing happens.
 */
EventQueue::eEvent* AutonLUA::initEvent()
{
	if(removed) return NULL;

	if(nofile) return NULL;

	lua_settop(L,0);

	int isnum;

	//Output::Inst()->kprintf("position %f, %f \n", posX, posY);
	//lua_settop(L,0);
	try{
		//Call the initiate event function:
		lua_getglobal(L,"initiateEvent");

		if(lua_pcall(L,0,4,0)!=LUA_OK)
		{
			Output::Inst()->kprintf("<b><font color=\"brown\">error on initiateEvent:\t %s\n</font></b></>",lua_tostring(L,-1));
			Output::RunSimulation.store(false);
			return NULL;
			//error(L, "error on initiateEvent %s", lua_tostring(L,-1));
		}

		std::string nullValue = "null";
		std::string validator = lua_tostring(L,-1);
		if(nullValue.compare(validator)==0){
			//Output::Inst()->kprintf("validator is : %s\n",validator.c_str());
			getSyncData();
			return NULL;
		}

		//Generate the internal event:
		EventQueue::eEvent* sendEvent = new EventQueue::eEvent();

		sendEvent->origin = this;
		sendEvent->activationTime = Phys::getCTime();
		sendEvent->id = ID::generateEventID();

		//the description string:
		sendEvent->targetID = lua_tonumber(L, -1);
		sendEvent->desc = lua_tostring(L,-2);
		sendEvent->table = lua_tostring(L,-3);

		lua_tonumberx(L,-4, &isnum);

		if(!isnum)
		{
			Output::Inst()->kprintf("LUA function handleExternal propagation speed must be a number\n");
			delete sendEvent;
			Output::RunSimulation.store(false);
			return NULL;
		} else sendEvent->propagationSpeed = lua_tonumber(L,-4);

		//Output::Inst()->kprintf("activationTime : %lld \t id : %lld \n desc : %s \t table : %s \n", sendEvent->activationTime,
		//		sendEvent->id, sendEvent->desc.c_str(), sendEvent->table.c_str());
		//sync positions:

		sendEvent->posX = posX;
		sendEvent->posY = posY;

		getSyncData();
		return sendEvent;
	}
	catch(std::exception& e)
	{
		Output::Inst()->kprintf("<b><font color=\"red\">Error on Initiate Event..%s</font></b></>", e.what());
		Output::RunSimulation = false;
	}

	return NULL;
}

/*
 * function for event processing
 * this function needs to be implemented in the target agent in order to
 * support event processing.
 * @param devent a pointer to the dataevent that needs to be processed.
 * @param time the simulation time that the event is to be processed at.
 * @param x, y target x and y position that the event is to be processed at.
 * @param zvalue, the address of the zvalue is to be written at.
 * @param duration, the address the events duration is to be written at.
 */
void AutonLUA::processFunction(EventQueue::dataEvent *devent, double time, double x, double y, double &zvalue, double &duration)
{
	if(removed) return;
	//Output::Inst()->ppprintf("X and Y is = %f,%f", 1.,1.);
	//zvalue = 1;
	//duration =0;
	try{
		lua_settop(L,0);

		lua_getglobal(L, "processFunction");
		lua_pushnumber(L, devent->originX);
		lua_pushnumber(L, devent->originY);
		lua_pushnumber(L, x);
		lua_pushnumber(L, y);
		lua_pushnumber(L,time);
		lua_pushstring(L, devent->table);

		if(lua_pcall(L,6,2,0)!=LUA_OK){
			Output::Inst()->ppprintf("error on calling processfunction : %s\n,",
									 lua_tostring(L,-1));
			Output::RunEventProcessing.store(false);
			return;
		} else
		{
			zvalue = lua_tonumber(L,-2);
			duration = lua_tonumber(L,-1);
		}

	}catch(std::exception& e)
	{
		Output::Inst()->ppprintf("<b><font color=\"red\">Error on processEvent..%s</font></b></>", e.what());
		Output::RunEventProcessing.store(false);
	}
	//Output::Inst()->ppprintf("zvalue: %f, duration %f", zvalue, duration);
}

void AutonLUA::setRemoved()
{

	Output::Inst()->kprintf("removing agent.#.%i",ID);
	removed = true;
	GridMovement::removePos(posX, posY, ID);
}

/**
 * Handler for internal events.
 * Will send all relevant event data to the LUA script which will then
 * process the event and either return a null string, or arguments
 * to initiate an external event.
 * @param event pointer to the internal event.
 * @return external event.
 */
EventQueue::eEvent* AutonLUA::actOnEvent(EventQueue::iEvent *ievent){

	if(removed) return NULL;

	if(nofile) return NULL;


	//If the event isn't broadcast and the targetID is not mine
	lua_settop(L,0);
	int isnum;

	try{
		//set the lua function:
		lua_getglobal(L,"handleEvent");
		//push required arguments for eventhandling to the stack:
		lua_pushnumber(L,ievent->event->origin->getPosX());
		lua_pushnumber(L,ievent->event->origin->getPosY());
		//push events ID to the stack:
		lua_pushnumber(L,ievent->event->origin->getID());
		//push the events description string to the stack
		lua_pushstring(L,ievent->event->desc.c_str());
		//push the table to the stack
		lua_pushstring(L,ievent->event->table.c_str());
		//make the function call with 5 arguments and 6 returnvalues
		if(lua_pcall(L,5,4,0)!=LUA_OK)
		{
			Output::Inst()->kprintf("<b><font color=\"brown\">error on 'handleEvent':\t %s</font></b></>",lua_tostring(L,-1));
			Output::RunSimulation.store(false);
			return NULL;
		}

		//Test if the handleevent method returns a null string
		std::string nullValue = "null";
		std::string validator = lua_tostring(L,-1);
		if(nullValue.compare(validator)==0)
			return NULL;

		//Generate the internal event:
		EventQueue::eEvent* sendEvent = new EventQueue::eEvent();
		//first set the two pointers to 'this' and the external event that spurred it:
		sendEvent->origin = this;
		sendEvent->activationTime = Phys::getCTime() + 1;
		sendEvent->id = ID::generateEventID();

		sendEvent->targetID = lua_tonumber(L, -1);
		sendEvent->desc = lua_tostring(L,-2);
		sendEvent->table = lua_tostring(L,-3);

		lua_tonumberx(L,-4, &isnum);
		if(!isnum){
			Output::Inst()->kprintf("LUA function 'handleInternalEvent' propagation speed must be a number\n");
			delete sendEvent;
			return NULL;
		} else sendEvent->propagationSpeed = lua_tonumber(L,-4);

		sendEvent->posX = posX;
		sendEvent->posY = posY;

		distroEEvent(sendEvent);
	}
	catch(std::exception& e)
	{
		Output::Inst()->kprintf("<b><font color=\"red\">Error on handleEvent..%s</font></b></>", e.what());
		Output::RunSimulation = false;
	}

	delete ievent;
	return NULL;

}

void AutonLUA::simDone(){

	if(nofile)
		return;

	try{
		lua_getglobal(L,"simDone");
		if(lua_pcall(L,0,0,0)!=LUA_OK)
		{
			Output::Inst()->kprintf("<b><font color=\"brown\">error on 'simDone':\t %s\n</font></b></>",lua_tostring(L,-1));
		}
	} catch(std::exception& e)
	{
		Output::Inst()->kprintf("<b><font color=\"red\">Error on simulationDone..%s</font></b></>", e.what());
	}
}

void AutonLUA::getSyncData(){

	if(removed) return;

	try{
		lua_getglobal(L,"getSyncData");

		if(lua_pcall(L,0,2,0)!=LUA_OK)
			Output::Inst()->kprintf("<b><font color=\"red\" error on getSyncData:getSyncData:\t %s\n</font></b></>",lua_tostring(L,-1));

		Auton::posX = lua_tonumber(L,-2);
		Auton::posY = lua_tonumber(L,-1);
	}catch(std::exception& e)
	{
		Output::Inst()->kprintf("<b><font color=\"red\">Error on getSyncData..%s</font></b></>", e.what());
	}
}


/*********************************************
 * LUA wrapper functions:
 *********************************************/

/**
 * Calls my printf wrapper function with a given string.
 * @param L LUA state pointer.
 * @return 0.
 */
int AutonLUA::l_debug(lua_State *L){
	Output::Inst()->kdebug(lua_tostring(L,-1));
	return 0;
}

int AutonLUA::l_print(lua_State *L){
	Output::Inst()->kprintf(lua_tostring(L,-1));
	return 0;
}

/**
 * Retrieves a unique ID for an event.
 * @param L LUA state pointer
 * @return 1, the ID
 */
int AutonLUA::l_generateEventID(lua_State *L){

	unsigned long long id = ID::generateEventID();
	lua_pushnumber(L,id);
	return 1;
}


/******* Environmental ***********************/

/**
 * Enables the lua auton to calculate the time step.
 * in which an emitted sound from an external source will reach it
 * @param L Lua state pointer on which the timestep should be pushed.
 * @return number of items pushed to the stack, ie number of results.
 */
int AutonLUA::l_speedOfSound(lua_State *L){	
	//push the arguments:
	double posX = lua_tonumber(L,-1);
	double posY = lua_tonumber(L,-2);
	double origX = lua_tonumber(L,-3);
	double origY = lua_tonumber(L,-4);
	double propagationSpeed = lua_tonumber(L,-5);

	unsigned long long t = Phys::speedOfSound(posX, posY, origX, origY, propagationSpeed);

	lua_pushnumber(L,t);

	return 1;
}

/**
 * Calculate the distance.
 * Enables the lua to call a C function that calculates the
 * distance between two points.
 * @see l_speedOfSound
 */
int AutonLUA::l_distance(lua_State *L){
	//push the arguments:
	double posX = lua_tonumber(L,-1);
	double posY = lua_tonumber(L,-2);
	double origX = lua_tonumber(L,-3);
	double origY = lua_tonumber(L,-4);

	unsigned long long d = Phys::calcDistance(origX, origY, posX, posY);

	lua_pushnumber(L,d);
	return 1;
}

/**
 * Get the current timestep.
 * @see l_speedOfSound
 */
int AutonLUA::l_currentTime(lua_State *L){
	unsigned long long t = Phys::getCTime();
	lua_pushnumber(L,t);
	return 1;
}

int AutonLUA::l_getEnvironmentSize(lua_State *L){
	lua_pushnumber(L,Phys::getEnvX()-1);
	lua_pushnumber(L,Phys::getEnvY()-1);
	return 2;

}


int AutonLUA::l_getMacroFactor(lua_State *L){
	int mf = Phys::getMacroFactor();
	lua_pushnumber(L,mf);
	return 1;
}

int AutonLUA::l_getTimeResolution(lua_State *L){	
	double tr = Phys::getTimeRes();
	lua_pushnumber(L,tr);
	return 1;
}

int AutonLUA::l_getMersenneFloat(lua_State *L){
	double low = lua_tonumber(L,-2);
	double high = lua_tonumber(L, -1);

	double number = Phys::getMersenneFloat(low,high);

	lua_pushnumber(L,number);
	return 1;
}

int AutonLUA::l_getMersenneInteger(lua_State *L){
	uint64_t low = lua_tonumber(L,-2);
	uint64_t high = lua_tonumber(L, -1);

	uint64_t number = Phys::getMersenneInteger(low,high);
	lua_pushnumber(L,number);
	return 1;
}
//MAP NAVIGATION AND MANIPULATION FUNCTIONS:

int AutonLUA::l_modifyMap(lua_State *L)
{
	int x = lua_tonumber(L, -5);
	int y = lua_tonumber(L, -4);

	rgba color;

	color.red = lua_tonumber(L, -3);
	color.green = lua_tonumber(L, -2);
	color.blue = lua_tonumber(L, -1);
	color.alpha = 0;

	bool success = MapHandler::setPixelInfo(x, y, color);
	lua_pushboolean(L, success);
	return 1;
}

int AutonLUA::l_checkMap(lua_State *L)
{
	int x = lua_tonumber(L, -2);
	int y = lua_tonumber(L, -1);

	rgba color = MapHandler::getPixelInfo(x, y);

	lua_pushnumber(L, color.red);
	lua_pushnumber(L, color.green);
	lua_pushnumber(L, color.blue);
	return 3;
}

int AutonLUA::l_updatePosition(lua_State *L)
{
	int oldX = lua_tonumber(L, -5);
	int oldY = lua_tonumber(L, -4);
	int newX = lua_tonumber(L, -3);
	int newY = lua_tonumber(L, -2);
	int id = lua_tonumber(L, -1);

	if(oldX != newX || oldY != newY)
		GridMovement::updatePos(oldX, oldY, newX, newY, id);

	return 0;
}

int AutonLUA::l_checkCollision(lua_State *L)
{
	int posX = lua_tonumber(L, -2);
	int posY = lua_tonumber(L, -1);

	bool collision = GridMovement::checkCollision(posX, posY);

	lua_pushboolean(L, collision);

	return 1;
}


int AutonLUA::l_checkPosition(lua_State *L)
{
	int posX = 0;
	int posY = 0;

	//try{
	posX = lua_tonumber(L, -2);
	posY = lua_tonumber(L, -1);

	pList agentList = GridMovement::checkPosition(posX, posY);

	lua_newtable(L);

	int i = 1;
	for(pList::iterator it = agentList.begin(); it != agentList.end(); ++it,i++)
	{
		lua_pushnumber(L, i);
		lua_pushnumber(L, *it);
		lua_settable(L, -3);
	}

	return 1;
}

int AutonLUA::l_scanRadial(lua_State *L)
{
	int radius = lua_tonumber(L, -4);
	std::string channel = lua_tostring(L, -3);
	int posX = lua_tonumber(L, -2);
	int posY = lua_tonumber(L, -1);

	MATRICE result = MapHandler::radialScan(radius, channel.at(1), posX, posY);

	lua_newtable(L);
	for (int i = 0; i < radius*2+1; i++)
	{
		for(int j = 0; j < radius*2+1; j++)
		{
			lua_pushnumber(L, j*i);
			lua_pushnumber(L, result[i][j]);
			lua_rawset(L, -3);
		}
	}
	return 1;
}

int AutonLUA::l_gridMove(lua_State *L)
{
	int oldX = lua_tonumber(L, -4);
	int oldY = lua_tonumber(L, -3);
	int newX = lua_tonumber(L, -2);
	int newY = lua_tonumber(L, -1);
	return 0;
}

int AutonLUA::l_stopSimulation(lua_State *L)
{
	Output::RunSimulation = false;
	return 0;
}

int AutonLUA::l_addSharedNumber(lua_State *L)
{
	std::string key = lua_tostring(L, -2);
	double value = lua_tonumber(L, -1);

	Shared::addNumber(key, value);

	return 0;
}

int AutonLUA::l_getSharedNumber(lua_State *L)
{
	std::string key = lua_tostring(L, -1);

	double value = Shared::getNumber(key);

	if (value == LLONG_MIN)
	{
		lua_pushstring(L, "no_value");
	} else
	{
		lua_pushnumber(L, value);
	}
	return 1;

}

int AutonLUA::l_addSharedString(lua_State *L)
{
	std::string key = lua_tostring(L, -2);
	std::string value = lua_tostring(L, -1);

	Shared::addString(key, value);

	return 0;
}

int AutonLUA::l_getSharedString(lua_State *L)
{
	std::string key = lua_tostring(L, -1);
	std::string value = Shared::getString(key);

	lua_pushstring(L, value.c_str());

	return 1;

}

int AutonLUA::l_getAgentPath(lua_State *L)
{
	lua_pushstring(L,Output::AgentPath.c_str());
	lua_pushstring(L,Output::AgentFile.c_str());

	return 2;
}

int AutonLUA::l_addAuton(lua_State *L)
{
	double posX = lua_tonumber(L, -5);
	double posY = lua_tonumber(L, -4);
	double posZ = lua_tonumber(L, -3);
	std::string path = lua_tostring(L, -2);
	std::string filename = lua_tostring(L, -1);

	int id = Doctor::addLuaAuton(posX, posY, posZ, path + filename);

	lua_pushinteger(L, id);

	return 1;
}

int AutonLUA::l_removeAuton(lua_State *L)
{

	int id = lua_tonumber(L, -1);
	bool removed = Doctor::removeAuton(id);
	lua_pushboolean(L, removed);

	return 1;
}

int AutonLUA::luapanic(lua_State *L)
{
	std::string str = lua_tostring(L, 1);
	Output::Inst()->kprintf("<b>PANIC,%s</b></>", str.c_str());
	Output::KillSimulation.store(true);

	return 0;
}



