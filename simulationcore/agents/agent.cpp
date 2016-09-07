
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
#include <memory>
#include "agent.h"
#include "../sector.h"

    Agent::Agent(int ID, double posX, double posY, double posZ, Sector *sector)
:ID(ID), macroFactorMultiple(1), posX(posX), posY(posY), posZ(posZ), sector(sector)
{

}

int Agent::getID(){
	return ID;
}

std::string Agent::getDesc(){
	return desc;
}

double Agent::getPosX(){
	return posX;
}

double Agent::getPosY(){
	return posY;
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



