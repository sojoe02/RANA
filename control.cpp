#include "control.h"
#include "mainwindow.h"
#include "output.h"



Control::Control(MainWindow* mainwindow)
    : agentDomain(NULL), mainwindow(mainwindow),runThread(NULL),
      running(false), generated(false), stopped(true)
{

}

void Control::runSimulation(int runTime)
{
    if(agentDomain != NULL && runThread == NULL)
    {
        mainwindow->changeRunButton("Stop");
        Output::Inst()->kprintf("Starting simulation thread");
        running = true;
        runThread = new std::thread(runSimulationThread,this, agentDomain, runTime);
    } else
        Output::Inst()->kprintf("You need an evironment to run...");

}

void Control::stopSimulation()
{
    if(agentDomain != NULL && runThread != NULL)
   {
       Output::Inst()->kprintf("Stopping Simulation");
       agentDomain->stopSimulation();
       runThread->join();
       delete runThread;
       runThread = NULL;
       mainwindow->changeRunButton("Run");
    }
}

void Control::generateEnvironment(QImage *map, double scale,
                                  double timeRes, double macroRes,
                                  int agentAmount, std::string agentPath)
{
    if(runThread == NULL){
        Output::Inst()->kprintf("Generating environment");
        delete agentDomain;

        agentDomain = new AgentDomain(this);

        agentDomain->generateEnvironment(map->width(),map->height(),1,0,0,
                                         agentAmount,timeRes,macroRes,agentPath);
    } else
        Output::Inst()->kprintf("Simulation thread is running, so you need to stop it");
   //retrieve and update the positions:

}

void Control::refreshPopPos(std::list<agentInfo> infolist)
{
    mainwindow->refreshPopulation(infolist);
}

void Control::runSimulationThread(Control *control,
                                  AgentDomain *agentDomain, int runTime)
{
    agentDomain->runSimulation(runTime);
    control->running = false;
}


bool Control::isRunning()
{
     if(runThread != NULL)
         return true;
     else
         return false;
}
