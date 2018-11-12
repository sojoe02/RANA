//--begin_license--
//
//Copyright 	2013-2014 	Søren Vissing Jørgensen.
//			2014-	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.
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

#include "control.h"
#include "output.h"

Control::Control()
        : flowControl(nullptr), running(false), generated(false), stopped(true), generating(false)
{
}

Control::~Control()
{
}

void Control::runSimulation(unsigned long long runTime)
{
    running = true;
    //runThread.setStackSize(1024*1024*1024);
    Output::SimRunning.store(true);
    //runner->setParameters(flowControl, runTime);
    //runThread->start();
}

void Control::stopSimulation()
{
    flowControl->stopSimulation();
}

void Control::generateEnvironment(int threads,
                                  double timeRes, int macroRes,
                                  int agentAmount, std::string agentPath, int width, int height)
{
    if (!running || !generating)
    {
        generating = true;
//        if(populateFuture.isRunning())
//        {
//            Output::Inst()->kprintf("A previous system was being populated, it will be cancelled");
//            populateFuture.cancel();
//            populateFuture.waitForFinished();
//        }
        Output::KillSimulation.store(true);

        Output::kprintf("Generating environment");

        if (flowControl != nullptr)
        {
            //Output::Inst()->kprintf("Deleting agent domain");
            delete flowControl;
            flowControl = nullptr;
        }

        flowControl = new FlowControl(this);
        flowControl->generateEnvironment(width, height, threads,
                                         agentAmount, timeRes, macroRes, agentPath);
        flowControl->populateSystem();
        //populateFuture.waitForFinished();
        //ture.waitForFinished();
        //QThread::msleep(1000);
        generating = false;
    }
    else
        Output::kprintf("Simulation is being generating or it is running");
    //retrieve and update the positions:
}

void Control::threadTest(std::string something)
{
    int i = 0;
}

void Control::on_simDone()
{
    running = false;
    Output::SimRunning.store(false);
    Output::kprintf("Simulation Done");
}

void Control::refreshPopPos(std::list<agentInfo> infolist)
{
}

bool Control::isRunning()
{
    return running;
}

bool Control::isGenerated()
{
    if (flowControl != nullptr)
    {
        return true;
    }

    return false;
}

void Control::saveEvents(std::string path)
{
    flowControl->saveExternalEvents(path.c_str());
}

void Control::toggleLiveView(bool enable)
{
    if (flowControl != NULL)
        flowControl->toggleLiveView(enable);
}
