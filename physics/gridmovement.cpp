#include <stdio.h>

#include "gridmovement.h"
#include "output.h"


std::vector< std::vector<int> > GridMovement::gridPos;
std::map< std::string, pList > GridMovement::posMap;
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


void GridMovement::addPos(int x, int y, int id)
{
   //Output::Inst()->kprintf("x%i, y%d", x, y);

   //add id to position map:
   char buffer[64];
   sprintf(buffer,"%i,%i",x,y);
   std::string index = buffer;

   if(posMap.find(index) == posMap.end() )
   {
       pList tmp;
       tmp.push_back(id);
       posMap[index] = tmp;
   } else
   {
       pList *tmp = &posMap.find(index)->second;
       tmp->push_back(id);
   }

   //Add position to the collision matrice;
   GridMovement::gridPos[x][y] += 1;
}

bool GridMovement::updatePos(int oldX, int oldY, int newX, int newY, int id)
{          

    //update the position matrice:
    if(newX > width || newY > height || oldX > width || oldY > height)
        return false;
    else{
        GridMovement::gridPos[oldX][oldY]--;
        GridMovement::gridPos[newX][newY]++;
    }

    //update the position map:
    char buffer[64];
    sprintf(buffer,"%i,%i",oldX,oldY);
    std::string index = buffer;

    std::list<int> *tmp = &posMap.find(index)->second;
    std::list<int>::iterator it = tmp->begin();

    for(; it != tmp->end(); ++it )
    {
        if (*it == id)
        {
            *tmp->erase(it);
            break;
        }

    }
    if( tmp->empty())
        posMap.erase(index);

    if(posMap.find(index) == posMap.end() )
    {
        pList tmp;
        tmp.push_back(id);
        posMap[index] = tmp;
    } else
    {
        pList *tmp = &posMap.find(index)->second;
        tmp->push_back(id);
    }

    sprintf(buffer,"%i,%i",newX,newY);
    index = buffer;

    return true;
}

pList GridMovement::checkPosition(int x, int y)
{
    char buffer[64];
    sprintf(buffer,"%i,%i",x,y);
    std::string index = buffer;

    pList *tmp = &posMap.find(index)->second;

    return *tmp;

}



int GridMovement::checkCollision(int newX, int newY)
{
    return GridMovement::gridPos[newX][newY];
}
