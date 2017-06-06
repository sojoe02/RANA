#ifndef ZMAP_H
#define ZMAP_H

#include <QPainter>
#include <QGraphicsItem>
#include "zblock.h"
#include "src/postprocessing/colorutility.h"

class ZMap : public QGraphicsItem
{
	//Q_OBJECT
public:
    ZMap();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void changeMode(ZMode zmode);
	void setTime(int time);
	void setSize(int sizeX, int sizeY);

private:
	int sizeX;
	int sizeY;
	ZMode currentZMode;
	int currentTime;
	ColorUtility::zvalue maxLevels;
	ColorUtility::zvalue minLevels;
};

#endif // ZMAP_H
