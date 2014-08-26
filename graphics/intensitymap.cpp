#include "intensitymap.h"

IntensityMap::IntensityMap()
{
}

QRectF IntensityMap::boundingRect() const
{

}

void IntensityMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QRectF rect = boundingRect();
	QPen pen (Qt::white, 1);
}
