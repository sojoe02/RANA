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

//#include "lua.hpp"
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
#include "src/simulationcore/agents/agentinterface.h"

AgentInterface::AgentInterface(int ID, double posX, double posY, double posZ, Sector *sector, std::string filename)
    :Agent(ID,posX,posY,posZ,sector),destinationX(posX),destinationY(posY),speed(1),moving(false),gridmove(false),
    filename(filename),nofile(false),removed(false)
{
	desc = "CPP";
    color.red=255;
    color.green=255;
    color.blue=0;
    Output::Inst()->addGraphicAgent(ID,-1,-1,color,0);

	moveFactor = Phys::getMacroFactor() * Phys::getTimeRes();
}

AgentInterface::~AgentInterface()
{
	if(initiated)
	{
        //  TODO: detele everything
	}
}

void AgentInterface::InitializeAgent()
{
	//	TODO
	if(removed) return;

    if(gridmove == true)
    {
        GridMovement::addPos(int(posX*GridMovement::getScale()),int(posY*GridMovement::getScale()),ID);
    }
    getSyncData();
}

/********************************************************
 * Simulation flow functions:
 ********************************************************/

/**
 * Handler for external events.
 * Will send all relevant event data to the LUA script which will then
 * process the event and either return a null string, or arguments
 * to initiate an internal event.
 * @param event pointer to the external event.
 * @return internal event.
 */
std::unique_ptr<EventQueue::iEvent> AgentInterface::processEvent(const EventQueue::eEvent *event)
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
            ievent->activationTime = Phys::speedOfEvent(event->posX, event->posY, posX, posY, event->propagationSpeed)+1;
        }

        ievent->id = ID::generateEventID();
        ievent->desc = "";
        ievent->originID = ID;

        return ievent;
    }
    else
    {
        return NULL;
    }
}

/**
 * Query if the auton will initiate an event.
 * This will call up the LUA autons initEvent function which will
 * either return a 'null' string or data to build an
 * external event.
 * @return EventQueue::eEvent pointer to an external event or
 * a null pointer in which case nothing happens.
 */
