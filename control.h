#ifndef CONTROL_H
#define CONTROL_H

#include <list>
#include <QtGui>

#include "mainwindow.h"
#include "agentengine/agentdomain.h"
#include "utility.h"

class AgentDomain;
class MainWindow;
class Control
{
public:

    Control(MainWindow* mainwindow);

    bool checkEnvPresence();

    void runSimulation(int runTime);

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
    void generateEnvironment(QImage *map, double scale,
                             double timeRes, double macroRes,
                             int agentAmount, std::string agentPath);

    std::list<double[3]> updatePositions();

    void saveExternalEvents(std::string filename);

    void refreshPopPos(std::list<agentInfo> infolist);

private:

    AgentDomain *agentDomain;
    MainWindow *mainwindow;

};

#endif // CONTROL_H
