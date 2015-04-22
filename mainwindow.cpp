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
#include <string>
#include <thread>
#include <chrono>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "physics/maphandler.h"
#include "physics/phys.h"
#include "physics/gridmovement.h"
#include "output.h"
#include "eventqueue.h"
#include "postprocessing/colorutility.h"

#include "eventdialog.h"


//QImage *MainWindow::mapItem = NULL;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),factor(1),
	mapImage(NULL), mapItem(NULL),scene(new QGraphicsScene()),
	control(new Control(this)),disableSimOutput(false),
	postControl(new PostControl(this)),zBlocks(NULL),
	eventScene(new QGraphicsScene()), zmap(NULL), eventMapScene(new QGraphicsScene()),
	zMapTimer(new QTimer(this)),playingMap(false)
{

	this->setWindowTitle("RANA QT version");

    ui->setupUi(this);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->graphicsView->setScene(scene);
    scene->setBackgroundBrush(Qt::gray);
	ui->runButton->setDisabled(true);
	ui->adv_spinBox->hide();

    qRegisterMetaType<INFOLIST>("INFOLIST");

    QObject::connect(this,SIGNAL(map_updateSignal(INFOLIST)),
                     this,SLOT(on_updateMap(INFOLIST)));

    QObject::connect(this,SIGNAL(writeStringSignal(QString)),
                     this,SLOT(on_writeOutput(QString)));

	QObject::connect(this,SIGNAL(writeRegularSignal(QString)),
					 this,SLOT(on_writeRegularOutput(QString)));

    QObject::connect(this,SIGNAL(writeStatusSignal(unsigned long long,unsigned long long,unsigned long long,unsigned long long)),
                     this,SLOT(on_udateStatus(unsigned long long,unsigned long long,unsigned long long,unsigned long long)));

	QObject::connect(this,SIGNAL(addGraphicAutonSignal(int,int,int)),
						this,SLOT(on_addGraphicAuton(int,int,int)));

	QObject::connect(this, SIGNAL(removeGraphicAutonSignal(int)),
					 this, SLOT(on_removeGraphicAuton(int)));

    //connect actions:
    QObject::connect(ui->action_Exit, SIGNAL(triggered()),this, SLOT(actionExit()));
    QObject::connect(ui->action_Info, SIGNAL(triggered()),this, SLOT(actionPrintInfo()));

	versionString = QString("<b><font color=\"green\">RANA</b></font> version 1.3.19.THREAD:0.6.1");

	ui->statusBar->addWidget(new QLabel(versionString));
	ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

	sim_controlTab = ui->simControlTab;
	sim_viewTab = ui->simLiveView;
	sim_advancedTab = ui->simAdvancedTab;
	sim_general = ui->simGeneralWidget;

	ppConstruction();
	dialogConstruction();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_generateButton_clicked
 * Generates an simulation, if a valid map has been loaded or generated.
 *
 */

void MainWindow::on_generateButton_clicked()
{
	int i = 0;
	for(auto iter=graphAgents.begin(); iter!=graphAgents.end(); ++iter, i++) {
		//Output::Inst()->kprintf("item #%i ", i);
		scene->removeItem(*iter);
		delete *iter;
	}


	ui->graphicsView->viewport()->update();

    graphAgents.clear();
    GridMovement::clearGrid();

	if(mapItem != NULL){

		//mapItem->setPixmap(QPixmap::fromImage(*mapImage));
		//mapItem->setZValue(1);

		Phys::setScale(ui->scaleDoubleSpinBox->value());
        //Output::Inst()->kprintf("Setting map scale to %f", Phys::getScale());

        ui->progressBar->setValue(0);
        QFile path(ui->agentPathLineEdit->text());
        if(path.exists())
		{
            double timeRes = 1/(double)ui->timeResSpinBox->value();
            double macroRes = ui->macroSpinBox->value();
            int agentAmount = ui->luaSpinBox->value();
			QString agentPath = ui->agentPathLineEdit->text();

			//set the global agent loader path variables:
			QFileInfo fi(agentPath);
			Output::AgentFile=fi.fileName().toStdString();
			Output::AgentPath=fi.path().toStdString().append("/");

            std::string stringPath = agentPath.toUtf8().constData();

			//generate the simulation and initialize agents
			control->generateEnvironment(mapImage, 1,timeRes, macroRes,
                                         agentAmount,stringPath);

            //Output::Inst()->kprintf("generating environment, %d, %s",
                          //          agentAmount, stringPath.c_str());
			ui->runButton->setEnabled(true);



        } else
			Output::Inst()->kprintf("Cannot generate Environment: Valid path not given");
    } else
		Output::Inst()->kprintf("No map has been loaded, please do that...");
}

/**
 * @brief Progress bar control
 * @param percentage value between 1 and 100 that depicts the current progress
 * of simulation.
 */
void MainWindow::advanceProgess(int percentage)
{
	//QCoreApplication::postEvent(scene, new QEvent(QEvent::UpdateRequest),
	//                          Qt::LowEventPriority);
    QMetaObject::invokeMethod(ui->progressBar, "setValue", Q_ARG(int, percentage));
    //ui->progressBar->setValue(percentage);
}

void MainWindow::actionExit(){
    QApplication::quit();
}

/**
 * @brief Enables the user to load regular images as a map.
 * Launches a filebrowser, that browses for a png,jpg or bmp image,
 * this image can then be used as a map.
 *
 */
void MainWindow::on_browseMapButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName
            (this, tr("Open Map File"),QDir::currentPath(),
             tr("Image Files (*.png *.jpg *.bmp)"));

    ui->mapPathLineEdit->setText(fileName);

    if(!fileName.isEmpty()){
        delete mapImage;
        mapImage = new QImage(ui->mapPathLineEdit->text());
        if(mapImage->isNull()){
            QMessageBox::information(this,tr("Image Viewer"),
                                     tr("Cannot Load %1.").arg(fileName));
            return;
        }
        defineMap();
    }
}

