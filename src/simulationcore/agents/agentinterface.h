#ifndef AGENTINTERFACE_H
#define AGENTINTERFACE_H

#include <random>
#include <vector>
#include <memory>

#include "src/simulationcore/sector.h"
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
    unsigned long long generateEventID();

	//	Physics
    unsigned long long speedOfSound(double x_origin, double y_origin, double x_dest, double y_dest, double propagationSpeed);
    double distance(double x_origin, double y_origin, double x_dest, double y_dest);
    unsigned long long currentTime();
    double currentTimeS();
	int getMacroFactor();
    double getTimeResolution();
    double getMersenneFloat(double low, double high);
    int64_t getMersenneInteger(int64_t low, int64_t high);

	//	Map and movement
	std::vector<int> getEnvironmentSize();
    bool modifyMap(double x, double y, int r, int g, int b);
    rgba checkMap(double x, double y);
    bool checkMapAndChange(double x, double y, int r1, int g1, int b1, int r2, int b2, int g2);
    int radialMapColorScan(int radius, int x, int y, int r, int g, int b);
    int radialMapScan(int radius, int x, int y);

    void addPosition(int x, int y, int id);
    std::list<int> checkPosition(int x, int y);
    void updatePosition(int oldX, int oldY, int newX, int newY, int id);
    bool checkCollision(int x, int y);
    bool checkCollisionRadial(int radius, int x, int y);
    int getMaskRadial(int radius, int x, int y);
    void gridMove(int oldX, int oldY, int newX, int newY);
    double getGridScale();
	void initializeGrid();
    int radialCollisionScan(int radius, int x, int y, int id);
    bool updatePositionIfFree(int x1, int y1, int x2, int y2, int id);

	//	Shared values
	std::string getSharedNumber(std::string key);
	void addSharedNumber(std::string key, double value);
	std::string getSharedString(std::string key);
	void addSharedString(std::string key, double value);

	//	Simulation core
	void stopSimulation();
	std::vector<std::string> getAgentPath();
    int addAgent(int posX, int posY, int posZ, std::__cxx11::string path, std::__cxx11::string filename);
    bool removeAgent(int id);

	//	Agents
    void emitEvent(int id, int posX, int posY, double speed, std::string tableString, std::string desc, int targetID, int targetGroup);
    bool addGroup(int groupID, int id);
    bool removeGroup(int groupID, int id);
    void setMacroFactorMultipler(int id, int macroFactorMultiple);
    bool changeAgentColor(int id, int r, int g, int b, int alpha);

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
    double speed;
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
