#ifndef GRIDMOVEMENT_H
#define GRIDMOVEMENT_H

#include <vector>
#include <list>
#include <map>

typedef std::list<int> pList;

class GridMovement
{
public:
    GridMovement(){};

    static void initGrid();
    static void addPos(int x, int y, int id);
    static void updatePos(int oldX, int oldY, int newX, int newY, int id);
    static bool checkCollision(int x, int y);
    static pList checkPosition(int x, int y);


    static void clearGrid();
private:

    //maps that contain the amount of agents at a givin x,y position
    static std::map<std::string, pList> *posMap;
    static std::map<std::string, pList>::iterator positr;

    static int width;
    static int height;


};

#endif // GRIDMOVEMENT_H
