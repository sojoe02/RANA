#include "gridmovement.h"


std::unordered_set<int> GridMovement::gridPosX;
std::unordered_set<int> GridMovement::gridPosY;




void GridMovement::addPos(int x, int y)
{
    GridMovement::gridPosX.insert(x);
    GridMovement::gridPosY.insert(y);
}

void GridMovement::updatePos(int oldX, int oldY, int newX, int newY)
{
    GridMovement::gridPosX.erase(oldX);
    GridMovement::gridPosY.erase(oldY);

    GridMovement::gridPosX.insert(newX);
    GridMovement::gridPosY.insert(newY);
}



bool GridMovement::checkCollision(int ID, int newX, int newY)
{
    bool collision;

    if (GridMovement::gridPosX.find(newX)==GridMovement::gridPosX.end() &&
            GridMovement::gridPosY.find(newY) == GridMovement::gridPosY.end())
    {
        collision = false;
    } else collision = true;


    return collision;
}
