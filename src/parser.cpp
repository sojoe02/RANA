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

#include "src/parser.h"


parser* parser::_instance = NULL;
parser* parser::getInstance()
{
    if(_instance == NULL)
        _instance = new parser(); //Not thread-safe version
    return _instance;

    //Note that _instance is *never* deleted -
    //it exists for the entire lifetime of the program!
}

void parser::parseInputArgs(int argc, char **argv)
{
    struct arg_int *thread  = arg_int0("t","thread",NULL,"number of threads (default is 1)");
    struct arg_file *path   = arg_filen(NULL,NULL,NULL,1,argc+2,NULL);
    struct arg_lit  *nogui  = arg_lit0(NULL,"nogui","starts rana with cli");
    struct arg_lit  *verbose= arg_lit0("v","verbose,debug",            "verbose messages");
    struct arg_lit  *help   = arg_lit0(NULL,"help","print this help and exit");
    struct arg_lit  *version= arg_lit0(NULL,"version","print version information and exit");
    struct arg_end  *end    = arg_end(20);
    struct arg_str  *ip     = arg_str0(NULL,"ip",NULL,"IP address for TCP connection");
    struct arg_int  *port   = arg_int0(NULL,"port",NULL,"Port for TCP connection");

    void* argtable[] = {thread,nogui,verbose,help,version,path,ip,port,end};
    arg_parse(argc,argv,argtable);

    if( argtable == NULL)
    {
        std::cout << "insufficient memory" << std::endl;
    }

    if(help->count > 0)
    {
        std::cout << "Usage: RANA" << std::endl;
        arg_print_glossary_gnu(stdout,argtable);
        exit(EXIT_SUCCESS);
    }

    if(version->count > 0)
    {
        std::cout << "RANA 1.8.dev" << std::endl;
        std::cout << "September 2017" << std::endl;
        exit(EXIT_SUCCESS);
    }

    _thread = thread->count;
    _path = *path->filename;
    _nogui = nogui->count;

    if(ip->count > 0 && port->count > 0){
        _enableTcpConnectionFlag = true;
        _ipadd = ip->sval[0];
        _port = port->ival[0];
    }

    std::cout << _ipadd << " asdf " << _port << std::endl;

    _verbose = verbose->count;
    _help = help->count;
    _version = version->count;
}

bool parser::startProgram()
{
    return (!_help && !_verbose && !_version);
}

bool parser::startGui()
{
    return !_nogui;
}

std::string parser::getFile()
{
    return _path;
}

bool parser::enableTcpConnection()
{
    return _enableTcpConnectionFlag;
}
