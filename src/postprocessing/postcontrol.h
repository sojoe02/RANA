#ifndef POSTCONTROL_H
#define POSTCONTROL_H

#include <QtGui>
#include <QString>
#include <QObject>

#include "eventrunner.h"
#include "eventprocessing.h"
#include "mainwindow.h"

class MainWindow;

class EventProcessing;

class EventRunner;

class PostControl : public QObject
{
    Q_OBJECT

public:

    PostControl(MainWindow *mainWindow);

    ~PostControl();

    EventQueue::simInfo *getEventInfo(QString path);

    void runProcessEvents(QRegExp regex, QString eventPath, int to, int from,
                          double timeResolution, QString agentPath,
                          int mapResolution, double zThresshold);

    bool isProcessing();

public
    slots:

            void

    on_processDone();

    signals:

            void
    startEventProcessing(QString
    eventPath,
    int from,
    int to,
    double timeResolution, QString
    agentPath,
    int mapResolution,
    double zThresshold
    );

private:

    MainWindow *mainWindow;
    EventProcessing *eventprocessor;
    EventRunner *runner;
    bool processing;


};

#endif // POSTCONTROL_H
