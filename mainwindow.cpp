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

#include "eventdialog.h"


//QImage *MainWindow::mapItem = NULL;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),factor(1),
	mapImage(NULL), mapItem(NULL),scene(new QGraphicsScene()),
	control(new Control(this)),
	postControl(new PostControl(this)),zBlocks(NULL),
	eventScene(new QGraphicsScene()), disableSimOutput(false)
{

	this->setWindowTitle("RANA QT version");

    ui->setupUi(this);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    ui->graphicsView->setScene(scene);
    scene->setBackgroundBrush(Qt::gray);
    ui->runButton->setDisabled(true);

    qRegisterMetaType<INFOLIST>("INFOLIST");

    QObject::connect(this,SIGNAL(map_updateSignal(INFOLIST)),
                     this,SLOT(on_updateMap(INFOLIST)));

    QObject::connect(this,SIGNAL(writeStringSignal(QString)),
                     this,SLOT(on_writeOutput(QString)));

    QObject::connect(this,SIGNAL(writeErrSignal(QString)),
                     this,SLOT(on_writeErrOutput(QString)));

    QObject::connect(this,SIGNAL(writeStatusSignal(unsigned long long,unsigned long long,unsigned long long,unsigned long long)),
                     this,SLOT(on_udateStatus(unsigned long long,unsigned long long,unsigned long long,unsigned long long)));

    //connect actions:
    QObject::connect(ui->action_Exit, SIGNAL(triggered()),this, SLOT(actionExit()));
    QObject::connect(ui->action_Info, SIGNAL(triggered()),this, SLOT(actionPrintInfo()));

	versionString = QString("<b><font color=\"green\">RANA</b></font> version 1.2.7_QT_incomplete");

	ui->statusBar->addWidget(new QLabel(versionString));
	ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

	sim_controlTab = ui->simControlTab;
	sim_viewTab = ui->simLiveView;
	sim_general = ui->simGeneralWidget;

	ppConstruction();
	dialogConstruction();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_generateButton_clicked()
{
    for(auto iter=graphAgents.begin(); iter!=graphAgents.end(); ++iter) {
        scene->removeItem(*iter);
        //delete *iter;
    }
    graphAgents.clear();
    GridMovement::clearGrid();

    if(mapItem != NULL){

		Phys::setScale(ui->scaleDoubleSpinBox->value());
		Output::Inst()->kprintf("Setting map scale to %f", Phys::getScale());

        ui->progressBar->setValue(0);
        QFile path(ui->agentPathLineEdit->text());
        if(path.exists())
        {
            double timeRes = 1/(double)ui->timeResSpinBox->value();
            double macroRes = ui->macroSpinBox->value();
            int agentAmount = ui->luaSpinBox->value();
            QString agentPath = ui->agentPathLineEdit->text();
            std::string stringPath = agentPath.toUtf8().constData();
            control->generateEnvironment(mapImage, 1,timeRes, macroRes,
                                         agentAmount,stringPath);

            Output::Inst()->kprintf("generating environment, %d, %s",
                                    agentAmount, stringPath.c_str());
            ui->runButton->setEnabled(true);

			QFileInfo fi(agentPath);
			Output::AgentFile=fi.fileName().toStdString();
			Output::AgentPath=fi.path().toStdString().append("/");
        } else
            Output::Inst()->kprintf("Cannot generate Environment: Valid path not given");
    } else
        Output::Inst()->kprintf("No map has been loaded, please do that...");
}

void MainWindow::advanceProgess(int percentage)
{
	//QCoreApplication::postEvent(scene, new QEvent(QEvent::UpdateRequest),
	  //                          Qt::LowEventPriority);
    QMetaObject::invokeMethod(ui->progressBar, "setValue", Q_ARG(int, percentage));
    //ui->progressBar->setValue(percentage);
}

void MainWindow::actionExit()
{
    QApplication::quit();
}

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

void MainWindow::on_writeOutput(QString string)
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    //QString prepend = "</>";
    //QString output = string.prepend(prepend);
	if(!disableSimOutput){
		ui->outputTextBrowser->insertHtml(string);
		ui->outputTextBrowser->append("");
	}
}

void MainWindow::write_output(QString argMsg)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    emit writeStringSignal(argMsg);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

}

void MainWindow::on_writeErrOutput(QString string)
{
	ui->outputTextBrowser->setTextColor(Qt::red);
	ui->outputTextBrowser->append(string);
	ui->outputTextBrowser->setTextColor(Qt::black);
}

void MainWindow::write_errOutput(QString argMsg)
{
    emit writeErrSignal(argMsg);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

}

