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
#ifndef AUTONLISTENER_H
#define AUTONLISTENER_H

//#include "nestene.h"
#include "auton.h"

class Nestene;
class AutonListener : public Auton
{
	public:
		AutonListener(int ID, double posX, double posY, double posZ, Nestene *nestene);
		bool operator==(AutonListener &other) const;
		bool operator!=(AutonListener &other) const;

	private:
		//function to receive an event from nestene responsible for this auton, returns an internal Event 'thinking':
		EventQueue::iEvent* handleEvent(EventQueue::eEvent* event);
		EventQueue::eEvent* actOnEvent(EventQueue::iEvent *event);

		//returns an event:
		EventQueue::eEvent* initEvent(double macroResolution, unsigned long long tmu);

		double eventChance;

		bool eventInitiated = false;

		friend class Nestene;
		//Nestene *nestene;
};

#endif // AUTONLISTENER_H
