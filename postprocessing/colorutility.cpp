#include "colorutility.h"
#include "output.h"
#include <climits>

double ColorUtility::Z_TRESSHOLD_PERCENTAGE = 0;
QMutex ColorUtility::MUTEX;
ColorUtility::zvalue ColorUtility::MAX_Z_LEVELS;
ColorUtility::zvalue ColorUtility::MIN_Z_LEVELS;
int ColorUtility::MAXTIME = 0;

ColorUtility::ColorUtility()
{
}

/**
 * @brief Initializes the colorutility class values
 * This is done prior to any event processing
 * @see MainWindow::on_vis_processEventsPushButton_clicked()
 * @see EventProcessing::processBinnedEvents()
 */
void ColorUtility::Init()
{
	MAXTIME = 0;

	MAX_Z_LEVELS.average = 0;
	MAX_Z_LEVELS.cumulative = 0;
	MAX_Z_LEVELS.highest = 0;
	MAX_Z_LEVELS.frequency = 0;

	MIN_Z_LEVELS.average = ULLONG_MAX;
	MIN_Z_LEVELS.highest = ULLONG_MAX;
	MIN_Z_LEVELS.cumulative = ULLONG_MAX;
	MIN_Z_LEVELS.frequency = ULLONG_MAX;
}

/**
 * @brief Allows the zblocks to add their local min and max z value
 * Once all event have been processed each zblock will add their local
 * @param min local min value
 * @param max local max value
 * @param maxTime last recorded time
 * @see ZBlock::registerMinMax()
 *
 */
void ColorUtility::AddMaxMinValues(zvalue min, zvalue max, int maxTime){

	QMutexLocker locker(&MUTEX);

	if( MAX_Z_LEVELS.average < max.average)
		 MAX_Z_LEVELS.average = max.average;

	if(MAX_Z_LEVELS.frequency < max.frequency)
		MAX_Z_LEVELS.frequency = max.frequency;

	if(MAX_Z_LEVELS.cumulative < max.cumulative)
		MAX_Z_LEVELS.cumulative = max.cumulative;

	if(MAX_Z_LEVELS.highest < max.highest)
		MAX_Z_LEVELS.highest = max.highest;

	if(MIN_Z_LEVELS.average > min.average)
		 MIN_Z_LEVELS.average = min.average;

	if(MIN_Z_LEVELS.frequency > min.frequency)
		MIN_Z_LEVELS.frequency = min.frequency;

	if(MIN_Z_LEVELS.cumulative > min.cumulative)
		MIN_Z_LEVELS.cumulative = min.cumulative;

	if(MIN_Z_LEVELS.highest > min.highest)
		MIN_Z_LEVELS.highest = min.highest;

	//Output::Inst()->ppprintf("max/min. average %f,%f :", MAX_Z_LEVELS.average, MIN_Z_LEVELS.average);
	if(MAXTIME < maxTime) MAXTIME = maxTime;

}

/**
 * @brief Transform a z value into a corresponding color
 * @param value Value that needs to be transformed into the corresponding color
 * @param min lowest possible value (pink)
 * @param max highest possible value (blue)
 * @return
 */
QColor ColorUtility::ZValueToColor(double value, double min, double max)
{
	double thress = (value)/(max);

	uchar r = 0;
	uchar g = 0;
	uchar b = 0;
	int sat = 255;

	if(thress <= 0.2)
	{
		b = sat;
		g = uchar(thress/0.2 * sat);

		return QColor(r,g,b);
	}
	else if(thress < 0.399)
	{
		g = sat;
		b = uchar(sat-(thress-0.4)/0.2 * sat);

		return QColor(r,g,b);
	}
	else if(thress < 0.599)
	{
		g = sat;
		r = uchar((thress-0.6)/0.2 * sat);

		return QColor(r,g,b);
	}
	else if(thress < 0.799)
	{
		r = sat;
		g = sat - uchar((thress-0.8)/0.2 * sat);

		return QColor(r,g,b);
	}
	else if(thress < 0.999)
	{
		r = sat;
		b = uchar((thress-1)/0.2 * sat);

		return QColor(r,g,b);
	}
	else if(thress >= 1)
	{
		r = sat;
		b = sat;
		return QColor(r,g,b);

	} else return QColor(255,255,255);
}

QColor ColorUtility::GetCumulativeColor(double value)
{
	return ZValueToColor(value, MIN_Z_LEVELS.cumulative,
						 MAX_Z_LEVELS.cumulative);
}

QColor ColorUtility::GetFreqColor(double value)
{
	return ZValueToColor(value, MIN_Z_LEVELS.frequency,
						 MAX_Z_LEVELS.frequency);
}

QColor ColorUtility::GetAvgColor(double value)
{
	return ZValueToColor(value, MIN_Z_LEVELS.average,
						 MAX_Z_LEVELS.average);
}

QColor ColorUtility::GetHighest(double value)
{
	return ZValueToColor(value, MIN_Z_LEVELS.highest,
						 MAX_Z_LEVELS.highest);
}

