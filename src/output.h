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
#ifndef OUTPUT_H
#define OUTPUT_H

#include <atomic>
#include <mutex>

class Output
{
public:
    static Output* Inst();

    void kprintf(const char* msg, ...);
    void kdebug(const char* msg, ...);

    void updateStatus(unsigned long long internalEvents, unsigned long long externalEvents);
    void progressBar(unsigned long long current, unsigned long long maximum);
    void updatePosition(int id, double x, double y);

    static std::atomic<int> DelayValue;
    static std::atomic<bool> RunSimulation;
    static std::atomic<bool> KillSimulation;
    static std::atomic<bool> RunEventProcessing;
    static std::atomic<bool> SimRunning;
    static std::atomic<bool> LegacyMode;

    static unsigned long long RUNTIME;
    static std::string AgentPath;
    static std::string AgentFile;
    static std::string RanaDir;

    void enableRunBotton(bool enabled);

private:

    Output();
    static Output* output;
    static std::mutex autonMutex;
};

#endif // OUTPUT_H
