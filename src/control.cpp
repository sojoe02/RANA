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
#include "src/cli.h"
#include "src/mainwindow.h"
#include "src/output.h"

#include "api/shared.h"

Control::Control(Cli* cli, std::string _file) :
    agentDomain(nullptr), cli(cli), running(false),
    generated(false), stopped(true), generating(false), agentPath(_file)
{
    initialSetup();
}

Control::Control(MainWindow* mainwindow) :
    agentDomain(nullptr), mainwindow(mainwindow), running(false),
    generated(false), stopped(true), generating(false)
{
    initialSetup();
}

Control::~Control()
{
    killRunthread();
}

void Control::initialSetup()
{
    Output::Inst()->RanaDir = QCoreApplication::applicationDirPath().toUtf8().constData();
    QString _agentPath = QString::fromUtf8(agentPath.c_str());
    QFileInfo fi(_agentPath);
    Output::AgentFile=fi.fileName().toStdString();
    Output::AgentPath=fi.path().toStdString().append("/");

    L = luaL_newstate();

    if( L == nullptr ){
        Output::Inst()->kprintf("<b><font color=\"brown\">Simulation config cannot be initialized. Lua(%s) is out of memory, Killing simulation</font></b></>", LUA_VERSION);
        Output::KillSimulation.store(true);
    }else{
        luaL_openlibs(L);

        // Register the path to the Rana specific lua modules
        lua_getglobal(L, "package");
        lua_getfield(L, -1, "path");
        std::string cur_path = lua_tostring(L, -1);
        std::string module_path = Output::Inst()->RanaDir;
        module_path.append("/src/modules/?.lua");
        std::string agent_path = Output::Inst()->RanaDir;
        agent_path.append("/src/lua_agents/?.lua");
        cur_path.append(";");
        cur_path.append(module_path);
        cur_path.append(";");
        cur_path.append(Output::Inst()->AgentPath);
        cur_path.append("?.lua;");
        cur_path.append(agent_path);
        cur_path.append(";");
        cur_path.append("?.lua");
        cur_path.append(";");

        lua_pop(L,1);
        lua_pushstring(L, cur_path.c_str());
        lua_setfield(L,-2,"path");
        lua_pop(L,1);

        //  Load the back end simulation functions.
        std::string simLib = Output::Inst()->RanaDir;
        simLib.append("/src/modules/lib_sim_config.lua");

        if( luaL_loadfile(L, simLib.c_str()) || lua_pcall(L,0,0,0) ){
            Output::Inst()->kprintf("\tsim file not found %s", simLib);
        }

        //  Remove .lua extension from a file, if there are no ".", just use filepath.
        size_t lastdot = agentPath.find_last_of(".");
        if (lastdot == std::string::npos){
            this->rawfile = agentPath;
        }else{
            this->rawfile = agentPath.substr(0, lastdot);
        }
    }
    connect(this, &Control::runSimulationSignal, this, &Control::runSimulation);
    connect(this, &Control::simulationRunBoptSignal, cli, &Cli::runBoptController);
}

