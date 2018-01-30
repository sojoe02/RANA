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

#include "src/output.h"
#include "src/api/shared.h"

std::unordered_map<std::string, double> Shared::sharedNumbers;
std::unordered_map<std::string, std::string> Shared::sharedStrings;
std::shared_timed_mutex Shared::numberMutex;
std::shared_timed_mutex Shared::stringMutex;

std::unordered_map<std::string, agentPathNum> Shared::sharedAgents;
std::shared_timed_mutex Shared::agentMutex;
std::unordered_map<std::string,std::vector<std::string>> Shared::tcpInput;
std::shared_timed_mutex Shared::tcpInputMutex;

//Shared::Shared()
//{
//}

void Shared::initShared()
{
	sharedNumbers.clear();
    sharedStrings.clear();
}

void Shared::addNumber(std::string key, double value)
{
    std::lock_guard<std::shared_timed_mutex> writerLock(numberMutex);

    sharedNumbers[key] = value;
}

double Shared::getNumber(std::string key)
{
    std::shared_lock<std::shared_timed_mutex> readerLock(numberMutex);

	auto sharedItr = sharedNumbers.find(key);

	if(sharedItr != sharedNumbers.end())
	{
        return sharedItr->second;

	} else return LLONG_MIN;
}

void Shared::addString(std::string key, std::string value)
{
    std::lock_guard<std::shared_timed_mutex> writerLock(stringMutex);

    sharedStrings[key] = value;
}

std::string Shared::getString(std::string key)
{
    std::shared_lock<std::shared_timed_mutex> readerLock(stringMutex);

	auto sharedItr = sharedStrings.find(key);

	if(sharedItr != sharedStrings.end())
	{
		return sharedItr->second;
    } else return "";
}

void Shared::addAgentPathNum(std::string key, agentPathNum value)
{
    std::lock_guard<std::shared_timed_mutex> writerLock(agentMutex);

    sharedAgents[key] = value;
}

agentPathNum Shared::getAgentPathNum(std::string key)
{
    std::shared_lock<std::shared_timed_mutex> readerLock(agentMutex);

    auto sharedItr = sharedAgents.find(key);

    if(sharedItr != sharedAgents.end())
    {
        return sharedItr->second;
    };
}

void Shared::addTcpInputToAgent(std::string key, std::vector<std::string> value)
{
    std::lock_guard<std::shared_timed_mutex> writerLock(tcpInputMutex);

    tcpInput[key] = value;
}

std::vector<std::string> Shared::getTcpInputToAgent(std::string key)
{
    std::shared_lock<std::shared_timed_mutex> readerLock(tcpInputMutex);

    auto sharedItr = tcpInput.find(key);

    if(sharedItr != tcpInput.end())
    {
        return sharedItr->second;
    };
}
