void MainWindow::on_udateStatus(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents)
{
    ui->label_status1->setText(QString().setNum(ms));
    ui->label_status2->setText(QString().setNum(eventInit));
    ui->label_status3->setText(QString().setNum(internalEvents));
    ui->label_status4->setText(QString().setNum(externalEvents));
}

void MainWindow::write_status(unsigned long long ms, unsigned long long eventInit, unsigned long long internalEvents, unsigned long long externalEvents)
{
    emit writeStatusSignal(ms, eventInit, internalEvents, externalEvents);
}

void MainWindow::updateMap(std::list<agentInfo> infolist)
{
    emit map_updateSignal(infolist);
}

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
            agentItem *gfxItem = new agentItem(QString::number(Id));
            gfxItem->setZValue(2);
            gfxItem->setX(x);
            gfxItem->setY(y);
            scene->addItem(gfxItem);

            graphAgents.insert(Id, gfxItem);
        } else
        {
            QMap<int, agentItem*>::const_iterator i = graphAgents.find(Id);
            agentItem *gfxItem = i.value();
            gfxItem->setX(x);
            gfxItem->setY(y);
        }
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

void MainWindow::on_browseLuaAgentButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName
            (this, tr("Open Map File"),QDir::currentPath(),
			 tr("Lua Files (*.lua)"));


    ui->agentPathLineEdit->setText(fileName);
}

void MainWindow::on_runButton_clicked()
{

    if(control->isRunning()){
        control->stopSimulation();
    } else
        control->runSimulation(ui->runTimeSpinBox->value());
}

void MainWindow::changeRunButton(QString text)
{
    ui->runButton->setText(text);
}

void MainWindow::runButtonHide()
{
    ui->runButton->setDisabled(true);
}

void MainWindow::defineMap()
{
    if(mapItem == NULL)
    {
        mapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*mapImage));
        scene->addItem(mapItem);
    }else
        mapItem->setPixmap(QPixmap::fromImage(*mapImage));

    Output::Inst()->kprintf("Map information generated");
    MapHandler::setImage(mapImage);
    Phys::setEnvironment(mapImage->width(),mapImage->height());
    //GridMovement::initGrid(mapImage->width(), mapImage->height());
}


void MainWindow::on_delaySpinBox_valueChanged(int arg1)
{
    Output::DelayValue = arg1;
}


void MainWindow::on_zoomSlider_valueChanged(int value)
{
    double scale = (double)value/100;

    ui->zoomLabel->setText(QString().setNum(value));

    ui->graphicsView->setTransform(QTransform::fromScale(scale,scale));
}

void MainWindow::on_pushButton_clicked()
{
    //ui->outputTextEdit->
	ui->outputTextBrowser->clear();
}

void MainWindow::actionPrintInfo()
{
	ui->outputTextBrowser->insertHtml(versionString);
	ui->outputTextBrowser->append("");
}

