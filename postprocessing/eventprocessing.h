#ifndef EVENTPROCESSING_H
#define EVENTPROCESSING_H

#include <QtGui>
#include <vector>
#include <string>
#include <unordered_map>

#include "../graphics/intensitymap.h"
#include "../agentengine/agents/autonLUA.h"

class EventProcessing
{
public:
    EventProcessing();
	void clearEvents();
	void binEvents(std::string path, int timeInterval);

private:
	std::vector<IntensityMap> intensityMaps;
	typedef std::vector<EventQueue::dataEvent> eventbin;
	std::unordered_map<int,eventbin> *events;
	std::unordered_map<int,eventbin>::iterator eventitr;
	EventQueue::simInfo dataInfo;
	EventQueue::dataEvent devent;
};

#endif // EVENTPROCESSING_H
