
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<set>

#include "physics/phys.h"
#include "eventprocessing.h"
#include "output.h"
#include "agentengine/agents/autonLUA.h"

EventProcessing::EventProcessing()

{
	zBlocks = new std::unordered_map<std::string, ZBlock>();
	simInfo = new EventQueue::simInfo;
}

EventProcessing::~EventProcessing()
{
	delete zBlocks;
	delete simInfo;
}

void EventProcessing::resetEventProcessor()
{
	zBlocks->clear();
	eventbin.clear();
}

EventQueue::simInfo* EventProcessing::readEventInfo(std::string path)
{
	std::ifstream file(path.c_str(), std::ifstream::binary);

	if(file.is_open())
	{
		file.read(reinterpret_cast<char*>(simInfo), sizeof(EventQueue::simInfo));
		Output::Inst()->ppprintf("<b>SIMULATION INFO</b>");
		Output::Inst()->ppprintf(simInfo->luaFileName);
		Output::Inst()->ppprintf("\nHeight [m]\t:\t%f\n", simInfo->areaY);
		Output::Inst()->ppprintf( "Width [m]\t:\t%f\n", simInfo->areaX);
		Output::Inst()->ppprintf("Auton Amount\t:\t%d\n", simInfo->numberOfAutons);
		Output::Inst()->ppprintf("Event Amount\t:\t%llu\n", simInfo->eventAmount);
		Output::Inst()->ppprintf("Time Resolution\t:\t%f\n", simInfo->timeResolution);
		Output::Inst()->ppprintf("MacroFactor\t:\t%d\n", simInfo->macroFactor);
		Output::Inst()->ppprintf("Simulation runtime\t:%llu\n",simInfo->tmuAmount);

		//while(!file.eof())
		//{
			//file.read(reinterpret_cast<char*>(&devent), sizeof(EventQueue::dataEvent));
			//eventbin.push_back(devent);
		//}
	}else
	{
		Output::Inst()->ppprintf("File not found ... or worse");
	}

	file.close();

	return simInfo;
}

EventQueue::simInfo* EventProcessing::getDataEvent()
{
	return simInfo;
}

void EventProcessing::binEvents(std::string path, int to, int from)
{
	std::ifstream file(path.c_str(), std::ifstream::binary);

	eventbin.clear();

	if(file.is_open())
	{
		file.read(reinterpret_cast<char*>(simInfo), sizeof(EventQueue::simInfo));

		while(!file.eof())
		{
			file.read(reinterpret_cast<char*>(&devent), sizeof(EventQueue::dataEvent));
			int activation = (devent.activationTime + simInfo->macroFactor)/(simInfo->timeResolution);

			if(activation > from && activation < to)
			{
				//Output::Inst()->ppprintf("binnig event with activation %llu",devent.activationTime);
				eventbin.push_back(devent);
			}
		}
	}else
		Output::Inst()->ppprintf("File not found ... or worse");

	file.close();
}

void EventProcessing::processBinnedEvents(double timeResolution, std::string path,
										  int mapResolution, double thresshold)
{
	int xAmount = simInfo->areaX/mapResolution;
	int yAmount = simInfo->areaY/mapResolution;

	//populate the zblok map:
	for (int x = 0; x >= xAmount; x++)
	{
		for(int y=0; y >= yAmount; y++)
		{
			ZBlock block(x,y);
			char buffer[32];
			sprintf(buffer,"%i,%i",x,y);
			std::string key = buffer;
			zBlocks->insert(std::pair<std::string, ZBlock>(key, block));
		}
	}

	//process all the binned events, new c11 style better get used to it:
	for(auto it = begin(eventbin); it != end(eventbin); ++it)
	{
		processEvent(&*it, thresshold,
					 mapResolution, timeResolution, path);
	}

}

void EventProcessing::processEvent(EventQueue::dataEvent *event,
								   double thresshold, double mapRes, double timeRes,
								   std::string path)
{
	AutonLUA *auton =
			new AutonLUA(event->id, event->originX, event->originY, 0, NULL, path);

	std::set<std::string> *visited = new std::set<std::string>();

	int width = simInfo->areaX/mapRes;
	int height = simInfo->areaY/mapRes;

	double z, duration;
	//calculate the z value at origin, to get thresshold value:
	auton->processFunction(event, event->originX, event->originY, z, duration);
	double thressholdZ = z * thresshold;

	recursiveZlevel(auton, event, visited,event->originX, event->originY,
					width, height,mapRes,timeRes, thressholdZ);

}

void EventProcessing::recursiveZlevel(AutonLUA *auton, EventQueue::dataEvent *event,
									  std::set<std::string> *visited,
									  int x, int y, int width, int height,
									  double mapRes, double timeRes, double thressholdZ)
{
	char buffer[32];
	sprintf(buffer,"%i,%i",x, y);
	std::string vkey = buffer;

	if(visited->find(vkey)!=visited->end()){
		return;
	} else{
		visited->insert(vkey);
	}

	double z;
	double duration;

	auton->processFunction(event,(double)x*mapRes,(double)y*mapRes,z,duration);

	double distance = Phys::calcDistance(event->originX, event->originY, x*mapRes, y*mapRes);
	double time = distance/(event->propagationSpeed*timeRes);

	//insert z value and take event duration into account:
	std::unordered_map<std::string, ZBlock>::iterator zitr = zBlocks->find(vkey);
	for(int i = 0; i < duration; i++){
		if(zitr != zBlocks->end())
		{
			zitr->second.addZValue(z, time+i);
		}
	}
	//return if z is below thresshold:
	if (z < thressholdZ)
	{
		return;
	}

	//do the recursive calls:
	if(x+1 <= width)
	{
		recursiveZlevel(auton,event,visited,x+1,y,width,height,mapRes,timeRes,thressholdZ);
	}

	if(y+1 <= height)
	{
		recursiveZlevel(auton,event,visited,x,y+1,width,height,mapRes,timeRes,thressholdZ);
	}

	if(x-1 >= 0)
	{
		recursiveZlevel(auton,event,visited,x-1,y,width,height,mapRes,timeRes,thressholdZ);
	}
	if(y-1 >= 0)
	{
		recursiveZlevel(auton,event,visited,x,y+1,width,height,mapRes,timeRes,thressholdZ);
	}

}
