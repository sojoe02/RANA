#include "doctor.h"
#include "output.h"

Master* Doctor::master = NULL;
std::map<int, std::string> Doctor::agentFilenames;

void Doctor::InitDoctor(Master *arg_master)
{
	master = arg_master;
	agentFilenames.clear();
	//activeNestene = arg_activeNestene;
}

int Doctor::addLuaAuton(double x, double y, double z, std::string path, std::string filename)
{

	int id = master->addAuton(x, y, z, path, filename, "Lua");
	Output::Inst()->addGraphicAuton(id,x,y);

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



