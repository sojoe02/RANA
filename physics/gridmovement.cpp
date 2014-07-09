#include "gridmovement.h"


std::vector< std::vector<int> > GridMovement::gridPos;


void GridMovement::initGrid(int height, int width)
{
   std::vector< std::vector<int>> tmpPos(width, std::vector<int>(height));

   GridMovement::gridPos = tmpPos;
}


void GridMovement::addPos(int x, int y)
{
   GridMovement::gridPos[x][y];
}

void GridMovement::updatePos(int oldX, int oldY, int newX, int newY)
{
    GridMovement::gridPos[oldX][oldY]--;
    GridMovement::gridPos[newX][newY]--;
}



bool GridMovement::checkCollision(int newX, int newY)
{
    if(GridMovement::gridPos[newX][newY] > 0)
        return true;
    else return false;

}
