#ifndef DOCTOR_H
#define DOCTOR_H

#include <map>

#include "auton.h"
#include "master.h"

class Doctor
{
public:
	Doctor();

private:
	static Master *master;
	static std::map<unsigned int, Auton> agents;
};

#endif // DOCTOR_H