/*
 * POSTPROCESSING PART:
 *
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

	eventScene->setBackgroundBrush(Qt::gray);
	ui->vis_graphicsView->setScene(eventScene);


}

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
			ui->simGeneralWidget->hide();

			ui->tabWidget->insertTab(ui->tabWidget->count()+1,vis_controlTab,"Event Process Control");
			//ui->tabWidget->insertTab(3,vis_mapTabptr,"Event Map");
		}else
		{
			ui->tabWidget->removeTab(ui->tabWidget->indexOf(vis_controlTab));
			ui->tabWidget->removeTab(ui->tabWidget->indexOf(vis_mapTab));

			ui->tabWidget->insertTab(ui->tabWidget->count()+1,sim_controlTab,"Control");
			ui->tabWidget->insertTab(ui->tabWidget->count()+1,sim_viewTab,"Live View");
			ui->simGeneralWidget->show();
		}
	}
}


void MainWindow::on_vis_processEventsPushButton_clicked()
{

	Output::RunEventProcessing.store(true);
	//clear the zBlock and remove the blocks from the graphicsScene:
	if(zBlocks != NULL)
	{
		//QHashIterator<QString, ZBlock*> zitr(*zBlocks);
		//while(zitr.hasNext())
		//{
		//	eventScene->removeItem(zitr.value());
	//	}
	}

	zBlocks = NULL;

	ui->vis_processEventsPushButton->setDisabled(true);

	double timeRes = ui->vis_timeResolutionDoubleSpinBox->value();
	QString agentPath = ui->vis_agentPathLineEdit->text();
	int mapRes = ui->vis_resolutionSpinBox->value();
	double thresshold = ui->vis_zThressholdDoubleSpinBox->value();
	QString eventPath = ui->vis_eventPathLineEdit->text();

	QFileInfo fi(ui->vis_eventPathLineEdit->text());
	QFileInfo efi(ui->vis_agentPathLineEdit->text());

	if( fi.isFile() && efi.isFile() )
	{
		int to = ui->vis_toTimeSpinBox->value();
		int from = ui->vis_fromTimeSpinBox->value();

		QRegExp regex(ui->vis_eventRegExLineEdit->text());

		postControl->runProcessEvents(regex,eventPath,to,from,
									  timeRes,agentPath,mapRes,thresshold);

	} else
		Output::Inst()->ppprintf("agent- %s or event path %s, not found",
								 agentPath.toStdString().c_str(),
								 eventPath.toStdString().c_str());
}


void MainWindow::setupVisualTab(QHash<QString, ZBlock *> *argZBlocks)
{
	zBlocks = argZBlocks;

	//QHashIterator<QString, ZBlock*> zitr(*zBlocks);

	//while(zitr.hasNext())
	//{
		//eventScene->addItem(zitr.value());
	//}
	for(auto it = zBlocks->begin(); it != zBlocks->end(); ++it)
	{
		//Output::Inst()->ppprintf("adding item to something fierce...");
		eventScene->addItem(it.value());
	}
	//add the map tab:
	ui->tabWidget->insertTab(ui->tabWidget->count()+1,vis_mapTab,"Event Map");

}

void MainWindow::setProcessEventButton(bool enabled)
{
	ui->vis_processEventsPushButton->setEnabled(enabled);
}

void MainWindow::advancePPProgess(int percentage)
{
	//QCoreApplication::postEvent(scene, new QEvent(QEvent::UpdateRequest),
	//						Qt::LowEventPriority);

	QMetaObject::invokeMethod(ui->vis_progressBar, "setValue", Q_ARG(int, percentage));
	//ui->progressBar->setValue(percentage);
}


void MainWindow::write_PPOutput(QString argMsg)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	emit writePPSignal(argMsg);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

}

void MainWindow::on_writePPOutput(QString string)
{
	ui->vis_outputTextBrowser->insertHtml(string);
	ui->vis_outputTextBrowser->append("");
}

void MainWindow::on_vis_eventBrowsePushButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName
			(this, tr("Open Event Save File"),QDir::currentPath(),
			 tr("Event Save File (*.kas)"));

	ui->vis_eventPathLineEdit->setText(fileName);
}

void MainWindow::on_vis_agentPathPushButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName
			(this, tr("Open Map File"),QDir::currentPath(),
			 tr("Lua Files (*.lua)"));

	ui->vis_agentPathLineEdit->setText(fileName);
}

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

		if (step == 0)
			step = 1;

		ui->vis_fromTimeSpinBox->setMinimum(0);
		ui->vis_fromTimeSpinBox->setMaximum(runtime-1);
		ui->vis_fromTimeSpinBox->setSingleStep(runtime/10);
		ui->vis_toTimeSpinBox->setMinimum(1);
		ui->vis_toTimeSpinBox->setMaximum(runtime);
		ui->vis_toTimeSpinBox->setSingleStep(runtime/10);
		ui->vis_agentPathLineEdit->setText(info->luaFileName);

		ui->vis_processEventsPushButton->setEnabled(true);

	}else
		Output::Inst()->ppprintf("path %s ,not found",path.toStdString().c_str());

}

void MainWindow::on_vis_mapTypeComboBox_currentIndexChanged(const QString &arg1)
{
	Output::Inst()->ppprintf("Current index is :%s", arg1.toStdString().c_str());
	ZMode zmode;

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
		//auto it = zBlocks->begin();
		//it.value()->changeMode(zmode);
	}
}

void MainWindow::on_vis_activeMapSpinBox_valueChanged(int arg1)
{
	if(zBlocks != NULL)
	{
		for(auto it = zBlocks->begin(); it != zBlocks->end(); ++it)
		{
			it.value()->setTime(arg1);
		}
	}
}

void MainWindow::on_vis_stopEventProcessingPushButton_clicked()
{
	Output::RunEventProcessing.store(false);
}

void MainWindow::on_vis_eventZoomSlider_valueChanged(int value)
{
	double scale = (double)value/100;

	ui->vis_zoomValueLabel->setText(QString().setNum(value));

	ui->vis_graphicsView->setTransform(QTransform::fromScale(scale,scale));

}

//DIALOGS
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
