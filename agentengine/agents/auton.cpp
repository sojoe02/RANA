
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
#include "auton.h"
#include "nestene.h"

	Auton::Auton(int ID, double posX, double posY, double posZ, Nestene* nestene)
:ID(ID), macroFactorMultiple(1), posX(posX), posY(posY), posZ(posZ), nestene(nestene)
{

}

int Auton::getID(){
	return ID;
}

std::string Auton::getDesc(){
	return desc;
}

double Auton::getPosX(){
	return posX;
}

double Auton::getPosY(){
	return posY;
}

bool Auton::removeGroup(int group)
{
	auto itr = groups.find(group);

	if(itr != groups.end())
	{
		groups.erase(itr);
		return true;
	}
	return false;
}

void Auton::addGroup(int group)
{
	groups.insert(group);
}

bool Auton::checkGroup(int group)
{
	auto itr = groups.find(group);

	if(itr != groups.end())
		return true;
	else return false;
}

int Auton::getMacroFactorMultipler()
{
    return macroFactorMultiple;
}

void Auton::setMacroFactorMultipler(int multipler)
{
    macroFactorMultiple = multipler;
}

void Auton::distroEEvent(std::unique_ptr<EventQueue::eEvent> event)
{
    //nestene->eEventsOutbox.push_back(std::move(event));
}


std::unique_ptr<EventQueue::iEvent> Auton::processEvent(EventQueue::eEvent* event)
{
	return NULL;
}

std::unique_ptr<EventQueue::eEvent> Auton::handleEvent(std::unique_ptr<EventQueue::iEvent> eventPtr)
{
    return NULL;
}

bool Auton::operator==(Auton &other) const
{
	return (this->ID == other.getID());
}

bool Auton::operator!=(Auton &other) const
{
	return !(*this == other);
}



