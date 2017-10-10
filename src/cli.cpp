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
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>

#include <string>
#include <thread>
#include <chrono>
#include <fstream>

#include "src/cli.h"
#include "src/output.h"
#include "src/api/maphandler.h"
#include "src/api/phys.h"
#include "src/api/gridmovement.h"
#include "src/simulationcore/eventqueue.h"
#include "src/postprocessing/colorutility.h"

#include "eventdialog.h"
#include "helpdialog.h"

Cli::Cli(std::string _file, QWidget *parent) :
    QMainWindow(parent),
    factor(1),
    mapImage(NULL),
    mapItem(NULL),
    control(new Control(this, _file)),
    initializeTimer(new QTimer(this)),
    runTimer(new QTimer(this)),
    parsedFilePath(_file)
{
    Output::Inst()->RanaDir = QCoreApplication::applicationDirPath().toUtf8().constData();

    QString agentPath = QString::fromUtf8(parsedFilePath.c_str());
    QFileInfo fi(agentPath);
    Output::AgentFile=fi.fileName().toStdString();
    Output::AgentPath=fi.path().toStdString().append("/");

    std::cout << "1: " << Output::Inst()->RanaDir << std::endl;
    std::cout << "2: " << Output::Inst()->AgentPath << std::endl;
    std::cout << "3: " << Output::Inst()->AgentFile << std::endl;

    this->generateMap();
    this->generateSimulation();
    this->runSimulation();
}

Cli::~Cli()
{
    std::cout << "hello";
}

void Cli::generateMap()
{
    if(mapImage != NULL)
    {
        delete mapImage;
    }

    mapImage = new QImage(100,100,QImage::Format_RGB32); //TODO: Parse map size
    mapImage->fill(Qt::GlobalColor::black);

    this->defineMap();
}

void Cli::defineMap()
{
    mapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*mapImage));
    MapHandler::setImage(mapImage);
    Phys::setEnvironment(mapImage->width(),mapImage->height());
}

void Cli::generateSimulation()
{
    qApp->processEvents();
    GridMovement::clearGrid();

    if( mapItem != NULL )
    {
        Phys::setScale(1); //TODO: Parse option

        if(!parsedFilePath.empty())
        {
            double exponent = 6;    //TODO: Parse option
            double timeRes = 1/(double)std::pow(10,exponent);
            double macroRes = 1000; //TODO: Parse option
            macroRes = (1/timeRes)/macroRes;

            int threads = 4;        //TODO: Parse option
            int agentAmount = 4;    //TODO: Parse option

            control->setEnvironmentVariables(mapImage, threads, timeRes, macroRes, agentAmount, parsedFilePath);
        } else
        {
            std::cout << "Cannot generate Environment: No valid path to agent" << std::endl;
        }
    } else
    {
        std::cout << "No map has been loaded, please do that..." << std::endl;
    }
    initializeTimer->start(400);
}

void Cli::runSimulation()
{
    if(control->isRunning()){
        control->stopSimulation();
    }
    else
    {
        control->startSimulation(100);  //TODO: Parse option
    }
}




































