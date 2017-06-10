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

#include "outbound.h"

Outbound *Outbound::outbound;

std::mutex Outbound::autonMutex;

Outbound *
Outbound::Inst()
{
  if(!outbound)
	outbound = new Outbound();

  return outbound;
}

Outbound::Outbound()
{
  std::cout << "This is the outbound class singleton" << std::endl;
}

void
Outbound::agentSay(std::string msg){

}

/**
 * @brief Ranas version of printf, prints a message to current active outbound
 * For example kprintf("sum of %i + %i = %i", 5, 5, 10) will outbound "sum of
 * 5 + 5 = 10", the string can be html formatted!
 * @param msg formated string that is to written to outbound
 * @see MainWindow::write_regularoutbound()
 */
void
Outbound::agentSay(const char *msg, ...)
{
  va_list args;
  va_start(args, msg);

  char buffer[4096] = {};
  vsprintf(buffer, msg, args);

  std::cout << buffer << std::endl;
  va_end(args);
}

/**
 * @brief Same as outbound::kprintf but it's outbound can be disabled via the menu
 * @see Outbound::kprintf()
 */
void
Outbound::agentShout(const char *msg, ...)
{
  va_list args;
  va_start(args, msg);

  char buffer[4096] = {};
  vsprintf(buffer, msg, args);

  std::cout << buffer << std::endl;

  va_end(args);
}

/**
 * @brief Updates the simulation status screen, with relevant values.
 * Only used during simulation.
 * @param ms current microstep
 * @param eventInit number of events initiated
 * @param internalEvents total number of internal events
 * @param externalEvents total number of external events
 */
void
Outbound::updateStatus(unsigned long long internalEvents, unsigned long long externalEvents)
{

}

/**
 * @brief Updates the simulation windows progress bar
 * @param current Current progress value, e.g current microstep
 * @param maximum Final microstep
 * @see MainWindow::advanceProgess()
 */
void
Outbound::progressBar(unsigned long long current, unsigned long long maximum)
{
  int progress = (current * 100) / maximum;
}

