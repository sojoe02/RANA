#ifndef INTERFACER_H
#define INTERFACER_H

#include <string>
#include <map>
#include <mutex>

#include "src/simulationcore/agents/agent.h"
#include "src/simulationcore/agents/agentluainterface.h"
#include "src/simulationcore/agents/agentinterface.h"
#include "src/simulationcore/sector.h"
#include "src/simulationcore/supervisor.h"

class Interfacer
{
public:
    static void initInterfacer(Supervisor* arg_master);

	static int addLuaAgent(double x, double y, double z, std::string path, std::string filename);
	static void addLuaAgentPtr(std::shared_ptr<AgentLuaInterface> luaPtr);
	static std::shared_ptr<AgentLuaInterface> getAgentPtr(int id);

	static int addCppAgent(double x, double y, double z, std::string path, std::string filename);
	static void addCppAgentPtr(std::shared_ptr<AgentInterface> cppPtr);
	static std::shared_ptr<AgentInterface> getAgentCppPtr(int id);

	static bool removeAgent(int ID);
	static std::map<int, std::string> getAgentInfo();
	static void submitEEvent(std::unique_ptr<EventQueue::eEvent> eEvent);
	void modifyAgentInfo(std::vector<agentInfo> infolist);

private:

    static Supervisor *master;
	static std::map<int, std::string> agentFilenames;
	static std::map<int, std::shared_ptr<AgentLuaInterface>> agents;
	static std::map<int, std::shared_ptr<AgentInterface>> agentsCpp;
    static std::mutex eventMutex;
    static std::mutex agentMutex;
    static std::mutex agentPtrMutex;
};

#endif // INTERFACER_H
