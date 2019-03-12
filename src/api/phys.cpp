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

#include "src/api/phys.h"
#include "src/output.h"

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

	double distance = sqrt( pow((x_origin-x_dest), 2) + pow((y_origin-y_dest),2) );

    unsigned long long tmp = uint64_t (distance / (343.2 * Phys::timeResolution));
    unsigned long long a_timestep = tmp + Phys::c_timeStep;

    return a_timestep;
}

unsigned long long Phys::speedOfEvent(double x_origin, double y_origin,
		double x_dest, double y_dest, double propagationSpeed)
{

	double distance = sqrt( pow((x_origin-x_dest), 2) + pow((y_origin-y_dest),2) );
    double tmp = distance / (propagationSpeed * Phys::timeResolution);
    unsigned long long a_timestep = tmp + Phys::c_timeStep;

    return a_timestep;
}

double Phys::calcDistance(double x_origin, double y_origin, 
                          double x_dest, double y_dest)
{
	return  sqrt( pow((x_origin-x_dest), 2) + pow((y_origin-y_dest),2) );
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

double Phys::getEnvZ()
{
	return 1;
}

double Phys::getMersenneFloat(double min=0, double max=1)
{
    double random = ((double)Phys::int_dist(Phys::rng)/(double)LLONG_MAX);
    //double random = ((double)std::uniform_int_distribution<int64_t>()(rand()))/(double)LLONG_MAX;
    //Output::Inst()->kdebug("%f", random);
    //double random = ((double)rand()/(double)(RAND_MAX));
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

int Phys::getBernouilliInt(double probability=0.5)
{
    std::bernoulli_distribution bernouilli(probability);
    int value = bernouilli(Phys::rng);
    return value;
}

double Phys::getGaussianFloat(double mean=0, double var=1)
{
    std::normal_distribution<> gauss(mean, var);
    double value = gauss(Phys::rng);
    return value;
}

double Phys::getPoissonFloat(int mean=1)
{
    double value = 0;
    // Poisson distribution only works with positive integers.
    if (mean > 0)
    {
        std::poisson_distribution<> poisson(mean);
        value = poisson(Phys::rng);
    }
    return value;
}
