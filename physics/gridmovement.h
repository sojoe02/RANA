#ifndef GRIDMOVEMENT_H
#define GRIDMOVEMENT_H

#include <vector>

class GridMovement
{
public:
    GridMovement(){
    };

    static void addPos(int x, int y);
    static bool updatePos(int oldX, int oldY, int newX, int newY);
    static bool checkCollision(int newX, int newY);

    static void initGrid(int height, int width);
private:

    //maps that contain the amount of agent at a givin x,y position
    static std::vector< std::vector<int> > gridPos;

    static int width;
    static int height;


};

#endif // GRIDMOVEMENT_H
