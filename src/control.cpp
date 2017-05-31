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

#include "src/control.h"
#include "src/mainwindow.h"
#include "src/output.h"

Control::Control(MainWindow* mainwindow)
    : agentDomain(NULL), mainwindow(mainwindow),
      running(false), generated(false), stopped(true),generating(false)
{
    L = luaL_newstate();

    if( L == NULL ){
        Output::Inst()->kprintf("<b><font color=\"brown\">Simulation config cannot be initialized. Lua(%s) is out of memory, Killing simulation</font></b></>", LUA_VERSION);
        Output::KillSimulation.store(true);
    }else{
        luaL_openlibs(L);

        // Register the path to the Rana specific lua modules
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "path");
        std::string cur_path = lua_tostring(L, -1);
        std::string module_path = Output::Inst()->RanaDir;
        //Output::Inst()->kdebug(module_path.c_str());
        module_path.append("./src/modules/?.lua");
        //Output::Inst()->kdebug(module_path.c_str());
        cur_path.append(";");
        cur_path.append(module_path);
        cur_path.append(";");
        cur_path.append(Output::Inst()->AgentPath);
        cur_path.append("?.lua");
        lua_pop(L,1);
        lua_pushstring(L, cur_path.c_str());
        lua_setfield(L,-2,"path");
        lua_pop(L,1);

        //  Register the path to the simulation config module
        std::string simLib = Output::Inst()->RanaDir;
        simLib.append("./src/modules/ranalib_simconfig.lua");   //TODO: problems with "RanaDir"

        //  Load the lua modules
        if( luaL_loadfile(L, simLib.c_str()) || lua_pcall(L,0,0,0) ){
            Output::Inst()->kprintf("\tsim file not found\n");
        }
        else{
            std::cout << "\tSim file loaded: " << simLib << std::endl;
        }
    }
    try{
        lua_settop(L,0);
        lua_getglobal(L,"_loadInitializeSimulation");
        if(lua_pcall(L,0,1,0)!=LUA_OK){
            Output::Inst()->kprintf("Control - Lua_simconfig - Doesn't work");
        }else{
            this->totalNumberOfSimulations  = lua_tonumber(L,-1);
        }
    }catch(std::exception& e){ Output::Inst()->kprintf("Control - Exception"); }

    runner = new Runner();
    runner->moveToThread(&runThread);

    connect(this, &Control::startDoWork, runner, &Runner::doWork);
    connect(&runThread, &QThread::finished, runner, &QObject::deleteLater);
    connect(runner, &Runner::simulationDone, this, &Control::on_simDone);
    //runThread.setStackSize(1024*1024*512);
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
    //runThread.setStackSize(1024*1024*1024);
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

void Control::generateEnvironment(QImage *map, int threads,
                                  double timeRes, double macroRes,
                                  int agentAmount, std::string agentPath)
{
    if(!running || !generating)
    {
        generating = true;
        if(populateFuture.isRunning())
        {
            Output::Inst()->kprintf("A previous system was being populated, it will be cancelled");
            populateFuture.cancel();
            populateFuture.waitForFinished();
        }
        Output::KillSimulation.store(true);

        Output::Inst()->kprintf("Generating environment");

        if(agentDomain != NULL)
        {
            //Output::Inst()->kprintf("Deleting agent domain");
            delete agentDomain;
            agentDomain = NULL;
        }

        agentDomain = new FlowControl(this);

        agentDomain->generateEnvironment(map->width(),map->height(),threads,
                                         agentAmount,timeRes,macroRes,agentPath);
        //agentDomain->populateSystem();

        populateFuture = QtConcurrent::run(agentDomain, &FlowControl::populateSystem);

        //populateFuture.waitForFinished();

        //ture.waitForFinished();
        //QThread::msleep(1000);
        generating = false;
    } else
        Output::Inst()->kprintf("Simulation is being generating or it is running");
    //retrieve and update the positions:
}

void Control::threadTest(std::string something)
{
    int i = 0;
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
