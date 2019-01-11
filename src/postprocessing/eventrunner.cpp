#include "eventrunner.h"
#include "output.h"

EventRunner::EventRunner()
        : eventprocessor(NULL)
{

}

void EventRunner::run()
{
    if (eventprocessor != NULL)
    {
        eventprocessor->binEvents(regex, eventPath.toStdString(), from, to);

        eventprocessor->processBinnedEvents(timeResolution, agentPath.toStdString(),
                                            mapResolution, zThresshold);

    }
    else
        Output::kprintf("parameters have not been set");

    emit processingDone();
}


void EventRunner::setParameters(QRegExp regex, EventProcessing *eventprocessor, QString eventPath, int from, int to,
                                double timeResolution, QString agentPath, int mapResolution, double zThresshold)
{
    Output::kdebug("path of agent is %s,\n event are at %s", agentPath.toStdString().c_str(),
                   eventPath.toStdString().c_str());
    this->regex = regex;
    this->eventprocessor = eventprocessor;
    this->eventPath = eventPath;
    this->from = from;
    this->to = to;
    this->timeResolution = timeResolution;
    this->agentPath = agentPath;
    this->mapResolution = mapResolution;
    this->zThresshold = zThresshold;
}


