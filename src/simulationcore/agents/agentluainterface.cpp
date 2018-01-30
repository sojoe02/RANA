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
#include <cmath>
#include <mutex>

#include "lua.hpp"
//#include "lauxlib.h"
//#include "lualib.h"

#include "src/ID.h"
#include "src/output.h"
#include "src/api/phys.h"
#include "src/api/gridmovement.h"
#include "src/api/maphandler.h"
#include "src/api/shared.h"
#include "src/api/scanning.h"
#include "src/simulationcore/interfacer.h"
#include "src/simulationcore/agents/agentluainterface.h"

AgentLuaInterface::AgentLuaInterface(int ID, double posX, double posY, double posZ, Sector *sector, std::string filename)
    :Agent(ID,posX,posY,posZ,sector),destinationX(posX),destinationY(posY),speed(1),moving(false),gridmove(false),
    filename(filename),nofile(false),removed(false),L(NULL)
{
    //Output::Inst()->kprintf("%f,%f", posX, posY);
    desc = "LUA";
    color.red=255;
    color.green=255;
    color.blue=0;

    //Setup up the LUA stack:
    L = luaL_newstate();
    if(L == NULL)
    {
        Output::Inst()->kprintf("<b><font color=\"brown\">A new Agent cannot be initialized. Lua(%s) is out of memory, Killing simulation</font></b></>", LUA_VERSION);
        Output::KillSimulation.store(true);
        removed = true;
    }
    else
    {
        luaL_openlibs(L);

        // Register the path to the Rana specific lua modules
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "path");
        std::string cur_path = lua_tostring(L, -1);
        std::string module_path = Output::Inst()->RanaDir;
        //Output::Inst()->kdebug(module_path.c_str());
        module_path.append("/src/modules/?.lua");
        //Output::Inst()->kdebug(module_path.c_str());
        cur_path.append(";");
        cur_path.append(module_path);
        cur_path.append(";");
        cur_path.append(Output::Inst()->AgentPath);
        cur_path.append("?.lua");
        lua_pop(L,1);
        lua_pushstring(L, cur_path.c_str());
        lua_setfield(L,-2,"path");
        lua_pop(L,1);

        //set the global values for the agent:
        lua_pushnumber(L,ID);
        lua_setglobal(L,"ID");
        lua_pushnumber(L,posX);
        lua_setglobal(L,"PositionX");
        lua_pushnumber(L,posY);
        lua_setglobal(L, "PositionY");
        lua_pushnumber(L, Phys::getMacroFactor()*Phys::getTimeRes());
        lua_setglobal(L, "STEP_RESOLUTION");
        lua_pushnumber(L, Phys::getTimeRes());
        lua_setglobal(L, "EVENT_RESOLUTION");
        lua_pushnumber(L, macroFactorMultiple);
        lua_setglobal(L, "StepMultiple");

        lua_pushnumber(L, Phys::getEnvX());
        lua_setglobal(L, "ENV_WIDTH");
        lua_pushnumber(L, Phys::getEnvY());
		lua_setglobal(L, "ENV_HEIGHT");
		lua_pushnumber(L, Phys::getEnvZ());
		lua_setglobal(L, "ENV_DEPTH");

        lua_pushnumber(L, destinationX);
        lua_setglobal(L, "DestinationX");
        lua_pushnumber(L,destinationY);
        lua_setglobal(L, "DestinationY");
        lua_pushnumber(L, speed);
        lua_setglobal(L, "Speed");
        lua_pushboolean(L, moving);
        lua_setglobal(L, "Moving");
        lua_pushboolean(L, gridmove);
		lua_setglobal(L, "GridMove");

		lua_pushnumber(L, mass);
		lua_setglobal(L, "Mass");
		lua_pushnumber(L, charge);
		lua_setglobal(L, "Charge");
		lua_pushnumber(L, radius);
		lua_setglobal(L, "Radius");

		lua_pushnumber(L, color.red);
		lua_setglobal(L, "ColorRed");
		lua_pushnumber(L, color.green);
		lua_setglobal(L, "ColorGreen");
		lua_pushnumber(L, color.blue);
		lua_setglobal(L, "ColorBlue");
		lua_pushnumber(L, color.alpha);
		lua_setglobal(L, "ColorAlpha");

        lua_pushnumber(L, 0);
        lua_setglobal(L, "Angle");

        //lua_newtable(L);
        //lua_setglobal(L, "EventTable");
        //Register all the API functions:

        //Interface.
        lua_register(L, "l_debug", l_debug);
        lua_register(L, "l_print", l_print);
        lua_register(L, "say", l_debug);
        lua_register(L, "shout", l_print);

        //Physics and generator.
        lua_register(L, "l_speedOfSound", l_speedOfSound);
        lua_register(L, "l_distance", l_distance);
        lua_register(L, "l_currentTime",l_currentTime);
        lua_register(L, "l_currentTimeS", l_currentTimeS);
        lua_register(L, "l_generateEventID", l_generateEventID);
        lua_register(L, "l_getMacroFactor", l_getMacroFactor);
        lua_register(L, "l_getTimeResolution", l_getTimeResolution);
        lua_register(L, "l_getMersenneFloat", l_getMersenneFloat);
        lua_register(L, "l_getRandomFloat", l_getMersenneFloat);
        lua_register(L, "l_getRandomInteger", l_getMersenneInteger);
        lua_register(L, "l_getMersenneInteger", l_getMersenneInteger);

        //Map and movement.
        lua_register(L, "l_getEnvironmentSize", l_getEnvironmentSize);
        lua_register(L, "l_modifyMap", l_modifyMap);
        lua_register(L, "l_checkMap", l_checkMap);
        lua_register(L, "l_checkMapAndChange", l_checkMapAndChange);
        lua_register(L, "l_radialMapScan", l_radialMapScan);
        lua_register(L, "l_radialMapColorScan", l_radialMapColorScan);

        lua_register(L, "l_checkPosition", l_checkPosition);
        lua_register(L, "l_updatePosition", l_updatePosition);
        lua_register(L, "l_addPosition", l_addPosition);
        lua_register(L, "l_checkCollision", l_checkCollision);
        lua_register(L, "l_checkCollisionRadial", l_checkCollisionRadial);
        lua_register(L, "l_gridMove", l_gridMove);
        lua_register(L, "l_getMaskRadial", l_getMaskRadial);
        lua_register(L, "l_radialCollisionScan", l_radialCollisionScan);
        lua_register(L, "l_initializeGrid", l_initializeGrid);
        lua_register(L, "l_getGridScale", l_getGridScale);
        lua_register(L, "l_updatePositionIfFree", l_updatePositionIfFree);

        //Shared values.
        lua_register(L, "l_getSharedNumber", l_getSharedNumber);
        lua_register(L, "l_addSharedNumber",l_addSharedNumber);
        lua_register(L, "l_getSharedString", l_getSharedString);
        lua_register(L, "l_addSharedString", l_addSharedString);
        lua_register(L, "l_getSharedTcpInput", l_getSharedTcpInput);

        //Simulation core.
        lua_register(L, "l_getAgentPath", l_getAgentPath);
        lua_register(L, "l_getAgentPath", l_getAgentPath);
        lua_register(L, "l_stopSimulation", l_stopSimulation);
        lua_register(L, "l_removeAgent", l_removeAgent);
        lua_register(L, "l_addAgent", l_addAgent);

        //Agent.
        lua_register(L, "l_emitEvent", l_emitEvent);
        lua_register(L, "l_addGroup", l_addGroup);
        lua_register(L, "l_removeGroup", l_removeGroup);
        lua_register(L, "l_setStepMultiplier", l_setMacroFactorMultipler);
        lua_register(L, "l_changeAgentColor", l_changeAgentColor);

        std::string auxLib = Output::Inst()->RanaDir;
        auxLib.append("/src/modules/auxiliary.lua");

        std::string settingsPath = filename;
        settingsPath.erase(settingsPath.end()-4,settingsPath.end());
        settingsPath.append("_settings.lua");

        if(luaL_loadfile(L, auxLib.c_str()) || lua_pcall(L,0,0,0))
        {
            Output::Inst()->kprintf("<font color=\"red\">error : %s <\font>", lua_tostring(L, -1));
            nofile = true;
            Output::Inst()->kprintf("Lua Agent disabled\n");
        }

        if(luaL_loadfile(L, filename.c_str() ) || lua_pcall(L,0,0,0))
        {
            Output::Inst()->kprintf("<font color=\"red\">error : %s <\font>", lua_tostring(L, -1));
            nofile = true;
            Output::Inst()->kprintf("Lua Agent disabled\n");
        }

        //if(sector != NULL)
        //Output::Inst()->kdebug("I belong to sector %i", sector->getID());

        getSyncData();
        //Call the Initialization function for the agent
        lua_settop(L,0);
    }

    moveFactor = Phys::getMacroFactor() * Phys::getTimeRes();
}

