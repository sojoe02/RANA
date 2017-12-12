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
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <stdio.h>
#include <iostream>

#include <argtable2.h>

class parser
{
public:
    parser(int argc, char **argv);
    ~parser();

    bool startProgram();
    bool startGui();
    std::string getFile();


private:
    int _thread = 1;
    std::string _path;
    bool _nogui = false;


    int _port;
    std::string _ipadd;


    bool _verbose = false;
    bool _help = false;
    bool _version = false;
};



#endif // PARSER_H