/**
 * @brief Enables the user to generate a map.
 * The user can define a random percentage of red, green and blue pixels,
 * each channel is handeled seperately.
 *
 */
void MainWindow::on_generateMap_clicked()
{
	if(mapImage != NULL)
		delete mapImage;

    mapImage = new QImage(ui->pxSpinBox->value(), ui->pySpinBox->value(),
                       QImage::Format_RGB32);

	mapImage->fill(Qt::GlobalColor::white);

    QRgb value = qRgb(0,0,0);

    for(int x = 0; x < mapImage->width(); x++)
    {
        for(int y = 0; y < mapImage->height(); y++)
        {
            int Bvalue = 0;
            int Gvalue = 0;
            int Rvalue = 0;

            if(Phys::getMersenneFloat(0,1) < ui->RdoubleSpinBox->value())
            {
                Rvalue = 255;
            }
            if(Phys::getMersenneFloat(0,1) < ui->GDoubleSpinBox->value())
            {
                Gvalue = 255;
            }
            if(Phys::getMersenneFloat(0,1) < ui->BDoubleSpinBox->value())
            {
                Bvalue = 255;
            }
            value = qRgb(Rvalue,Gvalue,Bvalue);
            mapImage->setPixel(x,y, value);
        }
    }

   defineMap();
}

/**
 * @brief Writes a debug string to the current active output window.
 * The active output window will change depending on which mode the simulator
 * is in. This output can be disabled in the menu.
 * @param string the string that will be written to the output.
 */
void MainWindow::on_writeOutput(QString string)
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    //QString prepend = "</>";
	ui->outputTextBrowser->setTextColor(Qt::black);

	if(!ui->action_Enable_Visualisation->isChecked())
	{
		ui->outputTextBrowser->append(string);
	} else
	{
		ui->vis_outputTextBrowser->append(string);
	}
	//QString output = string.prepend(prepend);

	//ui->outputTextBrowser->insertHtml(string);
	//ui->outputTextBrowser->append("");

}

