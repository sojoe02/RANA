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
#ifndef SHARED_H
#define SHARED_H

#include <string>
#include <unordered_map>
#include <shared_mutex>

class Shared {
public:
    Shared();

    static double getNumber(std::string key);

    static void addNumber(std::string key, double value);

    static std::string getString(std::string key);

    static void addString(std::string key, std::string value);

    static void initShared();

private:

    static std::shared_timed_mutex numberMutex;
    static std::shared_timed_mutex stringMutex;

    static std::unordered_map<std::string, double> sharedNumbers;
    static std::unordered_map<std::string, std::string> sharedStrings;

};


#endif // SHARED_H
