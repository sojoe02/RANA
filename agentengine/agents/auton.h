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

#ifndef AUTON_H
#define AUTON_H

#include <vector>

#include "eventqueue.h"

class EventQueue;
class Nestene;
class Auton
{
public:
    Auton(int ID, double posX, double posY, double posZ, Nestene *nestene);

	virtual ~Auton(){};

    virtual EventQueue::iEvent* handleEvent(EventQueue::eEvent* event);
    virtual EventQueue::eEvent* actOnEvent(EventQueue::iEvent* event);
    virtual EventQueue::eEvent* initEvent(int macroResolution, unsigned long long tmu);
	virtual EventQueue::eEvent* initEvent(){return NULL;};
	virtual void processFunction(EventQueue::dataEvent *devent, double mapRes,
								 double x, double y, double &zvalue, double &duration){};
    //virtual double eventChance();

	
    std::string getDesc();
    int getID();
    double getPosX();
    double getPosY();

    void simDone(){};

    bool operator==(Auton &other) const;
    bool operator!=(Auton &other) const;


protected:
    void distroEEvent(EventQueue::eEvent* event);

    int ID;
    std::string desc;
    double posX, posY, posZ;
    std::vector<double> statusVector;
    Nestene* nestene;

    friend class Nestene;
};

#endif // AUTON_H
