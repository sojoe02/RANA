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
#ifndef CONTROL_H
#define CONTROL_H

#include <QtGui>
#include <QObject>
#include <QtConcurrent/qtconcurrentrun.h>

//STL libraries:
#include <string>
#include <list>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "src/cli.h"
#include "src/mainwindow.h"
#include "src/utility.h"
#include "src/runner.h"
#include "src/simulationcore/flowcontrol.h"

class Runner;
class FlowControl;
class Cli;
class MainWindow;
class Control : public QObject
{
    Q_OBJECT

    //  Public methods
    public:
        Control(Cli* cli, std::string _file);
        Control(MainWindow* mainwindow);
        ~Control();

        /**
         * @brief initialSetup
         * Initialises LUA for the control.
         */
        void initialSetup();

        /**
        * @brief setEnvironmentVariables
        * Set parameters for the environment.
        * @param map pointer to the loaded image map
        * @param scale amount of m2 pr pixel
        * @param timeRes microstep resolution
        * @param macroRes macrostep resotution
        * @param agentAmount number of Lua agents
        * @param agentPath path to the agent
        */
        void setEnvironmentVariables(QImage *map, int threads, double timeRes,
                                 double macroRes, int agentAmount, std::string agentPath);

        /**
        * @brief generateEnvironment
        * Generates a new environment, an environment is needed to
        * start a simulation run.
        */
        void generateEnvironment();

        bool checkEnvPresence();
        void startSimulation(unsigned long long runTime);
        void stopSimulation();

        void saveExternalEvents(std::string filename);
        void refreshPopPos(std::list<agentInfo> infolist);

        bool isGenerated();
        bool isRunning();

        void saveEvents(QString path);

        void toggleLiveView(bool enable);

        void threadTest(std::string something);

        int newSimulation();

        lua_State * getControlLuaState(){ return L; }

    //  Public slot
    public slots:
        void on_simDone();

    //  Public attributes
    public:
        std::list<double[3]> updatePositions();

    //  Private Methods
    private:
        void readyAgentDomain();
        void killAgentDomain();
        void readyRunner();
        void killRunner();
        void killRunthread();

        bool runNewSimulation();

        static int l_addSharedNumber( lua_State* );
        static int l_addSharedAgent( lua_State* );

    //  Private slot
    private slots:
        void runSimulation();

    //  Private Attributes
    private:
        FlowControl *agentDomain;
        Cli *cli = nullptr;
        MainWindow *mainwindow = nullptr;
        Runner *runner;
        QThread runThread;
        QFuture<void> populateFuture;

        bool running;
        bool stopped;
        bool generated;
        bool generating;

        unsigned long long runTime;

        QImage *map;
        int threads;
        double timeRes;
        double macroRes;
        int agentAmount;
        std::string agentPath;

        int simulationNumber = 0;
        lua_State* L;

    signals:
        void startDoWork(FlowControl *agentDomain, unsigned long long runtime);
        void runSimulationSignal();
};

#endif // CONTROL_H
