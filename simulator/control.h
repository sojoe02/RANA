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

//STL libraries:
#include <string>
#include <list>

#include "simulator/core/flowcontrol.h"
#include "simulator/utility/utility.h"

class Runner;

class FlowControl;

class MainWindow;

class Control {
public:

    Control();

    ~Control();

    bool checkEnvPresence();

    void runSimulation(unsigned long long runTime);

    void stopSimulation();

    /**
     * @brief generateEnvironment
     * Generates a new environment, an environment is needed to
     * start a simulation run.
     * @param map pointer to the loaded image map
     * @param scale amount of m2 pr pixel
     * @param timeRes microstep resolution
     * @param macroRes macrostep resotution
     * @param agentAmount number of Lua agents
     * @param agentPath path to the agent
     */
    void generateEnvironment(int threads,
                             double timeRes, double macroRes,
                             int agentAmount, std::string agentPath);

    std::list<double[3]> updatePositions();

    void saveExternalEvents(std::string filename);

    void refreshPopPos(std::list<agentInfo> infolist);

    bool isGenerated();

    bool isRunning();

    void saveEvents(std::string path);

    void toggleLiveView(bool enable);

    void threadTest(std::string something);

    void on_simDone();

    void startDoWork(FlowControl *agentDomain, unsigned long long runtime);

private:

    FlowControl *agentDomain;
    MainWindow *mainwindow;
    Runner *runner;
    //QThread runThread;
    //QFuture<void> populateFuture;

    bool running;
    bool generated;
    bool stopped;
    bool generating;


};

#endif // CONTROL_H