AgentLuaInterface::~AgentLuaInterface()
{
    if(L != NULL)
    {
        //Output::Inst()->kprintf("Deleting agent %d", ID);
        lua_settop(L,0);
        //lua_gc(L,LUA_GCCOLLECT,0);
        lua_close(L);
        //delete L;
        //L = NULL;
    }
}

void AgentLuaInterface::InitializeAgent()
{
    if(removed)
    {
        return;
    }

    lua_settop(L,0);
    try
    {
        lua_getglobal(L, "_InitializeAgent");
        //Call the initAgent function (3 arguments, 0 results):
        if(lua_pcall(L,0,0,0)!=LUA_OK)
        {
            Output::Inst()->kprintf("<b><font color=\"brown\">Error on agent initialization, %s</font></b></>",	lua_tostring(L,-1));
            nofile = true;
            Output::Inst()->kprintf("Lua Agent disabled\n");
        }

    }
    catch(std::exception& e)
    {
        Output::Inst()->kprintf("<b><font color=\"red\">Error on Agent initialization. %s, %s</font></b></>" , filename.c_str() ,e.what());
        Output::RunSimulation = false;
    }

    if (gridmove == true)
    {
        GridMovement::addPos(int(posX*GridMovement::getScale()),int(posY*GridMovement::getScale()),ID);
    }
    getSyncData();
}

/********************************************************
 * Simulation flow functions:
 *
 ********************************************************/

/**
 * Handler for external events.
 * Will send all relevant event data to the LUA script which will then
 * process the event and either return a null string, or arguments
 * to initiate an internal event.
 * @param event pointer to the external event.
 * @return internal event.
 */
