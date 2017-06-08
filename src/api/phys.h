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
#ifndef PHYS_H
#define PHYS_H

#include <math.h>
#include <random>
#include <chrono>

class Phys
{
	public:
		Phys(){}

        static double getTimeRes();
        static int getMacroFactor();
        static void setMacroFactor(int macroFactor);
        static void setCTime(unsigned long long ctime);

		static unsigned long long speedOfEvent(double x_origin, double x_dest
											   ,double y_origin, double y_dest
											   ,double propagationSpeed);

		static unsigned long long speedOfEvent(double x_origin, double x_dest
											   , double y_origin, double y_dest
											   , double z_origin, double z_dest
											   , double propagationSpeed);

		static unsigned long long getCTime();
		static void incTime();
		static void setTimeRes(double timeResolution);

		static void seedMersenne();

		static double getMersenneFloat(double min, double max);
		static int64_t getMersenneInteger(int64_t min, int64_t max);

		static void setScale(double scale){Phys::scale = scale;}
		static double getScale(){return Phys::scale;}

private:

		static double env_x;
		static double env_y;

		static int macroFactor;
		static double timeResolution;
		static unsigned long long c_timeStep;

		//random distribution 0-INT_MAX
        static std::uniform_int_distribution<int64_t> int_dist;
		typedef std::mt19937_64 MyRNG;
        static std::mt19937_64 rng;

		static double scale;
};

#endif // PHYS_H
