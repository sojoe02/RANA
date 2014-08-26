#include "intensityblock.h"

IntensityBlock::IntensityBlock()
{
}

QRectF IntensityBlock::boundingRect() const
{
	//Still needs to be implemented properly so it really depicts size of
	// graphical representation of the agent.
	//qreal penWidth = 1;
	return QRectF(0,0,10,10);
}

void IntensityBlock::paint(QPainter *painter,
					  const QStyleOptionGraphicsItem *option,
					  QWidget *widget)
{
	QRectF rect = boundingRect();
	QPen pen (Qt::white, 1);

}
