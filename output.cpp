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

#include "output.h"


Output* Output::output;
MainWindow* Output::mainWindow;
std::mutex Output::lock;
std::atomic<int> Output::DelayValue;
std::atomic<bool> Output::RunSimulation;
std::atomic<bool> Output::KillSimulation;
unsigned long long Output::RUNTIME = 0;


Output* Output::Inst()
{
    if(!output)
        output = new Output();

    return output;
}


Output::Output()
{

}

void Output::kprintf(const char* msg, ...)
{
    //lock.lock();
    va_list args;
    va_start(args, msg);

    char buffer[2048];
    vsprintf(buffer, msg, args);

    QString string(buffer);

    mainWindow->write_output(string);

    va_end(args);
    //lock.unlock();
}

void Output::kerr(const char* msg, ...)
{
    //lock.lock();
    va_list args;
    va_start(args, msg);

    char buffer[2048];
    vsprintf(buffer, msg, args);

    QString string(buffer);

    mainWindow->write_errOutput(string);

    va_end(args);
    //lock.unlock();
}

void Output::updateStatus(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents)
{
    mainWindow->write_status(ms, eventInit, internalEvents, externalEvents);
}

void Output::progressBar(unsigned long long current, unsigned long long maximum)
{
    int progress = (current * 100)/maximum;
    mainWindow->advanceProgess(progress);
}

void Output::ppprogressbar(int current, int maximum)
{
	int progress = (current * 100)/maximum;
	mainWindow->advancePPProgess(progress);
}

void Output::setMainWindow(MainWindow *mainwindow)
{
	Output::mainWindow = mainwindow;
}

void Output::ppprintf(const char *msg,...)
{

	va_list args;
	va_start(args, msg);

	char buffer[2048];
	vsprintf(buffer, msg, args);

	QString string(buffer);

	mainWindow->write_PPOutput(string);

	va_end(args);
}
