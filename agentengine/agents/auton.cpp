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

#include "auton.h"
#include "nestene.h"
//#include "ID.h"



	Auton::Auton(int ID, double posX, double posY, double posZ, Nestene* nestene)
:ID(ID), posX(posX), posY(posY), posZ(posZ), nestene(nestene)
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

void Auton::distroEEvent(EventQueue::eEvent *event){
	nestene->eEventsOutbox.push_back(event);
}

EventQueue::iEvent* Auton::handleEvent(EventQueue::eEvent* event){
	return NULL;
}


EventQueue::eEvent* Auton::initEvent(int macroResolution, unsigned long long tmu){
	return NULL;
}

EventQueue::eEvent* Auton::actOnEvent(EventQueue::iEvent *event){
	return NULL;
}

bool Auton::operator==(Auton &other) const{
	return (this->ID == other.getID());
}

bool Auton::operator!=(Auton &other) const{
	return !(*this == other);
}



