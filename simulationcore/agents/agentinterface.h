#ifndef AGENTINTERFACE_H
#define AGENTINTERFACE_H

#include <random>
#include <vector>
#include <memory>

#include "../sector.h"
#include "agent.h"

class Sector;
class AgentInterface : public Agent
{
public:
	AgentInterface(int ID, double posX, double posY, double posZ, Sector *sector, std::string filename);
	~AgentInterface();

	bool operator==(AgentInterface &other) const;
	bool operator!=(AgentInterface &other) const;

    /*******************************************
    * Functions
    ********************************************/

	//	Interface
	void debug( std::string string );
	void print( std::string string );
	int generateEventID();

	//	Physics
	int speedOfSound();
	int distance();
	int currentTime();
	int currentTimeS();
	int getMacroFactor();
	int getTimeResolution();
	int getMersenneFloat();
	int getMersenneInteger();

	//	Map and movement
	std::vector<int> getEnvironmentSize();
	int modifyMap();
	rgba checkMap();
	int checkMapAndChange();
	int radialMapScan();
	int radialMapColorScan();

	void addPosition();
	int checkPosition();
	int updatePosition();
	int checkCollision();
	int checkCollisionRadial();
	int getMaskRadial();
	void gridMove();
	int getGridScale();
	void initializeGrid();
	int radialCollisionScan();
	int updatePositionIfFree();

	//	Shared values
	std::string getSharedNumber(std::string key);
	void addSharedNumber(std::string key, double value);
	std::string getSharedString(std::string key);
	void addSharedString(std::string key, double value);

	//	Simulation core
	void stopSimulation();
	std::vector<std::string> getAgentPath();
	int addAgent();
	int removeAgent();

	//	Agents
	void emitEvent();
	int addGroup();
	int removeGroup();
	void setMacroFactorMultipler();
	int changeAgentColor();

	//	Panic
	void panic(std::string string);

	void processFunction(EventQueue::dataEvent *devent, double mapRes, double x, double y, double &zvalue, double &duration);
	void InitializeAgent();

private:
	std::unique_ptr<EventQueue::iEvent> processEvent(const EventQueue::eEvent *event);
	std::unique_ptr<EventQueue::eEvent> handleEvent(std::unique_ptr<EventQueue::iEvent> eventPtr);
	std::unique_ptr<EventQueue::eEvent> takeStep();

	void movement();
    double destinationX;
    double destinationY;
    double speed; //meters pr second
    bool moving;
    bool gridmove;

    void setRemoved();
    void simDone();
    double eventChance();

    std::string filename;

    friend class Sector;
    friend class Supervisor;
    bool nofile;
    bool removed;

    double moveFactor;
    void getSyncData();

    bool initiated = false;
};

#endif // AGENTINTERFACE_H
