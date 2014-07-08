#ifndef GRIDMOVEMENT_H
#define GRIDMOVEMENT_H

#include <unordered_set>

class GridMovement
{
public:
    GridMovement(){
    };

    static void addPos(int x, int y);
    static void updatePos(int oldX, int oldY, int newX, int newY);
    bool checkCollision(int ID, int newX, int newY);

private:

    static std::unordered_set<int> gridPosY;
    static std::unordered_set<int> gridPosX;

};

#endif // GRIDMOVEMENT_H