/**
 * @brief MainWindow::write_output
 *
 * @see on_writeOutput()
 */
void MainWindow::write_output(QString argMsg)
{
	if(!disableSimOutput){
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		emit writeStringSignal(argMsg);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

}

/**
 * @brief Writes a regular string to the current active window.
 * Unlike on_writeOutput() this can not be disabled.
 * @param string
 */
void MainWindow::on_writeRegularOutput(QString string)
{
	ui->outputTextBrowser->setTextColor(Qt::black);
	if(!ui->action_Enable_Visualisation->isChecked())
	{
		ui->outputTextBrowser->append(string);
	} else
	{
		ui->vis_outputTextBrowser->append(string);
	}
}

/**
 *
 * @see on_writeRegularOutput()
 * @see on_writeOutput()
 */
void MainWindow::write_regularOutput(QString argMsg)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	emit writeRegularSignal(argMsg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

}

/**
 * @brief Updates the simulation status window
 * Updates dedicated status labels with the parameters given
 * @param ms current microstep
 * @param eventInit how many events in total that has been initiated
 * @param internalEvents how many internal events that has been initiated
 * @param externalEvents how many external events that has been initiated
 * @see Output::updateStatus()
 * @see Master::printStatus()
 */
void MainWindow::on_udateStatus(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents)
{
    ui->label_status1->setText(QString().setNum(ms));
    ui->label_status2->setText(QString().setNum(eventInit));
    ui->label_status3->setText(QString().setNum(internalEvents));
    ui->label_status4->setText(QString().setNum(externalEvents));
}

/**
 * @brief Controls the status window update signal
 * @see on_updateStatus
 */
void MainWindow::write_status(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents)
{
    emit writeStatusSignal(ms, eventInit, internalEvents, externalEvents);
}


void MainWindow::updateMap(std::list<agentInfo> infolist)
{
    emit map_updateSignal(infolist);
}

/**
 * @brief Updates the agents in the live view graphicsview
 * @param infolist list containing the ids and and positions of all
 * currently active agents .
 * @see Control::updatePositions()
 * @see Master::retrievePopPos()
 */
void MainWindow::on_updateMap(INFOLIST infolist)
{
    //Output::Inst()->kprintf("updating map fired...");

    mapItem->setPixmap(QPixmap::fromImage(*mapImage));
    mapItem->setZValue(1);

    INFOLIST::iterator itr;

    for(itr = infolist.begin(); itr != infolist.end(); itr++)
    {
        int x = itr->x;
        int y = itr->y;
		int Id = itr->id;

        if(!graphAgents.contains(Id))
        {
			addGraphicAuton(Id, x, y);

        } else
        {
            QMap<int, agentItem*>::const_iterator i = graphAgents.find(Id);
            agentItem *gfxItem = i.value();
            gfxItem->setX(x);
            gfxItem->setY(y);
        }
	}

	ui->graphicsView->viewport()->update();

}

void MainWindow::addGraphicAuton(int id, int posX, int posY)
{
	emit addGraphicAutonSignal(id, posX, posY);
}

void MainWindow::on_addGraphicAuton(int Id, int posX, int posY)
{
	agentItem *gfxItem = new agentItem(QString::number(Id));
	gfxItem->setZValue(2);
	gfxItem->setX(posX);
	gfxItem->setY(posY);

    //Output::Inst()->kprintf("ID is %i", Id);
    //Output::Inst()->kprintf("Size of the agent array %i", graphAgents.size());

	scene->addItem(gfxItem);
	graphAgents.insert(Id, gfxItem);

}


void MainWindow::removeGraphicAuton(int id)
{
	emit removeGraphicAutonSignal(id);
}

void MainWindow::on_removeGraphicAuton(int id)
{
	auto iter = graphAgents.find(id);
	if(iter != graphAgents.end())
	{
		scene->removeItem(*iter);
		delete *iter;
		graphAgents.remove(id);
	}
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
	//    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	//    // Scale the view / do the zoom
	//    double scaleFactor = 1.15;

	//    if(event->delta() > 0) {
	//        // Zoom in
	//        factor = .15 + factor;
	//        ui->graphicsView->scale(scaleFactor, scaleFactor);

	//    } else {
	//        // Zooming out
	//        factor =  factor - .15;
	//        ui->graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
	//    }

	//    ui->zoomLabel->setText(QString().setNum(factor*100));
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
	//MainWindow::resizeEvent(event);
	if(ui->tabWidget->currentIndex() == ui->tabWidget->indexOf(vis_mapTab))
	{
		if(zmap != NULL)
		{
			zmap->setPos(0,0);
			zmap->setSize(ui->vis_mapGraphicsView->width(),ui->vis_mapGraphicsView->height());
		}

		ui->vis_graphicsView->fitInView(eventScene->sceneRect(),
										Qt::KeepAspectRatio);
	} else if(ui->tabWidget->currentIndex() == ui->tabWidget->indexOf(sim_viewTab))
	{
		ui->graphicsView->fitInView(scene->sceneRect(),
									Qt::KeepAspectRatio);

	}
}

/**
 * @brief Allows browsing for valid lua agents.
 */
void MainWindow::on_browseLuaAgentButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName
			(this, tr("Open Map File"),QDir::currentPath(),
			 tr("Lua Files (*.lua)"));


	ui->agentPathLineEdit->setText(fileName);
}

