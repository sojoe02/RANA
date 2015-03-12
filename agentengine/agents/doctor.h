#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>

#include "auton.h"
#include "master.h"
#include "nestene.h"

class Doctor
{
public:

	static void InitDoctor(Master* arg_master);
	static int addLuaAuton(double x, double y, double z, std::string path);
	static bool removeAuton(int ID);

private:
	static Master *master;
};

#endif // DOCTOR_H
