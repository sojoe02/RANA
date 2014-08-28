#ifndef EVENTPROCESSING_H
#define EVENTPROCESSING_H

#include <QtGui>
#include <vector>
#include <string>

#include "mainwindow.h"
#include "../graphics/intensityblock.h"
#include "../agentengine/agents/autonLUA.h"

class EventProcessing
{
public:
	EventProcessing();
	void resetEventProcessor();

	void binEvents(std::string path, int from, int to);

	EventQueue::simInfo *readEventInfo(std::string path);
	EventQueue::simInfo *getDataEvent();
private:
	std::vector<EventQueue::dataEvent> eventbin;
	std::vector<IntensityBlock> *iBlocks;
	//std::unorderd_map<>
	EventQueue::simInfo *simInfo;
	EventQueue::dataEvent devent;
};

#endif // EVENTPROCESSING_H