/**
 * @brief Starts and stops the simulation
 * @see Control::stopSimulation()
 * @see Control::runSimulation()
 */
void MainWindow::on_runButton_clicked()
{

	if(control->isRunning()){
		control->stopSimulation();
	} else
		control->runSimulation(ui->runTimeSpinBox->value());
}

/**
 * @brief Changes the runbuttons text to reflect whether it starts
 * or stops the simulation
 * @param text
 */
void MainWindow::changeRunButton(QString text)
{
	ui->runButton->setText(text);
}

void MainWindow::runButtonHide()
{
	ui->runButton->setDisabled(true);
}

/**
 * @brief defines the current map
 * This is an internal helper function
 * @see MainWindow::on_browseMapButton_clicked()
 * @see MainWindow::on_generateMap_clicked()
 */
void MainWindow::defineMap()
{
	if(mapItem == NULL)
	{
		mapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*mapImage));
		scene->addItem(mapItem);
	}else
	{
		mapItem->setPixmap(QPixmap::fromImage(*mapImage));
		scene->setSceneRect(mapItem->boundingRect());
	}

	Output::Inst()->kprintf("Map information generated");
	MapHandler::setImage(mapImage);
	Phys::setEnvironment(mapImage->width(),mapImage->height());
	//GridMovement::initGrid(mapImage->width(), mapImage->height());
}

/**
 * @brief Sets a delay on the simulation
 * this can make it easier to observe agent behaviour, the delay set on every
 * macrostep, the value is translated to milliseconds
 * @see AgentDomain::runSimulation()
 */
void MainWindow::on_delaySpinBox_valueChanged(int arg1)
{
	Output::DelayValue = arg1;
}

/**
 * @brief Enables the user to zoom in on the agent live-view
 * @param value zoom value, in percentage
 */
void MainWindow::on_zoomSlider_valueChanged(int value)
{
	double scale = (double)value/100;

	ui->zoomLabel->setText(QString().setNum(value));

	ui->graphicsView->setTransform(QTransform::fromScale(scale,scale));
}

/**
 * @brief Clears the output window
 */
void MainWindow::on_pushButton_clicked()
{
	//ui->outputTextEdit->
    //ui->outputTextBrowser->cl
	ui->outputTextBrowser->clear();
}

/**
 * @brief Writes a info string to current output
 */
