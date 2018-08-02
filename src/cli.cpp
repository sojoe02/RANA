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

#include <string>
#include <fstream>

#include "src/cli.h"
#include "src/output.h"
#include "src/api/maphandler.h"
#include "src/api/phys.h"
#include "src/api/gridmovement.h"
#include "src/simulationcore/eventqueue.h"

#include <bayesopt/parameters.h>

#include "eventdialog.h"
#include "helpdialog.h"

Cli::Cli(std::string _file, bool flag ) :
    factor(1),
    mapItem(nullptr),
    mapImage(nullptr)
{
    if(flag){
        this->control = new Control(this, _file);
        this->parsedFilePath = _file;
        this->runController();
    }else{
        bayesopt::Parameters param = initialize_parameters_to_default();
        param.n_init_samples = 10;
        param.n_iterations = 1000;

        size_t numParam = 7;

        vectord lb(numParam), ub(numParam);
        lb(0) = 0.50; ub(0) = 2.00; //  -   p2
        lb(1) = 0.01; ub(1) = 0.10; //  -   p3
        lb(2) = 0.00; ub(2) = 0.35; //  -   p4
        lb(3) = 0.00; ub(3) = 0.10; //  -   p5
        lb(4) = 0.00; ub(4) = 1.00; //  -   p6
        lb(5) = 0.00; ub(5) = 0.10; //  -   p7
        lb(6) = 0.00; ub(6) = 0.10; //  -   p8

        this->bopt = new bopthook(this, numParam, param, 11200);
        this->bopt->setBoundingBox(lb, ub);

        bool restoreState = true;
        if( !restoreState ){

            auto initParams = bopt->p1InitializeOptimization();
            vectord initErrorResults(initParams.size1());

            std::cout << initParams << std::endl;
            std::cout << initParams.size1() << " " <<initParams.size2() << std::endl << std::endl;

            /**
              Run the initial bayesOpt sampling
            **/
            for( size_t rows = 0; rows < initParams.size1(); rows++){
                vectord rowOfParam(initParams.size2());
                for(size_t cols= 0; cols < initParams.size2(); cols++){
                    rowOfParam.insert_element(cols,initParams(rows, cols));
                }

                bopt->sentParametersViaTcp(rowOfParam);
                /**
                  Run all simulation sequences here
                **/
                bool tmpFlag = true;
                while(tmpFlag){
                    std::string filePath = bopt->getNextFile();
                    if(filePath != "noMoreFiles"){
                        /**
                            Run simulation
                        **/
                        std::cout <<"Run simulation with - " << filePath<< std::endl;
                        boptRunController(filePath);
                    }else{
                        std::cout << "No more simulations" << std::endl << std::endl;
                        bopt->resetFilePathList();
                        tmpFlag = false;
                    }
                }

                double processedError = bopt->messageSimDoneAndGetError("simsDone\n");
                initErrorResults.insert_element(rows, processedError);

                std::string doneMsg = "notDone\n";
                bopt->sentMessageViaTcp(doneMsg);
            }
            std::cout << "Results from initial sampling:" << std::endl;
            std::cout << initErrorResults << std::endl << std::endl;
            bopt->p2InitializeOptimization(initErrorResults);

        }else{
            bayesopt::BOptState state;
            state.loadFromFile("state_4_agent.dat", param);
            this->bopt->restoreOptimization(state);
        }

        /**
          Run the actual bayesOpt
        **/
        std::cout << "TEST: param.n_iterations" << param.n_iterations << std::endl;
        for(int i = bopt->getCurrentIter(); i < param.n_iterations; i++){
            bopt->stepOptimization();
            std::cout << std::endl;

            std::string doneMsg;
            if (i == param.n_iterations-1){
                doneMsg = "allDone\n";
            }else{
                doneMsg = "notDone\n";
            }
            bopt->sentMessageViaTcp(doneMsg);
            bopt->saveCurrentState();
        }
    }
}


Cli::~Cli()
{
    std::cout << "\tCLI destructor";
}

