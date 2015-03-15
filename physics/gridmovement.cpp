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

#include "gridmovement.h"
#include "output.h"



std::unordered_map< std::string, pList > *GridMovement::posMap;
std::unordered_map< std::string, pList >::iterator GridMovement::positr;

int GridMovement::width;
int GridMovement::height;

void GridMovement::initGrid()
{

	posMap = new std::unordered_map<std::string, pList>();
}

void GridMovement::clearGrid()
{
    posMap->clear();
}

void GridMovement::addPos(int x, int y, int id)
{
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
            //if(tmp->empty())
            //{
              //  posMap->erase(posItr);
            //}
        }
    }

}

void GridMovement::updatePos(int oldX, int oldY, int newX, int newY, int id)
{          
    //update the position map:
    char buffer[64];
    sprintf(buffer,"%i,%i",oldX,oldY);
    std::string index = buffer;

    auto posItr = posMap->find(index);

    pList *tmp = &posItr->second;

    if(posItr != posMap->end()){
        for(auto it=tmp->begin(); it != tmp->end(); it++ )
        {
            if (*it == id)
            {
                tmp->remove(id);
                break;
            }
        }
        if(tmp->empty()){
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
        //Output::Inst()->kprintf("inserting %i,%i, into new List", newX, newY);
    } else
    {
        pList *tmp = &posMap->find(index)->second;
        tmp->push_back(id);
    }
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
