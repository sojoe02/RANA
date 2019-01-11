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

#ifndef ID_H
#define ID_H

class ID {
public :

    static int generateAgentID() {
        ID::aID++;
        return ID::aID;
    }

    static int generatesectorID() {
        ID::nID++;
        return ID::nID;
    }

    static unsigned long long generateEventID() {
        ID::eID++;
        return ID::eID;
    }

    static unsigned long long incrementTime() {
        ID::tmu++;
        return ID::tmu;
    }

    static void resetSystem() {
        ID::aID = 0;
        ID::nID = 0;
        ID::eID = 0;
        ID::tmu = 0;
    }

private :
    static int aID;
    static unsigned long long eID;
    static unsigned long long tmu;
    static unsigned long long nID;

};


#endif // ID_H

