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
#include "mainwindow.h"
#include "output.h"

Control::Control(MainWindow* mainwindow)
    : agentDomain(NULL), mainwindow(mainwindow),
      running(false), generated(false), stopped(true)
{
	runner = new Runner();
	runner->moveToThread(&runThread);

	connect(this, &Control::startDoWork, runner, &Runner::doWork);
	connect(&runThread, &QThread::finished, runner, &QObject::deleteLater);
	connect(runner, &Runner::simulationDone, this, &Control::on_simDone);
	runThread.start();
}

Control::~Control()
{
	runThread.quit();
	runThread.wait();
}

void Control::runSimulation(unsigned long long runTime)
{
    running = true;
	Output::SimRunning.store(true);
	mainwindow->changeRunButton("Stop");
	emit startDoWork(agentDomain, runTime);
	//runner->setParameters(agentDomain, runTime);
	//runThread->start();
}

void Control::stopSimulation()
{
    agentDomain->stopSimulation();
}

void Control::generateEnvironment(QImage *map, double scale,
                                  double timeRes, double macroRes,
                                  int agentAmount, std::string agentPath)
{
    if(!running)
    {
        Output::Inst()->kprintf("Generating environment");

        if(agentDomain != NULL)
        {
            delete agentDomain;
            agentDomain = NULL;
        }

        agentDomain = new AgentDomain(this);
		agentDomain->generateEnvironment(map->width(),map->height(),4,0,0,
                                         agentAmount,timeRes,macroRes,agentPath);
    } else
        Output::Inst()->kprintf("Simulation thread is running, so you need to stop it");
    //retrieve and update the positions:
}

void Control::on_simDone()
{
    running = false;
    mainwindow->changeRunButton("Run");
    mainwindow->runButtonHide();
	Output::SimRunning.store(false);
    Output::Inst()->kprintf("Simulation Done");
}

void Control::refreshPopPos(std::list<agentInfo> infolist)
{
    mainwindow->updateMap(infolist);
}

bool Control::isRunning()
{
    return running;
}

bool Control::isGenerated()
{
	if(agentDomain!=NULL)
	{
		return true;
	}

	return false;
}

void Control::saveEvents(QString path)
{
	agentDomain->saveExternalEvents(path.toStdString());
}

void Control::toggleLiveView(bool enable)
{
    if(agentDomain != NULL)
        agentDomain->toggleLiveView(enable);
}
