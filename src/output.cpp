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
#include <cstring>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <thread>
#include <chrono>
#include <iostream>

#include "output.h"
//#include "src/api/phys.h"

Output* Output::output;

std::mutex Output::autonMutex;
std::atomic<int> Output::DelayValue;
std::atomic<bool> Output::RunSimulation;
std::atomic<bool> Output::KillSimulation;
std::atomic<bool> Output::RunEventProcessing;
std::atomic<bool> Output::SimRunning;

unsigned long long Output::RUNTIME = 0;

std::string Output::AgentPath = "";
std::string Output::AgentFile = "";
std::string Output::RanaDir = "";

Output* Output::Inst()
{
    if(!output)
        output = new Output();

    return output;
}


Output::Output()
{
	std::cout << "This is the output class singleton" << std::endl;

}

/**
 * @brief Ranas version of printf, prints a message to current active output
 * For example kprintf("sum of %i + %i = %i", 5, 5, 10) will output "sum of
 * 5 + 5 = 10", the string can be html formatted!
 * @param msg formated string that is to written to output
 * @see MainWindow::write_regularOutput()
 */
void Output::kprintf(const char* msg, ...)
{
    //lock.lock();
    va_list args;
    va_start(args, msg);

    char buffer[4096] = {};
    vsprintf(buffer, msg, args);

	std::cout << buffer << std::endl;
    va_end(args);
    //lock.unlock();
}

/**
 * @brief Same as Output::kprintf but it's output can be disabled via the menu
 * @see Output::kprintf()
 * @see MainWindow::write_output()
 */
void Output::kdebug(const char* msg, ...)
{
    //lock.lock();
    va_list args;
    va_start(args, msg);

    char buffer[4096] = {};
    vsprintf(buffer, msg, args);

	std::cout << buffer << std::endl;

    va_end(args);
    //lock.unlock();
}

/**
 * @brief Updates the simulation status screen, with relevant values.
 * Only used during simulation.
 * @param ms current microstep
 * @param eventInit number of events initiated
 * @param internalEvents total number of internal events
 * @param externalEvents total number of external events
 * @see MainWindow::on_udateStatus()
 */
void Output::updateStatus(unsigned long long internalEvents, unsigned long long externalEvents)
{

}

/**
 * @brief Updates the simulation windows progress bar
 * @param current Current progress value, e.g current microstep
 * @param maximum Final microstep
 * @see MainWindow::advanceProgess()
 */
void Output::progressBar(unsigned long long current, unsigned long long maximum)
{
    int progress = (current * 100)/maximum;
}


void Output::enableRunBotton(bool enabled)
{

}
