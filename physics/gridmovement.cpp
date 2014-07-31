#include "gridmovement.h"
#include "output.h"

std::vector< std::vector<int> > GridMovement::gridPos;
int GridMovement::width;
int GridMovement::height;

void GridMovement::initGrid(int height, int width)
{
   std::vector< std::vector<int> > tmpPos(width, std::vector<int>(height));
   GridMovement::gridPos = tmpPos;
   //Output::Inst()->kprintf("width %i, height", GridMovement::gridPos.size());
   GridMovement::width = width;
   GridMovement::height = height;
}


void GridMovement::addPos(int x, int y)
{
   //Output::Inst()->kprintf("x%i, y%d", x, y);
   GridMovement::gridPos[x][y] += 1;
}

bool GridMovement::updatePos(int oldX, int oldY, int newX, int newY)
{
    if(newX > width || newY > height || oldX > width || oldY > height)
        return false;
    else{
        GridMovement::gridPos[oldX][oldY]--;
        GridMovement::gridPos[newX][newY]++;
    }
    return true;
}



int GridMovement::checkCollision(int newX, int newY)
{
    return GridMovement::gridPos[newX][newY];
}
