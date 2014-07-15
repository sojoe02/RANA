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
