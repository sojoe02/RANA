#include "eventrunner.h"
#include "output.h"

EventRunner::EventRunner()
	:eventprocessor(NULL)
{

}

void EventRunner::run()
{
	if(eventprocessor != NULL)
	{

		eventprocessor->binEvents(eventPath.toStdString(),from,to);

		eventprocessor->processBinnedEvents(timeResolution,agentPath.toStdString(),
											mapResolution,zThresshold);

	} else
		Output::Inst()->ppprintf("parameters have not been set");

	emit processingDone();
}


void EventRunner::setParameters(EventProcessing *eventprocessor, QString eventPath, int from, int to, double timeResolution, QString agentPath, int mapResolution, double zThresshold)
{
	this->eventprocessor = eventprocessor;
	this->eventPath = eventPath;
	this->from = from;
	this->to = to;
	this->timeResolution = timeResolution;
	this->agentPath  = agentPath;
	this->mapResolution = mapResolution;
	this->zThresshold = zThresshold;
}


