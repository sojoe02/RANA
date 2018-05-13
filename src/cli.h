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
#ifndef CLI_H
#define CLI_H

#include <ctime>

#include <QObject>
#include <QList>
#include <map>

#include "src/control.h"
#include "src/utility.h"
#include "src/graphics/agentItem.h"
#include "src/postprocessing/postcontrol.h"
#include "src/postprocessing/graphics/zblock.h"
#include "src/postprocessing/graphics/zmap.h"

#include "src/bopthook.h"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

typedef std::list<agentInfo> INFOLIST;

class bopthook;
class PostControl;
class Control;

class Cli: public QObject
{
    Q_OBJECT

public:
    explicit Cli(std::string _file);
    ~Cli();

    void runController();
    void runBoptController();
    bool isSimulationRunning();

    bopthook *bopt = nullptr;

private:
    void defineMap();
    void generateMap();
    void generateSimulation();
    void runSimulation();

private:
    double factor;
    QGraphicsPixmapItem *mapItem;
    QImage *mapImage;
    Control *control;
    std::string parsedFilePath;

    QTimer *runTimer;

};


#endif // CLI_H






























