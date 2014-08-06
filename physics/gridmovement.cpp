#include <stdio.h>

#include "gridmovement.h"
#include "output.h"



std::map< std::string, pList > *GridMovement::posMap;
std::map< std::string, pList >::iterator GridMovement::positr;

int GridMovement::width;
int GridMovement::height;

void GridMovement::initGrid()
{

    posMap = new std::map<std::string, pList>();
}

void GridMovement::clearGrid()
{
    posMap->clear();
}

void GridMovement::addPos(int x, int y, int id)
{
    Output::Inst()->kprintf("ID..X:%i,Y:%i", x, y);

    //add id to position map:
    char buffer[64];
    sprintf(buffer,"%i,%i",x,y);
    std::string index = buffer;

    if(posMap->find(index) == posMap->end() )
    {
        pList tmp;
        tmp.push_back(id);
        posMap->insert(std::pair<std::string, pList >(index,tmp));

    } else
    {
        pList *tmp = &posMap->find(index)->second;
        tmp->push_back(id);
    }

}

void GridMovement::updatePos(int oldX, int oldY, int newX, int newY, int id)
{          
    //update the position map:
    char buffer[64];
    sprintf(buffer,"%i,%i",oldX,oldY);
    std::string index = buffer;

    std::map<std::string, pList>::iterator posItr = posMap->find(index);

    pList *tmp = &posItr->second;
    //pList *tmp = &posMap->find(index)->second;

    if(posItr != posMap->end()){
        //posMap->erase(posItr);
        //tmp = &posMap->find(index)->second;
        //pList *tmp = new pList();
        for(pList::iterator it=tmp->begin(); it != tmp->end(); it++ )
        {
            //Output::Inst()->kprintf("%i,%i, someone have been here!!!", *it,id);
            if (*it == id)
            {
                //Output::Inst()->kprintf("deleting my position %i-%i:%s",*it,id,index.c_str());
                tmp->remove(id);
                break;
            }
        }
        //if (!tmp->empty())
        //    posMap->insert(std::pair<std::string, pList>(index,*tmp));
    } if(tmp->empty()){
        posMap->erase(posItr);
    }

    //Output::Inst()->kprintf("=================");

    sprintf(buffer,"%i,%i",newX,newY);
    index = buffer;

    if(posMap->find(index) == posMap->end() )
    {
        pList tmp;
        tmp.push_back(id);
        posMap->insert(std::pair<std::string, pList >(index,tmp));
        //Output::Inst()->kprintf("inserting %i,%i, into new List", newX, newY);
    } else
    {
        pList *tmp = &posMap->find(index)->second;
        tmp->push_back(id);
        //Output::Inst()->kprintf("inserting %i,%i, into existing List", newX, newY);
    }
    //Output::Inst()->kprintf("%s", index.c_str());
}

bool GridMovement::checkCollision(int x, int y)
{
    char buffer[64];
    sprintf(buffer, "%i,%i",x,y);
    std::string index = buffer;

    positr = posMap->find(index);

    if(positr == posMap->end())
    {
        return false;
    }else return true;

}

pList GridMovement::checkPosition(int x, int y)
{
    char buffer[64];
    sprintf(buffer,"%i,%i",x,y);
    std::string index = buffer;

    pList tmp;

    positr = posMap->find(index);

    if (positr != posMap->end())
    {
        //Output::Inst()->kprintf("returning a list");
        return posMap->find(index)->second;
    }

    //Output::Inst()->kprintf("returning an empty list");
    //pList somelist = posMap.find(index)->second;

    return tmp;
}