void MainWindow::actionPrintInfo()
{
	write_output(versionString);
}

/*
 * POSTPROCESSING PART:
 *
 */

/**
 * @brief Constructs the postprossing widgets and their connections
 */
void MainWindow::ppConstruction()
{
	vis_controlTab = ui->vis_controlTab;
	vis_mapTab = ui->vis_mapTab;

	ui->tabWidget->removeTab(ui->tabWidget->indexOf(vis_controlTab));
	ui->tabWidget->removeTab(ui->tabWidget->indexOf(vis_mapTab));

	QObject::connect(ui->action_Enable_Visualisation, SIGNAL(changed()),this, SLOT(ppIsChecked()));


	QObject::connect(this,SIGNAL(writePPSignal(QString)),
					 this,SLOT(on_writePPOutput(QString)));

	ui->vis_processEventsPushButton->setEnabled(false);

	eventScene->setBackgroundBrush(Qt::black);
	eventMapScene->setBackgroundBrush(Qt::black);

	ui->vis_graphicsView->setScene(eventScene);
	ui->vis_mapGraphicsView->setScene(eventMapScene);

	QObject::connect(zMapTimer,SIGNAL(timeout()),this,SLOT(on_zMapTimerTimeout()));

}

/**
 * @brief Activates the event postprocessing mode
 *
 */
void MainWindow::ppIsChecked()
{

	if(Output::SimRunning.load())
	{
		Output::Inst()->
				kprintf("<b>Cannot Enable Postprocessing until the simulation is done</b>");
		ui->action_Enable_Visualisation->setChecked(false);
	} else
	{

		if(ui->action_Enable_Visualisation->isChecked())
		{
			ui->tabWidget->removeTab(ui->tabWidget->indexOf(sim_controlTab));
			ui->tabWidget->removeTab(ui->tabWidget->indexOf(sim_viewTab));
			ui->tabWidget->removeTab(ui->tabWidget->indexOf(sim_advancedTab));

			ui->simGeneralWidget->hide();

			ui->tabWidget->insertTab(ui->tabWidget->count()+1,vis_controlTab,"Event Process Control");
			//ui->tabWidget->insertTab(3,vis_mapTabptr,"Event Map");
		}else
		{
			ui->tabWidget->removeTab(ui->tabWidget->indexOf(vis_controlTab));
			ui->tabWidget->removeTab(ui->tabWidget->indexOf(vis_mapTab));

			ui->tabWidget->insertTab(ui->tabWidget->count()+1,sim_controlTab,"Control");
			ui->tabWidget->insertTab(ui->tabWidget->count()+1,sim_viewTab,"Live View");
			ui->tabWidget->insertTab(ui->tabWidget->count()+
									 2,sim_advancedTab,"Advanced");

			ui->simGeneralWidget->show();
		}
	}
}

/**
 * @brief Processes the loaded events using the parameters defined
 * @see PostControl::runProcessEvents()
 */
void MainWindow::on_vis_processEventsPushButton_clicked()
{
	zMapTimer->stop();

	QFileInfo fi(ui->vis_eventPathLineEdit->text());
	QFileInfo efi(ui->vis_agentPathLineEdit->text());
	QString agentPath = ui->vis_agentPathLineEdit->text();
	QString eventPath = ui->vis_eventPathLineEdit->text();

	if( fi.isFile() && efi.isFile())
	{

		eventScene->clear();
		ui->tabWidget->removeTab(ui->tabWidget->indexOf(vis_mapTab));

		Output::RunEventProcessing.store(true);
		//clear the zBlock and remove the blocks from the graphicsScene:

		zBlocks = NULL;

		ui->vis_processEventsPushButton->setDisabled(true);

		double timeRes = ui->vis_timeResolutionDoubleSpinBox->value();

		int mapRes = ui->vis_resolutionSpinBox->value();
		double thresshold = ui->vis_zThressholdDoubleSpinBox->value();

		int to = ui->vis_toTimeSpinBox->value();
		int from = ui->vis_fromTimeSpinBox->value();

		QRegExp regex(ui->vis_eventRegExLineEdit->text());

		postControl->runProcessEvents(regex,eventPath,to,from,
									  timeRes,agentPath,mapRes,thresshold);

		timeOffset = from/timeRes;
		ui->vis_activeMapSpinBox->setMinimum(timeOffset);

		QString stringTmp = QString::number(timeRes);
		ui->vis_activeTimeResolutionLabel->setText(stringTmp);
		stringTmp = QString::number(0);
		ui->vis_activeTimeLabel->setText(stringTmp);

	} else
		Output::Inst()->ppprintf("agent- %s or event path %s, not found",
								 agentPath.toStdString().c_str(),
								 eventPath.toStdString().c_str());
}

