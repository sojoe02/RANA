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
#include <iostream>
#include <cstring>
#include <string>
#include <random>
#include <chrono>

#include "lua.hpp"
#include "lauxlib.h"
#include "lualib.h"


#include "ID.h"
#include "autonLUA.h"
#include "../../physics/phys.h"
#include "../../physics/gridmovement.h"
#include "../../physics/maphandler.h"



AutonLUA::AutonLUA(int ID, double posX, double posY, double posZ, Nestene *nestene, std::string filename)
    : Auton(ID, posX, posY, posZ, nestene), filename(filename)
{
    desc = "LUA";



    //Output::Inst()->kprintf("%f,%f", posX, posY);
    /*
     * Setup up the LUA stack:
     */
    L = luaL_newstate();

    luaL_openlibs(L);
    /*
     * Register all the physics wrapper functions:
     */
    lua_register(L, "l_speedOfSound", l_speedOfSound);
    lua_register(L, "l_distance", l_distance);
    lua_register(L, "l_currentTime",l_currentTime);
    lua_register(L, "l_debug", l_debug);
    lua_register(L, "l_generateEventID", l_generateEventID);
    lua_register(L, "l_getMacroFactor", l_getMacroFactor);
    lua_register(L, "l_getTimeResolution", l_getTimeResolution);
    lua_register(L, "l_getMersenneFloat", l_getMersenneFloat);
    lua_register(L, "l_getMersenneInteger", l_getMersenneInteger);
    lua_register(L, "l_getEnvironmentSize", l_getEnvironmentSize);
    lua_register(L, "l_modifyMap", l_modifyMap);
    lua_register(L, "l_checkMap", l_checkMap);
    lua_register(L, "l_checkCollision", l_checkCollision);
    lua_register(L, "l_gridMove", l_gridMove);
    //Load the LUA frog:
    //std::string pre = "../src/frog.lua";
    //std::string file = pre;
    //

    if(luaL_loadfile(L, filename.c_str() ) || lua_pcall(L,0,0,0)){
        Output::Inst()->kprintf("error : %s \n", lua_tostring(L, -1));
        nofile = true;
        Output::Inst()->kprintf("Lua Auton disabled\n");
    }
    //init the LUA frog:
    lua_getglobal(L,"initAuton");
    lua_pushnumber(L,posX);
    lua_pushnumber(L,posY);
    lua_pushnumber(L,ID);
    int mf = Phys::getMacroFactor();
    double tr = Phys::getTimeRes();
    lua_pushnumber(L,mf);
    lua_pushnumber(L,tr);
    //Call the initAuton function (3 arguments, 0 results):
    if(lua_pcall(L,5,0,0)!=LUA_OK){
        Output::Inst()->kprintf("error on init autonLUA: %s\n",	lua_tostring(L,-1));
        nofile = true;
        Output::Inst()->kprintf("Lua Auton disabled\n");
    }
    //lua_settop(L,0);
    //sync positions:
    lua_getglobal(L,"getSyncData");

    if(lua_pcall(L,0,2,0)!=LUA_OK)
        Output::Inst()->kprintf("error on initiateEvent:getSyncData:\t %s\n",lua_tostring(L,-1));

    posX = lua_tonumber(L,-2);
    posY = lua_tonumber(L,-1);
    lua_settop(L,0);

    GridMovement::addPos(posX,posY);
}

AutonLUA::~AutonLUA(){
}


/**
 * Handler for external events.
 * Will send all relevant event data to the LUA script which will then
 * process the event and either return a null string, or arguments
 * to initiate an internal event.
 * @param event pointer to the external event.
 * @return internal event.
 */
