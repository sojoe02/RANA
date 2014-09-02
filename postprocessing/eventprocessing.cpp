#include "eventprocessing.h"


#include<iostream>
#include<fstream>
#include<stdio.h>

#include "output.h"

EventProcessing::EventProcessing()

{
	iBlocks = new std::vector<IntensityBlock>();
	simInfo = new EventQueue::simInfo;
}

void EventProcessing::resetEventProcessor()
{
	iBlocks->clear();
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

void EventProcessing::processBinnedEvents(double timeResolution,
										  int mapResolution, double zThresshold)
{

}
