#include <mutex>

#include "communication/outbound.h"
#include "interfacer.h"

Supervisor *Interfacer::supervisor = NULL;
std::map<int, std::string> Interfacer::agentFilenames;
std::map<int, std::shared_ptr<AgentLuaInterface>> Interfacer::agents;
std::map<int, std::shared_ptr<AgentInterface>> Interfacer::agentsCpp;
std::mutex Interfacer::eventMutex;
std::mutex Interfacer::agentMutex;
std::mutex Interfacer::agentPtrMutex;

std::atomic_bool Interfacer::KillSimulation;
std::atomic_bool Interfacer::RunSimulation;
std::atomic_bool Interfacer::SimRunning;

void
Interfacer::initInterfacer(Supervisor *supervisor)
{
  KillSimulation.store(false);
  RunSimulation.store(false);
  SimRunning.store(false);


  Interfacer::supervisor = supervisor;
  agentFilenames.clear();
  agents.clear();
  agentsCpp.clear();
  //activesector = arg_activesector;
}

int
Interfacer::addLuaAgent(double x, double y, double z, std::string path, std::string filename)
{
  std::lock_guard<std::mutex> guard(agentMutex);

  int id = supervisor->addAgent(x, y, z, path, filename, "Lua");
  //Output::Inst()->addGraphicAgent(id,x,y);

  return id;
}

void
Interfacer::addLuaAgentPtr(std::shared_ptr<AgentLuaInterface> luaPtr)
{
  std::lock_guard<std::mutex> guard(agentPtrMutex);
  agents.insert(make_pair(luaPtr->getID(), luaPtr));
}

std::shared_ptr<AgentLuaInterface>
Interfacer::getAgentPtr(int id)
{
  std::lock_guard<std::mutex> guard(agentPtrMutex);

  auto itr = agents.find(id);

  if(itr != agents.end())
        {
	  return itr->second;
	}
  else
	return NULL;
}

int
Interfacer::addCppAgent(double x, double y, double z, std::string path, std::string filename)
{
  std::lock_guard<std::mutex> guard(agentMutex);

  int id = supervisor->addAgent(x, y, z, path, filename, "Cpp");

  return id;
}

void
Interfacer::addCppAgentPtr(std::shared_ptr<AgentInterface> cppPtr)
{
  std::lock_guard<std::mutex> guard(agentPtrMutex);
  agentsCpp.insert(make_pair(cppPtr->getID(), cppPtr));
}

std::shared_ptr<AgentInterface>
Interfacer::getAgentCppPtr(int id)
{
  std::lock_guard<std::mutex> guard(agentPtrMutex);

  auto itr = agentsCpp.find(id);

  if(itr != agentsCpp.end())
	{
	  return itr->second;
	}
  else
	return NULL;
}

bool
Interfacer::removeAgent(int Id)
{
  std::lock_guard<std::mutex> guard(agentMutex);

  if(supervisor->removeAgent(Id))
	{
	  return true;
	}
  return false;
}

std::map<int, std::string>
Interfacer::getAgentInfo()
{
  return agentFilenames;
}

void
Interfacer::modifyAgentInfo(std::vector<agentInfo> infolist)
{
  for(agentInfo &info : infolist)
	{
	  auto &agent = agents[info.id];

	  agent->setPositions(2, 3, 8);

	}
}

void
Interfacer::submitEEvent(std::unique_ptr<EventQueue::eEvent> eEvent)
{
  std::lock_guard<std::mutex> guard(eventMutex);

  supervisor->receiveEEventPtr(std::move(eEvent));
}
