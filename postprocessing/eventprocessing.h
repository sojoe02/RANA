#ifndef EVENTPROCESSING_H
#define EVENTPROCESSING_H

#include <QtGui>
#include <vector>

#include "../graphics/intensitymap.h"
#include "../agentengine/agents/autonLUA.h"

class EventProcessing
{
public:
    EventProcessing();

private:
	std::vector<IntensityMap> intensityMaps;
};

#endif // EVENTPROCESSING_H
