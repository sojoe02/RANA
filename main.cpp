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

#include "mainwindow.h"
#include "ID.h"
#include "physics/phys.h"
#include "output.h"
#include "physics/gridmovement.h"


int ID::aID = 0;
unsigned long long ID::eID = 0;
unsigned long long ID::tmu = 0;
unsigned long long ID::nID = 0;


int main(int argc, char *argv[])
{
    srand(time(0));
    Phys::seedMersenne();
    Output::DelayValue = 0;
	Output::LegacyMode.store(false);
    GridMovement::initGrid();
    qDebug() << rand() << rand() << rand() << rand() << rand() << rand() << rand() ;

    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();

    Output::Inst()->setMainWindow(w);

    w->show();

    return a.exec();
}
