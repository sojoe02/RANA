#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <map>

#include "auton.h"
#include "master.h"
#include "nestene.h"
#include "autonLUA.h"

class Doctor
{
public:

	static void InitDoctor(Master* arg_master);
	static int addLuaAuton(double x, double y, double z, std::string path, std::string filename);
	static bool removeAuton(int ID);
	static std::map<int, std::string> getAutonInfo();	
	static void addLuaAutonPtr(std::shared_ptr<AutonLUA> luaPtr);
	static std::shared_ptr<AutonLUA> getAutonPtr(int id);
	static void submitEEvent(std::unique_ptr<EventQueue::eEvent> eEvent);

private:

	static Master *master;
	static std::map<int, std::string> agentFilenames;
	static std::map<int, std::shared_ptr<AutonLUA>> agents;
};

#endif // DOCTOR_H
