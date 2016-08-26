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
#include <QList>
#include <map>

#include "graphics/agentItem.h"
#include "postprocessing/postcontrol.h"
#include "postprocessing/graphics/zblock.h"
#include "utility.h"
#include "control.h"
#include "postprocessing/graphics/zmap.h"

typedef std::list<agentInfo> INFOLIST;

class PostControl;
class Control;

namespace Ui
{
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
	void write_status(unsigned long long internalEvents, unsigned long long externalEvents);
	void write_regularOutput(QString argMsg);


	//postprocessing:
	void advancePPProgess(int percentage);
	void write_PPOutput(QString argMsg);
	void setProcessEventButton(bool enabled);
	void setupVisualTab(QHash<QString, ZBlock *> *argZBlocks);
	void writeZValue(QString string);
	void setEventSceneRect(int x, int y);
	bool eventFilter(QObject *watched, QEvent *event);

	void addGraphicAuton(int Id, int posX, int posY);
	void removeGraphicAuton(int id);
	void changeGraphicAutonColor(int id, int r, int g, int b, int alpha);
    void enableRunButton(bool enabled);

	//dialogs:
	void dialogConstruction();


public slots:

    void wheelEvent(QWheelEvent *event);

private slots:

    void on_generateButton_clicked();
    void actionExit();
    void on_browseMapButton_clicked();
    void on_browseLuaAgentButton_clicked();
    void on_runButton_clicked();
    void on_updateMap(INFOLIST infolist);
	void on_udateStatus(unsigned long long internalEvents, unsigned long long externalEvents);
    void on_writeOutput(QString string);
	void on_writeRegularOutput(QString string);
    void on_delaySpinBox_valueChanged(int arg1);
    void on_zoomSlider_valueChanged(int value);
    void on_pushButton_clicked();
    void actionPrintInfo();

    void on_enableRunButton(bool enabled);

	void on_addGraphicAuton(int id, int posX, int posY);
    void on_changeGraphicAutonColor(int id, int r, int g, int b, int alpha);
    void on_removeGraphicAuton(int Id);

    void on_initializeTimerTimeout();
    void on_runTimerTimeout();

	//postprocessing:
	void ppIsChecked();
    void on_vis_processEventsPushButton_clicked();
	void on_writePPOutput(QString string);
	void on_vis_readInfoPushButton_clicked();
	void on_vis_agentPathPushButton_clicked();
	void on_vis_mapTypeComboBox_currentIndexChanged(const QString &arg1);
    void on_zMapTimerTimeout();
    void on_resizeTimerTimeout();

	//dialogs:
	void eventDialog(); 
	void on_vis_eventBrowsePushButton_clicked();
	void on_vis_activeMapSpinBox_valueChanged(int arg1);
	void on_vis_stopEventProcessingPushButton_clicked();
	void on_actionDisable_Simulation_Output_toggled(bool arg1);
	void on_vis_eventZoomSlider_valueChanged(int value);
	void on_tabWidget_tabBarClicked(int index);
	void resizeEvent(QResizeEvent* event);
	void on_tabWidget_currentChanged(int index);
	void on_vis_eventPlayPushButton_clicked();
	void on_vis_clearOutputPushButton_clicked();

    void helpDialog();
    void on_action_Enable_Visualisation_triggered(bool checked);
	void on_generateEmptyMapButton_clicked();
    void on_zoomSlider_actionTriggered(int action);
    void on_zoomSlider_sliderMoved(int position);
	void on_macroSpinBox_valueChanged(int arg1);
	void on_timeResSpinBox_valueChanged(int arg1);

	void on_vis_disableAgentsCheckBox_toggled(bool checked);
	void on_vis_disableAgentIDs_toggled(bool checked);
	
signals:

    void map_updateSignal(INFOLIST infolist);
    void writeStringSignal(QString something);
	void writeStatusSignal(unsigned long long internalEvents, unsigned long long externalEvents);
    void writeRegularSignal(QString something);
    void enableRunButtonSignal(bool);

	void addGraphicAutonSignal(int id, int posX, int posY);
    void changeGraphicAutonColorSignal(int id, int r, int b, int g, int alpha);
	void removeGraphicAutonSignal(int Id);

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
	QGraphicsItemGroup *agentGroup;

    QMutex lock;
    Control *control;
    void defineMap();

	QString versionString;

    //the menu bar:
    QMenu *menu;
    QAction* showAct;
    QAction* hideAct;
    QAction* exitAct;
	bool disableSimOutput;

	void ppConstruction();
	QWidget *vis_controlTab;
	QWidget *vis_mapTab;
	QWidget *sim_controlTab;
	QWidget *sim_viewTab;
	QWidget *sim_advancedTab;
	QWidget *sim_general;

	//eventProcessing things:
	PostControl *postControl;
	QHash<QString, ZBlock*> *zBlocks;
	QGraphicsScene *eventScene;
    ZMap *zmap;
    EventQueue::simInfo *siminfo;
	QGraphicsScene *eventMapScene;
	int timeOffset;

    QGraphicsItemGroup *PPactiveAgents;
    typedef QMap<int,agentInfo> agentIDMap;
    QMap<double, agentIDMap> agentpositionMap;
    QList<QGraphicsItem*> groupItems;

    QTimer resizeTimer;
	QTimer *zMapTimer;
    QTimer *initializeTimer;
    QTimer *runTimer;

    ZMode zmode;
	bool disableLiveView;
    bool playingMap;
    bool running;


};

#endif // MAINWINDOW_H
