//--begin_license--
//
//Copyright 	2013 	Søren Vissing Jørgensen.
//			2014	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.  
//
//This file is part of RANA.
//
//RANA is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//RANA is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with RANA.  If not, see <http://www.gnu.org/licenses/>.
//
//--end_license--
#include <stdio.h>

#include "src/api/gridmovement.h"
#include "src/output.h"

std::unordered_map< std::string, pList > *GridMovement::posMap = NULL;
std::shared_timed_mutex GridMovement::gridMutex;

int GridMovement::width;
int GridMovement::height;
double GridMovement::scale = 1;

void GridMovement::initGrid(int scale)
{
    std::lock_guard<std::shared_timed_mutex> writerLock(gridMutex);

    if(posMap != NULL)
    {
        delete posMap;
    }

    posMap = new std::unordered_map<std::string, pList>();
    GridMovement::scale = scale;
}

void GridMovement::clearGrid()
{
    std::lock_guard<std::shared_timed_mutex> writerLock(gridMutex);
    posMap->clear();
}

double GridMovement::getScale()
{
    return scale;
}

void GridMovement::addPos(int x, int y, int id)
{

    std::lock_guard<std::shared_timed_mutex> writerLock(gridMutex);
    //Output::Inst()->kprintf("ID..X:%i,Y:%i", x, y);

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

void GridMovement::removePos(int id)
{
    std::lock_guard<std::shared_timed_mutex> writerLock(gridMutex);

    for(auto posItr=posMap->begin(); posItr!=posMap->end(); ++posItr)
    {
        if(posItr != posMap->end())
        {
            pList *tmp = &posItr->second;

            for(auto it=tmp->begin(); it != tmp->end(); ++it)
            {
                if(it != tmp->end())
                {
                    if(*it == id)
                    {
                        //Output::Inst()->kprintf("remove something id'ish...%i,%i", *it,id);
                        tmp->remove(id);
                        break;
                    }
                }
            }
            if(tmp->empty())
            {
               posMap->erase(posItr);
            }
        }
    }

}

void GridMovement::updatePos(int oldX, int oldY, int newX, int newY, int id)
{          
    std::lock_guard<std::shared_timed_mutex> writerLock(gridMutex);
    //update the position map:
    char buffer[64];
    sprintf(buffer,"%i,%i",oldX,oldY);
    std::string index = buffer;

    auto posItr = posMap->find(index);
    pList *tmp = &posItr->second;

    if(posItr != posMap->end())
    {
        for(auto it=tmp->begin(); it != tmp->end(); it++ )
        {
            if (*it == id)
            {
                tmp->remove(id);
                break;
            }
        }
        if(tmp->empty())
        {
          posMap->erase(posItr);
        }
    }

    sprintf(buffer,"%i,%i",newX,newY);
    index = buffer;

    if(posMap->find(index) == posMap->end() )
    {
        pList tmp;
        tmp.push_back(id);
        posMap->insert(std::pair<std::string, pList >(index,tmp));
    }
    else
    {
        pList *tmp = &posMap->find(index)->second;
        tmp->push_back(id);
    }
}


bool GridMovement::updateIfFree(int oldX, int oldY, int newX, int newY, int id)
{
    std::lock_guard<std::shared_timed_mutex> writerLock(gridMutex);

    char buffer[64];
    sprintf(buffer,"%i,%i", oldX, oldY);
    std::string index = buffer;

    auto posItr = posMap->find(index);
    pList *tmp = &posItr->second;

    if(posItr != posMap->end())
    {

        for(auto it=tmp->begin(); it != tmp->end(); it++ )
        {
            if (*it == id)
            {
                tmp->remove(id);
                break;
            }
        }
        if(tmp->empty())
        {
            posMap->erase(posItr);
        }
    }

    sprintf(buffer,"%i,%i",newX,newY);
    index = buffer;

    if(posMap->find(index) == posMap->end() )
    {
        pList tmp;
        tmp.push_back(id);
        posMap->insert(std::pair<std::string, pList >(index,tmp));
    }
    else
    {
        return true;
    }
    return false;
}


bool GridMovement::checkCollision(int x, int y)
{
    std::shared_lock<std::shared_timed_mutex> readerLock(gridMutex);

    char buffer[64];
    sprintf(buffer, "%i,%i",x,y);
    std::string index = buffer;

    auto positr = posMap->find(index);

    if(positr == posMap->end())
    {
        return false;
    }else return true;

}

pList GridMovement::checkPosition(int x, int y)
{
    std::shared_lock<std::shared_timed_mutex> readerLock(gridMutex);
    char buffer[64];
    sprintf(buffer,"%i,%i",x,y);
    std::string index = buffer;

    pList tmp;

    auto positr = posMap->find(index);

    if (positr != posMap->end())
    {
        //Output::Inst()->kprintf("returning a list");
        return posMap->find(index)->second;
    }

    //Output::Inst()->kprintf("returning an empty list");
    //pList somelist = posMap.find(index)->second;

    return tmp;
}
