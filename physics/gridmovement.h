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
#ifndef GRIDMOVEMENT_H
#define GRIDMOVEMENT_H

#include <vector>
#include <list>
#include <unordered_map>

typedef std::list<int> pList;

class GridMovement
{
public:
    GridMovement(){};

    static void initGrid();
    static void addPos(int x, int y, int id);
    static void updatePos(int oldX, int oldY, int newX, int newY, int id);
    static bool checkCollision(int x, int y);
    static pList checkPosition(int x, int y);


	static void clearGrid();
    static void removePos(int id);
private:

    //maps that contain the amount of agents at a givin x,y position
	static std::unordered_map<std::string, pList> *posMap;
	static std::unordered_map<std::string, pList>::iterator positr;

    static int width;
    static int height;


};

#endif // GRIDMOVEMENT_H
