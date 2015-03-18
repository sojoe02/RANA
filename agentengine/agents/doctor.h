#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <map>

#include "auton.h"
#include "master.h"
#include "nestene.h"

class Doctor
{
public:

	static void InitDoctor(Master* arg_master);
	static int addLuaAuton(double x, double y, double z, std::string path, std::string filename);
	static bool removeAuton(int ID);
	static std::map<int, std::string> getAutonInfo();

private:

	static Master *master;
	static std::map<int, std::string> agentFilenames;
};

#endif // DOCTOR_H
