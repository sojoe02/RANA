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

#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include <memory>
#include <unordered_set>

#include "simulator/utility/utility.h"
#include "simulator/event/eventqueue.h"
//#include "simulator/core/sector.h"

class Sector;

class Agent
{
public:
    Agent(int ID, double posX, double posY, double posZ, Sector *sector);

    virtual ~Agent()
    {}

    virtual std::unique_ptr<EventQueue::iEvent> processEvent(EventQueue::eEvent *event);

    virtual std::unique_ptr<EventQueue::eEvent> handleEvent(std::unique_ptr<EventQueue::iEvent> event);

    virtual std::unique_ptr<EventQueue::eEvent> takeStep()
    { return NULL; }

    virtual void processFunction(EventQueue::dataEvent *devent, double mapRes,
                                 double x, double y, double &zvalue, double &duration)
    {}

    std::string getDesc();

    int getID();

    double getPosX();

    double getPosY();

    void setPositions(double x, double y, double z)
    {
        posX = x;
        posY = y;
        posZ = z;
    }

    bool removeGroup(int group);

    void addGroup(int group);

    void simDone()
    {}

    bool operator==(Agent &other) const;

    bool operator!=(Agent &other) const;

    bool checkGroup(int group);

    int getMacroFactorMultipler();

    void setMacroFactorMultipler(int multipler);

    void setColor(int r, int g, int b, int a = 255);

    //rgba getColor(){return color;}
    agentInfo getAgentInfo();

    double getPosZ();

protected:

    void distroEEvent(std::unique_ptr<EventQueue::eEvent> event);

    int ID;
    int macroFactorMultiple; //how many macrostep pr. macrostep(if 0 the agent will be ignored completely).
    std::unordered_set<int> groups;
    std::string desc;
    double posX, posY, posZ;
    std::vector<double> statusVector;
    Sector *sector;
    rgba color;

    std::mutex mutex;

    double radius;
    double mass;
    double charge;

    double angle;
};

#endif // AGENT_H