/**
 * @brief Sets up the visual tab
 * Using pointers to the generated zblocks the visual tabs will
 * display them in the dedicated event visualization view
 * @param argZBlocks pointers to all the relevant zblocks
 * @see ZBlock
 */
void MainWindow::setupVisualTab(QHash<QString, ZBlock *> *argZBlocks)
{
	zBlocks = argZBlocks;

	//Output::Inst()->ppprintf("adding item to something fierce...")
	eventScene->clear();
	//eventScene->setSceneRect(0,0,10,10);


	for(auto it = zBlocks->begin(); it != zBlocks->end(); ++it)
	{
		//Output::Inst()->ppprintf("adding item to something fierce...");
		eventScene->addItem(it.value());
	}

	//add the map tab:
	ui->tabWidget->insertTab(ui->tabWidget->count()+1,vis_mapTab,"Event Map");
	ui->vis_activeMapSpinBox->setMaximum(ColorUtility::GetMaxTime());
	ui->vis_mapTypeComboBox->setCurrentIndex(0);
	ui->vis_activeMapSpinBox->setValue(timeOffset);

	if(zBlocks != NULL)
	{
		for(auto it = zBlocks->begin(); it != zBlocks->end(); ++it)
		{
			it.value()->changeMode(ZMode::Cumulative);
		}
	}

	zmap = new ZMap();
	eventMapScene->addItem(zmap);

	zmap->setPos(0,0);
	zmap->setSize(ui->vis_mapGraphicsView->maximumWidth(),ui->vis_outputTextBrowser->height());

}

void MainWindow::setEventSceneRect(int x, int y)
{
	eventScene->setSceneRect(0,0,x,y);
}

/**
 * @brief Writes the 'activated' zvalue to a dedicated label
 * @param string value to be written
 * @see ZBlock::mouseDoubleClickEvent()
 */
void MainWindow::writeZValue(QString string)
{
	ui->vis_zValueLabel->setText(string);
}

/**
 * @brief Enables the process event button
 * If a valid event savefile has been loaded, and valid parameter has been set
 * then the processing can be started
 * @param enabled
 */
void MainWindow::setProcessEventButton(bool enabled)
{
	ui->vis_processEventsPushButton->setEnabled(enabled);
}

/**
 * @brief Controls the progressbar for the event processing part
 * @param percentage
 */
void MainWindow::advancePPProgess(int percentage)
{
	//QCoreApplication::postEvent(scene, new QEvent(QEvent::UpdateRequest),
	//						Qt::LowEventPriority);

	QMetaObject::invokeMethod(ui->vis_progressBar, "setValue", Q_ARG(int, percentage));
	//ui->progressBar->setValue(percentage);
}

/**
 * @brief Deprecated... use Output::kprintf() or MainWindow::write_output() instead
 * @param argMsg
 */
void MainWindow::write_PPOutput(QString argMsg)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	emit writePPSignal(argMsg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

}

/**
 *
 * @see MainWindow::write_PPOutput()
 */
void MainWindow::on_writePPOutput(QString string)
{
	ui->vis_outputTextBrowser->append(string);
	//ui->vis_outputTextBrowser->append("");
}

