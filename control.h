#ifndef CONTROL_H
#define CONTROL_H


#include <QtGui>

//STL libraries:
#include <string>
#include <list>

#include <QObject>

#include "mainwindow.h"
#include "agentengine/agentdomain.h"
#include "utility.h"
#include "runner.h"

class Runner;
class AgentDomain;
class MainWindow;
class Control : public QObject
{
    Q_OBJECT


public:

    Control(MainWindow* mainwindow);
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
    void generateEnvironment(QImage *map, double scale,
                             double timeRes, double macroRes,
                             int agentAmount, std::string agentPath);

    std::list<double[3]> updatePositions();

    void saveExternalEvents(std::string filename);

    void refreshPopPos(std::list<agentInfo> infolist);

    bool isGenerated();
    bool isRunning();


public slots:
    void simDone();

signals:
    void runOperate();

private:

    AgentDomain *agentDomain;
    MainWindow *mainwindow;
    Runner *runner;
    QThread *runThread;

    bool running;
    bool generated;
    bool stopped;


};

#endif // CONTROL_H
