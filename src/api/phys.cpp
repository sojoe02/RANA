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
#include "../interface/communication/outbound.h"

//auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937_64 Phys::rng;  // the Mersenne Twister with a popular choice of parameters
double Phys::timeResolution = 0;
int Phys::macroFactor = 0;
unsigned long long Phys::c_timeStep = 0;
std::uniform_int_distribution<int64_t> Phys::int_dist;
double Phys::env_x = 0;
double Phys::env_y = 0;
double Phys::scale = 1;


void Phys::seedMersenne()
{
    // rng.seed(std::random_device()());
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::random_device rd;
    //std::seed_seq seed2{seed, rd(), rd()};
    //rng.seed(seed);
    rng.seed(seed);
    //srand(time(0));
}


void Phys::incTime()
{
    Phys::c_timeStep++;
}

unsigned long long Phys::getCTime()
{
	return Phys::c_timeStep;
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


unsigned long long Phys::speedOfEvent(double x_origin, double x_dest
									  ,double y_origin, double y_dest
									  ,double propagationSpeed)
{
	return speedOfEvent(x_origin, x_dest, y_origin, y_dest, 0, 0, propagationSpeed);
}

unsigned long long Phys::speedOfEvent(double x_origin, double x_dest
									  ,double y_origin, double y_dest
									  ,double z_origin, double z_dest
									  ,double propagationSpeed)
{
	double distance = sqrt( pow((x_origin-x_dest), 2)
							+ pow((y_origin-y_dest),2)
							+ pow((z_origin-z_dest),2) );

    double tmp = distance / (propagationSpeed * Phys::timeResolution);
    unsigned long long a_timestep = tmp + Phys::c_timeStep;

    return a_timestep;
}

double Phys::getMersenneFloat(double min=0, double max=1)
{
    double random = ((double)Phys::int_dist(Phys::rng)/(double)LLONG_MAX);
    double diff = max - min;
    double r = random * diff;
    return min + r;
}

int64_t Phys::getMersenneInteger(int64_t min=0, int64_t max=ULLONG_MAX)
{
    max++;
    //return rand() %max;
    return min + Phys::int_dist(Phys::rng)%(max-min);
}
