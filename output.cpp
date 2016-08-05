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
#include "physics/phys.h"

#include "output.h"


Output* Output::output;
MainWindow* Output::mainWindow;

std::mutex Output::lock;
std::atomic<int> Output::DelayValue;
std::atomic<bool> Output::RunSimulation;
std::atomic<bool> Output::KillSimulation;
std::atomic<bool> Output::RunEventProcessing;
std::atomic<bool> Output::SimRunning;
std::atomic<bool> Output::LegacyMode;

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

	char buffer[2048] = {};
    vsprintf(buffer, msg, args);

    QString string(buffer);

	qDebug() << buffer;

	mainWindow->write_regularOutput(string);

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

	char buffer[2048] = {};
    vsprintf(buffer, msg, args);

    QString string(buffer);

	mainWindow->write_output(string);

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
void Output::updateStatus(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents)
{
	mainWindow->write_status(ms, eventInit,
							 internalEvents, externalEvents);
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
    mainWindow->advanceProgess(progress);
}

/**
 * @brief Updates the event postprocessing progress bar
 * @param current Current progress value
 * @param maximum Maximun progress value
 * @see MainWindow::advancePPProgess()
 */
void Output::ppprogressbar(int current, int maximum)
{
	int progress = (current * 100)/maximum;
	mainWindow->advancePPProgess(progress);
}

void Output::setEventSceneRect(int x, int y)
{
	mainWindow->setEventSceneRect(x, y);
}

void Output::setMainWindow(MainWindow *mainwindow)
{
	Output::mainWindow = mainwindow;
}

/**
 * @brief Deprecated use Output::kprintf() or Output::ppprintf()
 * @param msg
 */
void Output::ppprintf(const char *msg,...)
{

	va_list args;
	va_start(args, msg);

	char buffer[2048] = {};
	vsnprintf(buffer, 2047, msg, args);

	QString string(buffer);

	mainWindow->write_PPOutput(string);

	va_end(args);
}

/**
 * @brief Writes a string to the zvalue label, in event postprocessing.
 * @param string
 */
void Output::updateZvalue(QString string)
{
	Output::mainWindow->writeZValue(string);
}

void Output::removeGraphicAuton(int Id)
{
	mainWindow->removeGraphicAuton(Id);
}

void Output::addGraphicAuton(int Id, double posX, double posY)
{
    mainWindow->addGraphicAuton(Id, int(posX)/Phys::getScale(), int(posY)/Phys::getScale());
}