/**
 * @brief Browses for events
 * Launches a filebrowser that enables the user to load up a generated
 * event savefile
 * @see EventQueue::saveEEventData()
 *
 */
void MainWindow::on_vis_eventBrowsePushButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName
			(this, tr("Open Event Save File"),QDir::currentPath(),
			 tr("Event Save File (*.kas)"));

	ui->vis_eventPathLineEdit->setText(fileName);
}

/**
 * @brief Browsing for the agent that can process the loaded events
 * If the automatically generated agent path is no correct the
 * user can define the correct path using this function.
 */
void MainWindow::on_vis_agentPathPushButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName
			(this, tr("Open Map File"),QDir::currentPath(),
			 tr("Lua Files (*.lua)"));

	ui->vis_agentPathLineEdit->setText(fileName);
}

/**
 * @brief Reads the simulation info for the defined event file
 * this loads up all the data needed to configure the event processing
 * @see MainWindow::on_vis_processEventsPushButton_clicked()
 */
void MainWindow::on_vis_readInfoPushButton_clicked()
{
	QString path = ui->vis_eventPathLineEdit->text();
	QFileInfo fi(ui->vis_eventPathLineEdit->text());
	EventQueue::simInfo *info;

	if (fi.isFile())
	{
		Output::Inst()->ppprintf("path is again %s", path.toStdString().c_str());
		info = postControl->getEventInfo(path);

		int runtime = int(info->tmuAmount + info->macroFactor)/(info->timeResolution);
		int step = runtime/10;

		if (step == 0) step = 1;

		ui->vis_fromTimeSpinBox->setMinimum(0);
		ui->vis_fromTimeSpinBox->setMaximum(runtime-1);

		int stepSize = (int)runtime/100;
		if(stepSize < 1) stepSize = 1;

		ui->vis_fromTimeSpinBox->setSingleStep(stepSize);
		ui->vis_toTimeSpinBox->setMinimum(1);
		ui->vis_toTimeSpinBox->setMaximum(runtime);
		ui->vis_toTimeSpinBox->setSingleStep(stepSize);
		ui->vis_agentPathLineEdit->setText(info->luaFileName);

		ui->vis_processEventsPushButton->setEnabled(true);

	}else
		Output::Inst()->ppprintf("path %s ,not found",path.toStdString().c_str());

}

/**
 * @brief Changes the current mode af the event map, zblock
 * @param arg1 Current active mode
 */
void MainWindow::on_vis_mapTypeComboBox_currentIndexChanged(const QString &arg1)
{
	Output::Inst()->ppprintf("Current index is :%s", arg1.toStdString().c_str());
	ZMode zmode = ZMode::Average;

	//set the z mode:
	if(arg1.compare("Average") == 0)
		zmode = ZMode::Average;
	else if(arg1.compare("Highest") == 0)
		zmode = ZMode::Highest;
	else if(arg1.compare("Frequency") == 0)
		zmode = ZMode::Frequency;
	else if(arg1.compare("Cumulative") == 0)
		zmode = ZMode::Cumulative;

	if(zBlocks != NULL)
	{
		for(auto it = zBlocks->begin(); it != zBlocks->end(); ++it)
		{
			it.value()->changeMode(zmode);
		}
	}

	if(zmap != NULL)
	{
		zmap->changeMode(zmode);
	}
}

/**
 * @brief Changes the currently active map
 * This enables the user to browse the various maps, via an index
 * @param arg1 wished for map
 */
void MainWindow::on_vis_activeMapSpinBox_valueChanged(int arg1)
{
	if(zBlocks != NULL)
	{
		for(auto it = zBlocks->begin(); it != zBlocks->end(); ++it)
		{
			it.value()->setTime(arg1);
		}
	}


	double currentTime = ui->vis_activeMapSpinBox->value() *
			ui->vis_activeTimeResolutionLabel->text().toDouble();

	QString stringtmp = QString::number(currentTime);
	stringtmp.append("\t - \t");
	stringtmp.append(QString::number(currentTime + ui->vis_activeTimeResolutionLabel->text().toDouble()));
	ui->vis_activeTimeLabel->setText(stringtmp);
}

