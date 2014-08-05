#include <stdio.h>

#include "gridmovement.h"
#include "output.h"



std::map< std::string, pList > GridMovement::posMap;
int GridMovement::width;
int GridMovement::height;


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

}

bool GridMovement::updatePos(int oldX, int oldY, int newX, int newY, int id)
{          

    //update the position map:
    char buffer[64];
    sprintf(buffer,"%i,%i",oldX,oldY);
    std::string index = buffer;

    pList *tmp = &posMap.find(index)->second;
    pList::iterator it = tmp->begin();

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
