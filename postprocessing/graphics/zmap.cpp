#include "zmap.h"

#include "zblock.h"

#include "output.h"

ZMap::ZMap()
    :sizeX(10), sizeY(0),currentZMode(ZMode::Cumulative), currentTime(0)
{
	maxLevels = ColorUtility::GetMaxZLevels();
	minLevels = ColorUtility::GetMinZLevels();
    //Output::Inst()->ppprintf("color utility max values: %f,%f");
}
QRectF ZMap::boundingRect() const
{
 return QRectF(0,0,sizeX,sizeY);
}
void ZMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //Output::Inst()->ppprintf("value is: scene %i, sizeY %i", scene()->height(), sizeY);
    //sizeY = 200;
    for(int i = 1; i <= sizeY; i++)
	{
		QPen pen;
		pen.setWidth(1);
        pen.setColor(Qt::black);

		double value = 0;

		if(currentZMode == ZMode::Average)
		{
            value = (maxLevels.average) *
					((double)i/(double)sizeY);

            pen.setColor(ColorUtility::GetAvgColor(value));

		}else if(currentZMode == ZMode::Cumulative)
		{
            value = (maxLevels.cumulative) *
					((double)i/(double)sizeY);

            pen.setColor(ColorUtility::GetCumulativeColor(value));

		}else if(currentZMode == ZMode::Frequency)
		{
            value = (maxLevels.frequency) *
					((double)i/(double)sizeY);

            pen.setColor(ColorUtility::GetFreqColor(value));

		}else if(currentZMode == ZMode::Highest)
		{
            value = (maxLevels.highest) *
					((double)i/(double)sizeY);

            pen.setColor(ColorUtility::GetHighest(value));
        }

        //Output::Inst()->kprintf("value is :%f, %f", value, (double)i/(double)sizeY);

		painter->setPen(pen);
		painter->drawLine(0,i,sizeX,i);

		if( i%30 == 0 && i != 0 )
		{
			QString valueString;
			QTextStream(&valueString) << value;
			pen.setColor(Qt::black);
			painter->setPen(pen);
			painter->drawLine(0,i,5,i);
			painter->drawText(5,i,valueString);
		}
	}
}

void ZMap::setSize(int sizeX, int sizeY)
{
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->update();
}

void ZMap::changeMode(ZMode zmode)
{
	currentZMode = zmode;
	this->update();
}

void ZMap::setTime(int time)
{
	currentTime = time;
	this->update();
}
