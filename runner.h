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
#ifndef RUNNER_H
#define RUNNER_H

#include <QThread>
#include "agentengine/agentdomain.h"

class AgentDomain;
class Runner : public QThread
{
    Q_OBJECT
public:
    explicit Runner();
    void setParameters(AgentDomain *agentDomain, unsigned long long runTime);

signals:
    void simulationDone();

protected:
    void run();

public slots:

private:
    AgentDomain *agentDomain;
    unsigned long long runTime;
    //void startSimulation(AgentDomain *agentdomain, unsigned long long runTime);

};

#endif // RUNNER_H
