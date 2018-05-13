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

#include <QApplication>
#include <stdio.h>
#include <string>

#include "src/ID.h"
#include "src/cli.h"
#include "src/mainwindow.h"
#include "src/api/phys.h"
#include "src/api/gridmovement.h"
#include "src/output.h"

#include "src/parser.h"

#include "bayesopt/bayesopt.hpp"

#include "src/api/tcpserver.h"
#include "src/api/tcpclient.h"

int ID::aID = 0;
unsigned long long ID::eID = 0;
unsigned long long ID::tmu = 0;
unsigned long long ID::nID = 0;


int main(int argc, char **argv)
{
        parser *p = parser::getInstance();
        p->parseInputArgs(argc, argv);

        //srand(time(0));
        Phys::seedMersenne();
        Output::DelayValue = 0;
        Output::LegacyMode.store(false);
        GridMovement::initGrid(1);

        qDebug() << Phys::getMersenneInteger(1, RAND_MAX) << Phys::getMersenneInteger(1, RAND_MAX) << Phys::getMersenneFloat(1, RAND_MAX) <<Phys::getMersenneInteger(1, RAND_MAX) ;

        Cli *c = nullptr;

        if(p->startProgram())
        {
            if(p->startGui()){
                QApplication a(argc, argv);
                std::cout << "Start with gui" << std::endl;
                MainWindow *w = new MainWindow();
                Output::Inst()->setMainWindow(w);
                w->show();
                return a.exec();
            }else{
                std::cout << "Start without gui - " << p->getFile() << std::endl;

                std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
                Cli *c = new Cli(p->getFile());
                std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
                c->runController();
                std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__ << std::endl;
            }
        }







}






















