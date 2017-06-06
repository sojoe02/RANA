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
    : agentDomain(NULL), mainwindow(mainwindow), running(false),
      generated(false), stopped(true), generating(false)
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
        lua_getglobal(L,"_getInitialCurrentSimulation");
        if(lua_pcall(L,0,1,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Can't get current"); }
        else{ this->currentNumberOfSimulation = lua_tonumber(L,-1); }
        lua_settop(L,0);
        lua_getglobal(L,"_getTotalNumberSimulation");
        if(lua_pcall(L,0,1,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Can't get total"); }
        else{ this->totalNumberOfSimulations = lua_tonumber(L,-1); }
        lua_settop(L,0);
        lua_getglobal(L,"_initializeParameterForSimulation");
        if(lua_pcall(L,0,0,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Can't set params"); }

    }catch(std::exception& e){ Output::Inst()->kprintf("Control - Exception"); }

    connect(this, &Control::runSimulationSignal, this, &Control::runSimulation);
}

Control::~Control()
{
    killRunthread();
}

void Control::setEnvironmentVariables(QImage *map, int threads, double timeRes, double macroRes, int agentAmount, std::string agentPath)
{
    this->map = map;
    this->threads = threads;
    this->timeRes = timeRes;
    this->macroRes = macroRes;
    this->agentAmount = agentAmount;
    this->agentPath = agentPath;

    currentNumberOfSimulation = 1;

    generateEnvironment();
}

void Control::generateEnvironment()
{
    if(!running && !generating)
    {
        generating = true;

        if(populateFuture.isRunning())
        {
            Output::Inst()->kprintf("A previous system was being populated, it will be cancelled\n");
            populateFuture.cancel();
            populateFuture.waitForFinished();
        }
        Output::KillSimulation.store(true);

        readyAgentDomain();

        generating = false;
    } else{
        Output::Inst()->kprintf("Simulation is being generating or it is running");
    }
}

void Control::startSimulation(unsigned long long runTime)
{
    this->runTime = runTime;

    emit runSimulationSignal();
}

void Control::runSimulation()
{
    generateEnvironment();
    readyRunner();

    running = true;

    Output::SimRunning.store(true);
    mainwindow->changeRunButton("Stop");

    emit startDoWork(agentDomain, runTime);
}

void Control::on_simDone()
{
    killRunner();
    killAgentDomain();
    killRunthread();

    mainwindow->changeRunButton("Run");
    mainwindow->runButtonHide();

    Output::SimRunning.store(false);
    Output::Inst()->kprintf("Simulation Done\n");
    running = false;

    if(totalNumberOfSimulations > currentNumberOfSimulation){
        currentNumberOfSimulation++;
        updateLuaSimulationConfigs();
        emit runSimulationSignal();
    }
}

void Control::updateLuaSimulationConfigs(){

    try{
        lua_settop(L,0);
        lua_getglobal(L,"_setCurrentSimulation");
        lua_pushnumber(L, currentNumberOfSimulation);
        if(lua_pcall(L,1,0,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Set new Simulation"); }
        lua_settop(L,0);
        lua_getglobal(L,"_loadNewParameters");
        if(lua_pcall(L,0,0,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Save new parameters"); }
    }catch(std::exception& e){ Output::Inst()->kprintf("Control - Exception"); }

}

void Control::readyRunner()
{
    killRunner();

    runner = new Runner();
    runner->moveToThread(&runThread);

    connect(this, &Control::startDoWork, runner, &Runner::doWork);
    connect(&runThread, &QThread::finished, runner, &QObject::deleteLater);
    connect(runner, &Runner::simulationDone, this, &Control::on_simDone);

    runThread.start();
}

void Control::readyAgentDomain()
{
    killAgentDomain();

    agentDomain = new FlowControl(this);
    agentDomain->generateEnvironment(map->width(),map->height(),threads,agentAmount,timeRes,macroRes,agentPath);
    populateFuture = QtConcurrent::run(agentDomain, &FlowControl::populateSystem);
}

void Control::killAgentDomain()
{
    if(agentDomain != NULL){
        delete agentDomain;
        agentDomain = NULL;
    }
}

void Control::killRunner()
{
    if(runner != NULL){
        delete runner;
        runner = NULL;
    }
}

void Control::killRunthread(){
    runThread.quit();
    runThread.wait();
}

void Control::stopSimulation()
{
    agentDomain->stopSimulation();
}

void Control::threadTest(std::string something)
{
    int i = 0;
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
