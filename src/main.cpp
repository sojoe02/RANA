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
#include <stdio.h>
#include <string>
#include <iostream>

#include "version.h" 
//include "src/ID.h"
#include "phys.h"
#include "communication/outbound.h"
#include "communication/inbound.h"
//#include "src/api/gridmovement.h"
//#include "src/output.h"

//int ID::aID = 0;
//unsigned long long ID::eID = 0;
//unsigned long long ID::tmu = 0;
//unsigned long long ID::nID = 0;

int main(int argc, char *argv[])
{
    //srand(time(0));
	Phys::seedMersenne();
	//Output::DelayValue = 0;
	//Output::LegacyMode.store(false);

	double random = Phys::getMersenneFloat(1,90.2);

	std::cout << random << std::endl;

    std::cout << "this is the shit" << std::endl;
	
	std::cout << gGIT_VERSION << std::endl << gGIT_VERSION_SHORT << std::endl;

}
