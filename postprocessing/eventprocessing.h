#ifndef EVENTPROCESSING_H
#define EVENTPROCESSING_H

#include <QtGui>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <QHash>
#include <QMap>
#include <QMapIterator>


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

	void binEvents(QRegExp regex, std::string path, int from, int to);

	EventQueue::simInfo *readEventInfo(std::string path);
	EventQueue::simInfo *getDataEvent();
	void processBinnedEvents(double timeResolution, std::string path,
							 double mapResolution, double zThresshold);

	void processEvent(EventQueue::dataEvent *event, double thresshold,
					  double mapRes, double timeRes, std::string path);

	QHash<QString, ZBlock *> * getZBlocks();
private:

	std::vector<EventQueue::dataEvent> eventbin;
	QHash<QString, ZBlock*> *zBlocks;
	//std::unorderd_map<>
	EventQueue::simInfo *simInfo;
	EventQueue::dataEvent devent;

	int test_number;

	void recursiveZlevel(AutonLUA *auton, EventQueue::dataEvent *event,
						 QSet<QString> *visited, int x, int y, int displaceX,
						 int displaceY, int width, int height, double mapRes,
						 double timeRes, double thressholdZ);

};

#endif // EVENTPROCESSING_H
