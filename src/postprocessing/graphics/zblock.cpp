#include "output.h"
#include "zblock.h"
#include "src/postprocessing/colorutility.h"


ZBlock::ZBlock(int argX, int argY)
	: posX(argX), posY(argY), activeColor(Qt::white),defaultColor(Qt::black),
	  firstAddition(true), currentZMode(ZMode::Highest),
	  currentTime(0), maxTime(0)
{
	//Output::Inst()->ppprintf("zblock initialized at: %i,%i", x,y);
	this->setX(argX);
	this->setY(argY);

	min.cumulative = 0;
	min.average = 0;
	min.highest = 0;
	min.frequency = 0;
	max.frequency = 0;
	max.cumulative = 0;
	max.highest = 0;
	max.average = 0;
	setToolTip("Double click to display z value");
}

ZBlock::~ZBlock()
{

}

QRectF ZBlock::boundingRect() const
{
	return QRectF(0,0,1,1);
}

void ZBlock::paint(QPainter *painter,
					  const QStyleOptionGraphicsItem *option,
					  QWidget *widget)
{
	//QRectF rect = boundingRect();

	setColor(currentTime, currentZMode);
	//painter->setBrush(QColor(activeColor));
	QPen pen(activeColor,1);
	painter->setPen(pen);
	painter->drawPoint(0,0);
	//painter->drawPoint(1,1);
	//Output::Inst()->ppprintf("repainting at %i,%i", x,y);
	//painter->setBrush(QColor(Qt::red));
	//painter->drawRect(QRect(x,y,3,3));
}

void ZBlock::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){

	zitr = zmap.find(currentTime);

	if(zitr == zmap.end())
	{
		Output::kprintf("No z value");
	}else
	{
		QString string;

		if(currentZMode == ZMode::Average)
		{
			QTextStream(&string)<< zitr->second.average;
			//Output::updateZvalue(string);

		}else if(currentZMode == ZMode::Cumulative)
		{
			QTextStream(&string)<< zitr->second.cumulative;
			//Output::Inst()->updateZvalue(string);

		}else if(currentZMode == ZMode::Frequency)
		{
			QTextStream(&string)<< zitr->second.frequency;
			//Output::Inst()->updateZvalue(string);

		}else if(currentZMode == ZMode::Highest)
		{
			QTextStream(&string)<< zitr->second.highest;
			//Output::Inst()->updateZvalue(string);

		}
	}
}

void ZBlock::setColor(int time, ZMode zmode)
{
	currentZMode = zmode;

	if(zmode == ZMode::Average)
	{
		activeColor = getAverageColor(time);
	}else if(zmode == ZMode::Cumulative)
	{
		activeColor = getCumulativeColor(time);
	}else if(zmode == ZMode::Frequency)
	{
		activeColor = getFrequencyColor(time);
	}else if(zmode == ZMode::Highest)
	{
		activeColor = getHighestColor(time);
	}else
		activeColor = Qt::white;

}

void ZBlock::changeMode(ZMode zmode)
{
	currentZMode = zmode;
	this->update();
}

void ZBlock::setTime(int time)
{
	currentTime = time;
	this->update();
}

/*
 * Logic for adding a z value to this block.
 * Adding a zvalue will determine, the various types of values that will be added
 * to the given timeslot. It will also determine the blocks local min and max values.
 * @param zvalue the added z value.
 * @param time the discretized timeslot for this value.
 *
 */

void ZBlock::addZValue(double zvalue, int time)
{
	if(maxTime < time) maxTime = time;
	//set local minimun and maximum on first insertion:
	if(firstAddition)
	{
		min.cumulative = zvalue;
		min.average = zvalue;
		min.highest = zvalue;
		min.frequency = 1;
		max.frequency = 1;
		max.cumulative = zvalue;
		max.highest = zvalue;
		max.average = zvalue;
		firstAddition = false;
		//Output::Inst()->ppprintf("Min/Max initial average, %f,%f", min.average, max.average);
	}
	//Output::Inst()->ppprintf("Z value is: %f, time is: %i", zvalue , time);

	zitr = zmap.find(time);

	//Insertion logic for the different values:
	if(zitr == zmap.end())
	{
		ColorUtility::zvalue z;
		z.frequency = 1;

		z.cumulative = zvalue;

		if(zvalue > max.cumulative)	max.cumulative = zvalue;
		else if(zvalue < min.cumulative) min.cumulative = zvalue;

		z.average = zvalue;
		if(zvalue > max.average) max.average = zvalue;
		else if(zvalue < min.average)	min.average = zvalue;

		z.highest = zvalue;
		if(zvalue > max.highest)
			max.highest = zvalue;
		else if(zvalue < min.highest)
			min.highest = zvalue;

		zmap.insert(std::pair<unsigned long long,ColorUtility::zvalue>(time,z));

	} else
	{

		ColorUtility::zvalue *z = &zitr->second;
		z->frequency++;
		if(z->frequency > max.frequency) max.frequency = z->frequency;
		else if(z->frequency < min.frequency) min.frequency = z->frequency;

		z->cumulative += zvalue;
		if(z->cumulative > max.cumulative) max.cumulative = z->cumulative;
		else if(z->cumulative < min.cumulative) min.cumulative = z->cumulative;

		z->average = z->cumulative/z->frequency;
		if(z->average > max.average) max.average = z->average;
		else if(z->average < min.average) min.average = z->average;

		if(zvalue > z->highest)
		{
			z->highest = zvalue;
			if(z->highest > max.highest) max.highest = z->highest;
			else if(z->highest < min.highest) min.highest = z->highest;
		}

	}
	//Output::Inst()->ppprintf("Min/Max average, %i,%i", min.average, max.average);
}

/*
 * Will register the local min and maximum values with the ColorUtility class.
 * see ColorUtility::AddMaxMinValues(double, double, int) for more info on
 * the min max adding logic.
 *
 */
void ZBlock::registerMinMax()
{
	//Output::Inst()->ppprintf("Min/Max frequency, %f,%f", min.frequency, max.frequency);
	ColorUtility::AddMaxMinValues(min, max,maxTime);
}

/*
 *
 */
QColor ZBlock::getCumulativeColor(int time)
{
	zitr = zmap.find(time);

	if(zitr != zmap.end())
	{
		return ColorUtility::GetCumulativeColor(zitr->second.cumulative);
	}

	return defaultColor ;
}

QColor ZBlock::getFrequencyColor(int time)
{
	zitr = zmap.find(time);

	if(zitr != zmap.end())
	{
		return ColorUtility::GetFreqColor(zitr->second.frequency);
	}
	return defaultColor;
}

QColor ZBlock::getHighestColor(int time)
{
	zitr = zmap.find(time);

	if(zitr != zmap.end())
	{
		return ColorUtility::GetHighest(zitr->second.highest);
	}

	return defaultColor;
}

QColor ZBlock::getAverageColor(int time)
{
	zitr = zmap.find(time);

	if(zitr != zmap.end())
	{
		return ColorUtility::GetAvgColor(zitr->second.average);
	}

	return defaultColor;
}
