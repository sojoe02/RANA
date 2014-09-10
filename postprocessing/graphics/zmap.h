#ifndef ZMAP_H
#define ZMAP_H

#include <QPainter>
#include <QGraphicsItem>
//#include "zblock.h"

class ZMap : public QGraphicsItem
{
	//Q_OBJECT
public:
	ZMap();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


};

#endif // ZMAP_H
