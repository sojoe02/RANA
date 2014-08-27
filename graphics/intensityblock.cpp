#include "intensityblock.h"

IntensityBlock::IntensityBlock(int x, int y, int size)
	:x(x), y(y),size(size)
{
}

QRectF IntensityBlock::boundingRect() const
{
	//Still needs to be implemented properly so it really depicts size of
	//qreal penWidth = 1;
	return QRectF(0,0,size,size);
}

void IntensityBlock::paint(QPainter *painter,
					  const QStyleOptionGraphicsItem *option,
					  QWidget *widget)
{
	QRectF rect = boundingRect();
	QPen pen (Qt::white, 1);

	painter->setPen(pen);
	painter->drawRect(x,y,size,size);
}
