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
#ifndef AUTONLUA_H
#define AUTONLUA_H

#include <random>
#include <vector>

#include "lua.hpp"
#include "lauxlib.h"
#include "lualib.h"

#include "auton.h"


class Nestene;
class AutonLUA : public Auton
{
	public:
		AutonLUA(int ID, double posX, double posY, double posZ,	Nestene *nestene, std::string filename);
		~AutonLUA();

		bool operator==(AutonLUA &other) const;
		bool operator!=(AutonLUA &other) const;

		/*
		 * General LUA wrapper functions
		 */
		static int l_debug(lua_State *L);
		static int l_registerIEvent(lua_State *L);
		static int l_registerEEvent(lua_State *L);
		static int l_generateEventID(lua_State *L);
		
		/**
         * Defining the LUA wrapper functions.
         * This will enable the LUA auton to call physics function phys.h, to
		 * calculate speed of sound etc.
		 */
		static int l_speedOfSound(lua_State *L);
		static int l_currentTime(lua_State *L);
		static int l_distance(lua_State *L);
		static int l_getMacroFactor(lua_State *L);
		static int l_getTimeResolution(lua_State *L);
		static int l_getMersenneFloat(lua_State *L);
		static int l_getMersenneInteger(lua_State *L);
		static int l_getEnvironmentSize(lua_State *L);	
        static int l_modifyMap(lua_State *L);
        static int l_checkMap(lua_State *L);
        static int l_checkPosition(lua_State *L);
        static int l_updatePosition(lua_State *L);
        static int l_checkCollision(lua_State *L);
        static int l_scanRadial(lua_State *L);
        static int l_gridMove(lua_State *L);
        static int l_stopSimulation(lua_State *L);        
        static int l_getSharedNumber(lua_State *L);
        static int l_addSharedNumber(lua_State *L);

private:
			//function to receive an event from nestene responsible for this auton, returns an internal Event 'thinking':
			EventQueue::iEvent* handleEvent(EventQueue::eEvent* event);
			EventQueue::eEvent* actOnEvent(EventQueue::iEvent *event);
			//returns an event:
			EventQueue::eEvent* initEvent();
			void simDone();
			double eventChance();
			std::string filename;
			//The LUA state:
			lua_State* L;
			friend class Nestene;
            bool nofile = false;
            void getSyncData();
};

#endif // AUTONLUA_H