/**
 * @brief Stop the event processing, if active.
 * This stops the event processing, the data already calculated will not be deleted
 * so it can be browsed
 */
void MainWindow::on_vis_stopEventProcessingPushButton_clicked()
{
	Output::RunEventProcessing.store(false);
}

/**
 * @brief Enables the user to set a custom zoom value
 * If the automatic zoomlevel is not desirable the user can define one that
 * hopfully is
 * @param value zoom value in percentage
 */
void MainWindow::on_vis_eventZoomSlider_valueChanged(int value)
{
	double scale = (double)value/100;

	ui->vis_zoomValueLabel->setText(QString().setNum(value));

	ui->vis_graphicsView->setTransform(QTransform::fromScale(scale,scale));

}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
	//if(zmap != NULL)
	//zmap->setSize(50,ui->vis_mapGraphicsView->height());
}

/**
 * @brief ensures that the automatic zoom is only set on view change.
 * @param index current active tabs index
 */
void MainWindow::on_tabWidget_currentChanged(int index)
{
	if(index == ui->tabWidget->indexOf(vis_mapTab))
	{
		if(zmap != NULL)
		{
			zmap->setPos(0,0);
			zmap->setSize(ui->vis_mapGraphicsView->width(),ui->vis_mapGraphicsView->height());
		}

		ui->vis_graphicsView->fitInView(eventScene->sceneRect(),
										Qt::KeepAspectRatio);
	} else if(ui->tabWidget->currentIndex() == ui->tabWidget->indexOf(sim_viewTab))
	{
		ui->graphicsView->fitInView(scene->sceneRect(),
									Qt::KeepAspectRatio);

	}
}

/**
 * @brief Starts playback of the eventmaps
 * The playback is done by changing map index with a timed delay, defined by the
 * user
 */
void MainWindow::on_vis_eventPlayPushButton_clicked()
{
	if(!playingMap)
	{
		ui->vis_eventPlayPushButton->setText("Stop Playback");

		zMapTimer->start(ui->vis_controlEventPlaySpinBox->value());
		playingMap = true;

	}else
	{
		ui->vis_eventPlayPushButton->setText("Start Playback");

		zMapTimer->stop();
		playingMap = false;
	}
}

void MainWindow::on_zMapTimerTimeout()
{
	int index = ui->vis_activeMapSpinBox->value();
	index++;

	if(index > ui->vis_activeMapSpinBox->maximum())
	{
		index = 0;
	}

	on_vis_activeMapSpinBox_valueChanged(index);
	ui->vis_activeMapSpinBox->setValue(index);

}

/**
 * @brief Clears the event processing output
 */
void MainWindow::on_vis_clearOutputPushButton_clicked()
{
	ui->vis_outputTextBrowser->clear();

}

//DIALOGS:
void MainWindow::dialogConstruction()
{
	QObject::connect(ui->actionSave_Current_Events, SIGNAL(triggered()),this, SLOT(eventDialog()));
	//Output::Inst()->kprintf("dialog start now");
	//Output::Inst()->kprintf("No map has been loaded, please do that...");
}

void MainWindow::eventDialog()
{
	if(!control->isRunning())
	{
		EventDialog *dialog = new EventDialog(control, this);
		dialog->exec();
	} else if(!control->isGenerated())
	{
		Output::Inst()->kprintf("Cannot save events, there is no simulation data");
	}
	else
	{
		Output::Inst()->kprintf("Cannot save events, simulation is still running,");
	}
}

void MainWindow::on_actionDisable_Simulation_Output_toggled(bool arg1)
{
	disableSimOutput = arg1;
}


void MainWindow::on_action_Enable_Visualisation_triggered(bool checked)
{

}
