#include <mutex>

#include "doctor.h"
#include "output.h"

Master* Doctor::master = NULL;
std::map<int, std::string> Doctor::agentFilenames;
std::map<int, std::shared_ptr<AutonLUA>> Doctor::agents;
std::mutex Doctor::eventMutex;

void Doctor::InitDoctor(Master *arg_master)
{
	master = arg_master;
    agentFilenames.clear();
    agents.clear();
	//activeNestene = arg_activeNestene;
}

int Doctor::addLuaAuton(double x, double y, double z, std::string path, std::string filename)
{

	int id = master->addAuton(x, y, z, path, filename, "Lua");
	//Output::Inst()->addGraphicAuton(id,x,y);

	return id;
}

std::map<int, std::string> Doctor::getAutonInfo()
{
	return agentFilenames;
}

bool Doctor::removeAuton(int Id)
{
	if(master->removeAuton(Id))
	{
		Output::Inst()->removeGraphicAuton(Id);
		return true;
	} return false;
}

void Doctor::addLuaAutonPtr(std::shared_ptr<AutonLUA> luaPtr)
{
    agents.insert(make_pair(luaPtr->getID(), luaPtr));
}

void Doctor::submitEEvent(std::unique_ptr<EventQueue::eEvent> eEvent)
{
    std::lock_guard<std::mutex> guard(eventMutex);
    master->receiveEEventPtr(std::move(eEvent));
}

std::shared_ptr<AutonLUA> Doctor::getAutonPtr(int id)
{
	auto itr = agents.find(id);

	if(itr != agents.end())
	{
		return itr->second;
	} else return NULL;
}




