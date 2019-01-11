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

#include "simulator/Output.h"
#include "Shared.h"

std::unordered_map<std::string, double> Shared::sharedNumbers;
std::unordered_map<std::string, std::string> Shared::sharedStrings;
std::shared_timed_mutex Shared::numberMutex;
std::shared_timed_mutex Shared::stringMutex;

//Shared::Shared()
//{
//}

void Shared::initShared()
{
    //if (sharedNumbers != NULL)
    //{
    sharedNumbers.clear();
    sharedStrings.clear();
    //sharedNumbers;
    //	delete sharedStrings;
    //} else
    //{
    //	sharedNumbers = new std::unordered_map<std::string, double>();
    //	sharedStrings = new std::unordered_map<std::string, std::string()>;
    //}
}

void Shared::addNumber(std::string key, double value)
{
    std::lock_guard<std::shared_timed_mutex>
            writerLock(numberMutex);

    sharedNumbers[key] = value;
    /*if(sharedNumbers.find(key) == sharedNumbers.end())
		sharedNumbers.insert(std::pair<std::string, double>(key, value));
	else
	{
		sharedNumbers.erase(key);
		sharedNumbers.insert(std::pair<std::string, double>(key, value));
    }*/

}

double Shared::getNumber(std::string key)
{
    std::shared_lock<std::shared_timed_mutex>
            readerLock(numberMutex);

    auto sharedItr = sharedNumbers.find(key);

    if (sharedItr != sharedNumbers.end())
    {
        return sharedItr->second;

    }
    else return LLONG_MIN;
}

void Shared::addString(std::string key, std::string value)
{


    std::lock_guard<std::shared_timed_mutex>
            writerLock(stringMutex);

    sharedStrings[key] = value;
    //Output::Inst()->kprintf(value.c_str());

    /*if(sharedStrings.find(key) == sharedStrings.end())
		sharedStrings.insert(std::pair<std::string, std::string>(key, value));
	else
	{
		sharedStrings.erase(key);
		sharedStrings.insert(std::pair<std::string, std::string>(key, value));
    }*/
}

std::string Shared::getString(std::string key)
{
    std::shared_lock<std::shared_timed_mutex>
            writerLock(stringMutex);

    auto sharedItr = sharedStrings.find(key);

    if (sharedItr != sharedStrings.end())
    {
        return sharedItr->second;
    }
    else return "";
}

