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

#include <QApplication>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>

#include <string>
#include <fstream>

#include "src/cli.h"
#include "src/output.h"
#include "src/api/maphandler.h"
#include "src/api/phys.h"
#include "src/api/gridmovement.h"
#include "src/simulationcore/eventqueue.h"
#include "src/postprocessing/colorutility.h"

#include "eventdialog.h"
#include "helpdialog.h"

Cli::Cli(std::string _file, QWidget *parent) :
    QMainWindow(parent),
    factor(1),
    mapImage(nullptr),
    mapItem(nullptr),
    control(new Control(this, _file)),
    initializeTimer(new QTimer(this)),
    runTimer(new QTimer(this)),
    parsedFilePath(_file)
{
    /*
    bayesopt::Parameters param = initialize_parameters_to_default();
    param.n_init_samples = 1;
    param.n_iterations = 10;
    param.verbose_level = 0;

    this->bopt = new bopthook(this, 7, param);

    vectord tmp = bopt->samplePoint();
    */
    if(this->bopt != nullptr){

        vectord lb(7), ub(7);
        lb[0] = 0.5; ub[0] = 1.5;
        lb[1] = 0.0; ub[1] = 0.5;
        lb[2] = 0.3; ub[2] = 0.3;
        lb[3] = 0.0; ub[3] = 0.5;
        lb[4] = 0.0; ub[4] = 0.1;
        lb[5] = 0.0; ub[5] = 1.0;
        lb[6] = 0.0; ub[6] = 0.1;

        bopt->setBoundingBox(lb, ub);
        bopt->initializeOptimization();

    }else{
        runController();
    }
}

Cli::~Cli()
{
    std::cout << "\tCLI destructor";
}

void Cli::runBoptController()
{
    std::cout << __PRETTY_FUNCTION__ << " " << QThread::currentThread() << std::endl;

    std::string filepath = bopt->getNextFile();

    this->control = new Control(this, filepath);
    this->control->setupLuaSimulation();
    this->generateMap();
    this->generateSimulation();
    this->runSimulation();
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
    mapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*mapImage));
    MapHandler::setImage(mapImage);
    Phys::setEnvironment(mapImage->width(),mapImage->height());
}

void Cli::generateSimulation()
{
    qApp->processEvents();
    GridMovement::clearGrid();

    if( mapItem != nullptr )
    {
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
        } else
        {
            std::cout << "Cannot generate Environment: No valid path to agent" << std::endl;
        }
    } else
    {
        std::cout << "No map has been loaded, please do that..." << std::endl;
    }
    initializeTimer->start(400);
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

































