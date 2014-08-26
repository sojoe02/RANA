#ifndef INTENSITYMAP_H
#define INTENSITYMAP_H

#include <QPainter>
#include <QGraphicsItem>

#include <string>
#include <unordered_map>

#include "intensityblock.h"

class IntensityMap : public QGraphicsItem
{
	//Q_OBJECT
public:
	explicit IntensityMap();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:

	std::unordered_map<std::string, IntensityBlock> iBlocks;
	std::unordered_map<std::string, IntensityBlock>::iterator blockItr;

};

#endif // INTENSITYMAP_H
