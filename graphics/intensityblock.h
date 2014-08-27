#ifndef INTENSITYBLOCK_H
#define INTENSITYBLOCK_H

#include <QPainter>
#include <QGraphicsItem>

class IntensityBlock : public QGraphicsItem
{
public:
	IntensityBlock(int x, int y, int size);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:

	double cumulative;
	double frequency;
	double average;
	double highest;
	int x, y, size;
};

#endif // INTENSITYBLOCK_H