std::unique_ptr<EventQueue::iEvent> AgentLuaInterface::processEvent(const EventQueue::eEvent *event)
{
    if (removed) return NULL;

    if (event->targetID == 0 || event->targetID == ID)
    {
        std::unique_ptr<EventQueue::iEvent> ievent(new EventQueue::iEvent());

        ievent->origin = this;
        ievent->event = event;

        if (event->propagationSpeed == 0)
        {
            ievent->activationTime = Phys::getCTime()+1;

        } else
        {
            ievent->activationTime =
                    Phys::speedOfEvent(event->posX,
                                       event->posY,
                                       posX, posY, event->propagationSpeed)+1;
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
std::unique_ptr<EventQueue::eEvent> AgentLuaInterface::takeStep()
{
    if(removed) return NULL;
    if(nofile) return NULL;
    lua_settop(L,0);

    try
    {
        lua_getglobal(L, "_TakeStep");
        if(lua_pcall(L,0,0,0) !=LUA_OK)
        {
            Output::Inst()->kprintf("<b><font color=\"brown\">Lua error on takeStep. %s, %s</font></b></>", filename.c_str() ,lua_tostring(L,-1));
            Output::RunSimulation.store(false);
            return NULL;
        }

        if(moving)
        {
            movement();
        }

        getSyncData();

        return NULL;

    }
    catch(std::exception &e)
    {
        Output::Inst()->kprintf("<b><font color=\"red\">Exception on takeStep. %s, %s</font></b></>", filename.c_str()  ,e.what());
        Output::RunSimulation.store(false);
    }
    return NULL;
}

/**
 * Handler for internal events.
 * Will send all relevant event data to the LUA script which will then
 * process the event and either return a null string, or arguments
 * to initiate an external event.
 * @param event pointer to the internal event.
 * @return external event.
 */
std::unique_ptr<EventQueue::eEvent> AgentLuaInterface::handleEvent(std::unique_ptr<EventQueue::iEvent> eventPtr)
{
    if(removed) return NULL;
    if(nofile) return NULL;
    //If the event isn't broadcast and the targetID is not mine
    lua_settop(L,0);
    //Output::Inst()->kprintf("handles event");
    try
    {

        lua_getglobal(L,"_HandleEvent");
        lua_pushnumber(L, eventPtr->event->posX);
        lua_pushnumber(L, eventPtr->event->posY);
		lua_pushnumber(L, eventPtr->event->posZ);
        lua_pushnumber(L, eventPtr->event->originID);
        lua_pushstring(L, eventPtr->event->desc.c_str());
        lua_pushstring(L, eventPtr->event->luatable.c_str());

		if(lua_pcall(L,6,0,0)!=LUA_OK)
        {
            Output::Inst()->kprintf("<b><font color=\"brown\">Error on event handling.%s, %s</font></b></>",filename.c_str(),lua_tostring(L,-1));
            Output::RunSimulation.store(false);
            return NULL;
        }
        getSyncData();
        return NULL;

    }
    catch(std::exception &e)
    {
        Output::Inst()->kprintf("<b><font color=\"red\">Exception on event handling.%s, %s</font></b></>",filename.c_str(), e.what());
        Output::RunSimulation = false;
    }

    return NULL;
}

void AgentLuaInterface::movement()
{
    lua_getglobal(L, "GridMove");
    gridmove = lua_toboolean(L,-1);
    //bool collision = false;


    if(posX != destinationX || posY != destinationY)
    {
        double angle = std::atan2(destinationX-posX, destinationY-posY);
        double vY = speed * std::cos(angle);
        double vX = speed * std::sin(angle);

        double newPosX = posX + moveFactor * vX * macroFactorMultiple;
        double newPosY = posY + moveFactor * vY * macroFactorMultiple;

        if( newPosX > Phys::getEnvX() || newPosY > Phys::getEnvY() || newPosX < 0 || newPosY < 0)
        {
            destinationX = posX;
            destinationY = posY;
            moving = false;
            lua_pushboolean(L, false);
            lua_setglobal(L, "Moving");

            return;
        }


        //Check if the agent overshoots it's target destinations.
        if(		(posX >= destinationX && newPosX <= destinationX &&
                 posY >= destinationY && newPosY <= destinationY) ||
                (posX <= destinationX && newPosX >= destinationX &&
                 posY >= destinationY && newPosY <= destinationY) ||
                (posX >= destinationX && newPosX <= destinationX &&
                 posY <= destinationY && newPosY >= destinationY) ||
                (posX <= destinationX && newPosX >= destinationX &&
                 posY <= destinationY && newPosY >= destinationY)
                )
            //if(std::abs(newPosX-DestinationX)std::abs(posX-destinationX))
        {
            moving = false;
            lua_pushboolean(L, moving);
            lua_setglobal(L, "Moving");
            newPosX = destinationX;
            newPosY = destinationY;
        }

        if(gridmove)
        {
            if(int(posX*GridMovement::getScale())	!=	int(newPosX*GridMovement::getScale()) ||
                    int(posY*GridMovement::getScale())!=int(newPosY*GridMovement::getScale()) )
            {
                GridMovement::updatePos(int(posX*GridMovement::getScale()),
                                        int(posY*GridMovement::getScale()),
                                        int(newPosX*GridMovement::getScale()),
                                        int(newPosY*GridMovement::getScale()),
                                        ID);
            }
        }

        //if(collisioni)
        //{
        //v//  moving = false;
        //  lua_pushboolean(L,moving);
        //  lua_setglobal(L,"Moving");
        // }
        //else
        //{
        posX = newPosX;
        posY = newPosY;
        lua_pushnumber(L, posX);
        lua_setglobal(L, "PositionX");
        lua_pushnumber(L, posY);
        lua_setglobal(L, "PositionY");
        //}

    }
    else
    {
        moving = false;
        lua_pushboolean(L,moving);
        lua_setglobal(L,"Moving");

    }
}

/********************************************************
 * post processing.
 *
 ********************************************************/

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
void AgentLuaInterface::processFunction(EventQueue::dataEvent *devent, double time, double x, double y, double &zvalue, double &duration)
{
    if(removed) return;

    try{
        lua_settop(L,0);

        //Output::Inst()->kprintf("hugahuga--%s", devent->table);

        lua_getglobal(L, "_ProcessEventFunction");
        lua_pushnumber(L, devent->originX);
        lua_pushnumber(L, devent->originY);
        lua_pushnumber(L, x);
        lua_pushnumber(L, y);
        lua_pushnumber(L, time);
        lua_pushstring(L, devent->table);

        if(lua_pcall(L,6,2,0)!=LUA_OK){
            Output::Inst()->kprintf("Error on calling _ProcessEventFunction : %s\n,",lua_tostring(L,-1));
            Output::RunEventProcessing.store(false);
            return;
        } else
        {
            zvalue = lua_tonumber(L,-2);
            duration = lua_tonumber(L,-1);
        }

    }
    catch(std::exception &e)
    {
        Output::Inst()->kprintf("<b><font color=\"red\">Error on processEvent..%s</font></b></>", e.what());
        Output::RunEventProcessing.store(false);
    }
}

/********************************************************
 * Core utility.
 *
 ********************************************************/

void AgentLuaInterface::setRemoved()
{
    //Output::Inst()->kprintf("removing agent.#.%i",ID);
    removed = true;
    GridMovement::removePos(ID);
}

void AgentLuaInterface::simDone()
{
    if(nofile)
        return;
    try
    {
        lua_getglobal(L, "_CleanUp");

        if(lua_pcall(L,0,0,0)!=LUA_OK)
        {
            Output::Inst()->kprintf("<b><font color=\"brown\">Error on cleanUp. %s</font></b></>",lua_tostring(L,-1));
        }
    }
    catch(std::exception& e)
    {
        Output::Inst()->kprintf("<b><font color=\"red\">Exception on cleanUp. %s</font></b></>", e.what());
    }
}

void AgentLuaInterface::getSyncData()
{
    if(removed) return;
    try
	{
        lua_getglobal(L, "Angle");

		lua_getglobal(L, "ColorRed");
		lua_getglobal(L, "ColorGreen");
		lua_getglobal(L, "ColorBlue");
		lua_getglobal(L, "ColorAlpha");
		lua_getglobal(L, "PositionZ");

		lua_getglobal(L, "Mass");
		lua_getglobal(L, "Charge");
		lua_getglobal(L, "Radius");

        lua_getglobal(L, "GridMove");
        lua_getglobal(L, "StepMultiple");
        lua_getglobal(L, "PositionX");
        lua_getglobal(L, "PositionY");
        lua_getglobal(L, "DestinationX");
        lua_getglobal(L, "DestinationY");
        lua_getglobal(L, "Speed");
		lua_getglobal(L, "Moving");

        angle = lua_tonumber(L, -17);

		radius = lua_tonumber(L, -9);
		charge = lua_tonumber(L, -10);
		mass = lua_tonumber(L, -11);

		posZ = lua_tonumber(L, -12);

		color.alpha = lua_tonumber(L, -13);
		color.red = lua_tonumber(L, -16);
		color.green = lua_tonumber(L, -15);
		color.blue = lua_tonumber(L, -14);


        int stepMultiple = (int)lua_tonumber(L, -7);
        if(stepMultiple >=0 )
        {
            macroFactorMultiple = stepMultiple;
        }
        posX = lua_tonumber(L, -6);
        posY = lua_tonumber(L, -5);

        destinationX = lua_tonumber(L, -4);
        destinationY = lua_tonumber(L, -3);
        speed = lua_tonumber(L, -2);
        moving = lua_toboolean(L, -1);
        gridmove = lua_toboolean(L,- 8);

    }
    catch(std::exception &e)
    {
        Output::Inst()->kprintf("<b><font color=\"red\">Exception on retrieving sync data, %s</font></b></>", e.what());
        Output::RunSimulation.store(false);
        return;
    }
}

/********************************************************
 * Rana agent API functions.
 *
 ********************************************************/


/**
 * Calls Ranas debug msg. wrapper function with a given string.
 * Can be disabled.
 * @return 0.
 */
int AgentLuaInterface::l_debug(lua_State *L)
{
    std::string string = lua_tostring(L, -1);
    string.resize(4096);
    Output::Inst()->kdebug(string.c_str());
    return 0;
}

/**
 * @brief AgentLUA::l_print writes html string to output.
 * Cannot be disabled.
 * @return
 */

int AgentLuaInterface::l_print(lua_State *L)
{
    std::string string = lua_tostring(L, -1);
    string.resize(4096);
    Output::Inst()->kprintf(string.c_str());
    return 0;
}

/**
 * Retrieves a unique ID, taken from the Event ID pool(deprecated).
 * @param L LUA state pointer
 * @return 1, the ID
 */
int AgentLuaInterface::l_generateEventID(lua_State *L)
{
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
int AgentLuaInterface::l_speedOfSound(lua_State *L)
{
    //push the arguments:
    double posX = lua_tonumber(L,-1);
    double posY = lua_tonumber(L,-2);
    double origX = lua_tonumber(L,-3);
    double origY = lua_tonumber(L,-4);
    double propagationSpeed = lua_tonumber(L,-5);

    unsigned long long t =
            Phys::speedOfEvent(posX, posY, origX, origY, propagationSpeed);

    lua_pushnumber(L,t);

    return 1;
}

/**
 * Calculate the distance.
 * Enables the lua to call a C function that calculates the
 * distance between two points.
 * @see l_speedOfSound
 */
int AgentLuaInterface::l_distance(lua_State *L)
{
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
int AgentLuaInterface::l_currentTime(lua_State *L)
{
    unsigned long long t = Phys::getCTime();
    lua_pushnumber(L,t);
    return 1;
}

int AgentLuaInterface::l_currentTimeS(lua_State *L)
{
    double time = (double)Phys::getCTime()*Phys::getTimeRes();
    lua_pushnumber(L,time);
    return 1;
}

int AgentLuaInterface::l_getMacroFactor(lua_State *L)
{
    int mf = Phys::getMacroFactor();
    lua_pushnumber(L,mf);
    return 1;
}

int AgentLuaInterface::l_getTimeResolution(lua_State *L)
{
    double tr = Phys::getTimeRes();
    lua_pushnumber(L,tr);
    return 1;
}

int AgentLuaInterface::l_getMersenneFloat(lua_State *L)
{
    double low = lua_tonumber(L,-2);
    double high = lua_tonumber(L, -1);

    //Output::Inst()->kdebug("%f,%f", low, high);
    double number = Phys::getMersenneFloat(low,high);

    lua_pushnumber(L,number);
    return 1;
}

int AgentLuaInterface::l_getMersenneInteger(lua_State *L)
{
    int64_t low = lua_tonumber(L,-2);
    int64_t high = lua_tonumber(L, -1);
    int64_t number = low;

    if(low > high)
    {
        number = Phys::getMersenneInteger(high, low);
    }
    else if(high > low)
    {
        number = Phys::getMersenneInteger(low, high);
    }

    lua_pushnumber(L,number);
    return 1;
}

//Map and position.
int AgentLuaInterface::l_getEnvironmentSize(lua_State *L)
{
    lua_pushnumber(L,Phys::getEnvX());
    lua_pushnumber(L,Phys::getEnvY());
    return 2;

}


int AgentLuaInterface::l_modifyMap(lua_State *L)
{
    double x = lua_tonumber(L, -5);
    double y = lua_tonumber(L, -4);

    int modX = x * Phys::getScale();
    int modY = y * Phys::getScale();

    rgba color;

    color.red = lua_tonumber(L, -3);
    color.green = lua_tonumber(L, -2);
    color.blue = lua_tonumber(L, -1);
    color.alpha = 0;

    bool success = MapHandler::setPixelInfo(modX, modY, color);
    lua_pushboolean(L, success);
    return 1;
}

int AgentLuaInterface::l_checkMap(lua_State *L)
{
    double x = lua_tonumber(L, -2);
    double y = lua_tonumber(L, -1);

    int checkX = x * Phys::getScale();
    int checkY = y * Phys::getScale();

    rgba color = MapHandler::getPixelInfo(checkX, checkY);

    lua_pushnumber(L, color.red);
    lua_pushnumber(L, color.green);
    lua_pushnumber(L, color.blue);
    return 3;
}

int AgentLuaInterface::l_checkMapAndChange(lua_State *L)
{
    double x = lua_tonumber(L, -8);
    double y = lua_tonumber(L, -7);

    int modX = x * Phys::getScale();
    int modY = y * Phys::getScale();

    rgba check_color;

    check_color.red = lua_tonumber(L, -6);
    check_color.green = lua_tonumber(L, -5);
    check_color.blue = lua_tonumber(L, -4);
    check_color.alpha = 0;

    rgba change_color;

    change_color.red = lua_tonumber(L, -3);
    change_color.green = lua_tonumber(L, -2);
    change_color.blue = lua_tonumber(L, -1);
    change_color.alpha = 0;

    bool changed = false;

    changed = MapHandler::checkAndChange(modX, modY, check_color, change_color);

    lua_pushboolean(L, changed);

    return 1;
}

int AgentLuaInterface::l_radialMapColorScan(lua_State *L)
{
    int radius = lua_tonumber(L, -6);
    int posX = lua_tonumber(L, -5) - radius;
    int posY = lua_tonumber(L, -4) - radius;
    int r = lua_tonumber(L, -3);
    int g = lua_tonumber(L, -2);
    int b = lua_tonumber(L, -1);
    //Output::Inst()->kprintf("radius %d, posX %d, posY %d", radius, posX, posY);

    int count = 0;

    MatriceInt result = Scanning::radialMask(radius);

    lua_newtable(L);
    int index = 0;

    for (int i = 1; i < radius*2; i++)
    {
        for(int j = 1; j < radius*2; j++)
        {
            if( result[i][j] == 1)
                //(posX + i != posX+radius || posY + j != posY+radius ))
            {

                rgba color = MapHandler::getPixelInfo(posX+i, posY+j);

                if(r == color.red && g == color.green && b == color.blue)
                {
 //                   Output::Inst()->kprintf("%i,%i,%i", color.red, color.green, color.blue);
                    count++;
                    index++;

                    lua_pushnumber(L, index);
                    lua_newtable(L);
                    lua_pushstring(L,"posX");
                    lua_pushnumber(L, posX+i);
                    lua_settable(L, -3);
                    lua_pushstring(L, "posY");
                    lua_pushnumber(L, posY+j);
                    lua_settable(L, -3);
                    lua_pushstring(L,"R");
                    lua_pushnumber(L, color.red);
                    lua_settable(L, -3);
                    lua_pushstring(L,"G");
                    lua_pushnumber(L, color.green);
                    lua_settable(L, -3);
                    lua_pushstring(L,"B");
                    lua_pushnumber(L, color.blue);
                    lua_settable(L, -3);
                    lua_settable(L, -3);
                }
            }
        }
    }

    if(count == 0)
    {
        lua_pushnil(L);
    }
    return 1;
}

int AgentLuaInterface::l_radialMapScan(lua_State *L)
{
    int radius = lua_tonumber(L, -3);
    int posX = lua_tonumber(L, -2) - radius;
    int posY = lua_tonumber(L, -1) - radius;
    //Output::Inst()->kprintf("radius %d, posX %d, posY %d", radius, posX, posY);
    MatriceInt result = Scanning::radialMask(radius);

    lua_newtable(L);
    int index = 0;

    for (int i = 1; i < radius*2; i++)
    {
        for(int j = 1; j < radius*2; j++)
        {
            if( result[i][j] == 1)
                //(posX + i != posX+radius || posY + j != posY+radius ))
            {
                index++;

                rgba color = MapHandler::getPixelInfo(posX+i, posY+j);

                lua_pushnumber(L, index);
                lua_newtable(L);
                lua_pushstring(L,"posX");
                lua_pushnumber(L, posX+i);
                lua_settable(L, -3);
                lua_pushstring(L, "posY");
                lua_pushnumber(L, posY+j);
                lua_settable(L, -3);
                lua_pushstring(L,"R");
                lua_pushnumber(L, color.red);
                lua_settable(L, -3);
                lua_pushstring(L,"G");
                lua_pushnumber(L, color.green);
                lua_settable(L, -3);
                lua_pushstring(L,"B");
                lua_pushnumber(L, color.blue);
                lua_settable(L, -3);
                lua_settable(L, -3);

            }
        }
    }
    return 1;
}


int AgentLuaInterface::l_updatePosition(lua_State *L)
{
    int oldX = lua_tonumber(L, -5)*GridMovement::getScale()+.5;
    int oldY = lua_tonumber(L, -4)*GridMovement::getScale()+.5;
    int newX = lua_tonumber(L, -3)*GridMovement::getScale()+.5;
    int newY = lua_tonumber(L, -2)*GridMovement::getScale()+.5;
    int id = lua_tointeger(L, -1);

    if(oldX != newX || oldY != newY)
        GridMovement::updatePos(oldX, oldY, newX, newY, id);

    return 0;
}

int AgentLuaInterface::l_addPosition(lua_State *L)
{

    int x = lua_tonumber(L, -3);
    int y = lua_tonumber(L, -2);
    int id = lua_tonumber(L, -1);
    GridMovement::addPos(x, y, id);

    return 0;
}

int AgentLuaInterface::l_checkCollision(lua_State *L)
{
    int posX = lua_tonumber(L, -2);
    int posY = lua_tonumber(L, -1);

    bool collision = GridMovement::checkCollision(posX, posY);

    lua_pushboolean(L, collision);

    return 1;
}


int AgentLuaInterface::l_checkPosition(lua_State *L)
{
    int posX = 0;
    int posY = 0;

    posX = lua_tonumber(L, -2) *GridMovement::getScale()+.5;
    posY = lua_tonumber(L, -1) *GridMovement::getScale()+.5;

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

int AgentLuaInterface::l_updatePositionIfFree(lua_State *L)
{
    int oldX = int(lua_tonumber(L, -5)*GridMovement::getScale());
    int oldY = int(lua_tonumber(L, -4)*GridMovement::getScale());
    int newX = int(lua_tonumber(L, -3)*GridMovement::getScale());
    int newY = int(lua_tonumber(L, -2)*GridMovement::getScale());
    int id = lua_tointeger(L, -1);

    bool moved = false;

    if(oldX != newX || oldY != newY)
    {
        moved = GridMovement::updateIfFree(oldX, oldY, newX, newY, id);
    }

    lua_pushboolean(L, moved);

    return 1;
}


int AgentLuaInterface::l_checkCollisionRadial(lua_State *L)
{
    int radius = lua_tonumber(L, -1);
    int posX = lua_tonumber(L, -3) - radius;
    int posY = lua_tonumber(L, -2) - radius;

    //Output::Inst()->kdebug("collision checked at posX+i %i and posY+");
    MatriceInt result = Scanning::radialMask(radius);

    bool collision = false;

    for (int i = 1; i < radius*2; i++)
    {
        for(int j = 1; j < radius*2; j++)
        {
            if( result[i][j] == 1 && (posX+i != posX+radius || posY+j != posY+radius ))
            {
                //Output::Inst()->kdebug("collision checked at posX+i %i and posY+j %i", posX+i, posY+j);
                collision = GridMovement::checkCollision(posX+i, posY+j);

                if (collision)
                {
                    lua_pushboolean(L, collision);
                    return 1;
                }
            }
        }
    }

    lua_pushboolean(L, collision);

    return 1;
}

int AgentLuaInterface::l_getMaskRadial(lua_State *L)
{
    int radius = lua_tonumber(L, -3);
    int posX = lua_tonumber(L, -2) - radius;
    int posY = lua_tonumber(L, -1) - radius;
    //Output::Inst()->kprintf("radius %d, posX %d, posY %d", radius, posX, posY);
    MatriceInt result = Scanning::radialMask(radius);

    lua_newtable(L);
    int index = 0;

    for (int i = 1; i < radius*2; i++)
    {
        for(int j = 1; j < radius*2; j++)
        {
            if( result[i][j] == 1)
                //(posX + i != posX+radius || posY + j != posY+radius ))
            {
                index++;

                lua_pushnumber(L, index);
                lua_newtable(L);
                lua_pushstring(L,"posX");
                lua_pushnumber(L, posX + i);
                lua_settable(L, -3);
                lua_pushstring(L,"posY");
                lua_pushnumber(L, posY + j);
                lua_settable(L, -3);
                lua_settable(L, -3);
                /*if(result[i][j] == 1 && (posX +i != posX+radius || posY + j != posY+radius))
            /{
                rgba color;
                color.blue = 255;
                color.green = 0;
                color.red = 0;
                MapHandler::setPixelInfo(posX+i, posY+j, color);
            }
            else
            {
                rgba color2;
                color2.blue = 0;
                color2.green = 0;
                color2.red = 255;
                MapHandler::setPixelInfo(posX+i, posY+j, color2);
            }*/
            }
        }
    }
    return 1;
}




int AgentLuaInterface::l_radialCollisionScan(lua_State *L)
{
    int radius = lua_tonumber(L, -3);
    int posX = lua_tonumber(L, -2) - radius;
    int posY = lua_tonumber(L, -1) - radius;
    int id = lua_tonumber(L,-4);

    //Output::Inst()->kdebug("collision checked at posX %d and posY %d", posX, posY);

    //return 1;
    MatriceInt result = Scanning::radialMask(radius);

    bool collision = false;
    int counter = 0;

    lua_newtable(L);

    for (int i = 1; i < radius*2; i++)
    {
        for(int j = 1; j < radius*2; j++)
        {
            if( result[i][j] == 1)
            {
                collision = GridMovement::checkCollision(posX+i, posY+j);

                //Output::Inst()->kdebl_radialMapColorScanug("Checking: %d, %d", posX+i, posY+j);

                if (collision)
                {
                    //lua_newtable(L);
                    pList agentList = GridMovement::checkPosition(posX+i, posY+j);

                    for(auto it = agentList.begin(); it != agentList.end(); ++it)
                    {
                        if (id != *it)
                        {
                            counter++;
                            lua_pushnumber(L, counter);
                            lua_newtable(L);
                            lua_pushstring(L, "id");
                            lua_pushnumber(L, *it);
                            lua_settable(L, -3);
                            lua_pushstring(L, "posX");
                            lua_pushnumber(L, posX + i);
                            lua_settable(L, -3);
                            lua_pushstring(L, "posY");
                            lua_pushnumber(L, posY + j);
                            lua_settable(L, -3);
                            lua_settable(L, -3);
                        }
                    }

                }
            }
        }
    }
    //lua_settable(L,-3);
    //lua_pushboolean(L, collision);

    return 1;
}

int AgentLuaInterface::l_gridMove(lua_State *L)
{
    int oldX = lua_tonumber(L, -4);
    int oldY = lua_tonumber(L, -3);
    int newX = lua_tonumber(L, -2);
    int newY = lua_tonumber(L, -1);
    return 0;
}

int AgentLuaInterface::l_getGridScale(lua_State *L)
{
    lua_pushnumber(L, GridMovement::getScale());
    return 1;
}

int AgentLuaInterface::l_initializeGrid(lua_State *L)
{
    GridMovement::initGrid(lua_tonumber(L, -1));

    return 0;
}

//Shared values.

int AgentLuaInterface::l_addSharedNumber(lua_State *L)
{
    std::string key = lua_tostring(L, -2);
    double value = lua_tonumber(L, -1);

    Shared::addNumber(key, value);

    return 0;
}

int AgentLuaInterface::l_getSharedNumber(lua_State *L)
{
    std::string key = lua_tostring(L, -1);
    //Output::Inst()->kprintf(key.c_str());

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

int AgentLuaInterface::l_addSharedString(lua_State *L)
{
    std::string key = lua_tostring(L, -2);
    std::string value = lua_tostring(L, -1);

    Shared::addString(key, value);

    return 0;
}

int AgentLuaInterface::l_getSharedString(lua_State *L)
{
    std::string key = lua_tostring(L, -1);
    std::string value = Shared::getString(key);

    lua_pushstring(L, value.c_str());

    return 1;

}

int AgentLuaInterface::l_getSharedTcpInput(lua_State *L)
{
    std::string key = lua_tostring(L, -1);
    std::vector<std::string> tmp = Shared::getTcpInputToAgent(key);

    std::cout << "test 1: " << key << std::endl;

    lua_createtable(L, tmp.size(), 0);
    int newTable = lua_gettop(L);
    int index = 1;

    for(auto& it : tmp)
    {
        lua_pushstring(L, (it).c_str());
        lua_rawseti(L, newTable, index);
        ++index;
    }

    return 1;
}

//Simulation core.

int AgentLuaInterface::l_getAgentPath(lua_State *L)
{
    lua_pushstring(L,Output::AgentPath.c_str());
    lua_pushstring(L,Output::AgentFile.c_str());

    return 2;
}

int AgentLuaInterface::l_addAgent(lua_State *L)
{
    double posX = lua_tonumber(L, -5);
    double posY = lua_tonumber(L, -4);
    double posZ = lua_tonumber(L, -3);
    std::string path = lua_tostring(L, -2);
    std::string filename = lua_tostring(L, -1);

    int id = Interfacer::addLuaAgent(posX, posY, posZ, path, filename);

    lua_pushinteger(L, id);

    return 1;
}

int AgentLuaInterface::l_removeAgent(lua_State *L)
{

    int id = lua_tonumber(L, -1);
    bool removed = Interfacer::removeAgent(id);
    lua_pushboolean(L, removed);

    return 1;
}

int AgentLuaInterface::l_changeAgentColor(lua_State *L)
{
    int id = lua_tointeger(L, -5);
    int r = lua_tointeger(L, -4);
    int g = lua_tointeger(L, -3);
    int b = lua_tointeger(L, -2);
    int alpha = lua_tointeger(L, -1);
    bool success = true;

    if (r < 0 || r > 255 || g < 0 || g > 255 ||
            b < 0 || b > 255 || alpha < 0 || alpha > 255 )
    {
        success = false;
    } else
    {
        //Output::Inst()->changeGraphicAgentColor(id, r, g, b, alpha);
    }

    lua_pushboolean(L, success);


    return 1;
}

int AgentLuaInterface::l_emitEvent(lua_State *L)
{
    std::unique_ptr<EventQueue::eEvent>
            sendEvent(new EventQueue::eEvent());

	sendEvent->originID = lua_tonumber(L, -9);
	sendEvent->posX	= lua_tonumber(L, -8);
	sendEvent->posY = lua_tonumber(L, -7);
	sendEvent->propagationSpeed = lua_tonumber(L,-6);
    sendEvent->activationTime = Phys::getCTime()+1;
    sendEvent->id = ID::generateEventID();
	sendEvent->desc = lua_tostring(L, -5);
	sendEvent->targetID = lua_tonumber(L, -4);
	sendEvent->targetGroup = lua_tonumber(L, -3);
	sendEvent->luatable = lua_tostring(L, -2);
    sendEvent->posZ = lua_tonumber(L, -1);

    //Output::Inst()->kprintf("%f", sendEvent->propagationSpeed);

    Interfacer::submitEEvent(std::move(sendEvent));
    return 0;
}

int AgentLuaInterface::l_stopSimulation(lua_State *L)
{
    Output::RunSimulation.store(false);
    return 0;
}

int AgentLuaInterface::l_addGroup(lua_State *L)
{
    int id = lua_tonumber(L, -1);
    int group = lua_tonumber(L, -2);

    bool success = false;

    auto autonPtr = Interfacer::getAgentPtr(id);
    if (autonPtr != NULL)
    {
        autonPtr->addGroup(group);
        success = true;
    }

    lua_pushboolean(L,success);

    return 0;
}

int AgentLuaInterface::l_removeGroup(lua_State *L)
{
    int id = lua_tonumber(L, -2);
    int group = lua_tonumber(L, -1);
    bool removed = false;

    auto autonPtr = Interfacer::getAgentPtr(id);
    if (autonPtr != NULL)
    {
        removed = autonPtr->removeGroup(group);
    }

    lua_pushboolean(L, removed);
    return 1;
}

int AgentLuaInterface::l_setMacroFactorMultipler(lua_State *L)
{
    int id = lua_tonumber(L, -2);
    int macroFactorMultiple = lua_tonumber(L, -1);

    auto autonPtr = Interfacer::getAgentPtr(id);

    if(autonPtr != NULL)
        autonPtr->setMacroFactorMultipler(macroFactorMultiple);

    return 0;

}

//Emergency brake -- do not pull --!
int AgentLuaInterface::luapanic(lua_State *L)
{
    std::string str = lua_tostring(L, 1);
    Output::Inst()->kprintf("<b>PANIC,%s</b></>", str.c_str());
    Output::KillSimulation.store(true);

    return 0;
}



