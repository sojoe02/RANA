#ifndef INTENSITYBLOCK_H
#define INTENSITYBLOCK_H

#include <QPainter>
#include <QGraphicsItem>

class IntensityBlock
{
public:
    IntensityBlock();



private:

	double cumulative;
	double frequency;
	double average;
	double highest;
	int x, y;
};

#endif // INTENSITYBLOCK_H
