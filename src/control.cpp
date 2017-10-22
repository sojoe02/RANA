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
#include "src/simulationcore/parameterspace.h"

Control::Control(Cli* cli, std::string _file) :
    agentDomain(NULL), cli(cli), running(false),
    generated(false), stopped(true), generating(false), agentPath(_file)
{
    initialSetup();
}

Control::Control(MainWindow* mainwindow) :
    agentDomain(NULL), mainwindow(mainwindow), running(false),
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
        module_path.append("./src/modules/?.lua");
        std::string agent_path = Output::Inst()->RanaDir;
        agent_path.append("./src/lua_agents/?.lua");
        cur_path.append(";");
        cur_path.append(module_path);
        cur_path.append(";");
        cur_path.append(Output::Inst()->AgentPath);
        cur_path.append(";");
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
        simLib.append("./src/modules/test_file_back.lua");

        if( luaL_loadfile(L, simLib.c_str()) || lua_pcall(L,0,0,0) ){
            Output::Inst()->kprintf("\tsim file not found");
            std::cout << "\n\n\tfile path: " << simLib << "\n\n" << std::endl;
        }
        else{
            std::cout << "\n\n\tSim file loaded: " << simLib << std::endl;
        }

        //  Remove .lua extension from a file, if there are no ".", just use filepath.
        std::string rawfile;
        size_t lastdot = agentPath.find_last_of(".");
        if (lastdot == std::string::npos){
            rawfile = agentPath;
        }else{
            rawfile = agentPath.substr(0, lastdot);
        }

        if( this->cli != NULL){
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
                lua_getglobal(L,"_testFunc");
                if(lua_pcall(L,0,0,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Can't locate Sim file - 3"); }
            }

            /**
            * @brief lua_getglobal -
            * Function that will go in and do the permutation of parameters
            * the function will keep returning true, as long as a new permutation exists.
            */

            lua_getglobal(L,"_testParamMain");
            if(lua_pcall(L,0,0,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Can't locate Sim file - 4"); }

            bool checkNestedFunc = true;
            while(checkNestedFunc)
            {
                lua_getglobal(L,"_testParamMainCo");
                if(lua_pcall(L,0,1,0)!=LUA_OK){ Output::Inst()->kprintf("Control - Lua_simconfig - Can't locate Sim file - 5"); }

                checkNestedFunc = lua_toboolean(L,-1);
                if (checkNestedFunc)
                {
                    //  Run a new simulation with the new parameters.
                }else
                {
                    //  Just exit, all permutations of parameters done.
                }
            }
        }

    }



    connect(this, &Control::runSimulationSignal, this, &Control::runSimulation);
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
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    if(!running && !generating)
    {
        std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
        generating = true;
        std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
        if(populateFuture.isRunning())
        {
            Output::Inst()->kprintf("A previous system was being populated, it will be cancelled\n");
            populateFuture.cancel();
            populateFuture.waitForFinished();
        }
        Output::KillSimulation.store(true);
        std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
        readyAgentDomain();
        std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;

        generating = false;
    } else{
        Output::Inst()->kprintf("Simulation is being generating or it is running");
    }
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
}

void Control::startSimulation(unsigned long long runTime)
{
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    this->runTime = runTime;
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    emit runSimulationSignal();
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
}

void Control::runSimulation()
{
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    generateEnvironment();
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    readyRunner();
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    running = true;
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    Output::SimRunning.store(true);
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    if(mainwindow != NULL)
    {
        mainwindow->changeRunButton("Stop");
    }
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    emit startDoWork(agentDomain, runTime);
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
}

void Control::on_simDone()
{
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    killRunner();
    killAgentDomain();
    killRunthread();
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    if(mainwindow != NULL)
    {
        mainwindow->changeRunButton("Run");
        mainwindow->runButtonHide();
    }
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    Output::SimRunning.store(false);
    Output::Inst()->kprintf("Simulation Done\n");
    running = false;
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    if(totalNumberOfSimulations > currentNumberOfSimulation){
        currentNumberOfSimulation++;
        updateLuaSimulationConfigs();
        emit runSimulationSignal();
    }
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
}

void Control::updateLuaSimulationConfigs()
{
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
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    killRunner();
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    runner = new Runner();
    runner->moveToThread(&runThread);
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    connect(this, &Control::startDoWork, runner, &Runner::doWork);
    connect(&runThread, &QThread::finished, runner, &QObject::deleteLater);
    connect(runner, &Runner::simulationDone, this, &Control::on_simDone);
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    runThread.start();
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
}

void Control::readyAgentDomain()
{
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    killAgentDomain();
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    agentDomain = new FlowControl(this);

    /**
        When readying the agent domain, serialize the parameters, the agents should use.
    **/
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    agentDomain->generateEnvironment(map->width(),map->height(),threads,agentAmount,timeRes,macroRes,agentPath);
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
    populateFuture = QtConcurrent::run(agentDomain, &FlowControl::populateSystem);
    std::cout << __PRETTY_FUNCTION__ << " " << __LINE__<< " " << std::this_thread::get_id() << std::endl;
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
    if(mainwindow != NULL)
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
    if(agentDomain != NULL){
        agentDomain->toggleLiveView(enable);
    }
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


    std::cout << key << " " << agentPath << " " << value << std::endl;

    Shared::addAgentPathNum(key, temp);

    return 0;
}
