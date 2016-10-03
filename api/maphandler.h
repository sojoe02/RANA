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
#ifndef MAPHANDLER_H
#define MAPHANDLER_H

#include <QtGui>

#include "utility.h"
#include "mainwindow.h"

#include <unordered_map>
#include <vector>
#include <shared_mutex>

typedef std::vector< std::vector<int> > MatriceInt;

class MainWindow;
class MapHandler
{
public:

    MapHandler(MainWindow *parent);
    static void setImage(QImage *argImage);
    static rgba getPixelInfo(int argX, int argY);
    static bool setPixelInfo(int argX, int argY, rgba argValue);
    static MatriceInt drawCircle(int radius, char channel, int posX, int posY);
    static bool checkAndChange(int argX, int argY, rgba check_color, rgba change_color);

signals:

    void map_changed();

private:
    static QImage *image;
    static MainWindow *parent;
    static std::unordered_map<int, MatriceInt> radialMasks;
    static std::shared_timed_mutex mapMutex;
};

#endif // MAPHANDLER_H
