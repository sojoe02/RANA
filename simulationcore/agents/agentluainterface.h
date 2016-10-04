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

#ifndef AGENTLUAINTERFACE_H
#define AGENTLUAINTERFACE_H

#include <random>
#include <vector>
#include <memory>

#include "lua.hpp"
#include "lauxlib.h"
#include "lualib.h"

#include "../sector.h"
#include "agent.h"

#define LUA_OK          0
#define LUA_YIELD       1
#define LUA_ERRRUN      2
#define LUA_ERRSYNTAX   3
#define LUA_ERRMEM      4
#define LUA_ERRGCMM     5

class Sector;
class AgentLuaInterface : public Agent
{
public:
    AgentLuaInterface(int ID, double posX, double posY, double posZ,	Sector *sector, std::string filename);
    ~AgentLuaInterface();

    bool operator==(AgentLuaInterface &other) const;
    bool operator!=(AgentLuaInterface &other) const;

    /*******************************************
    * Lua wrapper functions
    *
    ********************************************/

    //Interface.
    static int l_debug(lua_State *L);
    static int l_print(lua_State *L);
    static int l_generateEventID(lua_State *L);

    //Physics.
    static int l_speedOfSound(lua_State *L);
    static int l_distance(lua_State *L);
    static int l_currentTime(lua_State *L);
	static int l_currentTimeS(lua_State *L);
    static int l_getMacroFactor(lua_State *L);
    static int l_getTimeResolution(lua_State *L);
    static int l_getMersenneFloat(lua_State *L);
    static int l_getMersenneInteger(lua_State *L);

    //Map and movement.
    static int l_getEnvironmentSize(lua_State *L);
    static int l_modifyMap(lua_State *L);
    static int l_checkMap(lua_State *L);
    static int l_checkMapAndChange(lua_State *L);
    static int l_radialMapScan(lua_State *L);

    static int l_addPosition(lua_State *L);
    static int l_checkPosition(lua_State *L);
    static int l_updatePosition(lua_State *L);
    static int l_checkCollision(lua_State *L);
    static int l_checkCollisionRadial(lua_State *L);
    static int l_getMaskRadial(lua_State *L);
    static int l_gridMove(lua_State *L);
    static int l_getGridScale(lua_State *L);
    static int l_initializeGrid(lua_State *L);
	static int l_radialCollisionScan(lua_State *L);
    static int l_updatePositionIfFree(lua_State *L);

    //Shared values.
    static int l_getSharedNumber(lua_State *L);
    static int l_addSharedNumber(lua_State *L);
    static int l_addSharedString(lua_State *L);
    static int l_getSharedString(lua_State *L);

    //Simulation core.
    static int l_stopSimulation(lua_State *L);
    static int l_getAgentPath(lua_State *L);
    static int l_addAgent(lua_State *L);
    static int l_removeAgent(lua_State *L);

    //Agents.
    static int l_emitEvent(lua_State *L);
    static int l_addGroup(lua_State *L);
    static int l_removeGroup(lua_State *L);
    static int l_setMacroFactorMultipler(lua_State *L);

    static int luapanic(lua_State *L);

    void processFunction(EventQueue::dataEvent *devent, double mapRes,
                         double x, double y,
                         double &zvalue, double &duration);

	static int l_changeAgentColor(lua_State *L);

    void InitializeAgent();
   private:

    std::unique_ptr<EventQueue::iEvent>
    processEvent(const EventQueue::eEvent* event);

    std::unique_ptr<EventQueue::eEvent>
    handleEvent(std::unique_ptr<EventQueue::iEvent> eventPtr);

    std::unique_ptr<EventQueue::eEvent> takeStep();

    void movement();
    double destinationX;
    double destinationY;
    double speed; //meters pr second
    bool moving;
    bool gridmove;

    void setRemoved();
    void simDone();
    double eventChance();

    std::string filename;
    //The LUA state:
    friend class Sector;
    friend class Supervisor;
    bool nofile;
    bool removed;
    lua_State* L;

    void getSyncData();
};

#endif // AUTONLUAINTERFACE_H
