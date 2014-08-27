#include "eventprocessing.h"

#include<iostream>
#include<fstream>
#include<stdio.h>

#include "output.h"

EventProcessing::EventProcessing()
{
	events = new std::unordered_map<int, eventbin>();
}

void EventProcessing::clearEvents()
{
	events->clear();
}

void EventProcessing::binEvents(std::string path, int timeInterval)
{
	std::ifstream file(path.c_str(), std::ifstream::binary);

	if(file.is_open())
	{
		file.read(reinterpret_cast<char*>(&dataInfo), sizeof(EventQueue::simInfo));
		Output::Inst()->ppprintf("<b>SIMULATION INFO</b>");
		Output::Inst()->ppprintf(dataInfo.luaFileName);
		Output::Inst()->ppprintf("\nHeight [m]\t:\t%f\n", dataInfo.areaY);
		Output::Inst()->ppprintf( "Width [m]\t:\t%f\n", dataInfo.areaX);
		Output::Inst()->ppprintf("Auton Amount\t:\t%d\n", dataInfo.numberOfAutons);
		Output::Inst()->ppprintf("Event Amount\t:\t%llu\n", dataInfo.eventAmount);
		Output::Inst()->ppprintf("Time Resolution\t:\t%f\n", (double)1/dataInfo.timeResolution);
		Output::Inst()->ppprintf("MacroFactor\t:\t%d\n", dataInfo.macroFactor);

		while(!file.eof())
			file.read(reinterpret_cast<char*>(&devent), sizeof(EventQueue::dataEvent));
		//printf("id: %s, activationtime %llu\n",devent.table, devent.activationTime);
		int bin = devent.activationTime/timeInterval;
		eventitr = events->find(bin);


		if(eventitr != events->end())
		{
			eventbin tmp;
			tmp.push_back(devent);
			events->insert(std::pair<int, eventbin>(bin, tmp));
		} else
		{
			eventbin *tmp = &eventitr->second;
			tmp->push_back(devent);
		}
	}else
		Output::Inst()->ppprintf("File not found ... or worse");
}
