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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include <map>

#include "graphics/agentItem.h"
#include "postprocessing/postcontrol.h"
#include "utility.h"
#include "control.h"

typedef std::list<agentInfo> INFOLIST;

class PostControl;
class Control;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void updateMap(QImage *image);

    ~MainWindow();

    void changeRunButton(QString text);
    void advanceProgess(int percentage);
    void runButtonHide();
    void updateMap(std::list<agentInfo> infolist);
    void write_output(QString argMsg);
    void write_status(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents);
    void write_errOutput(QString argMsg);
	//postprocessing:
	void advancePPProgess(int percentage);
	void write_PPOutput(QString argMsg);

	//dialogs:
	void dialogConstruction();
public slots:

    void wheelEvent(QWheelEvent *event);

private slots:

    void on_generateButton_clicked();
    void actionExit();
    void on_browseMapButton_clicked();
    void on_generateMap_clicked();
    void on_browseLuaAgentButton_clicked();
    void on_runButton_clicked();
    void on_updateMap(INFOLIST infolist);
    void on_udateStatus(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents);
    void on_writeOutput(QString string);
    void on_writeErrOutput(QString string);
    void on_delaySpinBox_valueChanged(int arg1);
    void on_zoomSlider_valueChanged(int value);
    void on_pushButton_clicked();
    void actionPrintInfo();

	//postprocessing:

	void ppIsChecked();
    void on_vis_processEventsPushButton_clicked();
	void on_writePPOutput(QString string);

	//dialogs:
	void eventDialog();
	void on_vis_eventBrowsePushButton_clicked();
	void on_vis_readInfoPushButton_clicked();
	void on_vis_agentPathPushButton_clicked();

signals:

    void map_updateSignal(INFOLIST infolist);
    void writeStringSignal(QString something);
    void writeStatusSignal(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents);
    void writeErrSignal(QString something);

	//postprocessing:
	void writePPSignal(QString something);

private:

    Ui::MainWindow *ui;
    void updatePosition(int Id, int x, int y);
    void advanceProgess();
    double factor;
    QImage *mapImage;
    QGraphicsPixmapItem *mapItem;
    QGraphicsScene *scene;
    QMap<int, agentItem* > graphAgents;
    QMutex lock;
    Control *control;
    void defineMap();

	QString versionString;

    //the menu bar:
    QMenu *menu;
    QAction* showAct;
    QAction* hideAct;
    QAction* exitAct;

	void ppConstruction();
    QWidget *vis_controlTabptr;
    QWidget *vis_mapTabptr;

	//eventProcessing things:
	PostControl *postControl;

};

#endif // MAINWINDOW_H
