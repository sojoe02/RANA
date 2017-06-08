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
#ifndef OUTBOUND_H
#define OUTBOUND_H

#include <atomic>
#include <mutex>
#include <unordered_map>

class Outbound
{
public:
	static Outbound* Inst();

	void say(std::string msg);
	void say(const char* msg, ...);
	void shout(const char* msg, ...);

    void updateStatus(unsigned long long internalEvents, unsigned long long externalEvents);
    void progressBar(unsigned long long current, unsigned long long maximum);
    void updatePosition(int id, double x, double y);

private:

	std::unordered_map<std::string, std::string> messages;


	Outbound();
	static Outbound* outbound;
	static std::mutex autonMutex;


};

#endif // OUTBOUND_H
