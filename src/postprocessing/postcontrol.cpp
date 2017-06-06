//--begin_license--
//
//Copyright 	2013-2014 	Søren Vissing Jørgensen.
//			2014-	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.
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

#include "src/output.h"
#include "src/postprocessing/postcontrol.h"
#include "src/simulationcore/eventqueue.h"

PostControl::PostControl(MainWindow *mainWindow)
	: mainWindow(mainWindow), eventprocessor(NULL),
	  runner(NULL), processing(false)
{
	runner = new EventRunner();
	QObject::connect(runner, SIGNAL(processingDone()), this, SLOT(on_processDone()));
}

PostControl::~PostControl()
{
	runner->quit();
	runner->wait();
}

EventQueue::simInfo* PostControl::getEventInfo(QString path)
{
	if(eventprocessor != NULL)
		delete eventprocessor;

	eventprocessor = new EventProcessing;

	return eventprocessor->readEventInfo(path.toStdString());
}

void PostControl::runProcessEvents(QRegExp regex, QString eventPath, int to, int from,
								double timeResolution,QString agentPath,
								int mapResolution, double zThresshold)
{
	ColorUtility::Init();

	processing = true;

	runner->setStackSize(256*1024*1024);
	runner->setParameters(regex, eventprocessor, eventPath, to,
						  from, timeResolution, agentPath,
						  mapResolution, zThresshold);
	runner->start();
}

bool PostControl::isProcessing()
{
	return processing;
}


void PostControl::on_processDone()
{
	processing = false;
	mainWindow->setProcessEventButton(true);
	mainWindow->setupVisualTab(eventprocessor->getZBlocks());


	Output::Inst()->ppprintf("Event processing done");
}


