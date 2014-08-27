#ifndef COLORUTILITY_H
#define COLORUTILITY_H

#include <QMutexLocker>
#include <QImage>

class colorUtility
{
public:
	colorUtility();

	struct zLevels{
		double high_average;
		double high_cumulative;
		double high_frequenzy;
		double low_average;
		double low_cumulative;
		double low_frequenzy;
		double highest;
	};

	static void addMaxMinValues(zLevels *argLevels);

	static QRgb zValueToColor(double value, double min, double max);

	static QRgb getCumulativeColor(double value);
	static QRgb getFreqColor(double value);
	static QRgb getAvgColor(double value);
	static QRgb getHighest(double value);

	static double Z_TRESSHOLD_PERCENTAGE;

private:

	static QMutex MUTEX;
	static zLevels *ABS_Z_LEVELS;
};

#endif // COLORUTILITY_H
