#include "control.h"
#include "mainwindow.h"
#include "output.h"

Control::Control(MainWindow* mainwindow)
    : agentDomain(NULL), mainwindow(mainwindow),
      running(false), generated(false), stopped(true)
{
    runner = new Runner();
    connect(runner, SIGNAL(simulationDone()), this, SLOT(simDone()));
}

Control::~Control()
{
    runThread->quit();
    runThread->wait();
}

void Control::runSimulation(unsigned long long runTime)
{
    running = true;
    mainwindow->changeRunButton("Stop");
    runner->setParameters(agentDomain, runTime);
    runner->start();
}

void Control::stopSimulation()
{
    agentDomain->stopSimulation();
}

void Control::generateEnvironment(QImage *map, double scale,
                                  double timeRes, double macroRes,
                                  int agentAmount, std::string agentPath)
{

    if(!running)
    {
        Output::Inst()->kprintf("Generating environment");

        if(agentDomain != NULL)
        {
            delete agentDomain;
            agentDomain = NULL;
        }

        agentDomain = new AgentDomain(this);
        agentDomain->generateEnvironment(map->width(),map->height(),1,0,0,
                                         agentAmount,timeRes,macroRes,agentPath);
    } else
        Output::Inst()->kprintf("Simulation thread is running, so you need to stop it");
    //retrieve and update the positions:
}

void Control::simDone()
{
    if(agentDomain != NULL)
    {
        delete agentDomain;
        agentDomain = NULL;
    }

    running = false;
    mainwindow->changeRunButton("Run");
    mainwindow->runButtonHide();
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
