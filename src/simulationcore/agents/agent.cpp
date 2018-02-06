
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
#include <algorithm>
#include <memory>

#include "src/simulationcore/sector.h"
#include "src/simulationcore/agents/agent.h"

Agent::Agent(int ID, double posX, double posY, double posZ, Sector *sector, std::string groupID)
:ID(ID), macroFactorMultiple(1), posX(posX), posY(posY), posZ(posZ), sector(sector),
  radius(0), mass(0), charge(0),angle(0)
{
    if (groupID != "false") {
        this->initAddAgentToGroups(groupID);
    }
}

bool Agent::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while(it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void Agent::initAddAgentToGroups(std::string groupID){
    std::string s = groupID;
    if (s.find("{[") != std::string::npos){
        std::string delimiter = ",";
        std::string token;
        while( token != s ){
            token = s.substr(0,s.find_first_of(delimiter));
            s.erase(0, s.find_first_of(delimiter) + 1);
            std::string s2id = token;
            s2id.erase(0,s2id.find_first_of("=]") + 2);
            if(!std::isdigit(s2id.back())) s2id.pop_back();
            addGroup(std::stoi(s2id));
        }
    }else{
        if (is_number(groupID)){
            addGroup(std::stoi(groupID));
        }
    }
}


int Agent::getID(){
	return ID;
}

std::string Agent::getDesc()
{
	return desc;
}

double Agent::getPosX()
{
	return posX;
}

double Agent::getPosY()
{
	return posY;
}

double Agent::getPosZ()
{
	return posZ;
}

void Agent::setColor(int r, int g, int b, int a)
{
    std::lock_guard<std::mutex> guard(mutex);
	color.red = r;
	color.green = g;
	color.blue = b;
	color.alpha = a;
}

agentInfo Agent::getAgentInfo()
{
	agentInfo info;

	info.color = color;
	info.charge = charge;
	info.mass = mass;
	info.id = ID;
	info.radius = radius;

	info.x = posX;
	info.y = posY;
	info.z = posZ;

    info.angle = angle;

	return info;
}

bool Agent::removeGroup(int group)
{
	auto itr = groups.find(group);

	if(itr != groups.end())
	{
		groups.erase(itr);
		return true;
	}
	return false;
}

void Agent::addGroup(int group)
{
	groups.insert(group);
}

bool Agent::checkGroup(int group)
{
	auto itr = groups.find(group);

	if(itr != groups.end())
		return true;
	else return false;
}

std::vector<int> Agent::memberOfGroups()
{
    std::vector<int> list;

    for(auto i : groups){
        list.push_back(i);
    }

    return list;
}

int Agent::getMacroFactorMultipler()
{
    return macroFactorMultiple;
}

void Agent::setMacroFactorMultipler(int multipler)
{
    macroFactorMultiple = multipler;
}

void Agent::distroEEvent(std::unique_ptr<EventQueue::eEvent> event)
{
    //sector->eEventsOutbox.push_back(std::move(event));
}


std::unique_ptr<EventQueue::iEvent> Agent::processEvent(EventQueue::eEvent* event)
{
	return NULL;
}

std::unique_ptr<EventQueue::eEvent> Agent::handleEvent(std::unique_ptr<EventQueue::iEvent> eventPtr)
{
    return NULL;
}

bool Agent::operator==(Agent &other) const
{
	return (this->ID == other.getID());
}

bool Agent::operator!=(Agent &other) const
{
	return !(*this == other);
}