EventQueue::iEvent* AutonLUA::handleEvent(EventQueue::eEvent *event){
    //	if(nofile)
    //		return NULL;

    //	lua_settop(L,0);

    //	int isnum;
    //	//set the lua function:
    //	lua_getglobal(L,"handleExternalEvent");
    //	//push required arguments for eventhandling to the stack:
    //	lua_pushnumber(L,event->origin->getPosX());
    //	lua_pushnumber(L,event->origin->getPosY());
    //	//push events ID to the stack:
    //	lua_pushnumber(L,event->id);
    //	//push propagation speed to the stack:
    //	lua_pushnumber(L,event->propagationSpeed);
    //	//push the events description string to the stack
    //	lua_pushstring(L,event->desc.c_str());
    //	//push the table to the stack
    //	lua_pushstring(L,event->table.c_str());
    //	//make the function call with 5 arguments and 3 results
    //	if(lua_pcall(L,6,3,0)!=LUA_OK)
    //		Output::Inst()->kprintf("error on handleEvent:\t %s\n",lua_tostring(L,-1));

    //	//Test if the handleevent method returns a null string
    //	std::string nullValue ="null";
    //	std::string validator = lua_tostring(L,-1);

    //	if(nullValue.compare(validator)==0)
    //		return NULL;
    //	//Generate the internal event:
    //	EventQueue::iEvent *ievent = new EventQueue::iEvent();
    //	//first set the two pointers to 'this' and the external event that spurred it:
    //	ievent->origin = this;
    //	ievent->event = event;

    //	//activation time:
    //	double activationTime = lua_tonumberx(L,-1, &isnum);
    //	if(!isnum){
    //		Output::Inst()->kprintf("LUA function handleEvent activation must be a number");
    //		delete ievent;
    //		return NULL;
    //	} else ievent->activationTime = activationTime;

    //	unsigned long long id = lua_tonumberx(L,-2, &isnum);
    //	if(!isnum){
    //		Output::Inst()->kprintf("LUA function hendleExternal id must be a number");
    //		delete ievent;
    //		return NULL;
    //	} else ievent->id = id;

    //	//the description string:
    //	ievent->desc = lua_tostring(L,-3);

    //	//the table string:

    //calculate arrivaltime:
    if (event->targetID == 0 || event->targetID == ID)
    {
        EventQueue::iEvent *ievent = new EventQueue::iEvent();

        ievent->origin = this;
        ievent->event = event;
        ievent->activationTime =
                Phys::speedOfSound(event->origin->getPosX(), event->origin->getPosY(),
                                   posX, posY, event->propagationSpeed);
        ievent->id = ID::generateEventID();
        ievent->desc = "standard";

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
EventQueue::eEvent* AutonLUA::initEvent(){
    if(nofile)
        return NULL;

    lua_settop(L,0);

    int isnum;

    //sync positions:
    lua_getglobal(L,"getSyncData");

    if(lua_pcall(L,0,2,0)!=LUA_OK)
        Output::Inst()->kprintf("error on initiateEvent:getSyncData:\t %s\n",lua_tostring(L,-1));

    posX = lua_tonumber(L,-2);
    posY = lua_tonumber(L,-1);

    //Output::Inst()->kprintf("position %f, %f \n", posX, posY);
    //lua_settop(L,0);

    //Call the initiate event function:
    lua_getglobal(L,"initiateEvent");

    if(lua_pcall(L,0,4,0)!=LUA_OK)
        Output::Inst()->kprintf("error on initiateEvent:\t %s\n",lua_tostring(L,-1));

    std::string nullValue = "null";
    std::string validator = lua_tostring(L,-1);
    if(nullValue.compare(validator)==0){
        //Output::Inst()->kprintf("validator is : %s\n",validator.c_str());
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
    if(!isnum){
        Output::Inst()->kprintf("LUA function handleExternal propagation speed must be a number\n");
        delete sendEvent;
        return NULL;
    } else sendEvent->propagationSpeed = lua_tonumber(L,-4);


    sendEvent->posX = posX;
    sendEvent->posY = posY;
    //Output::Inst()->kprintf("activationTime : %lld \t id : %lld \n desc : %s \t table : %s \n", sendEvent->activationTime,
    //		sendEvent->id, sendEvent->desc.c_str(), sendEvent->table.c_str());
    return sendEvent;
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
    if(nofile)
        return NULL;


    //If the event isn't broadcast and the targetID is not mine
    lua_settop(L,0);
    int isnum;
    //set the lua function:
    lua_getglobal(L,"handleInternalEvent");
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
        Output::Inst()->kprintf("error on 'handleInternalEvent':\t %s\n",lua_tostring(L,-1));

    //Test if the handleevent method returns a null string
    std::string nullValue = "null";
    std::string validator = lua_tostring(L,-1);
    if(nullValue.compare(validator)==0)
        return NULL;
    //Generate the internal event:
    EventQueue::eEvent* sendEvent = new EventQueue::eEvent();
    //first set the two pointers to 'this' and the external event that spurred it:
    sendEvent->origin = this;
    sendEvent->activationTime = Phys::getCTime();
    sendEvent->id = ID::generateEventID();

    //the description string:
    sendEvent->targetID = lua_tonumber(L, -1);
    sendEvent->desc = lua_tostring(L,-2);
    sendEvent->table = lua_tostring(L,-3);

    lua_tonumberx(L,-4, &isnum);
    if(!isnum){
        Output::Inst()->kprintf("LUA function 'handleInternalEvent' propagation speed must be a number\n");
        delete sendEvent;
        return NULL;
    } else sendEvent->propagationSpeed = lua_tonumber(L,-6);

    sendEvent->posX = posX;
    sendEvent->posY = posY;

    distroEEvent(sendEvent);


    return NULL;


}

void AutonLUA::simDone(){
    if(nofile)
        return;

    lua_getglobal(L,"simDone");
    if(lua_pcall(L,0,0,0)!=LUA_OK)
        Output::Inst()->kprintf("error on 'simDone':\t %s\n",lua_tostring(L,-1));

    lua_settop(L,0);
    lua_close(L);
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
    lua_pushnumber(L,Phys::getEnvX());
    lua_pushnumber(L,Phys::getEnvY());
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

    MapHandler::setPixelInfo(x, y, color);
    return 0;
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

int AutonLUA::l_gridMove(lua_State *L)
{

    int oldX = lua_tonumber(L, -4);
    int oldY = lua_tonumber(L, -3);
    int newX = lua_tonumber(L, -2);
    int newY = lua_tonumber(L, -1);

    GridMovement::updatePos(oldX, oldY, newX, newY);
    return 0;
}

int AutonLUA::l_checkCollision(lua_State *L)
{
    int x = lua_tonumber(L, -2);
    int y = lua_tonumber(L, -1);

    int agentAmount = GridMovement::checkCollision(x, y);

    lua_pushnumber(L, agentAmount);
    return 1;
}

//int AutonLUA::gridMove(lua_State *L){

//}


