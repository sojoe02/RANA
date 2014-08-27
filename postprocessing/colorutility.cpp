#include "colorutility.h"

colorUtility::zLevels *colorUtility::ABS_Z_LEVELS = new colorUtility::zLevels;
double colorUtility::Z_TRESSHOLD_PERCENTAGE = 0;
QMutex colorUtility::MUTEX;

colorUtility::colorUtility()
{
}

void colorUtility::addMaxMinValues(zLevels *argLevels){

	QMutexLocker locker(&MUTEX);

	if( ABS_Z_LEVELS->high_average < argLevels->high_average)
		 ABS_Z_LEVELS->high_average = argLevels->high_average;

	if(ABS_Z_LEVELS->high_frequenzy < argLevels->high_frequenzy)
		ABS_Z_LEVELS->high_frequenzy = argLevels->high_frequenzy;

	if(ABS_Z_LEVELS->high_cumulative < argLevels->high_cumulative)
		ABS_Z_LEVELS->high_cumulative = argLevels->high_cumulative;

	if( ABS_Z_LEVELS->low_average < argLevels->low_average)
		 ABS_Z_LEVELS->low_average = argLevels->low_average;

	if(ABS_Z_LEVELS->low_frequenzy < argLevels->low_frequenzy)
		ABS_Z_LEVELS->low_frequenzy = argLevels->low_frequenzy;

	if(ABS_Z_LEVELS->low_cumulative < argLevels->low_cumulative)
		ABS_Z_LEVELS->low_cumulative = argLevels->low_cumulative;

	if(ABS_Z_LEVELS->highest < argLevels->highest)
		ABS_Z_LEVELS->highest = argLevels->highest;

	delete argLevels;
}

QRgb colorUtility::zValueToColor(double value, double min, double max)
{
	double thress = (value-min)/(max-min);

	uchar r = 0;
	uchar g = 0;
	uchar b = 0;
	int sat = 255;

	if(thress < 0.2)
	{
		b = sat;
		g = uchar(thress/0.2* sat);
		return qRgb(r,g,b);
	}
	else if(thress < 0.3999)
	{
		g = sat;
		b = uchar (sat -(thress-0.4)/0.2 * sat);
		return qRgb(r,g,b);
	}
	else if(thress < 0.5999)
	{
		g = sat;
		r = uchar((thress-0.6)/0.2 * sat);
		return qRgb(r,g,b);
	}
	else if(thress < 0.7999)
	{
		r = sat;
		g = sat - uchar((thress-0.8)/0.2* sat);
		return qRgb(r,g,b);
	}
	else if(thress < 0.9999)
	{
		r = sat;
		b = uchar((thress-1)/0.2 * sat);
		return qRgb(r,g,b);
	}
	else if(thress <= 1)
	{
		r = sat;
		b = sat;
		return qRgb(r,g,b);

	} else return Qt::white;
}

QRgb colorUtility::getCumulativeColor(double value)
{
	return zValueToColor(value, ABS_Z_LEVELS->low_cumulative,
						 ABS_Z_LEVELS->high_cumulative);
}

QRgb colorUtility::getFreqColor(double value)
{
	return zValueToColor(value, ABS_Z_LEVELS->low_frequenzy,
						 ABS_Z_LEVELS->high_frequenzy);
}

QRgb colorUtility::getAvgColor(double value)
{
	return zValueToColor(value, ABS_Z_LEVELS->low_average,
						 ABS_Z_LEVELS->high_average);
}

QRgb colorUtility::getHighest(double value)
{
	return zValueToColor(value, 0, ABS_Z_LEVELS->highest);
}

