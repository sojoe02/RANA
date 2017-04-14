
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

#include "output.h"

#include "ID.h"
#include "agentinterface.h"
#include "../interfacer.h"
#include "../../api/phys.h"
#include "../../api/gridmovement.h"
#include "../../api/maphandler.h"
#include "../../api/shared.h"
#include "../../api/scanning.h"

AgentInterface::AgentInterface(int ID, double posX, double posY, double posZ, Sector *sector, std::string filename)
:Agent(ID, posX, posY, posZ, sector), destinationX(posX), destinationY(posY), speed(1),
 	 	 moving(false), gridmove(false), filename(filename), nofile(false), removed(false)
{
	desc = "CPP";
	Output::Inst()->addGraphicAgent(ID, -1,-1);

	moveFactor = Phys::getMacroFactor() * Phys::getTimeRes();
}

AgentInterface::~AgentInterface()
{
	if(initiated)
	{
		//TODO: Delete everything
	}
}

void AgentInterface::InitializeAgent()
{
	//	TODO

	if(removed) return;
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
	//TODO

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
std::unique_ptr<EventQueue::eEvent> AgentInterface::takeStep()
{
	if(removed) return NULL;
	if(nofile) return NULL;

	//	TODO

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
	//	TODO

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

int AgentInterface::generateEventID()
{
	unsigned long long id = ID::generateEventID();
	return id;
}

//	Physics
int AgentInterface::speedOfSound()
{
	return Phys::speedOfEvent(posX, posY, origX, origY, propagationSpeed);
}

int AgentInterface::distance()
{
	return Phys::calcDistance(origX, origY, posX, posY);
}

int AgentInterface::currentTime()
{
	return Phys::getCTime();
}

int AgentInterface::currentTimeS()
{
	return (double)Phys::getCTime()*Phys::getTimeRes();
}

int AgentInterface::getMacroFactor()
{
	return Phys::getMacroFactor();
}

int AgentInterface::getTimeResolution()
{
    return Phys::getTimeRes();
}

int AgentInterface::getMersenneFloat()
{
    double low = lua_tonumber(L,-2);		//TODO
    double high = lua_tonumber(L, -1);		//TODO

    return Phys::getMersenneFloat(low,high);
}

int AgentInterface::getMersenneInteger()
{
    int64_t low = lua_tonumber(L,-2);		//TODO
    int64_t high = lua_tonumber(L, -1);		//TODO
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

int AgentInterface::modifyMap()
{
	double x = 1;	//TODO
	double y = 1; 	//TODO

	int modX = x * Phys::getScale();
	int modY = y * Phys::getScale();

	rgba color;
	color.red = 1;
	color.green = 1;
	color.blue = 1;
	color.alpha = 0;

	return MapHandler::setPixelInfo(modX, modY, color);
}

rgba AgentInterface::checkMap()
{
	double x = 1;
	double y = 1;

	int checkX = x * Phys::getScale();
	int checkY = y * Phys::getScale();

	rgba color = MapHandler::getPixelInfo(checkX, checkY);

	return color;
}

int AgentInterface::checkMapAndChange()
{
	double x = 1;
	double y = 1;

	int modX = x * Phys::getScale();
	int modY = y * Phys::getScale();

	rgba check_color;
	check_color.red = 1;
	check_color.green = 1;
	check_color.blue = 1;
	check_color.alpha = 0;

	rgba change_color;
	change_color.red = 1;
	change_color.green = 1;
	change_color.blue = 1;
	change_color.alpha = 0;

	return MapHandler::checkAndChange(modX, modY, check_color, change_color);
}

int AgentInterface::radialMapColorScan()
{
	int radius = 1;
	int posX = 1 - radius;
	int posY = 1 - radius;
	int r = 1;
	int g = 1;
	int b = 1;

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

int AgentInterface::radialMapScan()
{
	int radius = 1;
	int posX = 1 - radius;
	int posY = 1 - radius;

	MatriceInt result = Scanning::radialMask(radius);
	int index = 0;

	for (int i = 1; i < radius*2; i++)
	{
		for (int j = 1; j < radius*2; j++)
		{
			if(result[i][j] == 1)
			{
				index++;

				rgba color = MapHandler::getPixelInfo(posX+i, posY+j);
			}
		}
	}
	return 1;
}

void AgentInterface::addPosition()
{

    int x = 1;
    int y = 1;
    int id = 1;
    GridMovement::addPos(x, y, id);
}

int AgentInterface::checkPosition()
{
    int posX = 0;
    int posY = 0;

    posX = 1 * GridMovement::getScale()+.5;
    posY = 1 * GridMovement::getScale()+.5;

    pList agentList = GridMovement::checkPosition(posX, posY);

    int i = 1;
    for(pList::iterator it = agentList.begin(); it != agentList.end(); ++it,i++)
    {
    	//TODO
        lua_pushnumber(L, i);
        lua_pushnumber(L, *it);
        lua_settable(L, -3);
    }

    return 1;
}

int AgentInterface::updatePosition()
{
	int oldX = 1;
	int oldY = 1;
	int newX = 1;
	int newY = 1;
	int id = 1;

	if(oldX != newX || oldY != newY)
	{
		GridMovement::updatePos(oldX, oldY, newX, newY, id);
		return 1;
	}

	return 0;
}

int AgentInterface::checkCollision()
{
    int posX = 1;
    int posY = 1;

    return GridMovement::checkCollision(posX, posY);
}

int AgentInterface::checkCollisionRadial()
{
    int radius = 1;
    int posX = 1 - radius;
    int posY = 1 - radius;

    MatriceInt result = Scanning::radialMask(radius);

    bool collision = false;

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

int AgentInterface::getMaskRadial()
{
    int radius = 1;
    int posX = 1 - radius;
    int posY = 1 - radius;

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
                lua_pushnumber(L, index);
                lua_newtable(L);
                lua_pushstring(L,"posX");
                lua_pushnumber(L, posX + i);
                lua_settable(L, -3);
                lua_pushstring(L,"posY");
                lua_pushnumber(L, posY + j);
                lua_settable(L, -3);
                lua_settable(L, -3);
            }
        }
    }
    return 1;
}

void AgentInterface::gridMove()
{
    int oldX = 1;
    int oldY = 1;
    int newX = 1;
    int newY = 1;
}

int AgentInterface::getGridScale()
{
    return GridMovement::getScale();
}

void AgentInterface::initializeGrid()
{
    GridMovement::initGrid(1);
}

int AgentInterface::radialCollisionScan()
{
    int radius = 1;
    int posX = 1 - radius;
    int posY = 1 - radius;
    int id = 1;

    MatriceInt result = Scanning::radialMask(radius);
    bool collision = false;
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

                            //	TODO
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

    return 1;
}

int AgentInterface::updatePositionIfFree()
{
    int oldX = int(1*GridMovement::getScale());
    int oldY = int(1*GridMovement::getScale());
    int newX = int(1*GridMovement::getScale());
    int newY = int(1*GridMovement::getScale());
    int id = 1;

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
    	return "no_value";
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
	std::vector<std::string> tmp;
	tmp.push_back(Output::AgentPath.c_str());
	tmp.push_back(Output::AgentFile.c_str());

    return tmp;
}

int AgentInterface::addAgent()
{
    double posX = 1;
    double posY = 1;
    double posZ = 1;
    std::string path = "1";
    std::string filename = "1";

    return Interfacer::addLuaAgent(posX, posY, posZ, path, filename);
}

int AgentInterface::removeAgent()
{
    return Interfacer::removeAgent(1);
}

//	Agents
void AgentInterface::emitEvent()
{
	std::unique_ptr<EventQueue::eEvent> sendEvent(new EventQueue::eEvent());

	sendEvent->originID = 1;
	sendEvent->posX	= 1;
	sendEvent->posY = 1;
	sendEvent->propagationSpeed = 1;
	sendEvent->activationTime = Phys::getCTime()+1;
	sendEvent->id = ID::generateEventID();
	sendEvent->desc = 1;
	sendEvent->targetID = 1;
	sendEvent->targetGroup = 1;
	sendEvent->luatable = 1;
	sendEvent->posZ = 1;

	Interfacer::submitEEvent(std::move(sendEvent));
}

int AgentInterface::addGroup()
{
    int id = 1;
    int group = 1;

    auto autonPtr = Interfacer::getAgentPtr(id);
    if (autonPtr != NULL)
    {
        autonPtr->addGroup(group);
        return true;
    }

    return false;
}

int AgentInterface::removeGroup()
{
    int id = 1;
    int group = 1;

    auto autonPtr = Interfacer::getAgentPtr(id);
    if (autonPtr != NULL)
    {
        removed = autonPtr->removeGroup(group);
        return true;
    }

    return false;
}

void AgentInterface::setMacroFactorMultipler()
{
    int id = 1;
    int macroFactorMultiple = 1;

    auto autonPtr = Interfacer::getAgentPtr(id);

    if(autonPtr != NULL)
    {
    	autonPtr->setMacroFactorMultipler(macroFactorMultiple);
    }
}

int AgentInterface::changeAgentColor()
{
    int id = 1;
    int r = 1;
    int g = 1;
    int b = 1;
    int alpha = 1;

    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || alpha < 0 || alpha > 255 )
    {
        return false;
    }
    else
    {
        Output::Inst()->changeGraphicAgentColor(id, r, g, b, alpha);
    }
    return true;

}

//	Panic
void AgentInterface::panic(std::string string)
{
    Output::Inst()->kprintf("<b>PANIC,%s</b></>", string.c_str());
    Output::KillSimulation.store(true);
}

//	TODO:	Find name for these functions below:

/********************************************************
 * Core utility.
 *
 ********************************************************/

void AgentInterface::setRemoved()
{
    //Output::Inst()->kprintf("removing agent.#.%i",ID);
    removed = true;
    GridMovement::removePos(ID);
}

void AgentInterface::simDone()
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

void AgentInterface::getSyncData()
{
    if(removed) return;
    try
	{
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






