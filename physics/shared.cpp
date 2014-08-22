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
#include <climits>

#include <iostream>
#include "shared.h"

std::map<std::string, double> *Shared::sharedNumbers = NULL;

Shared::Shared()
{
}

void Shared::initShared()
{
    if (sharedNumbers != NULL)
    {
        sharedNumbers->clear();
    } else
        sharedNumbers = new std::map<std::string, double>();
}

void Shared::addNumber(std::string key, double value)
{
    if(sharedNumbers->find(key) == sharedNumbers->end())
        sharedNumbers->insert(std::pair<std::string, double>(key, value));
    else
    {
        sharedNumbers->erase(key);
        sharedNumbers->insert(std::pair<std::string, double>(key, value));
    }

}

double Shared::getNumber(std::string key)
{
    if(sharedNumbers->find(key) != sharedNumbers->end())
    {
        return sharedNumbers->find(key)->second;
    } else return LLONG_MIN;
}



