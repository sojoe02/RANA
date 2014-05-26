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
		Phys(){
		};

		static unsigned long long speedOfSound(double x_origin, double y_origin,
				double x_dest, double y_dest);

		static unsigned long long speedOfSound(double x_origin, double y_origin,
				double x_dest, double y_dest, double propagationSpeed);

		static double calcDistance(double x_origin, double y_origin, 
				double x_dest, double y_dest);
		static unsigned long long getCTime();

		static void incTime();
		static void seedMersenne();
		static void setTimeRes(double timeResolution);
		static double getTimeRes();
		static int getMacroFactor();
		static void setMacroFactor(int macroFactor);
		static void setCTime(unsigned long long ctime);
		static double getMersenneFloat(double min, double max);
		static uint64_t getMersenneInteger(uint64_t min, uint64_t max);

		static void setEnvironment(double x, double y);
		static double getEnvX();
		static double getEnvY();

	private:
		static double env_x;
		static double env_y;

		static int macroFactor;
		static double timeResolution;
		static unsigned long long c_timeStep;
		//random distribution 0-INT_MAX
		static std::uniform_int_distribution<uint64_t> uint_dist;
		typedef std::mt19937_64 MyRNG;
		static MyRNG rng;

};



#endif // PHYS_H
