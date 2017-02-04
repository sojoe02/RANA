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

#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <map>

#define SSTR(x) dynamic_cast<std::ostringstream & >(\
		(std::ostringstream() << std::dec << x) ).str();


template <class T>
bool util_toNumber(T& t,const std::string& s,
		std::ios_base& (*f)(std::ios_base&)){
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

struct rgba
{
    int red = 255;
    int green = 255;
    int blue = 255;
    int alpha = 255;
};

struct agentInfo
{
	double x;
	double y;
	double z;
    int id;
	rgba color;

	double radius;
	double mass;
	double charge;

	double angle;
    //std::string filename;

};


struct agentTmu
{
    int x;
    int y;
    int id;
	unsigned long long tmu;
};

struct agentPosition
{
	int x;
	int y;
};

typedef std::vector< std::vector<int> > MatriceInt;
typedef std::vector< std::map<int, double>> EdgeAngles;


#endif // UTILITY_H
