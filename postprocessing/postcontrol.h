#ifndef POSTCONTROL_H
#define POSTCONTROL_H

#include <QThread>

class PostControl
{
public:
	PostControl();



private:
	QThread *runProcessing;

};

#endif // POSTCONTROL_H
