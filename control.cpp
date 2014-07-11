#include "control.h"
#include "mainwindow.h"
#include "output.h"



Control::Control(MainWindow* mainwindow)
    : agentDomain(NULL), mainwindow(mainwindow)
{

}

void Control::runSimulation(int runTime)
{
    if(agentDomain != NULL)
    {
        agentDomain->runSimulation(runTime);
    }

}

void Control::stopSimulation()
{
    if(agentDomain != NULL)
   {
       agentDomain->stopSimulation();
    }
}

void Control::generateEnvironment(QImage *map, double scale,
                                  double timeRes, double macroRes,
                                  int agentAmount, std::string agentPath)
{
   delete agentDomain;

   agentDomain = new AgentDomain(this);

   agentDomain->generateEnvironment(map->width(),map->height(),1,0,0,
                                    agentAmount,timeRes,macroRes,agentPath);
   //retrieve and update the positions:

}

void Control::refreshPopPos(std::list<agentInfo> infolist)
{
    mainwindow->refreshPopulation(infolist);
}

