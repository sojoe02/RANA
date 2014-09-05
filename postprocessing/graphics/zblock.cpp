#include "zblock.h"
#include "../colorutility.h"

ZBlock::ZBlock(int x, int y)
	:
	  x(x), y(y),
	  currentFrequencyColor(Qt::blue), currentCumulativeColor(Qt::blue),
	  currentAverageColor(Qt::blue), currentHighestColor(Qt::blue),
	  firstAddition(true)
{
}

void ZBlock::addZValue(double zvalue, int time)
{
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
	}

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
}

void ZBlock::registerMinMax()
{
	ColorUtility::AddMaxMinValues(min, max);

}

QRgb ZBlock::getCumulativeColor(int time)
{
	zitr = zmap.find(time);

	if(zitr != zmap.end())
	{
		currentCumulativeColor =
				ColorUtility::GetCumulativeColor(zitr->second.cumulative);
	}
	return currentCumulativeColor;
}

QRgb ZBlock::getFrequencyColor(int time)
{
	zitr = zmap.find(time);

	if(zitr != zmap.end())
	{
		currentFrequencyColor =
		ColorUtility::GetFreqColor(zitr->second.frequency);
	}
	return currentFrequencyColor;
}

QRgb ZBlock::getHighestColor(int time)
{
	zitr = zmap.find(time);

	if(zitr != zmap.end())
	{
		currentHighestColor =
		ColorUtility::GetHighest(zitr->second.highest);
	}
	return currentHighestColor;
}

QRgb ZBlock::getAverageColor(int time)
{
	zitr = zmap.find(time);

	if(zitr != zmap.end())
	{
		currentAverageColor =
		ColorUtility::GetAvgColor(zitr->second.average);
	}
	return currentAverageColor;
}
