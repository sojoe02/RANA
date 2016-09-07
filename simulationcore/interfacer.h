#ifndef INTERFACER_H
#define INTERFACER_H

#include <string>
#include <map>
#include <mutex>

#include "agents/agent.h"
#include "supervisor.h"
#include "sector.h"
#include "agents/agentluainterface.h"

class Interfacer
{
public:

    static void initInterfacer(Supervisor* arg_master);
	static int addLuaAgent(double x, double y, double z, std::string path, std::string filename);
	static bool removeAgent(int ID);
	static std::map<int, std::string> getAgentInfo();	
	static void addLuaAgentPtr(std::shared_ptr<AgentLuaInterface> luaPtr);
	static std::shared_ptr<AgentLuaInterface> getAgentPtr(int id);
	static void submitEEvent(std::unique_ptr<EventQueue::eEvent> eEvent);

private:

    static Supervisor *master;
	static std::map<int, std::string> agentFilenames;
	static std::map<int, std::shared_ptr<AgentLuaInterface>> agents;
    static std::mutex eventMutex;
    static std::mutex agentMutex;
    static std::mutex agentPtrMutex;
};

#endif // INTERFACER_H
