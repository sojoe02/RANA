#include "control.h"

#include "agentengine/agentdomain.h"

Control::Control(MainWindow* mainwindow)
    :agentDomain(NULL), mainwindow(mainwindow)
{

}

void Control::generateEnvironment(QImage *map, double scale,
                                  double timeRes, double macroRes,
                                  int agentAmount, std::string agentPath)
{
    delete agentDomain;

    agentDomain = new AgentDomain(mainwindow);

    agentDomain->generateEnvironment(map->width(),map->height(),1,0,0,
                                     agentAmount,timeRes,macroRes,agentPath
                                     );

    //retrieve and update the positions:

}

