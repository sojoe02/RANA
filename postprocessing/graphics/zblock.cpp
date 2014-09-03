#include "zblock.h"

ZBlock::ZBlock(int x, int y)
	:
	  x(x), y(y),
	  max_cumulative(0), max_frequency(0), max_highest(0),max_average(0),
	  firstAddition(true)
{
}

QRectF ZBlock::boundingRect() const
{
	//Still needs to be implemented properly so it really depicts size of
	//qreal penWidth = 1;
	return QRectF(0,0,size,size);
}

void ZBlock::paint(QPainter *painter,
					  const QStyleOptionGraphicsItem *option,
					  QWidget *widget)
{
	QRectF rect = boundingRect();
	QPen pen (Qt::white, 1);

	painter->setPen(pen);
	painter->drawRect(x,y,size,size);
}

void ZBlock::addZValue(double zvalue, unsigned long long time)
{
	//set local minimun on first insertion:
	if(firstAddition)
	{
		min_cumulative = zvalue;
		min_average = zvalue;
		min_highest = zvalue;
		min_frequency = 1;
		max_frequency = 1;
		firstAddition = false;
	}

	std::map<unsigned long long, ZBlock::zvalue>::iterator zitr;

	zitr = zmap.find(time);

	//Insertion logic for the different values:
	if(zitr == zmap.end())
	{
		ZBlock::zvalue z;
		z.frequency = 1;

		z.cumulative = zvalue;

		if(zvalue > max_cumulative)	max_cumulative = zvalue;
		else if(zvalue < min_cumulative) min_cumulative = zvalue;

		z.average = zvalue;
		if(zvalue > max_average) max_average = zvalue;
		else if(zvalue < min_average)	min_average = zvalue;

		z.highest = zvalue;
		if(zvalue > max_highest)
			max_highest = zvalue;
		else if(zvalue < min_highest)
			min_highest = zvalue;

		zmap.insert(std::pair<unsigned long long,ZBlock::zvalue>(time,z));
	} else
	{
		ZBlock::zvalue *z = &zitr->second;
		z->frequency++;
		if(z->frequency > max_frequency) max_frequency = z->frequency;
		else if(z->frequency <min_frequency) min_frequency = z->frequency;

		z->cumulative += zvalue;
		if(z->cumulative > max_cumulative) max_cumulative = z->cumulative;
		else if(z->cumulative < min_cumulative) min_cumulative = z->cumulative;

		z->average = z->cumulative/z->frequency;
		if(z->average > max_average) max_average = z->average;
		else if(z->average < min_average) min_average = z->average;

		if(zvalue > z->highest)
		{
			z->highest = zvalue;
			if(z->highest > max_highest) max_highest = z->highest;
			else if(z->highest < min_highest) min_highest = z->highest;
		}
	}
}