std::unique_ptr<EventQueue::eEvent> AgentInterface::takeStep()
{
	if(removed) return NULL;
	if(nofile) return NULL;

    try
    {
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
std::unique_ptr<EventQueue::eEvent> AgentInterface::handleEvent(std::unique_ptr<EventQueue::iEvent> eventPtr)
{
    if(removed) return NULL;
    if(nofile) return NULL;

    try
    {
        posX = eventPtr->event->posX;
        posY = eventPtr->event->posY;
        posZ = eventPtr->event->posZ;
    }
    catch(std::exception &e)
    {
        Output::Inst()->kprintf("<b><font color=\"red\">Exception on event handling.%s, %s</font></b></>",filename.c_str(), e.what());
        Output::RunSimulation = false;
    }

	return NULL;
}

/********************************************************
 * RANA agent API functions:
 ********************************************************/

//	Interface
void AgentInterface::debug( std::string string )
{
	string.resize(4096);
	Output::Inst()->kdebug( string.c_str() );
}

void AgentInterface::print( std::string string )
{
	string.resize(4096);
	Output::Inst()->kdebug( string.c_str() );
}

unsigned long long AgentInterface::generateEventID()
{
	unsigned long long id = ID::generateEventID();
	return id;
}

//	Physics
unsigned long long AgentInterface::speedOfSound(double x_origin, double y_origin, double x_dest, double y_dest, double propagationSpeed)
{
    return Phys::speedOfEvent(x_origin, y_origin, x_dest, y_dest, propagationSpeed);
}

double AgentInterface::distance(double x_origin, double y_origin, double x_dest, double y_dest)
{
    return Phys::calcDistance(x_origin, y_origin, x_dest, y_dest);
}

unsigned long long AgentInterface::currentTime()
{
	return Phys::getCTime();
}

double AgentInterface::currentTimeS()
{
	return (double)Phys::getCTime()*Phys::getTimeRes();
}

int AgentInterface::getMacroFactor()
{
	return Phys::getMacroFactor();
}

double AgentInterface::getTimeResolution()
{
    return Phys::getTimeRes();
}

double AgentInterface::getMersenneFloat( double low, double high )
{
    return Phys::getMersenneFloat(low,high);
}

int64_t AgentInterface::getMersenneInteger( int64_t low, int64_t high )
{
    int64_t number = low;

    if(low > high)
    {
        number = Phys::getMersenneInteger(high, low);
    }
    else if(high > low)
    {
        number = Phys::getMersenneInteger(low, high);
    }

    return number;
}

//	Map and position
std::vector<int> AgentInterface::getEnvironmentSize()
{
	std::vector<int> tmp;
	tmp.push_back(Phys::getEnvX());
	tmp.push_back(Phys::getEnvY());

	return tmp;
}

bool AgentInterface::modifyMap( double x, double y, int r, int g, int b )
{
	int modX = x * Phys::getScale();
	int modY = y * Phys::getScale();

	rgba color;
    color.red = r;
    color.green = g;
    color.blue = b;
	color.alpha = 0;

	return MapHandler::setPixelInfo(modX, modY, color);
}

rgba AgentInterface::checkMap( double x, double y )
{
	int checkX = x * Phys::getScale();
	int checkY = y * Phys::getScale();

	rgba color = MapHandler::getPixelInfo(checkX, checkY);

	return color;
}

bool AgentInterface::checkMapAndChange(double x, double y, int r1, int g1, int b1, int r2, int b2, int g2)
{
	int modX = x * Phys::getScale();
	int modY = y * Phys::getScale();

	rgba check_color;
    check_color.red = r1;
    check_color.green = g1;
    check_color.blue = b1;
	check_color.alpha = 0;

	rgba change_color;
    change_color.red = r2;
    change_color.green = g2;
    change_color.blue = b2;
	change_color.alpha = 0;

	return MapHandler::checkAndChange(modX, modY, check_color, change_color);
}

int AgentInterface::radialMapColorScan(int radius, int x, int y, int r, int g, int b)
{
    //  TODO
    int posX = x - radius;
    int posY = y - radius;

	MatriceInt result = Scanning::radialMask(radius);
	int count = 0;
	int index = 0;

	for (int i = 1; i < radius*2; i++)
	{
		for (int j = 1; j < radius*2; j++)
		{
			if(result[i][j] == 1)
			{
				rgba color = MapHandler::getPixelInfo(posX+i, posY+j);

				if(r == color.red && g == color.green && b == color.blue)
				{
					count++;
					index++;
				}
			}
		}
	}

	if(count == 0)
	{
		return 0;
	}
	return 1;
}

int AgentInterface::radialMapScan(int radius, int x, int y)
{
    //  TODO
    int posX = x - radius;
    int posY = y - radius;

	MatriceInt result = Scanning::radialMask(radius);
	int index = 0;

	for (int i = 1; i < radius*2; i++)
	{
		for (int j = 1; j < radius*2; j++)
		{
			if(result[i][j] == 1)
			{
				index++;

                //rgba color = MapHandler::getPixelInfo(posX+i, posY+j);
			}
		}
	}
	return 1;
}

void AgentInterface::addPosition(int x, int y, int id)
{
    GridMovement::addPos(x, y, id);
}

std::list<int> AgentInterface::checkPosition(int x, int y)
{
    posX = x * GridMovement::getScale() + 0.5;
    posY = y * GridMovement::getScale() + 0.5;

    return GridMovement::checkPosition(posX, posY);
}

void AgentInterface::updatePosition(int oldX, int oldY, int newX, int newY, int id)
{
	if(oldX != newX || oldY != newY)
	{
		GridMovement::updatePos(oldX, oldY, newX, newY, id);
	}
}

bool AgentInterface::checkCollision(int x, int y)
{
    return GridMovement::checkCollision(posX, posY);
}

bool AgentInterface::checkCollisionRadial(int radius, int x, int y)
{
    int posX = x - radius;
    int posY = y - radius;

    MatriceInt result = Scanning::radialMask(radius);

    for (int i = 1; i < radius*2; i++)
    {
        for(int j = 1; j < radius*2; j++)
        {
            if( result[i][j] == 1 && (posX+i != posX+radius || posY+j != posY+radius ))
            {
                if(GridMovement::checkCollision(posX+i, posY+j))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

int AgentInterface::getMaskRadial(int radius, int x, int y)
{
    //  TODO
    int posX = x - radius;
    int posY = y - radius;

    MatriceInt result = Scanning::radialMask(radius);
    int index = 0;

    for (int i = 1; i < radius*2; i++)
    {
        for(int j = 1; j < radius*2; j++)
        {
            if( result[i][j] == 1)
            {
                index++;

                //TODO
                /*lua_pushnumber(L, index);
                lua_newtable(L);
                lua_pushstring(L,"posX");
                lua_pushnumber(L, posX + i);
                lua_settable(L, -3);
                lua_pushstring(L,"posY");
                lua_pushnumber(L, posY + j);
                lua_settable(L, -3);
                lua_settable(L, -3);*/
            }
        }
    }
    return 1;
}

void AgentInterface::gridMove(int oldX, int oldY, int newX, int newY)
{

}

double AgentInterface::getGridScale()
{
    return GridMovement::getScale();
}

void AgentInterface::initializeGrid()
{
    GridMovement::initGrid(1);
}

int AgentInterface::radialCollisionScan(int radius, int x, int y, int id)
{
    //  TODO
    int posX = x - radius;
    int posY = y - radius;

    MatriceInt result = Scanning::radialMask(radius);
    int counter = 0;

    for (int i = 1; i < radius*2; i++)
    {
        for(int j = 1; j < radius*2; j++)
        {
            if( result[i][j] == 1)
            {
                if (GridMovement::checkCollision(posX+i, posY+j))
                {
                    pList agentList = GridMovement::checkPosition(posX+i, posY+j);

                    for(auto it = agentList.begin(); it != agentList.end(); ++it)
                    {
                        if (id != *it)
                        {
                            counter++;

                            /*lua_pushnumber(L, counter);
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
                            lua_settable(L, -3);*/
                        }
                    }

                }
            }
        }
    }

    return 1;
}

bool AgentInterface::updatePositionIfFree(int x1, int y1, int x2, int y2, int id)
{
    //  TODO: Kunne man ikke lade være med at parse alle 5 argumenter, og bare bruge 2 af dem der ligger i klassen?
    int oldX = int(x1*GridMovement::getScale());
    int oldY = int(y1*GridMovement::getScale());
    int newX = int(x2*GridMovement::getScale());
    int newY = int(y2*GridMovement::getScale());

    if(oldX != newX || oldY != newY)
    {
        GridMovement::updateIfFree(oldX, oldY, newX, newY, id);
        return true;
    }

    return false;
}

//	Shared values
std::string AgentInterface::getSharedNumber(std::string key)
{
    double value = Shared::getNumber(key);

    if (value == LLONG_MIN)
    {
        //  TODO: not sure if correct
        return std::string("no_value");
    }

    return std::to_string(value);
}

void AgentInterface::addSharedNumber(std::string key, double value)
{
    Shared::addNumber(key, value);
}

std::string AgentInterface::getSharedString(std::string key)
{
    return Shared::getString(key);
}

void AgentInterface::addSharedString(std::string key, double value)
{
    Shared::addString(key, std::to_string(value));
}

//	Simulation core
void AgentInterface::stopSimulation()
{
    Output::RunSimulation.store(false);
}

std::vector<std::string> AgentInterface::getAgentPath()
{
    //  TODO:   Seems like one of those below, aren't used.
	std::vector<std::string> tmp;
	tmp.push_back(Output::AgentPath.c_str());
	tmp.push_back(Output::AgentFile.c_str());

    return tmp;
}

int AgentInterface::addAgent(int posX, int posY, int posZ, std::string path, std::string filename)
{
    return Interfacer::addLuaAgent(posX, posY, posZ, path, filename);
}

bool AgentInterface::removeAgent(int id)
{
    return Interfacer::removeAgent(id);
}

//	Agents
void AgentInterface::emitEvent(int id, int posX, int posY, double speed, std::string tableString, std::string desc, int targetID, int targetGroup)
{
    //  TODO
    std::unique_ptr<EventQueue::eEvent> sendEvent(new EventQueue::eEvent());

    sendEvent->originID = id; //Check if true
    sendEvent->posX	= posX;
    sendEvent->posY = posY;
    sendEvent->propagationSpeed = speed;
	sendEvent->activationTime = Phys::getCTime()+1;
	sendEvent->id = ID::generateEventID();
    sendEvent->desc = desc;
    sendEvent->targetID = targetID;
    sendEvent->targetGroup = targetGroup;
	sendEvent->luatable = 1;
	sendEvent->posZ = 1;

	Interfacer::submitEEvent(std::move(sendEvent));
}

bool AgentInterface::addGroup(int groupID, int id)
{
    auto autonPtr = Interfacer::getAgentPtr(id);
    if (autonPtr != NULL)
    {
        autonPtr->addGroup(groupID);
        return true;
    }

    return false;
}

bool AgentInterface::removeGroup(int groupID, int id)
{
    auto autonPtr = Interfacer::getAgentPtr(id);
    if (autonPtr != NULL)
    {
        removed = autonPtr->removeGroup(groupID);
        return true;
    }

    return false;
}

void AgentInterface::setMacroFactorMultipler(int id, int macroFactorMultiple)
{
    auto autonPtr = Interfacer::getAgentPtr(id);

    if(autonPtr != NULL)
    {
    	autonPtr->setMacroFactorMultipler(macroFactorMultiple);
    }
}

bool AgentInterface::changeAgentColor(int id, int r, int g, int b, int alpha)
{
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || alpha < 0 || alpha > 255 )
    {
        return false;
    }
    //Output::Inst()->changeGraphicAgentColor(id, r, g, b, alpha);
    return true;
}

//	Panic
void AgentInterface::panic(std::string string)
{
    Output::Inst()->kprintf("<b>PANIC,%s</b></>", string.c_str());
    Output::KillSimulation.store(true);
}

//  Post processing
void AgentInterface::processFunction(EventQueue::dataEvent *devent, double mapRes, double x, double y, double &zvalue, double &duration)
{
    //  TODO
    if(removed) return;

    try
    {
        /*
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
            Output::Inst()->kprintf("Error on calling _ProcessEventFunction : %s\n,",
                                    lua_tostring(L,-1));
            Output::RunEventProcessing.store(false);
            return;
        } else
        {
            zvalue = lua_tonumber(L,-2);
            duration = lua_tonumber(L,-1);
        }
        */
    }
    catch(std::exception &e)
    {
        Output::Inst()->kprintf("<b><font color=\"red\">Error on processEvent..%s</font></b></>", e.what());
        Output::RunEventProcessing.store(false);
    }
}

//  Core utility.
void AgentInterface::setRemoved()
{
    //Output::Inst()->kprintf("removing agent.#.%i",ID);
    removed = true;
    GridMovement::removePos(ID);
}

void AgentInterface::simDone()
{
    if(nofile) return;
    /*
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
    */
}

void AgentInterface::getSyncData()
{
    //  TODO:
    if(removed) return;
    try
	{
        /*
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
        */
    }
    catch(std::exception &e)
    {
        Output::Inst()->kprintf("<b><font color=\"red\">Exception on retrieving sync data, %s</font></b></>", e.what());
        Output::RunSimulation.store(false);
        return;
    }
}

void AgentInterface::movement()
{
    //bool collision = false;

    if(posX != destinationX || posY != destinationY)
    {
        double angle = std::atan2(destinationX-posX, destinationY-posY);
        double vY = speed * std::cos(angle);
        double vX = speed * std::sin(angle);

        double newPosX = posX + moveFactor * vX * macroFactorMultiple;
        double newPosY = posY + moveFactor * vY * macroFactorMultiple;

        //Check if the agent overshoots it's target destinations.
        if( (posX >= destinationX && newPosX <= destinationX && posY >= destinationY && newPosY <= destinationY) ||
            (posX <= destinationX && newPosX >= destinationX && posY >= destinationY && newPosY <= destinationY) ||
            (posX >= destinationX && newPosX <= destinationX && posY <= destinationY && newPosY >= destinationY) ||
            (posX <= destinationX && newPosX >= destinationX && posY <= destinationY && newPosY >= destinationY) )
        {
            moving = false;
            newPosX = destinationX;
            newPosY = destinationY;
        }

        if(gridmove)
        {
            if( int(posX*GridMovement::getScale())	!=  int(newPosX*GridMovement::getScale()) ||
                int(posY*GridMovement::getScale())  !=  int(newPosY*GridMovement::getScale()) )
            {
                GridMovement::updatePos(
                    int(posX*GridMovement::getScale()),     int(posY*GridMovement::getScale()),
                    int(newPosX*GridMovement::getScale()),  int(newPosY*GridMovement::getScale()),
                    ID);
            }
        }

        posX = newPosX;
        posY = newPosY;
    }
    else
    {
        moving = false;
    }
}






