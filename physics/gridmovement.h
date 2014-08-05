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

    static void addPos(int x, int y, int id);
    static bool updatePos(int oldX, int oldY, int newX, int newY, int id);
    static int checkCollision(int newX, int newY);
    pList checkPosition(int x, int y);


private:

    //maps that contain the amount of agents at a givin x,y position
    static std::vector< std::vector<int> > gridPos;
    static std::map<std::string, pList> posMap;

    static int width;
    static int height;


};

#endif // GRIDMOVEMENT_H
