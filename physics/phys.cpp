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

#include <random>
#include <chrono>
#include <math.h>
#include <limits.h>

#include "phys.h"



typedef std::mt19937_64 MyRNG;  // the Mersenne Twister with a popular choice of parameters
double Phys::timeResolution = 0;
int Phys::macroFactor = 0;
unsigned long long Phys::c_timeStep = 0;
MyRNG Phys::rng;
std::uniform_int_distribution<uint64_t> Phys::uint_dist;
double Phys::env_x = 0;
double Phys::env_y = 0;
double Phys::scale = 1;


void Phys::seedMersenne()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	rng.seed(seed);
}


void Phys::incTime()
{
	Phys::c_timeStep++;
}

void Phys::setTimeRes(double timeResolution)
{
	Phys::timeResolution = timeResolution;
}

double Phys::getTimeRes()
{
	return Phys::timeResolution;
}

void Phys::setMacroFactor(int macroFactor)
{
	Phys::macroFactor = macroFactor;
}

int Phys::getMacroFactor()
{
	return Phys::macroFactor;
}

unsigned long long Phys::speedOfSound(double x_origin, double y_origin,
		double x_dest, double y_dest)
{

	double distance = sqrt( pow((x_origin-x_dest), 2) + pow((y_origin-y_dest),2) ) * scale;

	unsigned long long tmp = uint64_t (distance / (343.2 * Phys::timeResolution));
	unsigned long long a_timestep = tmp + Phys::c_timeStep;

	return a_timestep;
}

unsigned long long Phys::speedOfSound(double x_origin, double y_origin,
		double x_dest, double y_dest, double propagationSpeed)
{

	double distance = sqrt( pow((x_origin-x_dest), 2) + pow((y_origin-y_dest),2) ) * scale;

	double tmp = distance / (propagationSpeed * Phys::timeResolution);
	unsigned long long a_timestep = tmp + Phys::c_timeStep;

	return a_timestep;
}

double Phys::calcDistance(double x_origin, double y_origin, 
		double x_dest, double y_dest)
{
	return  sqrt( pow((x_origin-x_dest), 2) + pow((y_origin-y_dest),2) ) * scale;
}

unsigned long long Phys::getCTime()
{
	return Phys::c_timeStep;
}

void Phys::setCTime(unsigned long long ctime)
{
	Phys::c_timeStep = ctime;
}

void Phys::setEnvironment(double x, double y)
{
	Phys::env_x = x;
	Phys::env_y = y;
}

double Phys::getEnvX()
{
	return env_x * scale;
}

double Phys::getEnvY()
{
	return env_y * scale;
}


void Phys::move(double v, double posX, double posY,
                double desX, double desY,
                double &newX, double &newY)
{
    double vf = v*(Phys::macroFactor * Phys::timeResolution);
    double angle = atan2(desX-posX, desY-posY);

    double vX = vf * cos(angle);
    double vY = vf * sin(angle);

	newX = vX*scale + posX;
	newY = vY*scale + posY;
}

double Phys::getMersenneFloat(double min=0, double max=1)
{
	return min + (double)Phys::uint_dist(Phys::rng)/((double)ULLONG_MAX/(max-min));
}

uint64_t Phys::getMersenneInteger(uint64_t min=0, uint64_t max=ULLONG_MAX)
{
    max++;
    return min + Phys::uint_dist(Phys::rng)%(max-min);
}