void Cli::boptRunController(std::string fPath)
{
    this->control = new Control(this, fPath);
    this->parsedFilePath = fPath;
    this->runController();
}

void Cli::runController()
{
    this->control->setupLuaSimulation();
    this->generateMap();
    this->generateSimulation();
    this->runSimulation();
}

void Cli::generateMap()
{
    if(mapImage != nullptr)
    {
        delete mapImage;
    }
    lua_State * L = control->getControlLuaState();
    int width = 100;
    int height = 100;
    lua_settop(L,0); lua_getglobal(L,"_getSimulationConfigurationOption"); lua_pushstring(L,"mapWidth");
    if(lua_pcall(L,1,2,0)!=LUA_OK){ /* Using deafult value */ }
    if( lua_toboolean(L,1) ){ width = lua_tonumber(L,2); }
    lua_settop(L,0); lua_getglobal(L,"_getSimulationConfigurationOption"); lua_pushstring(L,"mapHeight");
    if(lua_pcall(L,1,2,0)!=LUA_OK){ /* Use deafult value */ }
    if( lua_toboolean(L,1) ){ height = lua_tonumber(L,2); }
    mapImage = new QImage(width,height,QImage::Format_RGB32);
    mapImage->fill(Qt::GlobalColor::black);
    this->defineMap();
}

void Cli::defineMap()
{
    //mapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*mapImage));
    MapHandler::setImage(mapImage);
    Phys::setEnvironment(mapImage->width(),mapImage->height());
}

void Cli::generateSimulation()
{
    qApp->processEvents();
    GridMovement::clearGrid();

    lua_State * L = control->getControlLuaState();

    lua_settop(L,0); lua_getglobal(L,"_getSimulationConfigurationOption"); lua_pushstring(L,"mapScale");
    if(lua_pcall(L,1,2,0)!=LUA_OK){ /* Use deafult value */ }
    if( lua_toboolean(L,1) ){ Phys::setScale(lua_tonumber(L,2)); }else{ Phys::setScale(1); }

    if(!parsedFilePath.empty())
    {
        int agentAmount = 1;
        int threads = 4;
        double exponent = 6;
        double timeRes = 1/(double)std::pow(10,exponent);
        double macroRes = 1000;
        macroRes = (1/timeRes)/macroRes;

        lua_settop(L,0); lua_getglobal(L,"_getSimulationConfigurationOption"); lua_pushstring(L,"eDistPrecision");
        if(lua_pcall(L,1,2,0)!=LUA_OK){ /* Use deafult value */ }
        if( lua_toboolean(L,1) ){ timeRes = lua_tonumber(L,2); }

        lua_settop(L,0); lua_getglobal(L,"_getSimulationConfigurationOption"); lua_pushstring(L,"stepPrecision");
        if(lua_pcall(L,1,2,0)!=LUA_OK){ /* Using deafult value */ }
        if( lua_toboolean(L,1) ){ macroRes = 1/lua_tonumber(L,2); }

        lua_settop(L,0); lua_getglobal(L,"_getSimulationConfigurationOption"); lua_pushstring(L,"simThreads");
        if(lua_pcall(L,1,2,0)!=LUA_OK){ /* Use deafult value */ }
        if( lua_toboolean(L,1) ){ threads = lua_tonumber(L,2); }

        control->setEnvironmentVariables(mapImage, threads, timeRes, macroRes, agentAmount, parsedFilePath);
    }else{
        std::cout << "Cannot generate Environment: No valid path to agent" << std::endl;
    }
}

void Cli::runSimulation()
{
    if(isSimulationRunning()){
        control->stopSimulation();
    }
    else
    {
        lua_State * L = control->getControlLuaState();

        lua_settop(L,0); lua_getglobal(L,"_getSimulationConfigurationOption"); lua_pushstring(L,"runTime");
        if(lua_pcall(L,1,2,0)!=LUA_OK){ /* Using deafult value */ }
        if( lua_toboolean(L,1) ){ control->startSimulation(lua_tonumber(L,2)); }else{ control->startSimulation(100); }
    }
}

bool Cli::isSimulationRunning(){
    return control->isRunning();
}

































