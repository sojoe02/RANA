#ifndef EVENTRUNNER_H
#define EVENTRUNNER_H

#include <QThread>
#include "eventprocessing.h"

class EventProcessing;

class EventRunner : public QThread
{
    Q_OBJECT
public:
    explicit EventRunner();

    void setParameters(QRegExp regex, EventProcessing *eventprocessor, QString eventPath,
                       int from, int to,
                       double timeResolution, QString agentPath,
                       int mapResolution, double zThresshold);

    signals:
            void

    processingDone();

protected:
    void run();

private:
    EventProcessing *eventprocessor;

    //parameters needed for event processing:
    QString eventPath;
    int from;
    int to;
    double timeResolution;
    QString agentPath;
    int mapResolution;
    double zThresshold;
    QRegExp regex;
};

#endif // EVENTRUNNER_H