void Control::setupLuaSimulation()
{
    lua_settop(L,0);
    lua_getglobal(L,"_getSimulationFile");
    lua_pushstring(L,rawfile.c_str());
    if(lua_pcall(L,1,0,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Can't locate Sim file - 1"); }

    lua_settop(L,0);
    lua_getglobal(L,"_checkIfInputFileIsSimulationType");
    if(lua_pcall(L,0,1,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Can't locate Sim file - 2"); }

    if( !lua_toboolean(L,-1) )
    {
        //  File is not simulation file
        std::cout << "Error in tables in Simulation file" << std::endl;
        exit(EXIT_FAILURE); //TODO: Skip with -force option
    }
    else
    {
        //  File is simulation file
        lua_register(L, "l_addSharedNumber",l_addSharedNumber);
        lua_register(L, "l_addSharedAgent",l_addSharedAgent);

        lua_settop(L,0);
        lua_getglobal(L,"_simulationConfigMainFunction");
        if(lua_pcall(L,0,0,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Can't locate Sim file - 3"); }
    }

    lua_getglobal(L,"_paramMain");
    if(lua_pcall(L,0,0,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Can't locate Sim file - 4"); }


    if(!runNewSimulation())
    {
        //  Ready new simulation by
        exit(EXIT_FAILURE);
    }
}

bool Control::runNewSimulation()
{
    /**
    * @brief lua_getglobal -
    * Function that will go in and do the permutation of parameters
    * the function will keep returning true, as long as a new permutation exists.
    */
    lua_getglobal(L,"_paramMainCoroutine");
    if(lua_pcall(L,0,1,0)!=LUA_OK){
        Output::Inst()->kprintf("Control - Lua_simconfig - Can't locate Sim file - 5");
        return false;
    }

    //  If true; Run a new simulation with the new parameters
    //  If false; Just exit, all permutations of parameters done.
    return lua_toboolean(L,-1);
}

void Control::setEnvironmentVariables(QImage *map, int threads, double timeRes, double macroRes, int agentAmount, std::string agentPath)
{
    this->map = map;
    this->threads = threads;
    this->timeRes = timeRes;
    this->macroRes = macroRes;
    this->agentAmount = agentAmount;
    this->agentPath = agentPath;

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
    readyRunner();
    running = true;
    Output::SimRunning.store(true);
    if(mainwindow != nullptr)
    {
        mainwindow->changeRunButton("Stop");
    }
    emit startDoWork(agentDomain, runTime);
}

void Control::on_simDone()
{
    killRunner();
    killAgentDomain();
    killRunthread();
    if(mainwindow != nullptr)
    {
        mainwindow->changeRunButton("Run");
        mainwindow->runButtonHide();
    }
    Output::SimRunning.store(false);
    Output::Inst()->kprintf("Simulation Done\n");
    running = false;

    if(cli != nullptr)
    {
        if(runNewSimulation()){
            //  While the simulation file still runs, keep the same
            generateEnvironment();
            emit runSimulationSignal();
        }else{
            if(cli->bopt != nullptr){
                //  Find a new simulation file to run.
                //emit simulationRunBoptSignal();
                std::cout << "Deleting Control" << std::endl;
            }else{
                std::cout << "All simulations done" << std::endl;
                exit(EXIT_SUCCESS);
            }
        }
    }
}

void Control::readyRunner()
{
    killRunner();
    runner = new Runner();
    runner->moveToThread(&runThread);
    connect(this, &Control::startDoWork, runner, &Runner::doWork);
    connect(runner, &Runner::simulationDone, this, &Control::on_simDone);
    connect(&runThread, &QThread::finished, runner, &QObject::deleteLater);
    runThread.start();

}

void Control::readyAgentDomain()
{
    killAgentDomain();
    agentDomain = new FlowControl(this);
    /**
        When readying the agent domain, serialize the parameters, the agents should use.
    **/
    agentDomain->generateEnvironment(map->width(),map->height(),threads,agentAmount,timeRes,macroRes,agentPath);
    if(mainwindow != nullptr){
        populateFuture = QtConcurrent::run(agentDomain, &FlowControl::populateSystem);  //  This will run the whole agent generation a second time.
    }
}

void Control::killRunner()
{
    if(runner != nullptr){
        //delete runner;    //Got seg fault on this.
        runner = nullptr;
    }
}

void Control::killAgentDomain()
{
    if(agentDomain != nullptr){
        delete agentDomain;
        agentDomain = nullptr;
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
    if(mainwindow != nullptr)
    {
        mainwindow->updateMap(infolist);
    }
}

bool Control::isRunning()
{
    return running;
}

bool Control::isGenerated()
{
    if(agentDomain != nullptr)
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
    if(agentDomain != nullptr){
        agentDomain->toggleLiveView(enable);
    }
}

/**
 * @brief Control::newSimulation
 * Just used as a debug message.
 */
int Control::newSimulation()
{
    return ++simulationNumber;
}


/**
 * @brief Control::l_addSharedNumber - Are used for the simulation file to access the shared memory
 * @param L
 * @return
 */
int Control::l_addSharedNumber(lua_State *L)
{
    std::string key = lua_tostring(L, -2);
    double value = lua_tonumber(L, -1);

    Shared::addNumber(key, value);

    return 0;
}

/**
 * @brief Control::l_addSharedAgent - Are used for the simulation file to access the shared memory
 * @param L
 * @return
 */
int Control::l_addSharedAgent(lua_State *L)
{
    std::string key = lua_tostring(L, -3);
    std::string agentPath = lua_tostring(L, -2);
    int value = lua_tointeger(L, -1);

    std::pair<std::string, int> temp(agentPath, value);

    Shared::addAgentPathNum(key, temp);

    return 0;
}
