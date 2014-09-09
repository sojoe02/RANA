#ifndef EVENTPROCESSING_H
#define EVENTPROCESSING_H

#include <QtGui>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>


#include "agentengine/agents/autonLUA.h"
#include "mainwindow.h"
#include "graphics/zblock.h"
#include "eventqueue.h"

class EventProcessing
{

public:
	EventProcessing();
	~EventProcessing();

	void resetEventProcessor();

	void binEvents(std::string path, int from, int to);

	EventQueue::simInfo *readEventInfo(std::string path);
	EventQueue::simInfo *getDataEvent();
	void processBinnedEvents(double timeResolution, std::string path, int mapResolution, double zThresshold);

	void processEvent(EventQueue::dataEvent *event, double thresshold, double mapRes, double timeRes, std::string path);
private:

	std::vector<EventQueue::dataEvent> eventbin;
	std::unordered_map<std::string, ZBlock> *zBlocks;
	//std::unorderd_map<>
	EventQueue::simInfo *simInfo;
	EventQueue::dataEvent devent;

	void recursiveZlevel(AutonLUA *auton, EventQueue::dataEvent *event, std::set<std::string> *visited, int x, int y, int width, int height, double mapRes, double timeRes, double thressholdZ);
};

#endif // EVENTPROCESSING_H
